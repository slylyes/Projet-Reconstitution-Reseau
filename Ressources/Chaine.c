#include "Chaine.h"

Chaines* lectureChaines(FILE* f){

	// LECTURE DE GAMMA ET NBCHAINE : 

	// Variables
	Chaines* ch = malloc(sizeof(Chaines));
	char buffer[256];
	int gamma,nbChaines;
	// Lecture
	fgets(buffer,256,f);
	sscanf(buffer,"NbChain: %d",&nbChaines);
	fgets(buffer,256,f);
	sscanf(buffer,"Gamma: %d",&gamma);
	// Attribution des valeurs
	ch->gamma = gamma;
	ch->nbChaines = nbChaines;
	
	// LECTURE DES CHAINES : 

	// Variables
	CellChaine* cellCh = malloc(sizeof(CellChaine));
	ch->chaines = cellCh;   		// premier élément de liste de chaines
	CellChaine* tmp = cellCh;   // Variable temporaire pour garder la tete de liste
	int num,nbPts;

	// Parcours des chaines
	for(int ligne=0; ligne<nbChaines; ligne++){
		
		// Lecture de num et nb points
		fscanf(f,"%d %d ",&num,&nbPts);
		// Attribution des valeurs
		tmp->numero = num;

		// LECTURE DES POINTS :
		CellPoint* cellP = malloc(sizeof(CellPoint));
		tmp->points = cellP; 		// Premier élément de la liste de points
		CellPoint* tmp2 = cellP;
		for(int i=0; i<nbPts; i++){
			double x,y;
			fscanf(f,"%lf %lf ",&x,&y);
			tmp2->x = x;
			tmp2->y = y;
			if(i != nbPts-1){
				tmp2->suiv = malloc(sizeof(CellPoint));
				tmp2 = tmp2->suiv;
			}
			else tmp2->suiv = NULL;
		}
		fscanf(f,"\n");
		if(ligne != nbChaines-1){
			tmp->suiv = malloc(sizeof(CellChaine));
			tmp = tmp->suiv;
		}
		else tmp->suiv = NULL;
		
	}
	return ch;
}


void ecrireChaines(Chaines *C, FILE *f){

	// Ecritre de nbChain et Gammaa : 
	fprintf(f,"NbChain: %d\n",C->nbChaines);
	fprintf(f,"Gamma: %d\n",C->gamma);

	// Parcours des chaines
	CellChaine* cellCh = C->chaines;
	for(int ligne=0; ligne < C->nbChaines; ligne++){

		// On parcours les points de la chaine pour connaitre le nombre de points
		CellPoint* cellP = cellCh->points;
		int i=0;
		while(cellP->suiv != NULL){
			i++;
			cellP = cellP->suiv;
		}
		// Ecriture du numero et nb points
		fprintf(f,"%d %d ",cellCh->numero,i);

		// Parcours des points de la chaine
		CellPoint* tmp = cellCh->points;
		while(tmp->suiv != NULL){
			fprintf(f,"%.2lf %.2lf ",tmp->x,tmp->y);
			tmp = tmp->suiv;
		}
		fprintf(f,"\n");
		cellCh = cellCh->suiv;

	}

}

void afficheChainesSVG(Chaines *C, char* nomInstance){
    int i;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}

double longueurChaine(CellChaine *c){
    if (c == NULL)
    {
        printf("Il est impossible de compter la longueur de chaine car il n'existe pas des chaines\n");
        return 0.0;
    }
    double res = 0.0;
    CellPoint *tmp = c->points;
    // Calculer la longueur d'entre 2 points de la chaîne
    while (tmp->suiv != NULL)
    {
        CellPoint* pointA = tmp;
        CellPoint* pointB = tmp->suiv;
        res = res + (double)sqrt(pow(pointB->x - pointA->x,2) + pow(pointB->y - pointA->y,2)); // Res = res + longueurEntre2points
        tmp = tmp->suiv;
    }

    return res;
}

double longueurTotale(Chaines *C){
	if (C == NULL)
    {
        printf("Il est impossible de compter la longueur de chaine car il n'existe pas des chaines\n");
        return 0.0;
    }
	CellChaine* tmp = C->chaines;
	double s = 0;
	while(tmp != NULL){
		s += longueurChaine(tmp);
		tmp = tmp->suiv;
	}
	return s;
}

int comptePointsTotal(Chaines *C){
	if (C == NULL)
    {
        printf("Il est impossible de compter les points car il n'existe pas des chaines\n");
        return -1;
    }
	CellChaine* ch = C->chaines;
	int s = 0;
	while(ch != NULL){
		CellPoint* pts = ch->points;
		while(pts != NULL){
			s++;
			pts = pts->suiv;
		}
		ch = ch->suiv;
	}
	return s;
}

// FONCTIONS LIBERATION DE LA MEMOIRE

void liberer_point(CellPoint *point) {
	if (point == NULL)
	{
		printf("Le point a déjà libéré\n");
		return;
	}
	
    free(point);
}

void liberer_liste_points(CellPoint *liste) {
	if (liste == NULL)
	{
		printf("Litse Points ont déjà libérés\n");
	}
	
    CellPoint *tmp = NULL;
    while (liste) {
        tmp = liste->suiv;
        liberer_point(liste);
        liste = tmp;
    }
}

void liberer_chaine(CellChaine *chaine) {
    if (chaine == NULL){
		printf("une chaine a déjà libérés\n");
		return;
	}
    liberer_liste_points(chaine->points);
    free(chaine);
}


void liberer_liste_chaines(CellChaine *liste) {
	if (liste == NULL)
	{
		printf("Des listes chaines a déjà libérés\n");
		return;
	}
	
    CellChaine *tmp = NULL;
    while (liste) {
        tmp = liste->suiv;
        liberer_chaine(liste);
        liste = tmp;
    }
}

void libererChaines(Chaines *all) {
    if (all == NULL){
		printf("Des Chaines a déjà libérés\n");
		return;
	}
    liberer_liste_chaines(all->chaines);
    free(all);
}