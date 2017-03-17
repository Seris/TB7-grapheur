#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/types.h"
#include "inc/error.h"

void print_error(char* formula, err_t err){
    if(err.type != NO_ERR){
        printf("Erreur dans l'expression:\nf(x) = %s", formula);

        int spaces;
        if(err.token.position > -1){
            spaces = err.token.position + 7;
        } else {
            spaces = strlen(formula) + 6;
        }

        for(int i = 0; i < spaces; i++) putchar(' ');
        printf("^\n");
        for(int i = 0; i < spaces; i++) putchar(' ');

        switch (err.type) {
            case PAR_F_ATTENDU:
            printf("Une parenthèse fermante est attendue.\n");
            break;

            case EXPR_ATTENDU:
            printf("Une expression est attendue.\n");
            break;

            case MANQ_TOK:
            printf("Un token est manquant.\n");
            break;

            case TOKEN_NON_ATTENDU:
            printf("Le token n'est pas attendu.\n");
            break;

            case MAUV_REEL:
            printf("Le réel est mal formaté.\n");
            break;

            case MAUV_FONC_CONST:
            printf("La fonction ou constante est inconnue.\n");
            break;

            case MAUV_CHAR:
            printf("Le caractère est non attendu.\n");
            break;

            default:
            printf("Erreur indefinie.");
            exit(EXIT_FAILURE);
        }
    }

}
