#ifndef STR_NUMBER_H
#define STR_NUMBER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
    size_t length;
    char* value;
} STR_INT;

STR_INT* new_str_int(size_t max_len){
    STR_INT* string = (STR_INT*) malloc(sizeof(STR_INT));
    string->value = (char*) malloc(max_len*sizeof(char));
}

char base(int b)
{
    if (b <= 9){
        return b + '0';
    }
    if (b <= 26){
        return b + 'a';
    }
    return '$';
}

int read(char* num, int base, FILE* f)
{
    int c;
    //we need to allow only numbers < base
    while ((c = getc(f)) < '0' || c > '9')//ignores white spaces in front
    {
        if (c == 'q') return 0;
    }
    int len = 1;
    char* num_it = num;
    *num_it++ = c;
    while ((c = getc(f)) >= '0' && c <= '9')
    {
        *num_it++ = c;
        len++;
    }
    if (num_it < num+100) *num_it = '$';//marks the end with $
    return len;
}

void print_num(char* num, FILE* f)//prints a number string to chosen output
{
    for (char* num_it = num; num_it < num+100 && *num_it != '$'; num_it++)
    {
        fprintf(f,"%c",*num_it);   
    }
    putchar('\n');

}

int mark(char* num){
    char* num_it = num;
    while (*num_it >= '0' && *num_it <= '9') num_it++;
    *num_it = '$';
    return 0;
}

#endif