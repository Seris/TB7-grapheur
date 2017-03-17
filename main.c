#include <stdio.h>
#include <stdlib.h>

#include "inc/interface.h"
#include "inc/syntax-check.h"
#include "inc/test.h"

int main(int argc, char* argv[]){
    // menu(argc, argv);

    tokenarb_t a1, a2, a3;
    a1.token.type = VAR;

    a2.token.type = OPERATEUR;
    a2.token.valeur.operateur = '/';

    a3.token.type = REEL;
    a3.token.valeur.reel = 1;

    a2.gauche = &a3;
    a2.droite = &a1;


    lancerGrapheur(argc, argv, &a2, -5, 5, 0.05);

    return EXIT_SUCCESS;
}
