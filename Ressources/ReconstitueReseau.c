#include "ArbreQuat.h"

#define TAILLEHACHAGE 500

void methodeUtilisation(){
    printf("\n===================\n");
    printf("\nBonjour, quelles sont les méthodes que vous voulez effectuer?\n\n");
    printf("0-Sortie du programme\n");
    printf("1-Liste de Chaîné\n");
    printf("2-Table de Hachage\n");
    printf("3-Arbre binaire\n\n");
    printf("Veuillez entrer le chiffre correspondant : ");
    printf("\n=================\n");
}


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

	Chaines* C = lectureChaines(fs);

	int rep;
    do{
		methodeUtilisation();
    	scanf("%d",&rep);
        Reseau* R = NULL;
        FILE *f = NULL;
	    switch (rep){

		case 1: // LISTE CHAINEE
			R = reconstitueReseauListe(C);
            f = fopen("new_reseau_LC.txt","w");
            if (f == NULL){
            	fprintf(stderr,"Erreur d'ouverture du fichier");
                exit(3);
            }
            ecrireReseau(R,f);
            afficheReseauSVG(R,"Reseau_LC");
            fclose(f);
            libererReseau(R);
            break;

        case 2:  // TABLE DE HACHAGE
            R = reconstitueReseauHachage(C,TAILLEHACHAGE);
            f = fopen("new_reseau_Hachage.txt","w");
            if (f == NULL){
                fprintf(stderr,"Erreur d'ouverture du fichier");
                exit(3);
            }
            ecrireReseau(R,f);
            afficheReseauSVG(R,"Reseau_Hachage");
            fclose(f);
            libererReseau(R);

            ///////////////////////////////////////////////
            int testH;
            printf("Pour tester les clé de hachage, appuyez sur 1, sinon appuyez sur 0 : ");
            scanf("%d",&testH);
            if(testH==1){
                int x = 1,y = 1;
                for (int i = 1; i <= 10; i++)
                {
                    y = 1;
                    for (int j = 0; j < 10; j++)
                    {
                        printf("f(%d,%d), cles: %ld, hachage: %ld \n",x,y,key(x,y),fonctionHachage(key(x,y),500));
                        y++;
                    }
                    x++;
                }
            }
            ///////////////////////////////////////////////////////
            break;
          
        case 3: // ARBRE    
            R = reconstitueReseauArbre(C);
            f = fopen("new_reseau_Arbre.txt","w");
            if (f == NULL)
            {
                fprintf(stderr,"Erreur d'ouverture du fichier");
                exit(3);
            }
            ecrireReseau(R,f);
            afficheReseauSVG(R,"Reseau_Arbre");
            fclose(f);
            libererReseau(R);
            break;

        default:
            break;
        }

    }while (rep != 0);

	
	fclose(fs);
	libererChaines(C);

    return 0;
}

