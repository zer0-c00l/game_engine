#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <sys/stat.h>
#include <stdbool.h>

#define DEBUG

/* small little library for manipulating files */
struct ge_file {
   const char* path;
   void* ptr; //mmap()'ed pointer to the content of the file
   int size; //in bytes

   //system specific
   int fd;
   struct stat info;
};

int ge_file(const char* path, struct ge_file *file);
void ge_print_file_info(struct ge_file *file);
//TODO: check errors
//void munmap_file(int len) { munmap(mem, len); }

static inline bool is_printable(char c)
{
   return (c >= 97 && c <= 122) || (c >= 65 && c <= 90) || (c >= 48 && c <= 57);
}
//a-z 97-122
//A-Z 65-90
//0-9 48-57

#endif // UTIL_H_INCLUDED
