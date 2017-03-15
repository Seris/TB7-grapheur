#include <stdio.h>
#include <stdlib.h>

#include "../inc/syntax-check.h"
#include "../inc/types.h"

/**
 * Local Macro Error Management (become we're all lazy)
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


tokenarb_t* parse_leaf_token(tokenlist_t** elem_cur){
    tokenarb_t* leaf = malloc(sizeof(tokenarb_t));
    leaf->token = (*elem_cur)->token;
    node_as_leaf(leaf);

    *elem_cur = (*elem_cur)->suivant;
    return leaf;
}

tokenarb_t* parse_operator(tokenlist_t** elem_cur){
    tokenarb_t* node = malloc(sizeof(tokenarb_t));
    node->token = (*elem_cur)->token;
    node_as_leaf(node);

    *elem_cur = (*elem_cur)->suivant;

    return node;
}

tokenarb_t* parse_function(tokenlist_t** elem_cur, err_t* err){
    tokenlist_t* elem = *elem_cur;
    tokenarb_t* node = malloc(sizeof(tokenarb_t));
    node->token = elem->token;
    node_as_leaf(node);

    elem = elem->suivant;
    if(elem != NULL){
        node->gauche = parse_group_token(&elem, err);
    } else {
        set_error_at_end(err, MANQ_TOK);
    }

    *elem_cur = elem;

    return node;
}

tokenarb_t* parse_parenthesis(tokenlist_t** elem_cur, err_t* err){
    tokenlist_t* elem = *elem_cur;
    tokenarb_t* abr = NULL;
    tokenarb_t* tmp;

    elem = elem->suivant;
    if(elem != NULL){
        abr = parse_group_token(&elem, err);
        if(elem != NULL){
            if(elem->token.type == OPERATEUR){
                tmp = parse_operator(&elem);
                tmp->gauche = abr;
                abr = tmp;

                if(elem != NULL){
                    abr->droite = parse_group_token(&elem, err);

                    if(elem->token.type == PAR_F){
                        elem = elem->suivant;
                    } else {
                        set_error(err, MAUV_TOK, elem);
                    }
                } else {
                    set_error_at_end(err, MANQ_TOK);
                }

            } else if(elem->token.type == PAR_F){
                elem = elem->suivant;
            } else {
                set_error(err, MAUV_TOK, elem);
            }
        } else {
            set_error_at_end(err, MANQ_TOK);
        }
    } else {
        set_error_at_end(err, MANQ_TOK);
    }

    *elem_cur = elem;
    return abr;
}

tokenarb_t* parse_group_token(tokenlist_t** elem_cur, err_t* err){
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
            set_error(err, MAUV_TOK, elem);
        }
    }

    *elem_cur = elem;

    return arb;
}


tokenarb_t* parse_token_list(tokenlist_t* list, err_t* err){
    tokenlist_t* elem = list;
    err->type = NO_ERR;

    tokenarb_t* arb = parse_group_token(&elem, err);
    if(elem != NULL){
        set_error(err, MAUV_TOK, elem);
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
