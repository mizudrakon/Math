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

//READ NUBMER FROM INPUT:
int read_num(STR_INT* num, FILE* f)
{
    int c;
    num->lastPartLength = 0;
    //we need to allow only numbers < base
    //IGNORE white spaces or any possibly separating symbols in front
    c = getc(f);
    while (c == '0' || is_digit(c, num->base) == 0)//ignore leading zeros
    {
        if (c == '$') return 1;//$ is escape character
        c = getc(f);
    }
    //READING THE NUMBER:
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

    //HERE we mirror the elements of the linked arrays of char, so that every number has its lowest digit on 1
    //VARIABLES:
    int fw_count = 0;
    int bw_count = 0;
    char tmp;
    STR_INT_PART* fw_part = num->head;
    STR_INT_PART* bw_part = num->tail;
    char* fw_data = fw_part->data;
    char* bw_data = bw_part->data+num->lastPartLength-1;
    //MIRRORING loop:
    while (1)
    {
        //CHECKS if the mirroring should stop:
        //we only need to cross into other parts when the two pointers point to different parts
        if (fw_part == bw_part->next)
            break;
        if (fw_part != bw_part){
            //forward data iterator overflowing
            if (fw_data == fw_part->data + num->partSz)
            {
                fw_part = fw_part->next;
                fw_data = fw_part->data;
            }
            //backward data iterator underflowing
            if (bw_data == bw_part->data-1)
            {
                bw_part = bw_part->prev;
                bw_data = bw_part->data+num->partSz-1;
            }
            
        }
        //reached the middle
        if (fw_data == bw_data) 
            break;
        //SWITCH:
        tmp = *fw_data;
        *fw_data = *bw_data;
        *bw_data = tmp;
        fw_data++;
        bw_data--;

    }

    return 0;//zero errors
}

void formated_print_str_int(STR_INT* num, FILE* f, int brk, size_t line_len)//prints a number string to chosen output
{
    //we keep the numbers so the least index is the least digit of the number, so we write out backwards
    for (STR_INT_PART* bw_part_it = num->tail; bw_part_it != NULL; bw_part_it = bw_part_it->prev)
    {
        /*we're moving from the last part at a specific place accross all the parts where we just start from the last element
         hence the ugly conditional assignment*/
        for (char* bw_data_it = (bw_part_it->next == NULL) ? bw_part_it->data+num->lastPartLength-1 : bw_part_it->data+num->partSz-1; 
        bw_data_it >= bw_part_it->data; bw_data_it--)
        {
            if (bw_part_it->prev == NULL && bw_data_it < bw_part_it->data)
                break;

            putc(*bw_data_it,f);
        }
    }
    putc('\n',f);
}

//BACKWARD PRINTING is actually just straightforward printing, because we keep the data backward
void backward_print_str_int(STR_INT* num, FILE* f, int brk, size_t line_len)
{
    for (STR_INT_PART* part_it = num->head; part_it != NULL; part_it = part_it->next)
    {
        for (char* data_it = part_it->data; data_it != part_it->data + num->partSz; data_it++)
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
