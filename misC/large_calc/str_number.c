#include "str_number.h"


//this adds a new part to a specified mother, does it need to return a pointer?
int new_si_part(STR_INT* mom)
{
    STR_INT_PART* part;
    if ( (part = (STR_INT_PART*) malloc(sizeof(STR_INT_PART))) == NULL )
    {    printf("str_int_part basic malloc failed\n"); return 1;}
    part->mother = mom;//assign mother
    if (mom->head == NULL)//if mother doesn't have a head -> part is head
    {
        mom->head = part;
        part->prev = NULL;
    }
    else 
    {
        part->prev = mom->tail;//new part's prev is mother's current tail
        mom->tail->next = part;//the part becomes the new tail
    }
    mom->tail = part;
    mom->totalParts++;//motehr now hes +1 part
    part->partNumber = mom->totalParts;//the new part's number is the current total n/n
    if ((part->data = (char*)malloc(mom->partSz * sizeof(char))) == NULL)//allocate storage
    {    printf("str_int_part data malloc failed\n"); return 1;}
    return 0;//no failure
}

STR_INT* new_str_int(char base, size_t part_len)
{
    STR_INT* strnum;
    if ((strnum = (STR_INT*) malloc(sizeof(STR_INT))) == NULL)//allocate struct
        printf("new_str_int malloc failed\n");
    strnum->partSz = part_len;
    strnum->base = base;
    strnum->head = NULL;
    //create the first part that is both head and tail
    if (new_si_part(strnum))
    {
        fprintf(stderr, "failed creating a new string_int_part!");
        return NULL;
    }//no prev or next yet
    strnum->tail = strnum->head;
    //we want init as 0 to be safe-r...
    strnum->lastPartLength = 1;
    strnum->head->data[0] = '0';
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
        if (num_it == part_it->data+num->partSz) //last element is data+num->partSZ-1, so this is the overflow
        {
            if (new_si_part(num)){
                fprintf(stderr, "new part creation failed!\n");
                return 1;
            }
            part_it = part_it->next;//created new part is already next in the linked list
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
    int p = 0;
    for (STR_INT_PART* part_it = num->head; part_it != NULL; part_it = part_it->next)
    //having part_it++ in there cases the NULL check to fail, obviously
    {
        for (char* data_it = part_it->data; *data_it != '\0'; data_it++)
        {
            if (part_it->next == NULL && data_it >= part_it->data+num->lastPartLength)
                break;

            putc(*data_it,f);
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
    *num_it = '\0';
    return 0;
}
