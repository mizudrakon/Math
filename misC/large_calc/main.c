#include <stdio.h>
#include <stdlib.h>
#include "str_number.h"
#define TEST 
#ifdef DEFS
//test definitions
#endif

int main(int argc, char** argv)
{

#ifdef TEST
    int b = 16;
    //char base = max_digit(b+1);
    const char base = 16;
    /*the thing is: we usually need the max_digit which is 15, not the actual base which would be 16
     not sure if I should change the code to enter the actual base...*/
    for (int i = 1; i <= 40; i++)
        printf("max digit for %d: %c\n", i, max_digit(i));
    printf("list of all numeric characters for base %c:\n",base);
    for (int i = 1; i <= 140; i++){
        if (is_digit(i,max_digit(base+1)))
            printf("%c is digit for base %c = %d\n",i,max_digit(base+1),b);
    }
    putchar('\n');
    STR_INT* a = new_str_int('g',5);
    printf("print empty init str_int: ");
    print_str_int(a,stdout);
    printf("enter number: ");
    read_num(a, stdin);
    printf("backward print: ");
    backward_print_str_int(a,stdout,0,0);
    printf("regular print: ");
    print_str_int(a,stdout);
#endif
#ifdef TEST2
    //realization: char IS an 8 bit number...
    //C doesn't support overloading
    char a = 3;
    char b = 5;
    if (b < 10)
        b += a;
    printf("%d\n",b);
#endif
}
