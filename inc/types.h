#ifndef GRAPH_TYPES_H
#define GRAPH_TYPES_H

typedef enum {
    REEL,
    OPERATEUR,
    VAR,
    FONCTION,
    PAR_O,
    PAR_F,
    CONSTANTE,
    ERREUR
} toktype_t;

typedef enum {
    COS, SIN, TAN,
    ARCCOS, ARCSIN, ARCTAN,
    COSH, SINH, TANH,
    SQRT,
    EXP, LN, LOG,
    ABS, ENT, DECIMAL,
    FACTORIEL,
} fonct_t;

typedef enum {
    CST_PI, CST_E
} const_t;

typedef enum {
    NO_ERR,
    PAR_NON_FERM,
    PAR_MANQ,
    MAUV_TOK,
    MANQ_TOK,
    MAUV_REEL,
    MAUV_FONC_CONST,
    MAUV_CHAR
} errtype_t;


typedef union {
    double reel;
    char operateur;
    fonct_t fonction;
    const_t constante;
} tokval_t;

typedef struct {
    toktype_t type;
    tokval_t valeur;
    int position;
} token_t;

typedef struct tokenarb_st tokenarb_t;
struct tokenarb_st {
    token_t token;
    tokenarb_t* gauche;
    tokenarb_t* droite;
};

typedef struct tokenlist_st tokenlist_t;
struct tokenlist_st {
    token_t token;
    tokenlist_t* suivant;
};


typedef struct {
    double x;
    double y;
} graphpt_t;


typedef struct {
    errtype_t type;
    token_t token;
} err_t;


#endif // GRAPH_TYPES_H
