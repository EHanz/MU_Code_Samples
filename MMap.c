/*
  Filename   : MMap.c
  Author     : Evan Hanzelman
  Course     : CSCI 380
*/

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void
mmap_copy (int og_copy, int new_copy, int file_size);

int
main (int argc, char **argv)
{
    if (argc != 3)
    {
        printf ("Usage: %s <original_file> <copy_file>\n", argv[0]);
        exit (0);
    }
    
    int og_copy = open (argv[1], O_RDONLY);

    if (og_copy < 0)
    {
        printf ("Unable to open the file: %s\n", argv[1]);
        return 0;
    }

    int new_copy = open (argv[2], O_RDWR | O_CREAT | O_TRUNC, 0600);

    if (new_copy < 0)
    {
        printf ("Unable to create the file: %s\n", argv[2]);
        return 0;
    }

    struct stat file_stat;

    fstat (og_copy, &file_stat);
    
    ftruncate (new_copy, file_stat.st_size);

    mmap_copy (og_copy, new_copy, file_stat.st_size);

    close (og_copy);
    close (new_copy);

    exit (0);
}

void
mmap_copy (int og_copy, int new_copy, int file_size)
{
    void *file_src, *file_dest;

    file_src = mmap (NULL, file_size, PROT_READ, MAP_SHARED, og_copy, 0);

    if (file_src == (caddr_t) -1)
    {
        printf ("mmap error for source file\n");
    }

    file_dest = mmap (NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, new_copy, 0);

    if (file_dest == (caddr_t) -1)
    {
        printf ("mmap error for destination file\n");
    }

    memcpy (file_dest, file_src, file_size);

    munmap (file_src, file_size);

    munmap (file_dest, file_size);
}