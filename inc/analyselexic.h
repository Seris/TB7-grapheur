#ifndef GRAPH_ANALYSE_LEXICAL
#define GRAPH_ANALYSE_LEXICAL
#include "../inc/types.h"
/**
 * @brief Creation d'une liste vide devant contenir les jetons

 * @return Une liste vide
 *
 * @pre De l'espace m�moire disponible
 * @post Une bibliotheque vide
 * @invariant rien
 */
tokenlist_t* initialisation();
/**
 * @brief Destruction d'une liste
 * @return NULL
 * @param token_p la liste � d�truire
 * @pre la liste existe
 * @post La liste n'existe plus
 * @invariant rien
 */
tokenlist_t* detruire_liste(tokenlist_t *token_p);

/**
 * @brief Ajoute un jeton correspondant � variable dans la liste
 * @param token_p le pointeur de la liste
 * @param position la position du jeton dans la formule
 * @return La litse avec le jeton en plus
 *
 * @pre La liste a bien �t� initialis�e
 * @post La liste contient un jeton suppl�mentaire
 * @invariant les autres jetons
 */
tokenlist_t*ajout_variable(tokenlist_t*token_p,int position);
/**
 * @brief Ajoute un jeton correspondant � une parenth�se ouvrante dans la liste
 * @param token_p le pointeur de la liste
 * @param position la position du jeton dans la formule
 * @return La litse avec le jeton en plus
 *
 * @pre La liste a bien �t� initialis�e
 * @post La liste contient un jeton suppl�mentaire
 * @invariant les autres jetons
 */


tokenlist_t*ajout_parenthese_ouvrante(tokenlist_t*token_p,int position);
/**
 * @brief Ajoute un jeton correspondant � une parenth�se fermante dans la liste
 * @param token_p le pointeur de la liste
 * @param position la position du jeton dans la formule
 * @return La litse avec le jeton en plus
 *
 * @pre La liste a bien �t� initialis�e
 * @post La liste contient un jeton suppl�mentaire
 * @invariant les autres jetons
 */


tokenlist_t*ajout_parenthese_fermante(tokenlist_t*token_p,int position);
/**
 * @brief Ajoute un jeton correspondant � un op�rateur dans la liste
 * @param token_p le pointeur de la liste
 * @param operateur l'operateur � ajouter
 * @param position la position du jeton dans la formule
 * @return La litse avec le jeton en plus
 *
 * @pre La liste a bien �t� initialis�e
 * @post La liste contient un jeton suppl�mentaire
 * @invariant les autres jetons
 */
 tokenlist_t*ajout_operateur(tokenlist_t*token_p,char operateur,int position);
 /**
 * @brief Ajoute un jeton correspondant � une fonction dans la liste ou une erreur si la fonction introduite ne correspond pas
 * @param token_p le pointeur de la liste
 * @param fonction la fonction � ajouter
 * @param comparaison la base de donn�e des fonctions pour la comparaison
 * @param constante  la base de donn�e des constantes pour la comparaison
 * @param position la position du jeton dans la formule
 * @param erreur s'il y a une erreur, une cellule d'erreur est remplie � la place.
 * @return La litse avec le jeton en plus
 *
 * @pre La liste a bien �t� initialis�e
 * @post La liste contient un jeton suppl�mentaire
 * @invariant les autres jetons
 */
tokenlist_t*ajout_fonction_constante(tokenlist_t*token_p,char * fonction,char comparaison[17][20],char constante [2][20],int position,err_t*erreur);
 /**
 * @brief R�cup�re une fonction � partir de la chaine de caract�re
 * @param arecup la partie de la formule qui contient la fonction
 * @param i la position du pointeur pour la d�caler pour la suite de lecture
 * @param comparaison la base de donn�e des fonctions pour la comparaison
 * @return La chaine contenant la fonction
 * @pre La chaine existe bien
 * @post la nouvelle chaine ne contient pas d'autres caract�res que des lettres
 * @invariant la chaine principale
 */
char * fonction_recuperation(char * arecup,int *i,char fonction_recupi[20] );
/**
 * @brief Transforme une cha�ne de caract�res en liste cha�n�e contenant chaque �l�ment de fonction dans un token
 * @param tableau la cha�ne de caract�re
 * @param erreur un pointeur de cellule pour enregistrer les erreurs
 * @return La litse cha�n�e avce tous les jetons
 *
 * @pre La cha�ne existe bien
 * @post La liste a bien �t� cr��e
 * @invariant la cha�ne n'est pas modifi�e
 */

tokenlist_t* analyselexical(char*tableau,err_t*erreur);

#endif // GRAPH_ANALYSE_LEXICAL
