#include "Reseau.h"

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){

	 if (R == NULL)
    {
        return NULL;
    }

	CellNoeud* n = R->noeuds;
	// Recherche du noeud
	while(n != NULL){
		if(n->nd->x == x && n->nd->y == y) return n->nd;  // Trouvé !
		else n = n->suiv;
	}
	// Création du noeud si pas trouvé
	Noeud* new = malloc(sizeof(Noeud));
	new->num = R->nbNoeuds+1;
	R->nbNoeuds++;
	new->x = x;
	new->y = y;
	new->voisins = NULL;
	// Création d'une nouvelle cellule pointant vers le nouveau noeud
	CellNoeud* newCell = malloc(sizeof(CellNoeud)); 
	newCell->nd = new;   // On fait pointer le noeud de la cellule vers le nouveau noeud
	newCell->suiv = R->noeuds; // Insertion en début de liste

    R->noeuds = newCell;
    return new;
}

//Création CellCommodite
CellCommodite* creerCommodite(Noeud* extrA, Noeud* extrB){
    if (extrA == NULL || extrB == NULL)
    {
        printf("Erreur création un CellCommodite\n");
        return NULL;
    }
    
    CellCommodite* commo = (CellCommodite*)malloc(sizeof(CellCommodite));
    commo->extrA = extrA;
    commo->extrB = extrB;
    
    return commo;
}

void mettreAJourVoisins(Reseau *R, Noeud* n, Noeud* v){
    if (n == NULL)
    {
        printf("Il n'existe pas de noeud pour mettre à jour des listes voisins\n");
        return;
    }
    if (v == NULL)
    {
        printf("Il n'existe pas de voisin pour mettre à jour des listes voisins\n");
        return;
    }
    
    // Verifier si ce noeud est déjà ajouté ce voisin
    CellNoeud* dejaPresent = n->voisins;
    while (dejaPresent != NULL)
    {
        if (dejaPresent->nd->x == v->x && dejaPresent->nd->y == v->y)
        {
            return;
        }
        dejaPresent = dejaPresent->suiv;
    }
    
    //Ajouter le nouveau noeud à la liste des voisins du voisin actuel
    CellNoeud *nouveauVoisin = (CellNoeud*)malloc(sizeof(CellNoeud));
    nouveauVoisin->nd = n;
    nouveauVoisin->suiv = v->voisins;
    v->voisins = nouveauVoisin;
    
    //Mettre à jour le voisin actuel à la liste des voisins du nouveau noeud
    CellNoeud *nouveauVoisinDuNoeud =(CellNoeud*)malloc(sizeof(CellNoeud));
    nouveauVoisinDuNoeud->nd = v;
    nouveauVoisinDuNoeud->suiv = n->voisins;
    n->voisins = nouveauVoisinDuNoeud;
}


Reseau* reconstitueReseauListe(Chaines *C){
    if(C == NULL){
        printf("Il n'existe pas de chaines pour reconstitueReseauListe\n");
        return NULL;
    }

    Reseau *R =(Reseau*)malloc(sizeof(Reseau));

    R->nbNoeuds = 0;
    R->gamma = C->gamma;
    R->noeuds = NULL;
    R->commodites = NULL;

    CellChaine * currentChaine = C->chaines;
    Noeud *extrA, *extrB, *nouveauNoeud;
    while(currentChaine != NULL){
        CellPoint * currentPoint = currentChaine -> points;
        Noeud *previous = NULL; // Noeud précédent

        while(currentPoint != NULL){
            // Recherche ou création du noeud correspondant au point
            nouveauNoeud = rechercheCreeNoeudListe(R,currentPoint->x,currentPoint->y);

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

    //Retourner le résseau reconstruit
    return R;
}

int nbLiaisons(Reseau *R){
    CellNoeud* listenoeuds = R->noeuds;
    int res = 0;
    //Parcourir la liste des noeuds
    while (listenoeuds != NULL)
    {
        //Parcourir la liste des voisins de chaque noeud
        CellNoeud* voisin = listenoeuds->nd->voisins;
        while (voisin != NULL)
        {
            res+=1;
            voisin = voisin->suiv;
        }
        listenoeuds = listenoeuds->suiv;
    }
    //Chaque liaison est comptée deux fois (une fois pour chaque noeud), donc on divise par 2
    return res/2;
}

int nbCommodites(Reseau *R){
    CellCommodite* listeCommo = R->commodites;
    int cpt = 0;
    while (listeCommo != NULL)
    {
        cpt++;
        listeCommo = listeCommo->suiv;
    }
    return cpt;
}

void ecrireReseau(Reseau* R, FILE *f){
    if(R == NULL || f == NULL){
        return;
    }
    fprintf(f,"NbNoeuds: %d\n", R->nbNoeuds);
    fprintf(f,"NbLiaisons: %d\n",nbLiaisons(R));
    fprintf(f,"NbCommodites: %d\n",nbCommodites(R));
    fprintf(f,"Gamma: %d\n", R->gamma);

    //Ecrire les noeuds du reseau
    CellNoeud *currentNoeud = R->noeuds;
    while(currentNoeud != NULL){
        Noeud *noeud = currentNoeud->nd;
        fprintf(f,"v %d %lf %lf\n", noeud->num,noeud->x,noeud->y);
        currentNoeud = currentNoeud->suiv;
    }
    fprintf(f,"\n");

    //Ecrire les liaisons de chaque noeud
    currentNoeud = R->noeuds;
    while(currentNoeud!= NULL){
        Noeud *noeud = currentNoeud->nd;
        CellNoeud *lesvoisins = noeud->voisins;
        while(lesvoisins != NULL){
            Noeud *voisin = lesvoisins->nd;
            if(noeud->num < voisin->num){
                fprintf(f,"l %d %d\n",noeud->num, voisin->num);
            }
            lesvoisins = lesvoisins->suiv;
        }
        currentNoeud = currentNoeud-> suiv;
    }
    fprintf(f,"\n");

    //Ecrire les commodite des noeuds
    CellCommodite *currentCellCo = R-> commodites;
    while(currentCellCo!= NULL){
        fprintf(f,"k %d %d\n",currentCellCo->extrA->num,currentCellCo->extrB->num);
        currentCellCo = currentCellCo->suiv;
    }

}

void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}

// FONCTIONS POUR LIBERER LA MEMOIRE

void libererReseau(Reseau* res){
    /*
    Libère toute la mémoire associée au réseau res.
    */
    if(res == NULL){
        return;
    }

    // Libération de la mémoire associée aux commodités:
    CellCommodite* tmp_com;
    while(res-> commodites){
        tmp_com = (res-> commodites);
        (res-> commodites) = (res-> commodites)-> suiv;
        free(tmp_com);
    }

    // Libération de la mémoire associée aux noeuds et à leurs voisins:

    CellNoeud* tmp;
    CellNoeud* tmp_voisin;

    while(res-> noeuds){

        tmp = (res-> noeuds);
        tmp_voisin = (tmp-> nd)-> voisins;
        
        while(tmp_voisin){
            (tmp-> nd-> voisins) = (tmp-> nd-> voisins)-> suiv;
            free(tmp_voisin);
            tmp_voisin = tmp-> nd-> voisins;
        }

        (res-> noeuds) = (res-> noeuds)-> suiv;

        free(tmp-> nd);
        free(tmp);
    }

    free(res);

}