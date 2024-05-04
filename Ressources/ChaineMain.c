#include "Chaine.h"


int main(int argc, char **argv){

	if(argc != 2){
		printf("Erreur : Manque / Trop d'arguments !\n"); 
		exit(1);
	} 

	FILE* fs = fopen(argv[1],"r");  // fichier source
	if(fs == NULL){
		printf("Erreur d'ouverture du fichier : Veuillez entrer un nom de fichier existant\n");
		exit(2);
	}
	Chaines* ch = lectureChaines(fs);

 	FILE* fd = fopen("new_chaine.txt","w");  // fichier destination 
 	ecrireChaines(ch,fd);

 	afficheChainesSVG(ch,"Instance");

	printf("Longueur de toutes les chaines: %.2lf\n",longueurTotale(ch));
    printf("Le nombre de points: %d\n",comptePointsTotal(ch));

	// Libération de la mémoire
 	fclose(fs);
 	fclose(fd);
	libererChaines(ch);
}