#ifndef GRAPH_INTERFACE_H
#define GRAPH_INTERFACE_H

#include <stdlib.h>
#include "graph.h"
#include "types.h"

void indiquerPosition(int p);

/// Fonctions graphiques

// génère un point à partir de coordonnées et d'un paramètre validité
graphpt_t nvPoint(float x, float y, char v);

// détermine un nouveau point dans le repère souhaité à partir d'un point du repère de la fenêtre, et des paramètres de mise à l'échelle
graphpt_t versRepere(graphpt_t p);

// réciproque de la fonction versRepere
graphpt_t versBase(graphpt_t p);

//détermine les paramètres de mise à l'échelles adéquats en étudiant les bornes en x et les extrema de la fonction
void adaptationEchelle();

// génère un petit carré centré sur le point
void dessinerPoint(graphpt_t p);

void tracerCourbe();

void tracerDerivee();

/// Fonctions des valeurs

// genere le tableau de points a afficher
void chargerValeurs();

// genere à chaque mouvement de souris un tableau contenant le point de la courbe au niveau du curseur, et le point en x + 0.05
void chargerDerivees();

/// Fonctions de la fenetre

// execute des instructions en fonction de la saisie
void frappe(int c);

void dessin(void);

void sourisOnclick(int x, int y);

/// Fonctions du menu

void effEcr();

void lireFichier(char * adresse);

// menu du programme
void menu(int ac, char *av[]);

// lance le grapheur avec des parmètres définis globalement
void lancerGrapheur(int ac, char *av[], tokenarb_t * a, float debut, float fin, float pas);

// lance la saisie utilisateur
void lancerQuestions(int ac, char *av[]);


#endif // GRAPH_INTERFACE_H
