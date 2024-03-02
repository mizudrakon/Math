#include <stdio.h>
#include <stdlib.h>
#define TEST 
#include "str_number.h"
#ifdef DEFS
//test definitions
#endif

int main(int argc, char** argv)
{

#ifdef TEST
    int b = 16;
    const char base = max_digit(b+1);
    /*the thing is: we usually need the max_digit which is 15, not the actual base which would be 16
     not sure if I should change the code to enter the actual base...*/
    for (int i = 1; i <= 40; i++)
        printf("max digit for %d: %c\n", i, max_digit(i));
    printf("list of all numeric characters for base %c:\n",base);
    for (int i = 1; i <= 140; i++){
        //if (is_digit(i,base))
        //    printf("%c is digit for base %c\n",i,base);
    }
    putchar('\n');
    STR_INT* a = new_str_int('f',5);
    printf("print empty init str_int: ");
    print_str_int(a,stdout);
    printf("enter number: ");
    read_num(a, stdin);
    printf("backward print: ");
    backward_print_str_int(a,stdout,0,0);
    printf("regular print: ");
    print_str_int(a,stdout);
#endif
#ifdef RUN
#endif
}
