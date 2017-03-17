#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/types.h"
#include "../inc/analyselexic.h"

#define nombre_fonction 17
#define nombre_constante 2
#define code_erreur 666

char liste_constantes [nombre_constante][20]={"pi","e"};
char liste_fonctions [nombre_fonction][20]={"cos","sin","tan","arccos",
                                     "arcsin","arctan","cosh",
                                     "sinh","tanh","sqrt","exp",
                                     "ln","log","abs","ent","dec","fact"};

tokenlist_t * initialisation()
{
    tokenlist_t * token = NULL;

    return token;
}

tokenlist_t * detruire_liste(tokenlist_t * token_p)
{
    tokenlist_t * suppression;

    if(token_p != NULL)
    {
        while(token_p != NULL)
        {
            suppression = token_p;
            token_p = token_p -> suivant;
            free(suppression);
            suppression = token_p;
        }
    }
    return token_p;
}

/* verif_operateur_et_parenthese : verifie si le character en question est un op�rateur : "+,-,*,/, espace, ou un parenthse ouvrante/fermante"*/
int verif_operateur_et_parentheses (char character)
{
    if(character == 43 || character == 45 || character == 47  || character == 42 || character == 40 || character == 41)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* verif_last_character : verifie si le dernier caracter du reel (identif� par strtof, cf plus bas) est un operateur, parenthese, espace, ou '\0'*/
int verif_last_character_exist(char * lastcharacter)
{
    if( verif_operateur_et_parentheses(lastcharacter[0]) || lastcharacter[0] == 32 || lastcharacter[0]=='\0'  || lastcharacter[0] == '\n')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* verif_reel : v�rifie si le character en question est bien un reel*/
int verif_reel (char * adresse_du_character)
{
    if(adresse_du_character[0] >= 48 && adresse_du_character[0] <= 57 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* compte_jusque_lastcharacter : compte du premier caract�re (d'o� commence le reel) jusq'au dernier caract�re identif� par strtof (cf fonction d'apr�s)*/
int compte_jusque_lastcharacter ( char * character , char * lastcharacter)
{
    int i=0;
    int position_lastcharacter=0;

    while (character+i < lastcharacter )
    {
        position_lastcharacter++;
        i++;
    }

    return position_lastcharacter;
}

/* renvoi_nombre_reel : v�rifie la saisi et renvoi -1 si le premier character n'est pas un reel,
 Ou si le dernier character (o� s'est arr�t� strtof, confere commentaire dans la fonction)
 n'est pas �gale � un op�rateur, espace ou '\0' -> on modifie la valeur o� pointe "erreur" initialement �gale = 0*/
float renvoi_nombre_reel (char * character, int * erreur , int * position_reel )
{
    if(verif_reel( character ))
    {
        char * lastcharacter;

        float reel;
        reel = strtof(character, &lastcharacter);
        //strtof : r�cup�re la valeur du reel saisi, soit, un nombre entier, soit, un nombre flottant, en s'arr�tant au dernier carac�tre non valide,
        //         c'est � dire, au deuxi�me points, ou �l�ment inconnu : lettre, espaace, '\0',..
        //         Il permet donc de r�cuperer la valeur du reel dans une variable, en renvoyant l'adresse du dernier caract�re


        if(verif_last_character_exist( lastcharacter )) // ici lastcharacter est �gale � l'adresse du dernier �l�ment, et on v�rifi� s'il existe
        {
            *position_reel = compte_jusque_lastcharacter(character, lastcharacter); // permet de r�cup�rer la position du dernier caract�re identifi� par 'strtof' en partant du premier caract�re reel
            return reel;
        }
        else
        {
            *erreur = 1; //permet de modifier directement la valeur de l'erreur dans la fonction renvoi_nombre_si_pas_erreur
            return reel;
        }
    }
    else
    {
        return -1 ;
    }
}

/* renvoi_nombre_si_pas_erreur : permet de retourner -1 s'il y a une erreur, sinon renvoi le reel */
float renvoi_nombre_si_pas_erreur (char * tab , int * position_reel)
{
    int erreur = 0;
    float reel = renvoi_nombre_reel(tab, &erreur , position_reel);

    if (erreur == 1 || reel == -1) // V�rifie si erreur est pass�e � 1, ou qu'on est pas de reel existant
    {
        return -1;
    }
    else //Retourne le reel sinon
    {
        return reel;
    }
}

/* adresse_dernier_maillon : chercher l'adresse du dernier maillon de la liste */
tokenlist_t * adresse_dernier_maillon ( tokenlist_t * list_token )
{
    tokenlist_t * token_ter;
    token_ter = list_token;

    while(token_ter -> suivant != NULL)
    {
         token_ter = token_ter -> suivant;
    }

    return token_ter;
}

/* ajout_maillon_reel : ajoute un nouveau maillon (� partir de la liste)  dans lequel le token est un reel*/
tokenlist_t * ajout_maillon_reel(tokenlist_t * token_p , int position , float reel)
{
    if(token_p == NULL) // Si la liste est vide, on cr�e un nouveau maillon, en y ajoutant les diff�rentes caract�ristiques
    {
        token_p = malloc(sizeof(tokenlist_t));
        token_p -> token.type = REEL;
        token_p -> token.valeur.reel = reel;
        token_p -> token.position = position;
        token_p -> suivant = NULL;
    }
    else // Sinon, on ajoute un nouveu maillon dans la liste d�j� existante, en y ajoutant �galement, ses diff�rentes caract�ristiques
    {
        tokenlist_t * tokenbis;
        tokenlist_t * token_ter;

        token_ter = adresse_dernier_maillon (token_p);

        tokenbis = malloc(sizeof(tokenlist_t)) ;
        tokenbis -> token.type = REEL ;
        tokenbis -> token.valeur.reel = reel ;
        tokenbis -> token.position = position ;
        tokenbis -> suivant = NULL ;
        token_ter -> suivant = tokenbis ;
    }

        return token_p;
}

/* ajout_reel : ajouter le mailon si le reel est positif (pas d'erreur) , sinon d�truit la liste */
tokenlist_t * ajout_reel( tokenlist_t * token_p , int position, float reel)
{
    if(reel >= 0) //Ajouter le maillon s'il n'y a pas d'erreur dans le reel saisi
    {
        token_p = ajout_maillon_reel ( token_p , position , reel) ;
    }
    else // Sinon, affiche un message d'erreur et d�truit la liste
    {
        printf("ERREUR !\n");
        printf("Destruction de la liste ! \n") ;
        token_p = detruire_liste(token_p);
    }
    return token_p;
}

/* ajout_fonction : permet d'ajouter un token variable dans la liste */
tokenlist_t * ajout_variable(tokenlist_t * token_p , int position)
{
    if(token_p == NULL) // Si la liste est vide, on cr�e un nouveau maillon, en y ajoutant les diff�rentes caract�ristiques
        {
            token_p = malloc(sizeof(tokenlist_t));
            token_p -> token.type = VAR;
            token_p -> token.position = position;
            token_p -> suivant = NULL;
        }
        else // Sinon, on ajoute un nouveu maillon dans la liste d�j� existante, en y ajoutant �galement, ses diff�rentes caract�ristiques
        {
            tokenlist_t * tokenbis;
            tokenlist_t * token_ter;

            token_ter = adresse_dernier_maillon (token_p);

            tokenbis = malloc(sizeof(tokenlist_t));
            tokenbis -> token.type = VAR;
            tokenbis -> token.position = position;
            tokenbis -> suivant = NULL;
            token_ter -> suivant = tokenbis;
        }

        return token_p;
}

/* ajout_parenthese_ouvrante : permet d'ajouter un token parenthese ouvrante dans la liste */
tokenlist_t * ajout_parenthese_ouvrante(tokenlist_t * token_p , int position)
{
    if(token_p == NULL) // Si la liste est vide, on cr�e un nouveau maillon, en y ajoutant les diff�rentes caract�ristiques
        {
            token_p = malloc( sizeof(tokenlist_t) );
            token_p -> token.type = PAR_O;
            token_p -> token.position = position;
            token_p -> suivant = NULL;
        }
        else // Sinon, on ajoute un nouveu maillon dans la liste d�j� existante, en y ajoutant �galement, ses diff�rentes caract�ristiques
        {
            tokenlist_t * tokenbis;
            tokenlist_t * token_ter;

            token_ter = adresse_dernier_maillon (token_p);

            tokenbis = malloc( sizeof(tokenlist_t));
            tokenbis -> token.type = PAR_O;
            tokenbis -> token.position = position;
            tokenbis -> suivant = NULL;
            token_ter -> suivant = tokenbis;
        }

        return token_p;
}

/* ajout_parenthese_fermante : permet d'ajouter un token parenthese fermante dans la liste */
tokenlist_t * ajout_parenthese_fermante(tokenlist_t * token_p , int position)
{
    if(token_p == NULL) // Si la liste est vide, on cr�e un nouveau maillon, en y ajoutant les diff�rentes caract�ristiques
        {
            token_p = malloc( sizeof(tokenlist_t));
            token_p -> token.type =PAR_F;
            token_p -> token.position = position;
            token_p -> suivant = NULL;
        }
        else // Sinon, on ajoute un nouveu maillon dans la liste d�j� existante, en y ajoutant �galement, ses diff�rentes caract�ristiques
        {
            tokenlist_t * tokenbis;
            tokenlist_t * token_ter;

            token_ter = adresse_dernier_maillon (token_p);

            tokenbis = malloc( sizeof(tokenlist_t));
            tokenbis -> token.type = PAR_F;
            tokenbis -> token.position = position;
            tokenbis -> suivant = NULL;
            token_ter -> suivant = tokenbis;
        }

        return token_p;
}

tokenlist_t * ajout_operateur(tokenlist_t * token_p , char operateur , int position)
{
        if(token_p == NULL) // Si la liste est vide, on cr�e un nouveau maillon, en y ajoutant les diff�rentes caract�ristiques
        {
            token_p = malloc( sizeof(tokenlist_t));
            token_p -> token.type = OPERATEUR;
            token_p -> token.position = position;
            token_p -> token.valeur.operateur = operateur;
            token_p -> suivant = NULL;
        }
        else // Sinon, on ajoute un nouveu maillon dans la liste d�j� existante, en y ajoutant �galement, ses diff�rentes caract�ristiques
        {
            tokenlist_t * tokenbis;
            tokenlist_t * token_ter;

            token_ter = adresse_dernier_maillon (token_p);

            tokenbis = malloc( sizeof(tokenlist_t));
            tokenbis -> token.type = OPERATEUR;
            tokenbis -> token.position = position;
            tokenbis -> token.valeur.operateur = operateur;
            tokenbis -> suivant = NULL;
            token_ter -> suivant = tokenbis;
        }

        return token_p;
}
/* ajout_fonction : permet d'ajouter un token fonction dans la liste ou de remplir un token erreur en cas de probl�me*/

tokenlist_t * ajout_fonction_constante(tokenlist_t * token_p , char * fonction_recuperee , char comparaison[nombre_fonction][20] , char constante [nombre_constante][20], int position, err_t * erreur)
{
    /* Comparaison de la fonction r�cup�r�e et de la base de donn�e des fonctions */
    int k = code_erreur;

    int fonction_compteur = 0;
    int constante_compteur = 0;
    int i, j;

    for(i = 0; (i < nombre_fonction) && (k == code_erreur); i++) //On parcourt toutes les fonctions existantes et on compare avec notre chaine de caract�res
    {
        if (strcmp(fonction_recuperee,comparaison[i]) == 0) //Compare la chaine de caract�re, soit, la saisie, � la fonction plac�e � l'emplacement 'i' du tableau fonction[nombre_fonction]
        {
            k = i; // Si la chaine de carac�tre correspond, on change le k permettant par la suite d'avertir qu'on � une chaine de caract�re qui correspond.
            fonction_compteur = 1; // Puis on incr�mente 'fonction_compeur' permettant d'indiquer qu'on a � faire � une fonction existante
        }
    }

    for(j = 0; (j <= nombre_constante) && (k==code_erreur); j++)  // M�me principe mais avec le tableau des constantes
    {
        if (strcmp(fonction_recuperee,constante[j]) == 0)
        {
            k = j;
            constante_compteur = 1;
        }
    }

    //Si la fonction correspond � une fonction de la base de donn�e
    if(k != code_erreur) // V�rifie si le k � chang� par rapport aux deux boucles d'avant
    {
        if(fonction_compteur == 1) // Regarde si la saisie correspond � une fonction existante
        {
          if(token_p == NULL) //Si la la fonction saisie correspond, et que la liste est vide, on cr�e un nouveau maillon avec ses diff�rentes caract�ristiques
          {
            token_p = malloc( sizeof(tokenlist_t));
            token_p -> token.type = FONCTION;
            token_p -> token.position = position;
            token_p -> token.valeur.fonction = k;
            token_p -> suivant = NULL;
          }
          else // Sinon, on ajoute un nouveu maillon dans la liste d�j� existante, en y ajoutant �galement, ses diff�rentes caract�ristiques
          {
            tokenlist_t * tokenbis;
            tokenlist_t * token_ter;

            token_ter = adresse_dernier_maillon (token_p);

            tokenbis = malloc( sizeof(tokenlist_t));
            tokenbis -> token.type = FONCTION;
            tokenbis -> token.position = position;
            tokenbis -> token.valeur.fonction = k;
            tokenbis -> suivant = NULL;
            token_ter -> suivant = tokenbis;

            }
        }
        else if(constante_compteur == 1) // M�me principe, si la constante existe
             {
                if(token_p==NULL) //Si la la constante saisie correspond, et que la liste est vide, on cr�e un nouveau maillon avec ses diff�rentes caract�ristiques
                {
                    token_p = malloc( sizeof(tokenlist_t));
                    token_p -> token.type = CONSTANTE;
                    token_p -> token.position = position;
                    token_p -> token.valeur.constante = k;
                    token_p -> suivant = NULL;
                }
                else // Sinon, on ajoute un nouveu maillon dans la liste d�j� existante, en y ajoutant �galement, ses diff�rentes caract�ristiques
                {
                    tokenlist_t * tokenbis;
                    tokenlist_t * token_ter;

                    token_ter = adresse_dernier_maillon (token_p);

                    tokenbis = malloc( sizeof(tokenlist_t));
                    tokenbis -> token.type = CONSTANTE;
                    tokenbis -> token.position = position;
                    tokenbis -> token.valeur.constante = k;
                    tokenbis -> suivant = NULL;
                    token_ter -> suivant = tokenbis;
                }
             }
    }

    // Si la fonction est mal �crite, ou ne correspond pas
    else
    {
        printf("ERREUR !\n");
        erreur -> type = MAUV_FONC_CONST;
        erreur -> token.position = position;
        printf("Destruction de la liste\n");
        token_p = detruire_liste(token_p);
    }

    return token_p;
}

/* fonction_recuperation : permet d'extraire une fonction dans la cha�ne de carateres*/
char * fonction_recuperation(char * arecup,int *i, char fonction_recupi[20] )
{
    int k;
    for(k = 0; arecup[*i] >= 'a' && arecup[*i] <= 'z'; k++) // Ajoute les caract�res saisies dans un tableau permettant par la suite, de comparer avec les fonctions d�j� existantes
    {
        fonction_recupi[k]=arecup[*i];
        (*i)++; // incr�mente la position de l'�l�ment du 'tableau de la saisie', permettant de continuer la suite de la lecture : cf fonction analyse
    }
    fonction_recupi[k] = '\0';

    return fonction_recupi;
}

/* verif_list_null : v�rifie si la liste est vide */
int verif_list_null (tokenlist_t * token_p )
{
    if(token_p == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* analyse : permet d'analyser la cha�ne de caract�re et de la transformer en liste cha�n�e*/
tokenlist_t * analyselexical(char * tableau , err_t * erreur)
{
    tokenlist_t * token_p;
    token_p = initialisation(); //Cr�e une liste vide
    erreur -> type = NO_ERR; // On initialise le type erreur, permettant de v�rifier � chaque incr�mentation s'il n'y a pas d'erreur

    int i;

    for(i = 0; tableau[i] != '\0' && erreur -> type == NO_ERR; i++) //Incr�mente caract�re par caract�re en v�rifiant si l'�l�ment existe : ajout dans la liste
    {
        switch( tableau[i] ) //Regarde si le caract�re � la case 'i' du tableau correspond � l'un de ces choix
        {
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
              token_p = ajout_operateur(token_p , tableau[i] , i); //Ajoute l'op�rateur dans la liste et renvoi son adresse
              break;

            case '(':
                token_p = ajout_parenthese_ouvrante( token_p , i) ; // M�me principe
                break;

            case ')':
                token_p = ajout_parenthese_fermante( token_p , i) ; // M�me principe
                break;

            case 'x':
            case 'X':
               token_p = ajout_variable( token_p , i); // M�me principe
               break;

            case ' ': // Casse le switch si l'on a un espace
            case '\n': // Casse le switch si l'on a un antislash 'n'
               break;

            default :
               if ( (tableau[i] >= 'a' ) && ( tableau[i] <= 'z')) // V�rifie si le caract�re est une lettre
               {
                   char fonction_recup [20];

                   fonction_recuperation( tableau, &i , fonction_recup); // r�cup�re la saisie (lettres) de l'utilisateur
                   token_p = ajout_fonction_constante( token_p , fonction_recup , liste_fonctions , liste_constantes , i, erreur); // renvoi l'adresse de la nouvelle liste si la saisie de la fonction ou constante a �t� correctement �tablie
                   i--;  // le 'i' correspond au dernier �l�ment de la ou les lettre(s) saisie(s)
               }
               else if (verif_reel (tableau + i) ) // V�rifie si le caract�re est un nombre
                    {
                        int position_reel ; // Correspond � la position du lastcharacter de 'strtof', � partir du caract�re en question (soit ici, le premier)
                        float r = renvoi_nombre_si_pas_erreur(tableau+i , &position_reel); // renvoi le reel dans la variable r si pas d'erreur
                        token_p = ajout_reel(token_p , i, r); // renvoi l'adresse de la nouvelle liste

                        if (verif_list_null (token_p) ) // Si la liste est vide, soit, une liste d�truite d� � une ou plusieurs erreurs, alors on modifie le type d'erreur permettant d'arr�ter l'incr�mentation dans le tableau
                        {
                            erreur -> type = MAUV_REEL;
                            erreur -> token.position = i; //Ajoute la position o� l'erreur est: modifi� dans la strucutre erreur
                        }
                        else
                        {
                            i = i + position_reel -1 ; // permet de r�cuperer la position du 'i' du dernier caract�re du reel saisi
                        }
                    }
                    else // Sinon, si aucun de ces cas l� existe, on d�truit la liste
                    {
                        printf("ERREUR ! Element inconnu\n") ;
                        erreur -> type = MAUV_CHAR ; // change le type d'erreur dans la structure 'erreur'
                        erreur -> token.position = i; // ajoute la position o� l'erreur est : modif� dans la structure 'erreur'
                        printf("Destruction de la liste\n");
                        token_p = detruire_liste(token_p);
                    }
            break;
        }
    }

    return token_p; // Renvoie l'adresse de la liste cr��e ou d�truite
}
