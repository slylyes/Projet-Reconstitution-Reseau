#include "Hachage.h"

unsigned long key(double x, double y){
    return y + ((x+y)*(x+y+1))/2;
}

unsigned long fonctionHachage(unsigned long key, int tailleMax){
    if (key<0)
    {
        printf("Impossible de hacher: la clef n'est pas valide\n");
        return -1;
    }
    const double A = (sqrt(5)-1)/2;
    //J'ai rajouté % tailleMax afin de l'indice de hachage est entre 0-500
    //sinon l'indice est large et ça ne range pas dans la table hachage
    return (unsigned long)(tailleMax * (key * A) - (int)(key * A)) % tailleMax;
}

Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage *H, double x, double y){

     if (R == NULL || H->nbElement<0)
    {
        printf("Il n'existe pas des éléments dans le réseau ou la table de hachage\n");
        return NULL;
    }
    
    //Calculer la clé à partir des coordonnées (x,y)
    unsigned long index = fonctionHachage(key(x,y),H->tailleMax);
    // Accéder à la liste chaînée correspondante dans la table de hachage
    CellNoeud* listeNoeuds = H->T[index];
    while (listeNoeuds != NULL){
        Noeud* noeud = listeNoeuds->nd;
        if (noeud->x == x && noeud->y == y){ // Trouvé !
            return noeud;
        }
        listeNoeuds = listeNoeuds->suiv;

    }

    // Si aucun nœud correspondant n'est trouvé, créer un nouveau nœud
    Noeud * nouveauNoeud = (Noeud*)malloc(sizeof(Noeud));
    nouveauNoeud->x = x;
    nouveauNoeud->y = y;
    nouveauNoeud->num = R->nbNoeuds+1;
    nouveauNoeud->voisins = NULL;


    // Ajouter le nouveau nœud à la liste chaînée dans la table de hachage
    CellNoeud* nouvelleCellule = (CellNoeud*)malloc(sizeof(CellNoeud));
    nouvelleCellule->nd = nouveauNoeud;
    nouvelleCellule->suiv = H->T[index];
    H->T[index] = nouvelleCellule;

    // Ajouter le nouveau nœud à la liste des nœuds du réseau R
    CellNoeud* nouvelleCelluleReseau = (CellNoeud*)malloc(sizeof(CellNoeud));
    nouvelleCelluleReseau->nd = nouveauNoeud;
    nouvelleCelluleReseau->suiv = R->noeuds;
    R->noeuds = nouvelleCelluleReseau;

    // Mettre à jour le nombre de noeuds dans le réseau
    R->nbNoeuds++;
    H->nbElement++;

    return nouveauNoeud;
}

TableHachage* creerTableHachage(int tailleMax){
    TableHachage *T = (TableHachage*)malloc(sizeof(TableHachage));
    T->nbElement = 0;
    T->tailleMax = tailleMax;
    // Calloc alloué de mémoire à partir de zero, il correspond pour la table de hachage plutot que la fonction malloc
    T->T = (CellNoeud**)calloc(tailleMax,sizeof(CellNoeud*));
    
    for (int i = 0; i < tailleMax; ++i) {
        T->T[i] = NULL; // Initialise chaque case comme une liste chaînée vide
    }

    return T;
}




Reseau* reconstitueReseauHachage(Chaines *C, int M){
    if (C == NULL || M<=0)
    {
        return NULL;
    }

    // Création du réseau
    Reseau* R = (Reseau*)malloc(sizeof(Reseau));
    R->nbNoeuds = 0;
    R->gamma = C->gamma;
    R->noeuds = NULL;
    R->commodites = NULL;
    
    // Création de la table de hachage
    TableHachage *H = creerTableHachage(M);

    if(H==NULL){
        printf("Erreur allocation mémoire\n");
        return NULL;
    }

    CellChaine* currentChaine = C->chaines;
    Noeud *extrA, *extrB, *nouveauNoeud;
    while (currentChaine != NULL)
    {
        CellPoint* currentPoint = currentChaine->points;
        Noeud* previous = NULL; // Noeud précédent
        
        while (currentPoint != NULL)
        {
            // Recherche ou création du noeud correspondant au point
            nouveauNoeud = rechercheCreeNoeudHachage(R,H,currentPoint->x,currentPoint->y);

            // Liaison avec le noeud précédent
            if(previous != NULL && previous != nouveauNoeud){
                // Mettre à jour des listes voisins
                mettreAJourVoisins(R,nouveauNoeud,previous);
            }
            else // if previous == NULL
            {
                // Affectation extrA est le premier noeud du chaine
                extrA = nouveauNoeud;
            }
            previous = nouveauNoeud;
            currentPoint= currentPoint->suiv;
        }
        //Conserver la commodité de la chaîne
        //Affectatation ExtrB est le dernier noeud du chaine
        extrB = nouveauNoeud;

        //création et ajout de la commodite
        CellCommodite *Commo = creerCommodite(extrA,extrB);
        Commo->suiv = R->commodites;
        R->commodites = Commo;

        currentChaine = currentChaine->suiv;
    }
    
    // Libération de la table de hachage
    libererTableHachage(H);

    return R;
}

// FONCTION DE LIBERATION DE LA MEMOIRE
void libererTableHachage(TableHachage *H) {
    if (H == NULL) {
        return;
    }
    for (int i = 0; i < H->tailleMax; ++i) {
        CellNoeud *current = H->T[i];
        while (current != NULL) {
            CellNoeud *temp = current;
            current = current->suiv;
            free(temp);
        }
    }
    free(H->T);
    free(H);
}