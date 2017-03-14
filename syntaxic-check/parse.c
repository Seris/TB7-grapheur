#include <stdio.h>
#include <stdlib.h>

#include "../inc/parse.h"
#include "../inc/types.h"

/**
 * Local Macro Error Management (become we're all lazy)
 */
#define set_error(error, errtype, elem) \
    error->type = errtype; \
    error->token = elem->token;

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
    node->gauche = parse_group_token(&elem, err);

    *elem_cur = elem;

    return node;
}

tokenarb_t* parse_parenthesis(tokenlist_t** elem_cur, err_t* err){
    tokenlist_t* elem = *elem_cur;
    tokenarb_t* abr = NULL;
    tokenarb_t* tmp;

    if(elem->suivant != NULL){
        elem = elem->suivant;

        if(elem->token.type != PAR_F){
            abr = parse_group_token(&elem, err);

            if(elem != NULL){
                if(elem->token.type == OPERATEUR){
                    tmp = parse_operator(&elem);
                    tmp->gauche = abr;
                    abr = tmp;

                    if(elem != NULL){
                        abr->droite = parse_group_token(&elem, err);

                        if(elem != NULL){
                            if(elem->token.type == PAR_F){
                                elem = elem->suivant;
                            } else {
                                set_error(err, MAUV_TOK, elem);
                            }
                        } else {
                            set_error(err, MANQ_TOK, elem);
                        }

                    } else {
                        set_error(err, MANQ_TOK, elem);
                    }

                } else if(elem->token.type == PAR_F){
                    elem = elem->suivant;
                }

            } else {
                set_error(err, MANQ_TOK, elem);
            }

        } else {
            elem = elem->suivant;
        }
    } else {
        set_error(err, MANQ_TOK, elem);
        elem = elem->suivant;
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

#undef node_as_leaf
#undef set_error
