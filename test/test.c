#include <stdio.h>
#include <stdlib.h>

#include "../inc/test.h"
#include "../inc/types.h"

tokenlist_t* nvreel(float reel, tokenlist_t* suiv){
    tokenlist_t* t = malloc(sizeof(tokenlist_t));
    t->token.type = REEL;
    t->token.valeur.reel = reel;
    t->suivant = suiv;
    return t;
}

tokenlist_t* nvop(char op, tokenlist_t* suiv){
    tokenlist_t* t = malloc(sizeof(tokenlist_t));
    t->token.type = OPERATEUR;
    t->token.valeur.operateur = op;
    t->suivant = suiv;
    return t;
}

tokenlist_t* nvparo(tokenlist_t* suiv){
    tokenlist_t* t = malloc(sizeof(tokenlist_t));
    t->token.type = PAR_O;
    t->suivant = suiv;
    return t;
}

tokenlist_t* nvparf(tokenlist_t* suiv){
    tokenlist_t* t = malloc(sizeof(tokenlist_t));
    t->token.type = PAR_F;
    t->suivant = suiv;
    return t;
}

tokenlist_t* nvvar(tokenlist_t* suiv){
    tokenlist_t* t = malloc(sizeof(tokenlist_t));
    t->token.type = VAR;
    t->suivant = suiv;
    return t;
}

tokenlist_t* nvfn(fonct_t fonction, tokenlist_t* suiv){
    tokenlist_t* t = malloc(sizeof(tokenlist_t));
    t->token.type = FONCTION;
    t->token.valeur.fonction = fonction;
    t->suivant = suiv;
    return t;
}

// ( 4 * ( x + 3 ) ) + sin(4 + x)
tokenlist_t* create_test_list(){
    tokenlist_t* root =
        nvparo(nvreel(4,nvop('*',nvparo(nvvar(nvop('+',nvreel(3,nvparf(
        nvparf(nvop('+',nvfn(SIN,nvparo(nvreel(4,nvop('+',nvvar(nvparf(
        NULL))))))))))))))));

    return root;
}

void print_test_list(tokenlist_t* list){
    if(list != NULL){
        switch(list->token.type){
            case REEL:
            printf("%.2f", list->token.valeur.reel);
            break;

            case OPERATEUR:
            printf(" %c ", list->token.valeur.operateur);
            break;

            case VAR:
            printf("x");
            break;

            case FONCTION:
            printf("fonction%d", list->token.valeur.fonction);
            break;

            case PAR_O:
            printf("(");
            break;

            case PAR_F:
            printf(")");
            break;

            case CONSTANTE:
            printf("a");
            break;
        }

        print_test_list(list->suivant);
    } else {
        printf("\n");
    }
}
