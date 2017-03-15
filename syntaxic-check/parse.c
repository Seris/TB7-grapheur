#include <stdio.h>
#include <stdlib.h>

#include "../inc/syntax-check.h"
#include "../inc/types.h"

/**
 * Local Macro for Error Management (because we're all lazy)
 */
#define set_error(err, errtype, elem) \
    err->type = errtype; \
    err->token = elem->token;

#define set_error_at_end(err, errtype) \
    err->type = errtype; \
    err->token.type = ERREUR; \
    err->token.position = -1;

#define node_as_leaf(node) \
    node->gauche = NULL; \
    node->droite = NULL;

/**
 * Traite un token qui sera toujours une feuille (CONSTANTE, VARIABLE, REEL)
 * @param  elem_cur le curseur dans la liste chainée qui sera incrémenté
 *                  par la fonction
 * @return          l'élement de l'arbre correspondant au token
 */
tokenarb_t* parse_leaf_token(tokenlist_t** elem_cur){
    tokenarb_t* leaf = malloc(sizeof(tokenarb_t));
    leaf->token = (*elem_cur)->token;
    node_as_leaf(leaf);

    *elem_cur = (*elem_cur)->suivant;
    return leaf;
}

/**
 * Traite un opérateur
 * @param  elem_cur curseur de la liste chainée qui sera incrémenté par la fonction
 * @return          l'élement de l'arbre correspondant à l'opérateur
 */
tokenarb_t* parse_operator(tokenlist_t** elem_cur){
    tokenarb_t* node = malloc(sizeof(tokenarb_t));
    node->token = (*elem_cur)->token;
    node_as_leaf(node);

    *elem_cur = (*elem_cur)->suivant;

    return node;
}

/**
 * Traite une fonction et s'occupe de traiter le token suivant qui lui est lié
 * @param  elem_cur curseur dans la liste qui sera incrémentée par la fonction
 * @param  err      MANQ_TOK: aucun token ne suit la fonction
 *                  e.g f(x) = log
 * @return          [description]
 */
tokenarb_t* parse_function(tokenlist_t** elem_cur, err_t* err){
    tokenlist_t* elem = *elem_cur;
    tokenarb_t* node = malloc(sizeof(tokenarb_t));
    node->token = elem->token;
    node_as_leaf(node);

    elem = elem->suivant;
    if(elem != NULL){
        node->gauche = parse_expression(&elem, err);
    } else {
        set_error_at_end(err, MANQ_TOK);
    }

    *elem_cur = elem;

    return node;
}

/**
 * Traite une parenthèse ouvrante. S'assure qu'elle sera bien fermée et si elle
 * contient soit une "feuille" ou un opérateur entouré de 2 "feuilles"
 * (ici feuille désigne des tokens de type CONSTANTE, VAR ou REEL)
 * @param  elem_cur Curseur dans la liste chainée qui sera incrémentée par la
 *                  fonction
 * @param  err      TOKEN_NON_ATTENDU: e.g ( 5 * ), ( ), ( 5 * 4 + ), ( 5 * 4 + 3 etc..
 *                  MANQ_TOK: parenthèse fermante attendue mais un autre token trouvé
 * @return          [description]
 */
tokenarb_t* parse_parenthesis(tokenlist_t** elem_cur, err_t* err){
    tokenlist_t* elem = *elem_cur;
    tokenarb_t* abr = NULL;
    tokenarb_t* tmp;

    elem = elem->suivant;

    abr = parse_expression(&elem, err);
    if(err->type == NO_ERR && elem != NULL){
        if(elem->token.type == OPERATEUR){
            tmp = parse_operator(&elem);
            tmp->gauche = abr;
            abr = tmp;

            abr->droite = parse_expression(&elem, err);
            if(err->type == NO_ERR){
                if(elem == NULL){
                    set_error_at_end(err, PAR_F_ATTENDU);
                } else if(elem->token.type != PAR_F){
                    set_error(err, PAR_F_ATTENDU, elem);
                } else {
                    elem = elem->suivant;
                }
            }
        } else if(elem->token.type == PAR_F){
            elem = elem->suivant;
        } else {
            set_error(err, TOKEN_NON_ATTENDU, elem);
        }
    } else if(err->type == NO_ERR) {
        set_error_at_end(err, MANQ_TOK);
    }

    *elem_cur = elem;
    return abr;
}

/**
 * [parse_expression description]
 * @param  elem_cur [description]
 * @param  err      [description]
 * @return          [description]
 */
tokenarb_t* parse_expression(tokenlist_t** elem_cur, err_t* err){
    tokenlist_t* elem = *elem_cur;
    tokenarb_t* arb = NULL;

    if(elem != NULL){
        switch(elem->token.type){
            case FONCTION:
            arb = parse_function(&elem, err);
            break;

            case PAR_O:
            arb = parse_parenthesis(&elem, err);
            break;

            case REEL:
            case CONSTANTE:
            case VAR:
            arb = parse_leaf_token(&elem);
            break;

            default:
            set_error(err, EXPR_ATTENDU, elem);
        }
    }

    *elem_cur = elem;

    return arb;
}

/**
 * [parse_token_list description]
 * @param  list [description]
 * @param  err  [description]
 * @return      [description]
 */
tokenarb_t* parse_token_list(tokenlist_t* list, err_t* err){
    tokenlist_t* elem = list;
    err->type = NO_ERR;

    tokenarb_t* arb = parse_expression(&elem, err);
    if(elem != NULL && err->type == NO_ERR){
        set_error(err, TOKEN_NON_ATTENDU, elem);
    }

    if(err->type != NO_ERR){
        free_arbre_token(arb);
        arb = NULL;
    }

    return arb;
}

#undef set_error_at_end
#undef set_error
#undef node_as_leaf
