#ifndef GRAPH_PARSE_H
#define GRAPH_PARSE_H

#include "types.h"

tokenarb_t* parse_leaf_token(tokenlist_t** elem_cur);
tokenarb_t* parse_operator(tokenlist_t** elem_cur);
tokenarb_t* parse_function(tokenlist_t** elem_cur, err_t* err);
tokenarb_t* parse_parenthesis(tokenlist_t** elem_cur, err_t* err);
tokenarb_t* parse_group_token(tokenlist_t** elem_cur, err_t* err);

tokenarb_t* parse_token_list(tokenlist_t* list, err_t* err);
void free_arbre_token(tokenarb_t* abr);

#endif // GRAPH_PARSE_H
