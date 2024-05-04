#include <stdio.h>
#include <stdlib.h>

#include "ArbreQuat.h"
#include "Hachage.h"

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){
    if (C == NULL)
    {
        printf("Il ne détermine pas des coordonnées min et max de chaines\n");
        return;
    }

    // Initialiser xmin, ymin, xmax, ymax au 1er points du chaine
    *xmin = C->chaines->points->x;
    *xmax = C->chaines->points->x;
    *ymax = C->chaines->points->y;
    *ymin = C->chaines->points->y;
    
    CellChaine* currentChaine = C->chaines;
    while (currentChaine != NULL)
    {
        CellPoint* currentPoint = currentChaine->points;
        while (currentPoint != NULL)
        {
            //Mettre à jour les coordonnées maximales et minimales des points dans la chaines du réseau 
            if (currentPoint->x < *xmin)
            {
                *xmin = currentPoint->x;
            }
            if (currentPoint->x > *xmax)
            {
                *xmax = currentPoint->x;
            }
            if (currentPoint->y < *ymin)
            {
                *ymin = currentPoint->y;
            }
            if (currentPoint->y > *ymax)
            {
                *ymax = currentPoint->y;                
            }
            currentPoint = currentPoint->suiv;
        }
        currentChaine = currentChaine->suiv;
    }
}


ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){
    ArbreQuat* newArbreQuat = (ArbreQuat*)malloc(sizeof(ArbreQuat));

    newArbreQuat->coteX = coteX;
    newArbreQuat->coteY = coteY;
    newArbreQuat->xc = xc;
    newArbreQuat->yc = yc;
    newArbreQuat->noeud = NULL;
    newArbreQuat->so = NULL;
    newArbreQuat->se = NULL;
    newArbreQuat->no = NULL;
    newArbreQuat->ne = NULL;

    return newArbreQuat;
}

void libererArbreQuat(ArbreQuat* a){
    if (a == NULL)
    {
        printf("ArbreQuat est déjà libéré\n");
        return;
    }
    a->noeud = NULL;

    if (a->so != NULL)
    {
        libererArbreQuat(a->so);
    }
    if (a->se != NULL)
    {
        libererArbreQuat(a->se);
    }
    if (a->no != NULL)
    {
        libererArbreQuat(a->no);
    }
    if (a->ne != NULL)
    {
        libererArbreQuat(a->ne);
    }
    
    free(a);
}

ArbreQuat** determinerDirection(ArbreQuat* arbre, double x, double y){
    if (x < arbre->xc)
    {
        // Sud-Ouest
        if (y < arbre->yc)
        {
            return &(arbre->so);
        }
        // Nord-Ouest
        else
        {
            return &(arbre->no);
        }
    }
    else
    {
        //Nord-Est
        if (y >= arbre->yc)
        {
            return &(arbre->ne);
        }
        //Sud-Est
        else
        {
            return &(arbre->se);
        }
    }
}

void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent) {
    
    // CAS : ARBRE VIDE
    if (*a == NULL)
    {
        double xc,yc,coteX,coteY;
        coteX = (parent->coteX)/2; // current Longueur coteX = 5 si coteX origine = 10
        coteY = (parent->coteY)/2; // current Hauteur coteY = 5 si coteY origine = 10

        // Partie Sud Ouest
        if (n->x < parent->xc && n->y < parent->yc)
        {
            // les coordonnées et les dimensions du centre 3eme rectangulaire 
            xc = (parent->xc) - (parent->coteX)/4; //Si parent->xc = 0, parent->coteX = 10 ==> xc = -2,5
            yc = (parent->yc) - (parent->coteY)/4; //Si parent->yc = 0, parent->coteY = 10 ==> yc = -2,5
        }

        // Partie Nord Ouest
        if (n->x < parent->xc && n->y > parent->yc)
        {
            // les coordonnées et les dimensions du centre 1eme rectangulaire
            xc = (parent->xc) - (parent->coteX)/4; // xc = -2,5
            yc = (parent->yc) + (parent->coteY)/4; // yc = 2.5
        }
        
        // Partie Sud Est
        if (n->x > parent->xc && n->y < parent->yc) 
        {
            // les coordonnées et les dimensions du centre 4eme rectangulaire
            xc = (parent->xc) + (parent->coteX)/4; // xc = 2.5
            yc = (parent->yc) - (parent->coteY)/4; // yc = -2.5
        }

        // Partie Nord Est
        if (n->x > parent->xc && n->y > parent->yc)
        {
                // les coordonnées et les dimensions du centre 2eme rectangulaire
                xc = (parent->xc) + (parent->coteX)/4; // xc = 2.5
                yc = (parent->yc) + (parent->coteY)/4; // yc = 2.5
        }
        

        // Création de l'arbre
        *a = creerArbreQuat(xc,yc,coteX,coteY);
        (*a)->noeud = n;
    }

    // CAS : FEUILLE
    else if ((*a)->noeud != NULL) {
        // ancien Noeud de la feuille
        Noeud* ancienNoeud = (*a)->noeud;
        (*a)->noeud = NULL;  // On enleve le noeud et mettre NULL

        // Inserer de l'ancien noeud dans la quat de la feuille
        // Déterminer les coordonnées sont quels quats
        insererNoeudArbre(ancienNoeud, determinerDirection(*a,ancienNoeud->x,ancienNoeud->y), *a);

        // Inserer de le nouveau noeud dans la quat de la feuille
        insererNoeudArbre(n, determinerDirection(*a,n->x,n->y), *a);
    }

    // CAS : CELLULE INTERNE
    else { //((*a)->noeud == NULL && (*a) != NULL)
        insererNoeudArbre(n,determinerDirection(*a,n->x,n->y),*a);
    }
}


Noeud* creerNoeud(int num, double x, double y){
    Noeud* nouveauNoeud = (Noeud*)malloc(sizeof(Noeud));
    nouveauNoeud->num = num;
    nouveauNoeud->x = x;
    nouveauNoeud->y = y;
    nouveauNoeud->voisins = NULL; // Initialiser la liste des voisins à NULL
    return nouveauNoeud;
}

void insererNoeudReseau(Reseau* R, Noeud* nouveauNoeud) {
    CellNoeud* nouvelleCellule = (CellNoeud*)malloc(sizeof(CellNoeud));
    nouvelleCellule->nd = nouveauNoeud;
    nouvelleCellule->suiv = R->noeuds;
    R->noeuds = nouvelleCellule;
    R->nbNoeuds++;
}


Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y){
    if (R == NULL)
    {
        printf("Il n'existe pas de réseau\n");
        return NULL;
    }
    if (parent == NULL)
    {
        printf("Il n'existe pas parent de l'abre\n");
        return NULL;
    }
    
    // ARBRE VIDE
    if (*a == NULL)
    {
        Noeud* nouveauNoeud = creerNoeud(R->nbNoeuds+1,x,y); // Création du noeud
        insererNoeudReseau(R,nouveauNoeud); // On l'insere dans le reseau
        
        insererNoeudArbre(nouveauNoeud,a,parent); // On l'insere dans l'arbre
        return nouveauNoeud;
    }
    
    // FEUILLE
    else if ((*a)->noeud != NULL)
    {
        // Le noeud recherché est déjà dans cette feuille
        if ((*a)->noeud->x == x && (*a)->noeud->y == y)
        {
            return (*a)->noeud;
        }
        else
        {
            // Créer le nouveau nœud et l'ajouter au réseau
            Noeud* nouveauNoeud = creerNoeud(R->nbNoeuds+1,x,y);
            insererNoeudReseau(R,nouveauNoeud);
            // Insérer le nouveau nœud dans l'arbre quaternaire
            insererNoeudArbre(nouveauNoeud,a,parent);
            return nouveauNoeud;
        }
    }

    // CELLULE INTERNE
    else // (*a != NULL) && (*a)->noeud == NULL
    {
        // Déterminer dans quelle cellule de l'arbre chercher le noeud
        return rechercheCreeNoeudArbre(R,determinerDirection(*a,x,y),*a,x,y);
    }
}



Reseau* reconstitueReseauArbre(Chaines *C){
    if (C == NULL)
    {
        return NULL;
    }

    // Création Reseau
    Reseau* R = (Reseau*)malloc(sizeof(Reseau));
    R->nbNoeuds = 0;
    R->gamma = C->gamma;
    R->noeuds = NULL;
    R->commodites = NULL;

    // Determination coordonnées min et max de Arbre
    double xmin,ymin,xmax,ymax;
    chaineCoordMinMax(C,&xmin,&ymin,&xmax,&ymax);
    double coteX = xmax-xmin; // EX : xmax = 5, xmin = -5 -> coteX = 10
    double coteY = ymax-ymin; // EX : ymax = 5, ymin = -5 -> coteY = 10
    double xc = (xmax+xmin)/2; // xc = (5-5)/2 = 0 -> coorX est centrale
    double yc = (ymax+ymin)/2; // yc = (5-5)/2 = 0 -> coorY est centrale
    
    //Création ArbreQuat
    ArbreQuat* arbre = creerArbreQuat(xc,yc,coteX,coteY);

    // Parcourir la liste des chaînes
    CellChaine* currentChaine = C->chaines;
    Noeud *extrA, *extrB, *noeud;
    ArbreQuat** arbreFils;
    while (currentChaine != NULL)
    {
        // Parcourir les points de la chaîne
        CellPoint* currentPoint = currentChaine->points;
        Noeud *previous = NULL;
        while (currentPoint != NULL)
        {
            arbreFils = determinerDirection(arbre,currentPoint->x,currentPoint->y);

            // Rechercher ou créer un nœud correspondant dans l'arbre quaternaire
            noeud = rechercheCreeNoeudArbre(R,arbreFils,arbre,currentPoint->x,currentPoint->y);

            if (previous != NULL && previous != noeud)
            {
                // Mettre à jour des listes voisins
                mettreAJourVoisins(R,noeud,previous);
            }
            else // if previous == NULL
            {
                // Affectation extrA est le premier noeud du chaine
                extrA = noeud;
            }
            previous = noeud;
            currentPoint = currentPoint->suiv;
        }

        //Conserver la commodité de la chaîne
        //Affectatation ExtrB est le dernier noeud du chaine
        extrB = noeud;

        //création et ajout de la commodite
        CellCommodite *Commo = creerCommodite(extrA,extrB);
        Commo->suiv = R->commodites;
        R->commodites = Commo;

        currentChaine = currentChaine->suiv;
    }
    
    // Libération de l'arbreQuat
    libererArbreQuat(arbre);

    return R;
}



