//
//  main.c
//  cmp_word
//
//  Created by Amin Benarieb on 02/12/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#include <stdio.h>
#include "string.h"

#define MAX_STRING 256

int main(int argc, const char * argv[]) {

    char a[MAX_STRING+1], b[MAX_STRING+1];
    
    printf("Enter first word: ");
    scanf("%s", a);
    printf("Enter second word: ");
    scanf("%s", b);
    printf("\n%d\n\n", strncmp(a, b, MAX_STRING));
    
    return 0;
}
