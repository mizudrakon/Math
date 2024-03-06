#include "str_number.h"

STR_INT_ITERATOR* make_iterator(STR_INT* mom)
{
    STR_INT_ITERATOR* it;
    it->mom = mom;
    it->part_it = mom->head;
    it->data_it = mom->head->data;
}

int iterator_fw(STR_INT_ITERATOR* it)
{
    if (it->part_it == it->mom->tail && it->data_it > it->mom->end) return 0;
    
    it->data_it++;
    
    if (it->data_it == it->part_it->data + it->mom->partSz)
    {
        it->part_it = it->part_it->next;
        it->data_it = it->part_it->data;
    }
    return 1; //return true 
}

int iterator_bw(STR_INT_ITERATOR* it)
{
    it->data_it--;

    if (it->data_it < it->part_it->data)
    {
        if (it->part_it == it->mom->head) return 0;
        it->part_it = it->part_it->prev;
        it->data_it = it->part_it->data + it->mom->partSz - 1;
    }
    return 1;
}

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
    part->next = NULL;
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
    strnum->tailLength = 1;
    strnum->head->data[0] = '0';
    strnum->end = strnum->head->data+1;
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
c is char to be tested against the base (c < base, since base isn't a numeral in it's own system)
* this is a basic is_digit function only to tell us if the number is */
int is_digit(char c, const char base){
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

/*A horrible function that returns a truth value BUT also changes the char value that is being evaluated
 * the thing is our str_int uses char ordinal values of chars, not the symbols, so this is how we want to keep them
 * the value used by reference is actually a copy from the source already and we don't care what happens to it here*/
int is_digit_convert(char* p_c, const char* p_base){
    //reduce capital letters to small, since we're not sure which come first
    if (*p_c >= '0' && *p_c <= '9')
        *p_c = *p_c - '0';
    else if (*p_c >= 'A' && *p_c <= 'Z')
        *p_c = 10 + (*p_c - 'A');
    else if (*p_c >= 'a' && *p_c <= 'z')
        *p_c = 10 + (*p_c - 'a');
    else return 0; 
    //return truth value
        return *p_c >= 0 && *p_c < *p_base;
}

//READ NUBMER FROM INPUT:
int read_num(STR_INT* num, FILE* f)
{
    char c;
    num->tailLength = 0;
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
    char* data_it = num->head->data; //iterator
    while (is_digit_convert(&c,&num->base))
    {
        //if we reach the end of the data array:
        if (data_it == part_it->data+num->partSz) //last element is data+num->partSZ-1, so this is the overflow
        {
            if (new_si_part(num)){
                fprintf(stderr, "new part creation failed!\n");
                return 1;
            }
            part_it = part_it->next;//created new part is already next in the linked list
            data_it = part_it->data;
            num->tailLength = 0;//we start from 0, the previous parts are all full
        }
        *data_it++ = c;
        num->tailLength++;
        c = getc(f);

    }
    if (data_it < part_it->data+num->partSz) *data_it = '\0';//marks the end with $
    num->end = data_it;

    //HERE we mirror the elements of the linked arrays of char, so that every number has its lowest digit on 1
    //VARIABLES:
    int fw_count = 0;
    int bw_count = 0;
    char tmp;
    STR_INT_PART* fw_part = num->head;
    STR_INT_PART* bw_part = num->tail;
    char* fw_data = fw_part->data;
    char* bw_data = bw_part->data+num->tailLength-1;
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
        if (fw_part == bw_part && fw_data >= bw_data) 
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
        for (char* bw_data_it = (bw_part_it == num->tail) ? num->end-1 : bw_part_it->data+num->partSz-1; 
        bw_data_it >= bw_part_it->data; bw_data_it--)
        {
            if (bw_part_it == num->head && bw_data_it < bw_part_it->data)
                break;

            if (*bw_data_it < 10)
                putc(*bw_data_it + '0',f);
            else
                putc(*bw_data_it - 10 + 'A',f);
        }
    }
    putc('\n',f);
}

//BACKWARD PRINTING is actually just straightforward printing, because we keep the data backward
void backward_print_str_int(STR_INT* num, FILE* f, int brk, size_t line_len)
{
    int fin = 0;
    for (STR_INT_PART* part_it = num->head; part_it != NULL; part_it = part_it->next)
    {   
        int i = 0;
        if (fin) break;
        for (char* data_it = part_it->data; data_it < part_it->data + num->partSz; data_it++)
        {
            if (part_it == num->tail && data_it == num->end)
            {    
                fin = 1;
                break;
            }

            if (*data_it < 10)
                putc(*data_it + '0',f);
            else
                putc(*data_it - 10 + 'A',f);
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

int str_int_add(STR_INT* a, STR_INT* b, STR_INT* target)
{
    if (a->base != b->base) return 1;
    target->base = a->base;//target needs to be created first with the base specified, but it can be changed
    char ovf = 0;
    STR_INT_PART* a_pt_it = a->head;
    STR_INT_PART* b_pt_it = b->head;
    char* a_it = a->head->data,
          b_it = b->head->data;

    while (a_it != a->end && b_it != b->end)
    {
        if (a_it == a_pt_it->data + a->partSz){}
    }
    return 0;
}