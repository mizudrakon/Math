#include <stdio.h>
#include <stdlib.h>
//#include "long_number.h"
//#define DYN 
//reads the number as a field - ok, this should probably be a seperate thing - read, operations...
int read(char* num, FILE* f)
{
    int c;
    while ((c = getc(f)) < '0' || c > '9')
    {
        if (c == 'q') return 1;
    }
    char* num_it = num;
    *num_it++ = c;
    while ((c = getc(f)) >= '0' && c <= '9')
    {
        *num_it++ = c;
    }
    if (num_it < num+100) *num_it = '$';
    return 0;
}

void print_num(char* num, FILE* f)
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

int main(int argc, char** argv)
{
    int q_num = 0;
    //char num[2][100];
    char** num;
    if (q_num + argc-1 < 2)
    {
        num = (char**) malloc((2-argc+1)*sizeof(char*));
    }
    while (q_num + argc-1 < 2)
    {
        printf("please provide a number: ");
        *(num+q_num) = (char*) malloc(100*sizeof(char));
        read(*(num+q_num),stdin);
        print_num(*(num+q_num),stdout);
        q_num++;
    }

    char* a;
    char* b;
    printf("Argument read check:\n");
    if (argc == 1){
        printf("no args provided on call, two prompted:\n");
        a = *num;
        b = *(num+1);
    }
    else if (argc == 2){
        printf("one arg provided on call, second number prompted:\n");
        a = *(argv+1);
        mark(a);
        b = *num;
    }
    else {
        printf("Both args provided on call:\n");
        a = *(argv+1);
        mark(a);
        b = *(argv+2);
        mark(b);
    }
    print_num(a,stdout);
    print_num(b,stdout);
    
    for (char** n = num; n < num+q_num; n++){
        free((void*) n);
        //for (char* p_n = *n; p_n < *n+100; p_n++)
        //    free((void*)p_n);
        printf("freed a string\n");
        //free((void*) n);
    }
    free((void*) num);
    printf("freed num\n");
    
}
