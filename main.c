#include <stdio.h>
#include <stdlib.h>
#include "inc/syntax-check.h"
#include "inc/types.h"
#include "inc/test.h"

int main(int argc, char* argv[]){
    tokenlist_t* test = create_test_list();
    print_test_list(test);

    err_t error;

    tokenlist_t* cursor = test;
    tokenarb_t* abr = parse_token_list(cursor, &error);
    print_error(&error);
    if(error.type == NO_ERR){
        print_tree(abr);
    }

    return EXIT_SUCCESS;
}
