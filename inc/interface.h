#ifndef GRAPH_INTERFACE_H
#define GRAPH_INTERFACE_H

#include <stdlib.h>
#include "graph.h"
#include "types.h"

graphpt_t nvPoint(float x, float y, char v);

graphpt_t versRepere(graphpt_t p);

graphpt_t versBase(graphpt_t p);

void dessinerPoint(graphpt_t p);

void frappe(int c);

void dessin(void);

void menu(int ac, char *av[]);

void lancerGrapheur(int ac, char *av[]);

void chargerValeurs();

/**
* main
*
* La fonction principale avec deux arguments permettant de r�cup�rer les �l�ments en ligne de commande.
* Dans  cet  exemple  les  fonctions  (dites  callback)  myDraw  et  myKey  sont  install�es  ici  par
* l'appel  InitGraph  en  tant  que fonctions  r�agissantes  aux  �v�nements  de  "re-dessinage"  (pour  myDraw)
* et  aux  �v�nements  d'appui  sur  une  touche  du clavier (myKey).
* @parma ac : nombre de parametres
* @parma av : tableau contenant les parametres
*/
int main(int ac, char *av[]);

#endif // GRAPH_INTERFACE_H
