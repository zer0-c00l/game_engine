#include "blend_format.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void print_blend(struct blend_format *blend)
{
   ///HEADER
   printf("\n\nid:\t\t%.7s", blend->header.id);
   printf("\npointer size:\t%i\t(%c)", blend->ptr_size, *blend->header.ptr_size);
   char *endi = (blend->little_endian == true) ? "little" : "big";
   printf("\nendianness:\t%s\t(%c)", endi, *blend->header.endianness);
   printf("\nversion:\t%c.%c%c\t(%.3s)",
          blend->header.version[0], blend->header.version[1], blend->header.version[2],
          blend->header.version);
   printf("\n\n--------------File Blocks--------------\n");
   int i;
   for (i = 0; i < blend->num_blocks; i++)
      printf("\nBLOCK: %.4s\tsize: %i \told_ptr: %p\t\tsdna_index: %i\t\tcount: %i\t\t",
             blend->blocks[i].code, *blend->blocks[i].size,
             *blend->blocks[i].old_ptr, *blend->blocks[i].sdna_index,
             *blend->blocks[i].count);
   printf("\n\nnumber of blocks: %i\n", blend->num_blocks);
}

#define blend_get_next_member(structure, item, size) \
   structure.item = file+total; /*= memcpy(data+total, file+total, size); */ \
   total += size;

//returns total
///FILE BLOCK HEADER
int parse_block(uint8_t* file, uint8_t* data, int total,
                struct blend_format *blend)
{
   if (blend->num_blocks >= blend->size_blocks &&
       (blend->blocks = realloc(blend->blocks, sizeof(struct blend_block)*(blend->size_blocks *= 2))) == NULL)
   {/*error while allocating more space.*/}

   blend_get_next_member(blend->blocks[blend->num_blocks-1], code, 4);
   blend_get_next_member(blend->blocks[blend->num_blocks-1], size, 4);
   blend_get_next_member(blend->blocks[blend->num_blocks-1], old_ptr, blend->ptr_size);
   blend_get_next_member(blend->blocks[blend->num_blocks-1], sdna_index, 4);
   blend_get_next_member(blend->blocks[blend->num_blocks-1], count, 4);

   total += *(int*)(blend->blocks[blend->num_blocks-1].size);
   blend->blocks[blend->num_blocks-1].raw = file+total;

   blend->num_blocks++;
   return total;
}

int blend_parse(struct ge_file *ge_file,
                struct blend_format *blend)
{
   uint8_t* file = ge_file->ptr;
   //we allocate memory for struct blend_format members only once
   uint8_t* data = malloc(ge_file->size);
   int total = 0;

   ///HEADER
   blend_get_next_member((*blend), header.id, 7);
   blend_get_next_member((*blend), header.ptr_size, 1);
   blend_get_next_member((*blend), header.endianness, 1);
   blend_get_next_member((*blend), header.version, 3);

   if (*blend->header.ptr_size == '_')
      blend->ptr_size = 4;
   else if (*blend->header.ptr_size == '-')
      blend->ptr_size = 8;
   else {
      printf("unknown pointer size in *.blend file");
      exit(-1);
   }

   if (*blend->header.endianness == 'v')
      blend->little_endian = true;
   else if (*blend->header.endianness == 'V')
      blend->little_endian = false;
   else {
      printf("unknown endianness in *.blend file");
      exit(-1);
   }


   ///FILE BLOCK
   //BLOCK HEADER
   blend->blocks = malloc(sizeof(struct blend_block));
   blend->size_blocks = blend->num_blocks = 1;

   int i = 0;
   do {
      if (i++ >= 1000) break; //printf("%i\n", i);

      total = parse_block(file, data, total, blend);
   } while (total < ge_file->size && strncmp("ENDB", blend->blocks[blend->num_blocks - 2].code, 4));
   blend->num_blocks--;
   print_blend(blend);
   return 0;
}

int get_block_by_name(struct blend_format *blend, const char name[4], int start_search)
{
   int i;
   for (i = start_search; i < blend->num_blocks; i++)
      if (0 == strncmp(name, blend->blocks[i].code, 4))
         return i;
   return -1;
}

/* index (n) obtained from get_block_by_name(). */
struct blend_block*
get_block_by_index(struct blend_format *blend, int n)
{
   return blend->blocks[n];
}

int blend_parse_DNA1(uint8_t* file, struct blend_block* block, struct blend_DNA1 *ret)
{
   int total = 0;
   blend_get_next_member((*ret), id, 4);

   blend_get_next_member((*ret), name_id, 4);
   blend_get_next_member((*ret), num_names, 4);

   ret->names = malloc(sizeof(char*) * ret->num_names);
   int i;
   for (i = 0; i < ret->num_names; i++) {
      int len = strlen()
   }

   blend_get_next_member((*ret), names, );

}



