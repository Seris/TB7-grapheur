#include "../inc/interface.h"
#include "../inc/types.h"
#include "../inc/evaluateur.h"
#include "../inc/syntax-check.h"
#include "../inc/analyselexic.h"
#include "../inc/test.h"

extern int curseurX;
extern int curseurY;
extern int windowWidth;
extern int windowHeight;

char clique = 0;
graphpt_t ancreGlisse;

float echelleX = 1.0;
float echelleY = 1.0;
float decalageX = 0.0;
float decalageY = 0.0;

char formule[255];

tokenlist_t * listeJetons;
tokenarb_t * arbre;
graphpt_t * points;
graphpt_t * valeursDerivees;
char aucunPoint = 0;
float pointDepartX = -1.0;
float pointArriveeX = 1.0;
float pas = 1;

/// Fonctions d'information

void indiquerPosition(int p)
{
    int i;
    for(i = 0; i < p; i++)
    {
        printf(" ");
    }
    printf("^\n");
}

/// Fonctions graphiques

graphpt_t nvPoint(float x, float y, char v)
{
    graphpt_t p;
    p.x = x;
    p.y = y;
    p.valide = v;
    return p;
}

graphpt_t versRepere(graphpt_t p)
{
    graphpt_t nv;
    nv.x = echelleX * p.x + decalageX;
    nv.y = echelleY * p.y + decalageY;
    nv.valide = p.valide;
    return nv;
}

graphpt_t versBase(graphpt_t p)
{
    graphpt_t nv;
    nv.x = (p.x - decalageX) / echelleX;
    nv.y = (p.y - decalageY) / echelleY;
    nv.valide = p.valide;
    return nv;
}

void adaptationEchelle()
{
    int nbValeurs = ceil((pointArriveeX-pointDepartX)/pas)+1;
    float xMin = points[0].x;
    float xMax = pointArriveeX;
    float yMin;
    float yMax;
    int i=0;
    while(points[i].valide != 1 && i < nbValeurs)
    {
        i++;
    }
    if (i == nbValeurs)
    {
        yMin = -1;
        yMax = 1;
    }
    else
    {
        yMin = points[i].y;
        yMax = points[i].y;
        for(;i < nbValeurs; i++)
        {
            if(points[i].valide == 1)
            {
                if(points[i].y<yMin)
                    yMin = points[i].y;
                if(points[i].y>yMax)
                    yMax = points[i].y;
            }
        }
    }
    if (yMin == yMax)
    {
        yMin--;
        yMax++;
    }
    float diff = yMax - yMin;
    yMin = yMin - (0.20 * diff);
    yMax = yMax + (0.20 * diff);
    echelleX = (-xMin + xMax) / 2;
    echelleY = (-yMin + yMax) / 2;
    decalageX = (xMin + xMax) / 2;
    decalageY = (yMin + yMax) / 2;
}

void dessinerPoint(graphpt_t p)
{
    setcolor(0.0, 1.0, 0.0);
    float petittruc = 0.005;
    bar(p.x-petittruc, p.y-petittruc, p.x+petittruc, p.y+petittruc);
}

void tracerCourbe()
{
    printf("On trace la courbe.\n");
    setcolor(0, 1, 0);
    int nbValeurs = ceil((pointArriveeX-pointDepartX)/pas)+1;
    char ancreExiste = 0;
    int i;
    graphpt_t pointAncre, p1, p2;
    for(i = 0; i < nbValeurs; i++)
    {

        if (points[i].valide == 1)
        {
            dessinerPoint(versBase(points[i]));
            if (ancreExiste)
            {
                p1 = versBase(pointAncre);
                p2 = versBase (points[i]);
                line(p1.x, p1.y, p2.x, p2.y);
            }
            else
            {
                ancreExiste = 1;
            }
            pointAncre = points[i];
        }
        else
        {
            ancreExiste = 0;
        }
    }
}

void tracerDerivee()
{
    graphpt_t pointNegatif;
    graphpt_t pointPositif;
    if (valeursDerivees[0].valide == 0 || valeursDerivees[1].valide == 0)
        printf("Pas de derivee en ce point");
    else
    {
        pointNegatif.valide = 1;
        pointPositif.valide = 1;
        //pointNegatif.x = -valeursDerivees[0].x;
        pointNegatif.x = pointDepartX;
        //pointPositif.x = 3 * valeursDerivees[0].x;
        pointPositif.x = pointArriveeX;
        //pointNegatif.y = (valeursDerivees[1].y - valeursDerivees[0].y)/(0.05) * (pointNegatif.x - valeursDerivees[0].x) + valeursDerivees[0].y;
        //pointPositif.y = (valeursDerivees[1].y - valeursDerivees[0].y)/(0.05) * (pointPositif.x - valeursDerivees[0].x) + valeursDerivees[0].y;
        pointNegatif.y = ((valeursDerivees[1].y - valeursDerivees[0].y)/0.05) * pointNegatif.x + (valeursDerivees[0].y-((valeursDerivees[1].y - valeursDerivees[0].y)/0.05) * valeursDerivees[0].x);
        pointPositif.y = ((valeursDerivees[1].y - valeursDerivees[0].y)/0.05) * pointPositif.x + (valeursDerivees[0].y-((valeursDerivees[1].y - valeursDerivees[0].y)/0.05) * valeursDerivees[0].x);
        graphpt_t pnTranslate = versBase(pointNegatif);
        graphpt_t ppTranslate = versBase(pointPositif);
        line(pnTranslate.x, pnTranslate.y, ppTranslate.x, ppTranslate.y);
    }

}

/// Fonctions des valeurs

void chargerValeurs()
{
    supprime_tbx(points);
    points = NULL;
    printf("Tableau supprime.\n");
    points = generer_points(arbre, pointDepartX, pointArriveeX, pas);
    printf("Points generes.\n");
    aucunPoint = 0;
}

void chargerDerivees()
{
    graphpt_t curseur = nvPoint(2*(curseurX/(float)windowWidth)-1, -2*(curseurY/(float)windowHeight)+1, 1);
    graphpt_t curseurVersRepere = versRepere(curseur);
    valeursDerivees = generer_points(arbre, curseurVersRepere.x, curseurVersRepere.x+0.05, 0.06);
}

/// Fonctions de la fen�tre

void frappe(int c)
{
    printf("Appui sur une touche : %c\n", c);
    switch(c)
    {
    case '+':
        echelleX /= 2;
        echelleY /= 2;
        break;
    case '-':
        echelleX *= 2;
        echelleY *= 2;
        break;
    case '8':
        echelleY /= 2;
        break;
    case '2':
        echelleY *= 2;
        break;
    case '6':
        echelleX /= 2;
        break;
    case '4':
        echelleX *= 2;
        break;
    case 'o':
        decalageX = 0;
        decalageY = 0;
        break;
    case 'z':
        decalageY += echelleY * 0.3;
        break;
    case 's':
        decalageY -= echelleY * 0.3;
        break;
    case 'q':
        decalageX -= echelleX * 0.3;
        break;
    case 'd':
        decalageX += echelleX * 0.3;
        break;
    case 'c':
        adaptationEchelle();
        break;
    case 'r':
        {
            printf("RELOAD.\n");
            int nb = ceil((pointArriveeX-pointDepartX)/pas)+1;
            pointDepartX = versRepere(nvPoint(-1, 0, 1)).x;
            pointArriveeX = versRepere(nvPoint(1, 0, 1)).x;
            pas = (pointArriveeX - pointDepartX)/(nb-1);
            chargerValeurs();
            break;
        }
    case 'l':
        free_arbre_token(arbre);
        supprime_tbx(points);
        points = NULL;
        aucunPoint = 1;
        break;
    }
}

void dessin(void)
{
    printf("Dessin.\n");

    char output[50];

    // Dessin des axes
    setcolor(1.0, 1.0, 1.0); // Blanc
    graphpt_t p1, p2;
    p1 = versBase(nvPoint(-10000000, 0, 1));
    p2 = versBase(nvPoint(10000000, 0, 1));
    line(p1.x, p1.y, p2.x, p2.y);
    p1 = versBase(nvPoint(0, -10000000, 1));
    p2 = versBase(nvPoint(0, 10000000, 1));
    line(p1.x, p1.y, p2.x, p2.y);

    // Affichage du curseur
    graphpt_t curseur = nvPoint(2*(curseurX/(float)windowWidth)-1, -2*(curseurY/(float)windowHeight)+1, 1);
    graphpt_t curseurVersRepere = versRepere(curseur);
    char prefixeX[50] = "x=";
    snprintf(output, 50, "%f", curseurVersRepere.x);
    strcat(prefixeX, output);
    outtextxy(0.70, -0.85, prefixeX);
    char prefixeY[50] = "y=";
    snprintf(output, 50, "%f", curseurVersRepere.y);
    strcat(prefixeY, output);
    outtextxy(0.70, -0.95, prefixeY);

    setcolor(1, 0, 0);
    line(curseur.x - 0.025, curseur.y, curseur.x + 0.025, curseur.y);
    line(curseur.x, curseur.y - 0.025, curseur.x, curseur.y + 0.025);

    // Affichage du menu
    setcolor(1, 1, 1);
    outtextxy(-0.95, -0.5, "ZQSD : Deplacement");
    outtextxy(-0.95, -0.6, "+- / 2468 : Zoom");
    outtextxy(-0.95, -0.7, "C / O : Centrer / Origine");
    outtextxy(-0.95, -0.8, "R : Recharger");
    outtextxy(-0.95, -0.9, "L : Liberer la memoire");
    outtextxy(-0.95, 0.90, formule);

    // Affichage du rep�re
    p1 = versRepere(nvPoint(-1, -1, 1));
    p2 = versRepere(nvPoint(1, 1, 1));

    snprintf(output, 50, "%f", p1.x);
    outtextxy(-1, 0, output);
    snprintf(output, 50, "%f", p2.x);
    outtextxy(0.8, 0, output);
    snprintf(output, 50, "%f", p1.y);
    outtextxy(-0.15, -0.9, output);
    snprintf(output, 50, "%f", p2.y);
    outtextxy(-0.15, 0.9, output);

    if(!aucunPoint)
        tracerCourbe();

    chargerDerivees();
    tracerDerivee();
}

void sourisOnclick(int x, int y)
{

}

/// Fonctions du menu

void effEcr() // Fonction de qualit�
{
    //printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void lireFichier(char * adresse)
{
    FILE* fichier = NULL;
    fichier = fopen(adresse, "r");
    if(fichier == NULL)
        printf("Le fichier %s n'existe pas sac a foin.", adresse);
    else
    {
        char ligne[80] = {' '};
        void * retour;
        do {
            printf("%s", ligne);
            retour = fgets(ligne, 80, fichier);

        } while(retour != NULL);
        fclose(fichier);
    }
}

void menu(int ac, char *av[])
{
    char chaine[5] = {' '};
    while(strcmp(chaine, "0\n") != 0)
    {
        effEcr();
        printf("===== Grapheur =====\n\n\n\n");
        printf("Veuillez choisir une option :\n\n");
        printf("1. Saisir une formule\n");
        printf("2. Consulter le manuel\n");
        printf("3. Voir les credits\n");
        printf("0. Quitter\n\n\n");

        fgets(chaine, 5, stdin);

        if(!strcmp(chaine, "1\n"))
        {
            lancerGrapheur(ac, av);
        }
        else if(!strcmp(chaine, "2\n"))
        {
            effEcr();
            lireFichier("README/Manuel.txt");
            getchar();
        }
        else if(!strcmp(chaine, "3\n"))
        {
            effEcr();
            lireFichier("README/Credits.txt");
            getchar();
        }
    }
}

void lancerGrapheur(int ac, char *av[])
{
    int succes = 0;
    err_t erreur;
    printf("Bonjour ! Que diriez-vous d'entrer une formule ?\n\n--->");
    do
    {
        fgets(formule, 255, stdin);
        while(strlen(formule) <= 1)
        {
            printf("Il faut ecrire quelque chose...\n\n--->");
            fgets(formule, 255, stdin);
        }
        printf("Vous avez tape : %s", formule);

        listeJetons = analyselexical(formule, &erreur);
        if(listeJetons != NULL){
            arbre = parse_token_list(listeJetons, &erreur);
            detruire_liste(listeJetons);
            if(arbre != NULL){
                succes = 1;
            } else {
                print_error(formule, erreur);
            }
        } else {
            print_error(formule, erreur);
        }
    } while(!succes); // Tant qu'il y a une erreur

    char chaine[255];
    succes = 0;
    printf("Nous vous en remercions.\n\nTapez la borne inferieure, SVP :\n\n--->");
    while(!succes)
    {
        fgets(chaine, 255, stdin);
        pointDepartX = atof(chaine);
        if(strlen(chaine) <= 1) printf("Une chaine vide n'est pas egale a 0, si vous vous posez la question.\n\n--->");
        else if(pointDepartX == 0 && strcmp(chaine, "0\n") != 0) printf("Une VRAIE borne, SVP.\n\n--->");
        else succes = 1;
    }
    printf("Borne inferieure : %f\n", pointDepartX);

    succes = 0;
    printf("\nFormidable !\nC'est le tour de la borne superieure, maintenant :\n\n--->");
    while(!succes)
    {
        fgets(chaine, 255, stdin);
        pointArriveeX = atof(chaine);
        if(strlen(chaine) <= 1) printf("Une chaine vide n'est toujours pas egale a 0.\n\n--->");
        else if(pointArriveeX == 0 && strcmp(chaine, "0\n") != 0) printf("Une borne est un nombre, cela n'a pas change depuis la derniere fois.\n\n--->");
        else if(pointArriveeX <= pointDepartX) printf("Le minimum syndical, ce serait que la valeur d'arrivee soit superieure a la valeur de depart, non ?\n\n--->");
        else succes = 1;
    }
    printf("Borne superieure : %f\n", pointArriveeX);

    succes = 0;
    printf("\nUne derniere chose, le pas de calcul :\n\n--->");
    while(!succes)
    {
        fgets(chaine, 255, stdin);
        pas = atof(chaine);
        if(strlen(chaine) <= 1) printf("<Rien> n'est pas accepte.\n\n--->");
        else if(strcmp(chaine, "0\n") == 0) printf("Un pas de 0 n'est pas logique.\n\n--->");
        else if(pas <= 0) printf("IL FAUT UN VRAI NOMBRE POSITIF !!!\n\n--->");
        else
        {
            succes = 1;
            if(pas > pointArriveeX - pointDepartX) printf("Le pas devrait normalement etre plus petit que l'intervalle, mais ca passe pour cette fois.\n\n");
        }
    }
    printf("Pas de calcul : %f\n", pas);

    // 3e groupe
    chargerValeurs();

    adaptationEchelle();
    InitGraph(ac, av, "Fen�tre", 1000, 700, dessin, frappe, sourisOnclick);

    printf("Ce message ne s'affichera jamais parce que OpenGL est mal fait.");
}
