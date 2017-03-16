#ifndef ANALYSELEXIC_H_INCLUDED
#define ANALYSELEXIC_H_INCLUDED

#include "types.h"
#define nombre_fonction 17
#define nombre_constante 2
#define code_erreur 666
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
 * @brief V�rifie le dernier caract�re pris en charge pas strtof
 * @param lastcharacter le dernier character pris par strtof
 * @return Un entier en fonction de la v�rification
 *
 * @pre Le caract�re existe
 * @post on a un resultat
 * @invariant le caract�re
 */
int verif_last_character_exist(char * lastcharacter);
/**
 * @brief V�rifie le  caract�re est un op�rateur ou une parenth�se
 * @param character le caract�re � �tudier
 * @return Un entier en fonction de la v�rification
 *
 * @pre Le caract�re existe
 * @post on a un resultat
 * @invariant le caract�re
 */
int verif_operateur_et_parentheses (char character);
/**
 * @brief V�rifie le  caract�re est un r�el
 * @param adresse_du_character le caract�re � �tudier
 * @return Un entier en fonction de la v�rification
 *
 * @pre Le caract�re existe
 * @post on a un resultat
 * @invariant le caract�re
 */
int verif_reel (char * adresse_du_character);
/**
 * @brief Compte du premier charact�re jusqu'au dernier caract�re d�sign� par strtof
 * @param character adresse du premier caract�re
 * @param lastcharacter adresse du dernier caract�re
 * @return la position du dernier caract�re
 *
 * @pre Les caract�res existent
 * @post la position est retourn�e
 * @invariant les caract�res
 */
int compte_jusque_lastcharacter ( char * character , char * lastcharacter);
/**
 * @brief v�rifie la saisi et renvoi -1 si le premier character n'est un reel,
 Ou si le dernier character (o� s'est arr�t� strtof, confere commentaire dans la fonction)
 n'est pas �gale � un op�rateur, espace ou '\0' -> on modifie la valeur o� pointe "erreur" initialement �gale = 0
 * @param character adresse du premier caract�re
 * @param erreur adresse de varable erreur
 * @param position_reel adresse contenant la position du r�el
 * @return le r�el ou -1
 *
 * @pre le caract�re existe
 * @post on a un resultat
 * @invariant les caract�res
 */

float renvoi_nombre_reel (char * character, int * erreur , int * position_reel );
/**
 * @brief permet de retourner -1 s'il y a une erreur, sinon renvoi le reel
 * @param tab l'adresse du caract�re
 * @param position_reel la posistion du dernier caract�re du groupe de r�el
 * @return le r�el ou -1
 *
 * @pre Les caract�res existent
 * @post on a un reel
 * @invariant les caract�res
 */

float renvoi_nombre_si_pas_erreur (char * tab , int * position_reel);
/**
 * @brief cherche l'adresse du dernier maillon de la liste
 * @param list_token l'adresse de la liste
 * @return l'adresse du dernier maillon
 *
 * @pre La liste existe
 * @post on a une adresse
 * @invariant la liste
 */
tokenlist_t * adresse_dernier_maillon ( tokenlist_t * list_token );
/**
 * @brief ajoute un maillon dont le type est un r�el
 * @param token_p l'adresse de la liste actuelle
 * @param position la position dans la liste
 * @param reel le reel � ajouter
 * @return l'adresse de la nouvelle liste
 *
 * @pre La liste existe
 * @post on a un maillon suppl�mentaire
 * @invariant rien
 */
tokenlist_t * ajout_maillon_reel(tokenlist_t * token_p , int position , float reel);
/**
 * @brief ajoute un maillon dont le type est un r�el ou d�truit la liste si erreur
 * @param token_p l'adresse de la liste actuelle
 * @param position la position dans la liste
 * @param reel le reel � ajouter
 * @return l'adresse de la nouvelle liste
 *
 * @pre La liste existe
 * @post on a un maillon suppl�mentaire
 * @invariant rien
 */
tokenlist_t * ajout_reel( tokenlist_t * token_p , int position, float reel);

/**
 * @brief Ajoute un jeton correspondant � variable dans la liste
 * @param token_p le pointeur de la liste
 * @param position la position du jeton dans la formule
 * @return La liste avec le jeton en plus
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
tokenlist_t*ajout_fonction_constante(tokenlist_t*token_p,char * fonction,char comparaison[nombre_fonction][20],char constante [nombre_constante][20],int position,err_t*erreur);
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

tokenlist_t* analyselexical(char*tableau, err_t*erreur);

#endif // ANALYSELEXIC_H_INCLUDED
