#ifndef STR_NUMBER_H
#define STR_NUMBER_H

#include <stdio.h>
#include <stdlib.h>

//let's have us a structure with all info coupled in
typedef struct 
{
    size_t length;
    char base;
    char* value;
} STR_INT;

char max_digit(size_t b);//given a base b, get the char representing the maximum single digit
int is_digit(char c, char b);
size_t read_num(char* num, char base, FILE* f);//read from input
void print_num(char* num, FILE* f);
void print_str_int(STR_INT* num, FILE* f);
int mark(char* num, char base);//mark the end of the number with \0

STR_INT* new_str_int(size_t base, size_t max_len, FILE* f)
{
    STR_INT* strnum = (STR_INT*) malloc(sizeof(STR_INT));
    strnum->value = (char*) malloc(max_len*sizeof(char));
    strnum->base = max_digit(base);
    strnum->length = read_num(strnum->value, strnum->base, f);
    return strnum;
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

int is_digit(char c, char b){
    if (c >= 'A' && c <= 'Z')
    {
        c = 'a' + (c - 'A');
    }

    if (b >= 'a' && b <= 'z'){
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= b);
    }
    return c >= '0' && c <= '9';
}

size_t read_num(char* num, char base, FILE* f)
{
    int c;
    int len = 0;
    //we need to allow only numbers < base
    //ignore white spaces or any possibly separating symbols in front
    c = getc(f);
    while (is_digit(c, base) == 0)
    {
        if (c == '$') return 0;
        c = getc(f);
    }
    char* num_it = num; //iterator
    while (is_digit(c,base))
    {
        *num_it++ = c;
        len++;
        c = getc(f);
    }
    if (num_it < num+100) *num_it = '\0';//marks the end with $
    return len;
}

void print_num(char* num, FILE* f)//prints a number string to chosen output
{
    for (char* num_it = num; num_it < num+100 && *num_it != '\0'; num_it++)
    {
        fprintf(f,"%c",*num_it);   
    }
    putchar('\n');
}

void print_str_int(STR_INT* num, FILE* f)
{
    print_num(num->value, f);
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