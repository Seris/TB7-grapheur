#include "../inc/evaluateur.h"

int operateur(tokenarb_t* arbre){
    if (arbre->token.type == OPERATEUR)
        return 1;
    return 0;
}

int variable(tokenarb_t* arbre){
    if (arbre->token.type == VAR)
        return 1;
    return 0;
}

int constante(tokenarb_t* arbre){
    if (arbre->token.type == CONSTANTE)
        return 1;
    return 0;
}

int reel(tokenarb_t* arbre){
    if (arbre->token.type == REEL)
        return 1;
    return 0;
}

int fonction(tokenarb_t* arbre){
    if (arbre->token.type == FONCTION)
        return 1;
    return 0;
}

float identifier_operateur(tokenarb_t* arbre, float i, char * valide){
    float gauche = evaluer(arbre->gauche, i, valide);
    float droit = evaluer(arbre->droite, i, valide);
    switch(arbre->token.valeur.operateur){
    case '+':
        return gauche + droit;
        break;
    case '*':
        return gauche * droit;
        break;
    case '/': // le d�nominateur ne doit pas �tre nul
        if (fabsf(droit) > (pow(10, -4)))
            return gauche / droit;
        *valide = 0;
        return 0;
        break;
    case '-':
        return gauche - droit;
        break;
    case '^': // un nombre n�gatif ne peut pas �tre �lev� � une puissance d�cimale
        if (!((gauche < 0) && (droit != floor(droit))) && !((fabsf(gauche) < (pow(10, -4))) && (droit <= 0)))
            return powf(evaluer(arbre->gauche, i, valide), evaluer(arbre->droite, i, valide));
        *valide = 0;
        return 0;
        break;
    case '%':
        return fmodf(evaluer(arbre->gauche, i, valide), evaluer(arbre->droite, i, valide));
        break;
    default:
        *valide = 0;
        return 0;
        break;
    }
}

float evaluer(tokenarb_t* arbre, float i, char * valide){
    if(reel(arbre))
        return arbre->token.valeur.reel;
    else if(variable(arbre))
        return i;
    else if(operateur(arbre))
    {
        return identifier_operateur(arbre, i, valide);
    }

    else if(constante(arbre))
    {
        return identifier_constante(arbre, i, valide);
    }

    else if(fonction(arbre))
    {
        return identifier_fonction(arbre, i, valide);
    }
    else
    {
        *valide = 0;
        return 0;
    }
}



float identifier_constante(tokenarb_t* arbre, float i, char * valide){
    switch(arbre->token.valeur.constante){ // on utilise les constantes de la librairie math
    case CST_PI:
        return M_PI;
        break;
    case CST_E:
        return M_E;
        break;
    default:
        *valide = 0;
        return 0;
        break;
    }
}

float factoriel(float a){
    if(a > 0)
        return a*factoriel(a-1);
    return 1;
}

float identifier_fonction(tokenarb_t* arbre, float i, char * valide){
    float d = evaluer(arbre->gauche, i, valide);
    switch(arbre->token.valeur.fonction){
    case COS:
        return cosf(d);
        break;
    case SIN:
        return sinf(d);
        break;
    case TAN:
        return tanf(d);
        break;
    case SQRT: // le contenu de la racine doit �tre positif
        if(d>=0)
            return sqrtf(d);
        *valide = 0;
        return 0;
        break;
    case ARCCOS: // le cosinus inverse a [-1;1] pour ensemble de d�finition
        if((d>=(-1)) && (d<=1))
            return acosf(d);
        *valide = 0;
        return 0;
        break;
    case ARCSIN:  // le sinus inverse a [-1;1] pour ensemble de d�finition
        if((d>=(-1)) && (d<=1))
            return asinf(d);
        *valide = 0;
        return 0;
        break;
    case ARCTAN:
        return atanf(d);
        break;
    case COSH:
        return coshf(d);
        break;
    case SINH:
        return sinhf(d);
        break;
    case TANH:
        return tanhf(d);
        break;
    case FACTORIEL: // la factorielle d'un nombre d�cimal ou n�gatif n'existe pas
        if((fabsf(d-(int)d) < (pow(10, -4))) && (d>=0))
            return factoriel(d);
        *valide = 0;
        return 0;
        break;
    case EXP:
        return expf(d);
        break;
    case LN: // la fonction ln est d�finie � partir de 0
        if(d > 0)
            return logf(d);
        *valide = 0;
        return 0;
        break;
    case LOG: // la fonction log est d�finie � partir de 0
        if(d > 0)
            return log10f(d);
        *valide = 0;
        return 0;
        break;
    case ABS:
        if(d >= 0)
            return d;
        return -d;
        break;
    case ENT:
        return floor(d);
        break;
    case DECIMAL:
        return d-((float)((int)d));
        break;
    default:
        *valide = 0;
        return 0;
        break;
    }
}

void supprime_tbx(graphpt_t * points_tbx){
    if(points_tbx != NULL)
        free(points_tbx);
}

graphpt_t * generer_points(tokenarb_t * arbre, float xmin, float xmax, float pas){

    int nbpoints;

    //calcul du nombre de points � renvoyer
    if ((((xmax - xmin) / pas)-((int)((xmax - xmin) / pas))) != 0)
    {
        // le dernier point se trouve avant la valeur max.
        // on en ajoute un.
        nbpoints = (int)((xmax - xmin)/pas) +2;
    }
    else
    {
        // le dernier point est la valeur max.
        nbpoints = (int)((xmax - xmin)/pas) +1;
    }

    graphpt_t * points_tbx = (graphpt_t *)malloc(nbpoints*sizeof(graphpt_t));
    if(points_tbx == NULL)
        printf("OMG ARRETEZ TOUT LE MALLOC A FAIL.");
    graphpt_t point;
    char * valide = &point.valide;

    float i;
    int ind=0;
    for(i = xmin; i < xmax; i += pas) // calcul des ordonn�es avant la fin de la fenetre
    {
        point.valide = 1;
        point.x = i;
        point.y = evaluer(arbre, i, valide);
        points_tbx[ind] = point;
        ind++;
    }
    point.valide = 1; //calcul du dernier point de la fenetre ne respectant pas le pas
    point.x = xmax;
    point.y = evaluer(arbre, xmax, valide);
    points_tbx[nbpoints-1] = point;

    return points_tbx;
}
