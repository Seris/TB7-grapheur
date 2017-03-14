#include <stdio.h>
#include <stdlib.h>
#include "inc/types.h"
#include "inc/test.h"

/**
 * Workflow :
 * 
 */

int main(int argc, char* argv[]){
    tokenlist_t* test = create_test_list();
    print_test_list(test);

    return EXIT_SUCCESS;
}
