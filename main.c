#include <stdio.h>
#include <stdlib.h>

#include "inc/interface.h"
#include "inc/syntax-check.h"
#include "inc/test.h"

int main(int argc, char* argv[]){
    // menu(argc, argv);

    tokenlist_t* list = create_test_list();
    print_test_list(list);

    err_t error;
    tokenarb_t* arb = parse_token_list(list, &error);

    if(arb != NULL){
        print_tree(arb);
    } else {
        print_error("fonction de test", error);
    }

    return EXIT_SUCCESS;
}
