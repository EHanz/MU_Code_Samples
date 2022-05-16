//Tiny Shell Program

/*
 * Filename   : tsh.c
 * Author     : Evan Hanzelman
 * 
 * Discussion Segment:
 * The signals were handled by utlizing the sigint and sigtsp for intercepting the signals, sending them to 
 * whatever the given running process is, and then the sigchild handler would take over and handle the rest.
 * WNOHANG and WUNTRACED were utilized because WNOHANG doesn't halt or affect the shell when processes are sent
 * to run in the background, while WUNTRACED is used so that when the sigtstp stops a process, the child handler
 * has the ability to detect the change. The sig child handler is used to preserve the errno in a temp err variable
 * and then the errno can be restored to its former value if errno ever got set during an execution. The foreground
 * job works for an infinite amount of time until something affecting the child is found by the sigsuspend signal.
 */

// Series of libraries and headers used for shell
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef void handler_t (int);

// defining sizes of arguments and line
#define MAX_SIZE 1024 
#define MAX_ARGS 128

// variables utilized throughout program
extern char** environ;   

char shell_prompt [] = "tsh> ";

char buff_size [MAX_SIZE];

volatile pid_t running_pid = 0;
volatile pid_t suspended_pid = 0; 

// Signal blockers
sigset_t signal_mask;
sigset_t signal_prev;

// Declarations of the shell's function prototypes
int
tokenize (const char* command, char** argv);

// Prototypes for the handler functions necessary to operate the shell
void
sigint_handler (int signal);

void
sigtstp_handler (int signal);

void
sigchld_handler (int signal);

void
sigquit_handler (int signal);

// helper functions
void
system_error (char* err_msg);

void
application_error (char* err_msg);

handler_t*
Signal (int sig_num, handler_t* handler);

void
evaluate (const char* cmd_input);

int
cmd_exists (char** args);

void
waitfg (const pid_t pid);

pid_t
Fork ();

int
main (int argc, char** argv)
{
  dup2 (1, 2);

  // prep signal handlers- ctrl-C, ctrl-Z, stopped child, and quit
  Signal (SIGINT, sigint_handler);   
  Signal (SIGTSTP, sigtstp_handler); 
  Signal (SIGCHLD, sigchld_handler); 
  Signal (SIGQUIT, sigquit_handler); 

  while (1)
  {
    // Creates input command array
    char cmd_input[MAX_SIZE];

    if(feof(stdin))
    {
      break;
    }

    printf (shell_prompt);

    // Verify fgets is not null
    if (fgets (cmd_input, MAX_SIZE, stdin) == NULL)
    {
      break;
    }
      
    evaluate(cmd_input);

    fflush(stdout);
  }

  // quit the program
  exit (0);
}

// evaluates the input the user provides
void
evaluate (const char* cmd_input)
{
  // create an array for commands
  char* args [MAX_ARGS];
  int in_bg = tokenize (cmd_input, args);

  // verifies if a line is null/empty
  if (args[0] == NULL)
  {
    return;
  }

  // verfies if a command is part of the system or not
  if(cmd_exists(args))
  {
    return;
  }

  sigemptyset (&signal_mask);
  sigaddset (&signal_mask, SIGCHLD);
  sigprocmask (SIG_BLOCK, &signal_mask, &signal_prev);

  // fork the process if the command is not part of the system
  running_pid = Fork();

  // Verifies if what is provided is a child process or not
  if(running_pid == 0)
  {
    setpgid (0, 0);

    sigprocmask (SIG_SETMASK, &signal_prev, NULL);

    int ret = execve (args[0], args, environ);

    if (ret < 0)
    {
      fprintf (stderr, "%s: Command not found\n", *args);
      exit (1);
    }
  }

  sigprocmask (SIG_SETMASK, &signal_prev, NULL);

  // halts and waits for process to finish as long as it is not running in the background
  if (!in_bg)
  {
    waitfg (running_pid);
  }
  else 
  {
    printf("(%d) %s", running_pid, cmd_input);
    fflush(stdout);
  }
}

// if the provided command is a built-in command, it is executed and returns a 1
int
cmd_exists (char** args)
{
  if (strcmp (args[0], "quit") == 0)
  {
    exit (0);
  }

  if (strcmp(args[0], "fg") == 0)
  {
    if (suspended_pid > 0)
    {
      running_pid = suspended_pid;
      suspended_pid = 0;

      kill (-running_pid, SIGCONT);
      waitfg (running_pid);
    }

    return 1;
  }

  return 0;
}

pid_t
Fork ()
{
  // Fork the process
  pid_t child = fork ();

  // verify if the given process failed or not
  if (child < 0)
  {
    fprintf (stderr, "fork error (%s) -- quitting\n", strerror(errno));
    exit (-1);
  }

  return child;
}

// function waits for the process to finish
void
waitfg (const pid_t pid)
{
  while (running_pid != 0)
  {
    sigsuspend (&signal_prev);
  }
}

// function prepares char and arg arrays from the given command line array
int
tokenize (const char* command, char** argv)
{
  // variables that hold a copy of the command line and traverses the command line
  static char command_arr [MAX_SIZE]; 
  char* buff_ptr = command_arr;

  // delimeter for the first space, number of given args, and background jobs
  char* delimeter;                
  int arg_nums;                   
  int in_bg;                     

  strcpy (buff_ptr, command);
  buff_ptr [strlen (buff_ptr) - 1] = ' ';  

  // ignores the leading spaces
  while (*buff_ptr && (*buff_ptr == ' ')) 
  {
    buff_ptr++;
  }

  arg_nums = 0;

  if (*buff_ptr == '\'')
  {
    buff_ptr++;
    delimeter = strchr (buff_ptr, '\'');
  }
  else
  {
    delimeter = strchr (buff_ptr, ' ');
  }

  while (delimeter)
  {
    argv [arg_nums++] = buff_ptr;
    *delimeter = '\0';
    buff_ptr = delimeter + 1;

    // ignores the spaces
    while (*buff_ptr && (*buff_ptr == ' ')) 
    {
      buff_ptr++;
    }
      
    if (*buff_ptr == '\'')
    {
      buff_ptr++;
      delimeter = strchr (buff_ptr, '\'');
    }
    else
    {
      delimeter = strchr (buff_ptr, ' ');
    }
  }

  argv[arg_nums] = NULL;

  // if a blank line is encountered it is ignored
  if (arg_nums == 0) 
  {
    return 1;
  }

  // verifies if the given job should be run in the background
  if ((in_bg = (*argv[arg_nums - 1] == '&')) != 0)
  {
    argv[--arg_nums] = NULL;
  }

  return in_bg;
}

// functions making up all the signal handlers
// a sigchild is sent to the kernel if a child's job is terminated or if a sigstop/sigtstp is received
void
sigchld_handler (int signal)
{
  // assigns the errno to a temp to keep it around
  int temp_errno = errno;

  // contains the PID info and its status
  int wstatus;
  pid_t waitID;

  while ((waitID = waitpid (-1, &wstatus, WNOHANG|WUNTRACED)) > 0)
  {
    if (WIFSIGNALED (wstatus)) 
    {
      printf ("Job (%d) terminated by signal %d\n", waitID, WTERMSIG (wstatus));
      fflush (stdout);
    }
    else if (WIFSTOPPED(wstatus)) 
    {
      printf ("Job (%d) stopped by signal %d\n", waitID, WSTOPSIG (wstatus));
      fflush (stdout);
      suspended_pid = running_pid;
    }

    running_pid = 0;
  }

  // Restores errno to what it was before
  errno = temp_errno;

  return;
}

// a sigint is sent to the kernel if ctrl-c is entered by the user and it is then sent as a foreground job
void
sigint_handler (int signal)
{
  if (running_pid > 0)
  {
    kill(-running_pid, SIGINT);
  }

  return;
}

// a sigtstp is sent to the kernel if ctrl-z is entered by the user and the foreground job is then suspended
void
sigtstp_handler (int signal)
{
  if (running_pid > 0)
  {
    kill (-running_pid, SIGTSTP);
  }

  return;
}

// Series of helper functions
// thrown if system/unix error is encountered
void
system_error (char* err_msg)
{
  fprintf (stdout, "%s: %s\n", err_msg, strerror (errno));
  exit (1);
}

// thrown if application error is encountered
void
application_error (char* err_msg)
{
  fprintf (stdout, "%s\n", err_msg);
  exit (1);
}

// sigaction's wrapper
handler_t*
Signal (int sig_num, handler_t* handler)
{
  struct sigaction current_action, prev_action;

  current_action.sa_handler = handler;
  sigemptyset (&current_action.sa_mask); 

  // attempts to restart syscalls
  current_action.sa_flags = SA_RESTART;  

  if (sigaction (sig_num, &current_action, &prev_action) < 0)
  {
    system_error ("Signal error");
  }
    
  return (prev_action.sa_handler);
}

// if sigquit is sent, the shell is terminated
void
sigquit_handler (int signal)
{
  printf ("Terminating after receipt of SIGQUIT signal\n");
  exit (1);
}