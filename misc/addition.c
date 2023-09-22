#include <stdio.h>
#include <stdlib.h>
//#define DYN 
//reads the number as a field - ok, this should probably be a seperate thing - read, operations...
int read(char* num)
{
    int c;
    while ((c = getchar()) < '0' || c > '9')
    {
        if (c == 'q') return 1;
    }
    char* num_it = num;
    *num_it++ = c;
    while ((c = getchar()) >= '0' && c <= '9')
    {
        *num_it++ = c;
    }
    if (num_it < num+100) *num_it = '$';
    return 0;
}

void print_num(char* num)
{
    for (char* num_it = num; num_it < num+100 && *num_it != '$'; num_it++)
    {
        printf("%c",*num_it);   
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
    int q_num = argc-1;
    //char num[2][100];
    char** num;
    if (q_num < 2)
    {
        num = (char**) malloc((2-q_num)*sizeof(char*));
    }
    while (q_num < 2)
    {
        printf("please provide a number: ");
        *(num+q_num) = (char*) malloc(100*sizeof(char));
        read(*(num+q_num));
        print_num(*(num+q_num));
        q_num++;
    }

    char* a;
    char* b;
    if (argc == 1){
        printf("no args provided\n");
        a = *num;
        b = *(num+1);
    }
    else if (argc == 2){
        printf("one arg provided\n");
        a = *(argv+1);
        mark(a);
        b = *(num+1);
    }
    else {
        printf("two or more args provided\n");
        a = *(argv+1);
        mark(a);
        b = *(argv+2);
        mark(b);
    }
    print_num(a);
    print_num(b);
}
