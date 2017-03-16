#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/test.h"
#include "../inc/types.h"

void print_token(token_t token){
    switch(token.type){
        case REEL:
        printf("%d ", (int) token.valeur.reel);
        break;

        case OPERATEUR:
        printf("%c ", token.valeur.operateur);
        break;

        case VAR:
        printf("x ");
        break;

        case FONCTION:
        printf("f ");
        break;

        case CONSTANTE:
        printf("a ");
        break;

        case PAR_O:
        printf("( ");
        break;

        case PAR_F:
        printf(") ");
        break;

        default:
        printf(" |?????| ");
    }
}

void print_node(tokenarb_t* arb, int level){
    printf("%.2d: ", level);
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

void print_error(char* formula, err_t err){
    if(err.type != NO_ERR){
        printf("Erreur dans l'expression:\nf(x) = %s", formula);

        int spaces;
        if(err.token.position > -1){
            spaces = err.token.position + 6;
        } else {
            spaces = strlen(formula) + 6;
        }
        for(int i = 0; i < spaces; i++) putchar(' ');
        printf("^\n");
        for(int i = 0; i < spaces; i++) putchar(' ');

        switch (err.type) {
            case PAR_F_ATTENDU:
            printf("Parenthèse fermante attendue\n");
            break;

            case EXPR_ATTENDU:
            printf("Expression attendue\n");
            break;

            case MANQ_TOK:
            printf("Token manquant\n");
            break;

            case TOKEN_NON_ATTENDU:
            printf("Token non attendu\n");
            break;

            case MAUV_REEL:
            printf("Réel mal formatté\n");
            break;

            case MAUV_FONC_CONST:
            printf("Fonction ou constante inconnue\n");
            break;

            case MAUV_CHAR:
            printf("Caractère non attendu\n");
            break;

            default:
            printf("WTFFFFF");
            exit(EXIT_FAILURE);
        }
    }

}


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

tokenlist_t* nvcst(const_t cst, tokenlist_t* suiv){
    tokenlist_t* t = malloc(sizeof(tokenlist_t));
    t->token.type = CONSTANTE;
    t->token.valeur.constante = cst;
    t->suivant = suiv;
    return t;
}

void print_test_list(tokenlist_t* list){
    if(list != NULL){
        print_token(list->token);
        print_test_list(list->suivant);
    } else {
        printf("\n");
    }
}

//sqrt(sin(4a))
tokenlist_t* create_test_list(){
    tokenlist_t* root =
        nvfn(SIN,nvparo(nvreel(1, nvop('/', nvvar(nvparf(NULL))))));

    tokenlist_t* curlist = root;
    for (size_t i = 0; curlist != NULL; i++, curlist = curlist->suivant) {
        curlist->token.position = i;
    }

    return root;
}
