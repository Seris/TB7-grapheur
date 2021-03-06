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
    FACTORIEL, GAMMA
} fonct_t;

typedef enum {
    CST_PI, CST_E
} const_t;

typedef enum {
    NO_ERR,

    // Analyse lexicale
    MAUV_REEL,
    MAUV_FONC_CONST,
    MAUV_CHAR,

    // Analyse syntaxique
    PAR_F_ATTENDU,
    EXPR_ATTENDU,
    TOKEN_NON_ATTENDU,
    MANQ_TOK,
} errtype_t;


typedef union {
    float reel;
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
    float x;
    float y;
    char valide;
} graphpt_t;


typedef struct {
    errtype_t type;
    token_t token;
} err_t;


#endif // GRAPH_TYPES_H
