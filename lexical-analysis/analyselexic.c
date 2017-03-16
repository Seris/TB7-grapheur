#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/types.h"
#include "../inc/analyselexic.h"

char liste_fonctions[17][20]={"cos","sin","tan","arccos","arcsin","arctan","cosh","sinh","tanh","sqrt","exp","ln","log","abs","ent","dec","fact"};
char liste_constantes[2][20]={"pi","e"};


tokenlist_t* initialisation()
{
    tokenlist_t*token=NULL;

    return token;

}

tokenlist_t* detruire_liste(tokenlist_t *token_p)
{
    tokenlist_t * suppression;

    if(token_p != NULL)
    {
        while(token_p != NULL)
        {
            suppression = token_p;
            token_p=token_p->suivant;
            free(suppression);
            suppression=token_p;
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

/* verif_last_character : verifie si le dernier character du reel (identif� par strtof, cf plus bas) est un operateur, parenthese, espace, ou '\0'*/
int verif_last_character_exist(char * lastcharacter)
{
    if( verif_operateur_et_parentheses(lastcharacter[0]) || lastcharacter[0] == 32 || lastcharacter[0]=='\0' || lastcharacter[0] == '\n'  )
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

/* compte_jusque_lastcharacter : compte du premier character (d'o� commence le reel) jusq'au dernier identif� par strtof (cf fonction d'apr�s)*/
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

/* renvoi_nombre_reel : v�rifie la saisi et renvoi -1 si le premier character n'est un reel,
 Ou si le dernier character (o� s'est arr�t� strtof, confere commentaire dans la fonction)
 n'est pas �gale � un op�rateur, espace ou '\0' -> on modifie la valeur o� pointe "erreur" initialement �gale = 0*/
float renvoi_nombre_reel (char * character, int * erreur , int * position_reel )
{
    if(verif_reel(character))
    {
        char * lastcharacter;

        float reel;
        reel = strtof(character, &lastcharacter);


        if(verif_last_character_exist(lastcharacter))
        {
            *position_reel = compte_jusque_lastcharacter(character, lastcharacter);
            return reel;
        }
        else
        {
            *erreur= 1;
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

    if (erreur == 1 || reel == -1)
    {
        return -1;
    }
    else
    {
        return reel;
    }
}

/* adresse_dernier_maillon : chercher l'adresse du dernier maillon de la liste */
tokenlist_t * adresse_dernier_maillon ( tokenlist_t * list_token )
{
    tokenlist_t * token_ter;
    token_ter = list_token;

    while(token_ter->suivant!=NULL)
    {
         token_ter=token_ter->suivant;
    }

    return token_ter;
}

/* ajout_maillon_reel : ajoute un nouveau maillon (� partir de la liste)  dans lequel le token est un reel*/
tokenlist_t * ajout_maillon_reel(tokenlist_t * token_p , int position , float reel)
{
    if(token_p == NULL)
    {
        token_p = malloc(sizeof(tokenlist_t));
        token_p -> token.type = REEL;
        token_p -> token.valeur.reel = reel;
        token_p -> token.position = position;
        token_p -> suivant = NULL;
    }
    else
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
    if(reel >= 0)
    {
        token_p = ajout_maillon_reel ( token_p , position , reel) ;
    }
    else
    {
        token_p = detruire_liste(token_p);

    }


    return token_p;
}

/* ajout_fonction : permet d'ajouter un token variable dans la liste */
tokenlist_t*ajout_variable(tokenlist_t*token_p,int position)
{
    if(token_p==NULL)
        {
            token_p=malloc(sizeof(tokenlist_t));
            token_p->token.type=VAR;
            token_p->token.position=position;
            token_p->suivant=NULL;
        }
        else
        {
            tokenlist_t*tokenbis;
            tokenlist_t*token_ter;
            token_ter=token_p;
            while(token_ter->suivant!=NULL)
            {
                token_ter=token_ter->suivant;
            }
            tokenbis=malloc(sizeof(tokenlist_t));
            tokenbis->token.type=VAR;
            tokenbis->token.position=position;
            tokenbis->suivant=NULL;
            token_ter->suivant=tokenbis;
        }

        return token_p;


}

/* ajout_parenthese_ouvrante : permet d'ajouter un token parenthese ouvrante dans la liste */
tokenlist_t*ajout_parenthese_ouvrante(tokenlist_t*token_p,int position)
{
    if(token_p==NULL)
        {
            token_p=malloc(sizeof(tokenlist_t));
            token_p->token.type=PAR_O;
            token_p->token.position=position;
            token_p->suivant=NULL;
        }
        else
        {
            tokenlist_t*tokenbis;
            tokenlist_t*token_ter;
            token_ter=token_p;
            while(token_ter->suivant!=NULL)
            {
                token_ter=token_ter->suivant;
            }
            tokenbis=malloc(sizeof(tokenlist_t));
            tokenbis->token.type=PAR_O;
            tokenbis->token.position=position;
            tokenbis->suivant=NULL;
            token_ter->suivant=tokenbis;
        }

        return token_p;


}

/* ajout_parenthese_fermante : permet d'ajouter un token parenthese fermante dans la liste */
tokenlist_t*ajout_parenthese_fermante(tokenlist_t*token_p,int position)
{
    if(token_p==NULL)
        {
            token_p=malloc(sizeof(tokenlist_t));
            token_p->token.type=PAR_F;
            token_p->token.position=position;
            token_p->suivant=NULL;
        }
        else
        {
            tokenlist_t*tokenbis;
            tokenlist_t*token_ter;
            token_ter=token_p;
            while(token_ter->suivant!=NULL)
            {
                token_ter=token_ter->suivant;
            }
            tokenbis=malloc(sizeof(tokenlist_t));
            tokenbis->token.type=PAR_F;
            tokenbis->token.position=position;
            tokenbis->suivant=NULL;
            token_ter->suivant=tokenbis;
        }

        return token_p;


}

tokenlist_t*ajout_operateur(tokenlist_t*token_p,char operateur,int position)
{
        if(token_p==NULL)
        {
            token_p=malloc(sizeof(tokenlist_t));
            token_p->token.type=OPERATEUR;
            token_p->token.position=position;
            token_p->token.valeur.operateur=operateur;
            token_p->suivant=NULL;
        }
        else
        {
            tokenlist_t*tokenbis;
            tokenlist_t*token_ter;
            token_ter=token_p;
            while(token_ter->suivant!=NULL)
            {
                token_ter=token_ter->suivant;
            }
            tokenbis=malloc(sizeof(tokenlist_t));
            tokenbis->token.type=OPERATEUR;
            tokenbis->token.position=position;
            tokenbis->token.valeur.operateur=operateur;
            tokenbis->suivant=NULL;
            token_ter->suivant=tokenbis;
        }

        return token_p;


}
/* ajout_fonction : permet d'ajouter un token fonction dans la liste ou de remplir un token erreur en cas de probl�me*/

tokenlist_t * ajout_fonction_constante(tokenlist_t*token_p, char* fonction, char comparaison[17][20],char constante [2][20],int position,err_t*erreur)
{
    /* Comparaison de la fonction r�cup�r�e et de la base de donn�e des fonctions */
    int k=666;
    int fonction_compteur=0;
    int constante_compteur=0;
    int i;
    int j;
    for(i=0;(i<=16)&&(k==666);i++)
    {
        if (strcmp(fonction,comparaison[i])==0)
        {
            k=i;
            fonction_compteur=1;

        }


    }
    for(j=0;(j<=1)&&(k==666);j++)
    {
        if (strcmp(fonction,constante[j])==0)
        {
            k=j;
            constante_compteur=1;

        }

    }
    //Si la fonction correspond � une fonction de la base de donn�e
    if(k!=666)
    {
        if(fonction_compteur==1)
        {


          if(token_p==NULL)
        {
            token_p=malloc(sizeof(tokenlist_t));
            token_p->token.type=FONCTION;
            token_p->token.position=position;
            token_p->token.valeur.fonction=k;
            token_p->suivant=NULL;
        }
        else
        {
            tokenlist_t*tokenbis;
            tokenlist_t*token_ter;
            token_ter=token_p;
            while(token_ter->suivant!=NULL)
            {
                token_ter=token_ter->suivant;
            }
            tokenbis=malloc(sizeof(tokenlist_t));
            tokenbis->token.type=FONCTION;
            tokenbis->token.position=position;
            tokenbis->token.valeur.fonction=k;
            tokenbis->suivant=NULL;
            token_ter->suivant=tokenbis;
        }
        }
        else if(constante_compteur==1)
        {
             if(token_p==NULL)
        {
            token_p=malloc(sizeof(tokenlist_t));
            token_p->token.type=CONSTANTE;
            token_p->token.position=position;
            token_p->token.valeur.constante=k;
            token_p->suivant=NULL;
        }
        else
        {
            tokenlist_t*tokenbis;
            tokenlist_t*token_ter;
            token_ter=token_p;
            while(token_ter->suivant!=NULL)
            {
                token_ter=token_ter->suivant;
            }
            tokenbis=malloc(sizeof(tokenlist_t));
            tokenbis->token.type=CONSTANTE;
            tokenbis->token.position=position;
            tokenbis->token.valeur.constante=k;
            tokenbis->suivant=NULL;
            token_ter->suivant=tokenbis;
        }

        }

    }
    // Si la fonction est mal �crite, ou ne correspond pas
    else
    {
            erreur->type=MAUV_FONC_CONST;
            erreur->token.position=position;
            token_p = detruire_liste(token_p);
    }
return token_p;

}

/* fonction_recuperation : permet d'extraire une fonction dans la cha�ne de charateres*/
char * fonction_recuperation(char * arecup,int *i,char fonction_recupi[20] )
{

    int k;
    for(k=0;arecup[*i]>='a'&&arecup[*i]<='z';k++)
    {
        fonction_recupi[k]=arecup[*i];
        (*i)++;

    }
    fonction_recupi[k]='\0';


    return fonction_recupi;
}

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

/* analyselexical : permet d'analyser la cha�ne de caract�re et de la transformer en liste cha�n�e*/
tokenlist_t* analyselexical(char*tableau,err_t* erreur)
{
    tokenlist_t*token_p;
    token_p=initialisation();
    erreur->type = NO_ERR;
    int i;
    int position=1;




    for(i=0; tableau[i]!='\0' && erreur->type == NO_ERR;i++)
    {

        switch(tableau[i])
        {


        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '^':

          token_p=ajout_operateur(token_p,tableau[i],position);
          position++;
          break;


        case '(':

            token_p=ajout_parenthese_ouvrante(token_p,position) ;
            position++;
            break;

        case ')':

            token_p=ajout_parenthese_fermante(token_p,position) ;
            position++;
            break;

        case 'x':case 'X':

           token_p=ajout_variable(token_p,position);
           position++;
           break;

        case ' ':
        case '\n':
        break;



        default :

           if ((tableau[i]>='a')&&(tableau[i]<='z'))
           {
               char fonction_recup [20];

               fonction_recuperation(tableau,&i,fonction_recup);
               token_p=ajout_fonction_constante(token_p,fonction_recup,liste_fonctions, liste_constantes ,position,erreur);
               i--;
               position++;
           }

           else if (verif_reel (tableau + i) )
            {
                int position_reel ;
                float r= renvoi_nombre_si_pas_erreur(tableau+i , &position_reel);

                token_p = ajout_reel(token_p , position, r);

                if (verif_list_null (token_p) )
                {
                    erreur -> type = MAUV_REEL;
                    erreur -> token.position = position;
                }
                else
                {

                    i = i + position_reel -1 ;
                    position++;
                }

            }
           else
           {
            erreur->type=MAUV_CHAR;
            erreur->token.position=position;
            token_p = detruire_liste(token_p);
           }


           break;



        }

    }

    return token_p;
}
