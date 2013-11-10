#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>

/* takes file path and struct k_file, and returns error (0 if none) */
int ge_file(const char* path, struct ge_file *file)
{
   file->path = path;
   if ((file->fd = open(path, O_RDWR)) < 0)
      printf("error"); //TODO: check errors

   fstat(file->fd, &file->info); //TODO: check errors
   file->size = file->info.st_size;

   //TODO: check errors
   file->ptr = mmap(0, file->size, PROT_READ | PROT_WRITE, MAP_SHARED, file->fd, 0);
   //printf("%i", errno);
#ifdef DEBUG
   ge_print_file_info(file);
#endif // DEBUG
   return 0;
}

void ge_print_file_info(struct ge_file *file)
{
   printf("file:\t\t%s\nsize (bytes):\t%i\nfd:\t\t%i\n",
          file->path, file->size, file->fd);
}
