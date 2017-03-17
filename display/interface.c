#include "../inc/interface.h"
#include "../inc/types.h"
#include "../inc/evaluateur.h"
#include "../inc/syntax-check.h"
#include "../inc/analyselexic.h"
#include "../inc/test.h"
#define PAS_DIFF 0.001


extern int curseurX; //mot-clef extern pour faire le lien avec les variables globales de graph.cpp
extern int curseurY;
extern int windowWidth;
extern int windowHeight;


float echelleX = 1.0; // définition de paramètres de mise à l'échelle pour passer du repère mathématique au repère GLUT et vice versa
float echelleY = 1.0;
float decalageX = 0.0;
float decalageY = 0.0;

char formule[255];

tokenlist_t * listeJetons;
tokenarb_t * arbre;
graphpt_t * points;
graphpt_t * valeursDerivees;
char aucunPoint = 0; // vaut 1 quand points est libéré
float pointDepartX = -1.0; // correspond à la première valeur du tableau
float pointArriveeX = 1.0;// correspond à la dernière valeur du tableau
float pas = 1; // le pas de progression en x dans le tableau

/// Fonctions d'information

// montre à l'utilisateur la position de l'erreur dans sa formule
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

// génère un point à partir de coordonnées et d'un paramètre validité
graphpt_t nvPoint(float x, float y, char v)
{
    graphpt_t p;
    p.x = x;
    p.y = y;
    p.valide = v;
    return p;
}

// détermine un nouveau point dans le repère souhaité à partir d'un point du repère de la fenêtre, et des paramètres de mise à l'échelle
graphpt_t versRepere(graphpt_t p)
{
    graphpt_t nv;
    nv.x = echelleX * p.x + decalageX;
    nv.y = echelleY * p.y + decalageY;
    nv.valide = p.valide;
    return nv;
}

// réciproque de la fonction versRepere
graphpt_t versBase(graphpt_t p)
{
    graphpt_t nv;
    nv.x = (p.x - decalageX) / echelleX;
    nv.y = (p.y - decalageY) / echelleY;
    nv.valide = p.valide;
    return nv;
}

//détermine les paramètres de mise à l'échelles adéquats en étudiant les bornes en x et les extrema de la fonction
void adaptationEchelle()
{
    int nbValeurs = ceil((pointArriveeX-pointDepartX)/pas)+1; // détermine le nombre de points dans le tableau à l'aide des bornes en x et du pas
    float xMin = points[0].x;
    float xMax = pointArriveeX;
    float yMin;
    float yMax;
    int i=0;
    while(points[i].valide != 1 && i < nbValeurs) // parcourt le tableau jusqu'à tomber sur un point valide
    {
        i++;
    }
    if (i == nbValeurs) // pas de point valide, on définit une fenêtre arbitrairement
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
            if(points[i].valide == 1) // si le point est valide, compare son ordonnée avec yMin et yMax et stocke dans l'un des deux en cas d'extremum
            {
                if(points[i].y<yMin)
                    yMin = points[i].y;
                if(points[i].y>yMax)
                    yMax = points[i].y;
            }
        }
    }
    if (yMin == yMax) // règle le problème d'une fonction constante qui donnerait une fenêtre sans hauteur
    {
        yMin--;
        yMax++;
    }
    float diff = yMax - yMin;
    yMin = yMin - (0.20 * diff); // ajoute une marge verticale pour faciliter la lecture
    yMax = yMax + (0.20 * diff);
    echelleX = (-xMin + xMax) / 2; // définit les paramètres de mise à l'échelle à l'aide de xMin, xMax, yMin, yMax
    echelleY = (-yMin + yMax) / 2;
    decalageX = (xMin + xMax) / 2;
    decalageY = (yMin + yMax) / 2;
}

// génère un petit carré centré sur le point
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
    char ancreExiste = 0; // définit l'existence ou non d'un point de départ pour une ligne
    int i;
    graphpt_t pointAncre, p1, p2; // définition d'un point de départ pour une ligne, et de deux points dans le repère de la fenêtre
    for(i = 0; i < nbValeurs; i++) // parcourt le tableau de points
    {

        if (points[i].valide == 1) // dans le cas d'un point valide...
        {
            dessinerPoint(versBase(points[i]));
            if (ancreExiste) // ... s'assure qu'un point de départ pour une ligne existait déjà...
            {
                p1 = versBase(pointAncre);
                p2 = versBase (points[i]);
                line(p1.x, p1.y, p2.x, p2.y); // trace la ligne depuis le point de départ jusqu'au point actuel
            }
            else
            {
                ancreExiste = 1; // ... sinon, crée un point de départ
            }
            pointAncre = points[i];  // redéfinit le point de départ avec le point actuel
        }
        else
        {
            ancreExiste = 0; // plus de point de départ si le point n'est pas valide
        }
    }
}

void tracerDerivee()
{
    graphpt_t pointNegatif; // définit des points en bordures gauche et droite de la fenêtre
    graphpt_t pointPositif;
    if (valeursDerivees[0].valide == 0 || valeursDerivees[1].valide == 0) // condition rudimentaire sur la derivabilite qui teste l'existence d'une image en x et en x+0.05
        printf("Pas de derivee en ce point");
    else
    {
        pointNegatif.valide = 1;
        pointPositif.valide = 1;
        pointNegatif.x = pointDepartX;
        pointPositif.x = pointArriveeX;
        pointNegatif.y = ((valeursDerivees[1].y - valeursDerivees[0].y)/PAS_DIFF) * pointNegatif.x + (valeursDerivees[0].y-((valeursDerivees[1].y - valeursDerivees[0].y)/PAS_DIFF) * valeursDerivees[0].x); // calcul des ordonnées des points de la droite en bordures gauche et droite de fenêtre, par prolongement de la tangente au point
        pointPositif.y = ((valeursDerivees[1].y - valeursDerivees[0].y)/PAS_DIFF) * pointPositif.x + (valeursDerivees[0].y-((valeursDerivees[1].y - valeursDerivees[0].y)/PAS_DIFF) * valeursDerivees[0].x);
        graphpt_t pnTranslate = versBase(pointNegatif);
        graphpt_t ppTranslate = versBase(pointPositif);
        line(pnTranslate.x, pnTranslate.y, ppTranslate.x, ppTranslate.y); // trace la derivee au point d'un bord à l'autre de la fenetre
    }

}

/// Fonctions des valeurs

// genere le tableau de points a afficher
void chargerValeurs()
{
    supprime_tbx(points); // vide le tableau avant de le regenerer
    points = NULL; // permet de ne pas essayer de libérer deux fois l'espace alloué
    printf("Tableau de points supprime.\n");
    points = generer_points(arbre, pointDepartX, pointArriveeX, pas);
    printf("Points generes.\n");
    aucunPoint = 0;
}

// genere à chaque mouvement de souris un tableau contenant le point de la courbe au niveau du curseur, et le point en x + 0.05
void chargerDerivees()
{
    graphpt_t curseur = nvPoint(2*(curseurX/(float)windowWidth)-1, -2*(curseurY/(float)windowHeight)+1, 1); // curseur dans le repere de la fenetre
    graphpt_t curseurVersRepere = versRepere(curseur); // curseur dans le repere mathematiques
    supprime_tbx(valeursDerivees); // vide le tableau des valeurs pour la dérivée précédent
    valeursDerivees = NULL;
    valeursDerivees = generer_points(arbre, curseurVersRepere.x, curseurVersRepere.x+PAS_DIFF, 2*PAS_DIFF);
}

/// Fonctions de la fenetre

// execute des instructions en fonction de la saisie
void frappe(int c)
{
    printf("Appui sur une touche : %c\n", c);
    switch(c)
    {
    case '+': // zoome selon les deux axes
        echelleX /= 2;
        echelleY /= 2;
        break;
    case '-': //dezoome selon les deux axes
        echelleX *= 2;
        echelleY *= 2;
        break;
    case '8': // zoome selon l'axe y
        echelleY /= 2;
        break;
    case '2': //dezoome selon l'axe y
        echelleY *= 2;
        break;
    case '6': // zoome selon l'axe x
        echelleX /= 2;
        break;
    case '4': // dezoome selon l'axe x
        echelleX *= 2;
        break;
    case 'o': // recentre sur l'origine
        decalageX = 0;
        decalageY = 0;
        break;
    case 'z': // effectue un déplacement approprié
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
    case 'c': // affiche la courbe selon les bornes en entrée avec détection des extrema
        adaptationEchelle();
        break;
    case 'r': // recharge les points necessaires de la courbe
        {
            if (!aucunPoint)
            {
                printf("Rechargement des valeurs.\n");
                int nb = ceil((pointArriveeX-pointDepartX)/pas)+1;
                pointDepartX = versRepere(nvPoint(-1, 0, 1)).x;
                pointArriveeX = versRepere(nvPoint(1, 0, 1)).x;
                pas = (pointArriveeX - pointDepartX)/(nb-1);
                chargerValeurs();
            }
            break;
        }
    case 'l': // libere la memoire
        free_arbre_token(arbre);
        supprime_tbx(points);
        supprime_tbx(valeursDerivees);
        points = NULL;
        valeursDerivees = NULL;
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
    p1 = versBase(nvPoint(-10*echelleX, 0, 1));
    p2 = versBase(nvPoint(10*echelleX, 0, 1));
    line(p1.x, p1.y, p2.x, p2.y);
    p1 = versBase(nvPoint(0, -10*echelleY, 1));
    p2 = versBase(nvPoint(0, 10*echelleY, 1));
    line(p1.x, p1.y, p2.x, p2.y);

    // Affichage du menu
    setcolor(1, 1, 1);
    outtextxy(-0.95, -0.5, "ZQSD : Deplacement");
    outtextxy(-0.95, -0.6, "+- / 2468 : Zoom");
    outtextxy(-0.95, -0.7, "C / O : Centrer / Origine");
    outtextxy(-0.95, -0.8, "R : Recharger");
    outtextxy(-0.95, -0.9, "L : Liberer la memoire");

    // Affichage du repère
    p1 = versRepere(nvPoint(-1, -1, 1)); // point situé en bas à gauche de la fenetre
    p2 = versRepere(nvPoint(1, 1, 1)); // en bas a droite

    snprintf(output, 50, "%f", p1.x);  // convertit un float en une chaine de caracteres
    outtextxy(-1, 0, output); // affiche dans la fenetre openGL
    snprintf(output, 50, "%f", p2.x);
    outtextxy(0.8, 0, output);
    snprintf(output, 50, "%f", p1.y);
    outtextxy(-0.15, -0.9, output);
    snprintf(output, 50, "%f", p2.y);
    outtextxy(-0.15, 0.9, output);

    if(!aucunPoint) // si un tableau de valeurs existe
    {
        chargerDerivees(); // charge le tableau de valeurs pour la derivee
        graphpt_t curseur = nvPoint(2*(curseurX/(float)windowWidth)-1, -2*(curseurY/(float)windowHeight)+1, 1); // curseur dans la fenetre
        graphpt_t curseurVersRepere = versRepere(curseur); // curseur dans le repere mathematiques
        char prefixeX[50] = "x=";
        snprintf(output, 50, "%f", curseurVersRepere.x);
        strcat(prefixeX, output);
        outtextxy(0.70, -0.75, prefixeX); // affichage de la coordonnee x du point de la courbe correspondant au curseur
        char prefixeY[50] = "y=";
        snprintf(output, 50, "%f", valeursDerivees[0].y);
        strcat(prefixeY, output);
        outtextxy(0.70, -0.85, prefixeY); // coordonnee y
        char prefixeD[50] = "f'(x)=";
        snprintf(output, 50, "%f", ((valeursDerivees[1].y - valeursDerivees[0].y)/0.05));
        strcat(prefixeD, output);
        outtextxy(0.70, -0.95, prefixeD); // affichage de la valeur de la derivee au point
        setcolor(1, 0, 0);
        graphpt_t auPoint = versBase(valeursDerivees[0]);
        line(curseur.x - 0.025, auPoint.y, curseur.x + 0.025, auPoint.y); // affiche un curseur sur la courbe
        //line(curseur.x, curseur.y - 0.025, curseur.x, curseur.y + 0.025);
        line(curseur.x, auPoint.y - 0.025, curseur.x, auPoint.y + 0.025);
        tracerCourbe(); // trace la courbe
        tracerDerivee(); // trace la tangente à la courbe
    }
}

void sourisOnclick(int x, int y)
{
    // récupère les coordonnées de la souris en cas de clic (pas utilisé)
}

/// Fonctions du menu

void effEcr() // nettoie la console selon l'OS
{
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void lireFichier(char * adresse) // lit le contenu textuel d'un fichier
{
    FILE* fichier = NULL;
    fichier = fopen(adresse, "r");
    if(fichier == NULL)
        printf("Le fichier %s n'existe pas.\n", adresse);
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

// menu du programme
void menu(int ac, char *av[])
{
    char chaine[5] = {' '};
    while(strcmp(chaine, "0\n") != 0) // tant que l'utilisateur n'a pas écrit 0
    {
        effEcr();
        printf("===== Grapheur =====\n\n\n\n");
        printf("Veuillez choisir une option :\n\n");
        printf("1. Saisir une formule\n");
        printf("2. Consulter le manuel\n");
        printf("3. Voir les credits\n");
        printf("0. Quitter\n\n\n");

        fgets(chaine, 5, stdin);

        if(!strcmp(chaine, "1\n")) // fgets retourne une chaîne de caractères avec un retour à la ligne
        {
            lancerQuestions(ac, av);
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

// lance le grapheur avec des parmètres définis globalement
void lancerGrapheur(int ac, char *av[])
{
    chargerValeurs();
    adaptationEchelle();
    InitGraph(ac, av, "Fenetre", 1000, 700, dessin, frappe, sourisOnclick);
}

// lance la saisie utilisateur
void lancerQuestions(int ac, char *av[])
{
    int succes = 0;
    err_t erreur; // Variable dans laquelle on stocke l'erreur
    printf("Veuillez entrer une formule :\n\n--->");
    do
    {
        fgets(formule, 255, stdin);
        while(strlen(formule) <= 1) // verification anti-chaîne vide
        {
            printf("Il faut ecrire quelque chose...\n\n--->");
            fgets(formule, 255, stdin);
        }
        listeJetons = analyselexical(formule, &erreur); // lance le module 1
        if(listeJetons != NULL){
            arbre = parse_token_list(listeJetons, &erreur); // lance le module 2
            detruire_liste(listeJetons);
            if(arbre != NULL){
                succes = 1;
            } else {
                print_error(formule, erreur); // affichage de l'erreur du module 2
            }
        } else {
            print_error(formule, erreur); // affichage de l'erreur du module 1
        }
    } while(!succes); // Tant qu'il y a une erreur

    char chaine[255];
    succes = 0;
    printf("\n\nBorne inferieure :\n\n--->");
    while(!succes)
    {
        fgets(chaine, 255, stdin);
        pointDepartX = atof(chaine); // convertit la chaine de caracteres en float
        if(strlen(chaine) <= 1) printf("Veuillez taper quelque chose.\n\n--->");
        else if(pointDepartX == 0 && strcmp(chaine, "0\n") != 0) printf("La borne doit etre reelle\n\n--->");
        else succes = 1;
    }
    printf("Borne inferieure : %f\n", pointDepartX);

    succes = 0;
    printf("\nBorne superieure :\n\n--->");
    while(!succes)
    {
        fgets(chaine, 255, stdin);
        pointArriveeX = atof(chaine);
        if(strlen(chaine) <= 1) printf("Veuillez taper quelque chose.\n\n--->");
        else if(pointArriveeX == 0 && strcmp(chaine, "0\n") != 0) printf("La borne doit etre reelle\n\n--->");
        else if(pointArriveeX <= pointDepartX) printf("La borne supérieure doit etre plus grande\n\n--->");
        else succes = 1;
    }
    printf("Borne superieure : %f\n", pointArriveeX);

    succes = 0;
    printf("\nPas de calcul :\n\n--->");
    while(!succes)
    {
        fgets(chaine, 255, stdin);
        pas = atof(chaine);
        if(strlen(chaine) <= 1) printf("Veuillez taper quelque chose.\n\n--->");
        else if(strcmp(chaine, "0\n") == 0) printf("Le pas doit etre different de 0.\n\n--->");
        else if(pas <= 0) printf("Le pas doit etre un nombre positif.\n\n--->");
        else
        {
            succes = 1;
        }
    }
    printf("Pas de calcul : %f\n", pas);

    lancerGrapheur(ac, av); // met en route l'initialisation du grapheur
}
