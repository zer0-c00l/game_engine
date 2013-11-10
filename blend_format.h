#ifndef BLEND_FORMAT_H_INCLUDED
#define BLEND_FORMAT_H_INCLUDED

#include "util.h"
#include <stdint.h>

struct blend_block {
   void* raw; //carefull, this points to the file //unparsed file
   //void* mem; //actual content
   uint8_t*    code;       //4   ID of file-block
   uint32_t*   size;       //4   block length (excluding header)
   uint8_t*    old_ptr;    //4/8 address of struct when it was written to disk
   uint32_t*   sdna_index; //4
   uint32_t*   count;      //4   number of struct's in this block
};

struct blend_format {
   int ptr_size;
   bool little_endian;

   ///FILE HEADER
   struct {
      uint8_t*    id;         //7   BLENDER
      uint8_t*    ptr_size;   //1   '_' = 4 bytes, '-' = 8 bytes
      uint8_t*    endianness; //1   'v' = little, 'V' = big
      uint8_t*    version;    //3   'abc' = a.bc; ex: '246' = 2.46
   } header;

   ///BLOCKS
   struct blend_block *blocks;
   int num_blocks, size_blocks;
};

/* Takes struct ge_file *file and parses it
 * into struct blend_format *blend.
 */
int blend_parse(struct ge_file *ge_file,
                struct blend_format *blend);

/* "code" or id in blend_block */
int get_block_by_name(struct blend_format *blend, const char name[4], int start_search);

/* index (n) obtained from get_block_by_name(). */
struct blend_block*
get_block_by_index(struct blend_format *blend, int n);

/*
dna
{
	char[4] 					id 				= "SDNA";

	char[4] 					name_id 			= "NAME";
	uint32 					num_names;
	char*[num_names] 		names;

	char[4]					type_id			= "TYPE";
	uint32					num_types;
	char*[num_types]  	types;
	//
	char[4]					len_id			= "TLEN";
	char*[num_types]		lens;

	char[4]					struct_id		= "STRC";
	uint32					num_structs;
	char[2][num_structs]	structs, fields;
		for (fields[i].type)
		for (fields[i].name)
}
*/

struct blend_DNA1 {
   uint8_t*    id;         //4   "SDNA"

   uint8_t*    name_id;    //4   "NAME"
   uint32_t*   num_names;  //4   number of names to follow
   uint8_t**   names;      //char*[num_names];     array of names, e.g. '*vertex[3]\0'

   uint8_t*    type_id;    //4   "TYPE"
   uint32_t*   num_types;  //4   number of types to follow
   uint8_t**   types;      //char*[num_types];     array of types, e.g. 'int\0'

   uint8_t*    len_id;     //4   "TLEN"
   uint8_t**   lens;       //char[2][num_types];   len., in bytes of each type

   uint8_t*    struct_id;  //4   "STRC"
   uint32_t*   num_structs;//4   number of structs to follow
   uint8_t**   structs;    //char[2][num_structs]; index in types containing the name of the structure
   uint8_t**   num_fields; //char[2][num_structs]; number of fields for this struct
   uint8_t***  type_field; //char[2][num_fields][num_structs]; index in type
   uint8_t***  name_field  //char[2][num_fields][num_structs]; index in name

};

struct blend_Scene {
};

#endif // BLEND_FORMAT_H_INCLUDED
