#include <stdio.h>
#include <stdlib.h>
#include "inc/syntax-check.h"
#include "inc/types.h"
#include "inc/test.h"

void print_node(tokenarb_t* arb, int level){
    switch (arb->token.type) {
        case REEL:
        printf("%.2d: REEL(%.2f)\n", level, arb->token.valeur.reel);
        break;

        case OPERATEUR:
        printf("%.2d: OP(%c)\n", level, arb->token.valeur.operateur);
        break;

        case VAR:
        printf("%.2d: VAR\n", level);
        break;

        case FONCTION:
        printf("%.2d: FONCTION\n", level);
        break;

        case CONSTANTE:
        printf("%.2d: CONSTANTE\n", level);
        break;

        default:
        printf("%.2d: ??????\n", level);
    }
}

void print_tree_inc(tokenarb_t* arb, int level){
    if(arb->gauche != NULL){
        printf("G");
        print_node(arb->gauche, level+1);
        print_tree_inc(arb->gauche, level+1);
    }

    if(arb->droite != NULL){
        printf("D");
        print_node(arb->droite, level+1);
        print_tree_inc(arb->droite, level+1);
    }
}

void print_tree(tokenarb_t* arb){
    if(arb != NULL){
        printf("R");
        print_node(arb, 0);
        print_tree_inc(arb, 0);
    }
}

void print_error(err_t* err){
    switch (err->type) {
        case NO_ERR:
        printf("Aucune erreur");
        break;

        case PAR_NON_FERM:
        printf("Parenthèse non fermée");
        break;

        case PAR_MANQ:
        printf("Parenthèse manquante");
        break;

        case MANQ_TOK:
        printf("Token manquant");
        break;

        case MAUV_TOK:
        printf("Mauvais token");
        break;

        default:
        printf("????");
    }

    if(err->type != NO_ERR){
        printf(" (pos: %d)", err->token.position);
    }

    printf("\n");
}

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
