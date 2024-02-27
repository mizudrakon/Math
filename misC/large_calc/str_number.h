#ifndef STR_NUMBER_H
#define STR_NUMBER_H
#include <stdio.h>
#include <stdlib.h>

//let's have us a structure with all info coupled in
//basically a supplement for the long-number-kept-as-string class

//typedef struct STR_INT;
//STR_INT_PART is a certain chunk (of size part_sz) of the number we're trying to save as STR_INT

typedef struct str_int_part 
{
    unsigned short partNumber;
    size_t partSz;
    struct str_int_part* prev;
    struct str_int_part* next;
    struct str_int* mother;
    char* data;
} STR_INT_PART;

typedef struct str_int
{
    unsigned short totalParts;
    size_t lastPartLength;
    size_t partSz;
    char base;
    STR_INT_PART* head;
    STR_INT_PART* tail;
} STR_INT;

char max_digit(size_t b);//given a base b, get the char representing the maximum single digit
int is_digit(char c, char base);//tests a char for being a numeric given the base
int read_num(STR_INT* num, FILE* f);//read from input
void formated_print_str_int(STR_INT* num, FILE* f,int brk, size_t line_len);//print with possibility of adding new_lines
void print_str_int(STR_INT* num, FILE* f);//just print 
int mark(char* num, char base);//mark the end of the number with \0

STR_INT_PART* new_si_part(STR_INT* mom, STR_INT_PART* prev, STR_INT_PART* next)
{
    STR_INT_PART* part;
    if ( (part = (STR_INT_PART*) malloc(sizeof(STR_INT_PART))) == NULL )
        printf("str_int_part basic malloc failed\n");
    mom->totalParts++;
    mom->tail = part;
    part->partNumber = mom->totalParts;
    if ((part->data = (char*)malloc(mom->partSz)) == NULL)
        printf("str_int_part data malloc failed\n");
    part->mother = mom;
    part->prev = prev;
    part->next = next;
    return part;
}

STR_INT* new_str_int(size_t base, size_t part_len)
{
    STR_INT* strnum;
    if ((strnum = (STR_INT*) malloc(sizeof(STR_INT))) == NULL)
        printf("new_str_int malloc failed\n");
    strnum->head = new_si_part(strnum, NULL, NULL);
    strnum->tail = strnum->head;
    strnum->base = max_digit(base);
    strnum->lastPartLength = 1;
    strnum->head->data[0] = '0';//init to 0
    return strnum;
}

//cleaning STR_INT data
int deleteSTR_INT(STR_INT* corpse)
{
    STR_INT_PART* part_it = corpse->head;
    for (; part_it != NULL; part_it = part_it->next)
    {
        free((void*)part_it->data);
        if (part_it->prev!=NULL)
            free((void*)part_it->prev);
    }
    free((void*)part_it);
    free((void*)corpse);
}

STR_INT* convert(){}

//we want a useful char from number
//basically the symbol representing the largest allowed digit 1-9 a(10)-z(36)
char max_digit(size_t b)
{
    if (b <= 9){
        return b + '0' - 1; //the largest digit is one lower then the base 
    }
    if (b == 10) return '9';
    if (b <= 36){
        return b - 10 + 'a' - 1;
    }
    return '$';
}

/*numbers 10+ are represented by letters, so we need to test characters and not just regular numerals
c is char to be tested against the base (c < base, since base isn't a numeral in it's own system)*/
int is_digit(char c, char base){
    //reduce capital letters to small, since we're not sure which come first
    if (c >= 'A' && c <= 'Z')
    {
        c = 'a' + (c - 'A');
    }
    //the case we need to deal with the 10+ base
    if (base >= 'a' && base <= 'z'){
        return (c >= '0' && c <= '9') || (c >= 'a' && c < base);
    }
    return c >= '0' && c <= '9';
}

int read_num(STR_INT* num, FILE* f)
{
    int c;
    num->lastPartLength = 0;
    //we need to allow only numbers < base
    //ignore white spaces or any possibly separating symbols in front
    c = getc(f);
    while (is_digit(c, num->base) == 0)
    {
        if (c == '$') return 1;//$ is escape character
        c = getc(f);
    }
    STR_INT_PART* part_it = num->head;
    char* num_it = num->head->data; //iterator
    while (is_digit(c,num->base))
    {
        //if we reach the end of the data array:
        if (num_it == part_it->data+num->partSz){
            part_it->next = new_si_part(num,part_it,NULL);//create new part and switch to it
            part_it = part_it->next;
            num_it = part_it->data;
            num->lastPartLength = 0;//we start from 0, the previous parts are all full
        }
        *num_it++ = c;
        num->lastPartLength++;
        c = getc(f);

    }
    if (num_it < part_it->data+num->partSz) *num_it = '\0';//marks the end with $

//HERE I NEED TO MIRROR THE ELEMENTS


    return 0;//zero errors
}

void formated_print_str_int(STR_INT* num, FILE* f, int brk, size_t line_len)//prints a number string to chosen output
{
    size_t len = 0;
    for (STR_INT_PART* part_it = num->head; part_it != NULL; part_it++)
    {
        for (char* num_it = part_it->data; num_it < part_it->data+num->partSz && *num_it != '\0'; num_it++)
        {
            if (brk && (len % line_len == 0))
                putc('\n',f);
            fprintf(f,"%c",*num_it);
            len++;   
        }
    }
    putc('\n',f);
}

void print_str_int(STR_INT* num, FILE* f)
{
    formated_print_str_int(num, f, 0, 0);
}


int mark(char* num, char base)
{
    char* num_it = num;
    while (is_digit(*num_it,base)) num_it++;
    //while (*num_it >= '0' && *num_it <= '9') num_it++;
    *num_it = '\0';
    return 0;
}

#endif