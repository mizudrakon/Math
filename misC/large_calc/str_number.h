#ifndef STR_NUMBER_H
#define STR_NUMBER_H
#include <stdio.h>
#include <stdlib.h>

//basically a supplement for the long-number-kept-as-string class
/*STR_INT_PART is wrapper for a c array with pointers to previous and next
 it also has a pointer to it's mother which is the str_int struct defined next*/
typedef struct str_int_part 
{
    unsigned short partNumber;//it a kth part in str_int::totalParts
    struct str_int_part* prev;
    struct str_int_part* next;
    struct str_int* mother;//pointer to str_int struct that collects the overlaying info
    char* data;//number characters stuffed into the part
} STR_INT_PART;

//structure with all info about the str_int coupled in
typedef struct str_int
{
    unsigned short totalParts;//number of str_int_parts in total
    size_t lastPartLength;//how far is the last part of allocated memory filled
    size_t partSz;//size of a single part
    //-> so the length of the whole number is totlaParts * partSz + lastPartLength
    char base;//numeric base of the number
    //pointers to first and last part:
    STR_INT_PART* head;
    STR_INT_PART* tail;
} STR_INT;


STR_INT_PART* new_si_part(STR_INT* mom, STR_INT_PART* prev, STR_INT_PART* next);
/*create new str_int_part = c array that we can read the number into
prev = NULL for head, or some other str_int_part
next = NULL untill we need more space, when it gets allocated and filled by the read_num function*/

STR_INT* new_str_int(size_t base, size_t part_len);
/*creating a new handle for a number*/

int deleteSTR_INT(STR_INT* corpse);
/*cleaning function to free the memory*/


char max_digit(size_t b);
//given a base b, get the char representing the maximum single digit

int is_digit(char c, char base);
//tests a char for being a numeric given the base

int read_num(STR_INT* num, FILE* f);
/*read from the specified input into the specified number handler
 * writes the digits one by one into the head part
 * calls new_str_int_part() if necessary
 * returns 1 for success and 0 for failure, which might switch, since the opposite might be handier for counting total errors*/

void formated_print_str_int(STR_INT* num, FILE* f,int brk, size_t line_len);
//print with possibility of adding new_lines, brk is meant as bool, line len is the required lenght
//but it might be nice to make brk a separator specifier and get the truth value just for line_len x<=0 being false

void print_str_int(STR_INT* num, FILE* f);
//just print with no \n-s

int mark(char* num, char base);
/*marks the end of the number in the tail
 * this is only useful when we need to assess an unknown array of digits
*/

#include "str_number.c"
#endif