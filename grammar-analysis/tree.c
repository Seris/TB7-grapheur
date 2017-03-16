#include <stdlib.h>

#include "../inc/syntax-check.h"
#include "../inc/types.h"

void free_arbre_token(tokenarb_t* abr){
    if(abr != NULL){
        free_arbre_token(abr->gauche);
        free_arbre_token(abr->droite);
        free(abr);
    }
}
