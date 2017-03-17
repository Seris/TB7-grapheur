#include <stdio.h>
#include <stdlib.h>

#include "inc/interface.h"
#include "inc/syntax-check.h"
#include "inc/test.h"

int main(int argc, char* argv[]){
    // menu(argc, argv);

    tokenarb_t a;
    a.token.type = VAR;

    lancerGrapheur(argc, argv, &a, -5, 5, 0.05);

    return EXIT_SUCCESS;
}
