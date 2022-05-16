/*
*   Filename: Redirect.c
*   Author: Evan Hanzelman
*/

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int 
mysystem (const char* command);

int 
main (int argc, char* argv [])
{
    if (argc != 2)
    {
      printf ("Command can only take one argument\n");
      
      exit (1);
    }

    int my_sys;

    my_sys = mysystem (argv [1]);
    
    return my_sys;
}

int 
mysystem (const char* sys_command)
{
  int sys_temp;

  __pid_t pid = fork ();

  if (pid == 0)
  {
      sys_temp = open ("System.out", O_RDWR | O_CREAT | O_TRUNC, 0600);

      dup2 (sys_temp, 1);

      execl ("/bin/sh", "sh", "-c", sys_command, (char *) NULL);
           
  }

  int sys_status;

	pid_t wpid;

	while ((wpid = waitpid (-1, &sys_status, WNOHANG)) > 0)
  {
		if (WIFEXITED (sys_status))
    {
      fflush (stdout);
		}
		else if (WIFSIGNALED (sys_status))
    {
			printf ("The Job (%d) was terminated by signal %d\n", wpid, WTERMSIG (sys_status));

      fflush (stdout);
		}

  }

  return sys_temp;
}