#include "blend_format.h"

int main(int nargs, char** args)
{
   int err = 0;

   struct ge_file blend_file;
   err = ge_file(((nargs == 1) ? "untitled.blend" : args[1]),
                  &blend_file);

   struct blend_format blend;
   err = blend_parse(&blend_file, &blend);

/*
   //in bytes
   int curr_read = 0, total_read = 0, buffer_size = 10000, chunk_size = 10000;

   char *buffer, *start;
   buffer = start = malloc(sizeof(char)*10000);


   do {
      curr_read = read(fd, buffer, chunk_size);
      total_read += curr_read;

      if (total_read + chunk_size > buffer_size) {
         buffer_size *= 2;
         buffer = realloc(buffer, buffer_size);
      }

      //fwrite(buffer, 1, bytes_read, stdout);
      //printf("\n");
   } while (curr_read == 10000);
*/

   return 0;
}

