#include <stdio.h>
#include <stdlib.h>
#define RUN
#include "str_number.h"
#ifdef DEFS
//reads a number as a field from input
//we work with strings of 100 characters
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

int main(int argc, char** argv)
{

#ifdef TEST
    for (int i = 0; i <= 120; i++)
        printf("%c = %d, ",i, base_check(i,'f'));
    putchar('\n');
#endif
#ifdef RUN
    int q_num = 0;
    const int base = 10, size = 100;
    //char num[2][100];
    STR_INT** num_list;
    //char** num;
    if (q_num + argc-1 < 2)
    {
        num_list = (STR_INT**) malloc((2-argc+1)*sizeof(STR_INT*));
    }
    while (q_num + argc-1 < 2)
    {
        printf("please provide a number: ");
        *(num_list+q_num) = new_str_int(base,size,stdin);
        //*(num_list+q_num) = (char*) malloc(100*sizeof(char));
        //read(*(num+q_num),10,stdin);
        printf("number read: ");
        print_str_int(*(num_list+q_num),stdout);
        q_num++;
    }

    char* a;
    char* b;
    printf("Argument read check:\n");
    if (argc == 1){
        printf("no args provided on call, two prompted:\n");
        a = (*num_list)->value;
        b = (*(num_list+1))->value;
    }
    else if (argc == 2){
        printf("one arg provided on call, second number prompted:\n");
        a = *(argv+1);
        mark(a,base);
        b = (*num_list)->value;
    }
    else {
        printf("Both args provided on call:\n");
        a = *(argv+1);
        mark(a,base);
        b = *(argv+2);
        mark(b,base);
    }
    print_num(a,stdout);
    print_num(b,stdout);
    
    for (STR_INT** n = num_list; n < num_list+q_num; n++){
        free((void*) *n);
        //printf("freed a string\n");
    }
    free((void*) num_list);
    //printf("freed num\n");
#endif
}
