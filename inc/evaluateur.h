#ifndef EVALUATEUR_H_INCLUDED
#define EVALUATEUR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "types.h"

int operateur(tokenarb_t* arbre);

int variable(tokenarb_t* arbre);

int constante(tokenarb_t* arbre);

int fonction(tokenarb_t* arbre);

int reel(tokenarb_t* arbre);

float identifier_operateur(tokenarb_t* arbre, float i, char * valide);

float evaluer(tokenarb_t* arbre, float i, char * valide);

float identifier_constante(tokenarb_t* arbre, float i, char * valide);

float factoriel(float a);

float identifier_fonction(tokenarb_t* arbre, float i, char * valide);

void supprime_tbx(graphpt_t * points_tbx);

graphpt_t * generer_points(tokenarb_t * arbre, float xmin, float xmax, float pas);

#endif // EVALUATEUR_H_INCLUDED
