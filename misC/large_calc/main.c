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
    char base = 'f';
    for (int i = 1; i <= 120; i++){
        if (is_digit(i,base))
            printf("%c is digit for base %c\n",i,base);
    }
    putchar('\n');
    STR_INT* a = new_str_int('f',5);
    printf("%c\n", a->head->data[0]);
    print_str_int(a,stdout);
    //read_num(a, stdin);
#endif
#ifdef RUN
#endif
}
