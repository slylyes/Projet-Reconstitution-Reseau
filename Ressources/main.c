#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ArbreQuat.h"
#include "Hachage.h"
#include "Reseau.h"
#include "Chaine.h"

#define TAILLEHACHAGE 5000

Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax){
    Chaines *C = (Chaines*)malloc(sizeof(Chaines));
    C->nbChaines = nbChaines;
    C->gamma = 3;
    C->chaines = NULL;

    srand(time(NULL)); //Initialisation du générateur aléatoire
    double x,y;

    for (int i = 0; i < nbChaines; i++)
    {
        // Créer CelluleChaine et ajout en tete 
        CellChaine* nouvellChaine = (CellChaine*)malloc(sizeof(CellChaine));
        nouvellChaine->numero = i+1;
        nouvellChaine->points = NULL;
        nouvellChaine->suiv = C->chaines;
        C->chaines = nouvellChaine; 
        for (int j = 0; j < nbPointsChaine; j++)
        {
            x = (double)(rand() % (xmax+1)); //Aléatoire x entre 0 -> xmax inlus
            y = (double)(rand() % (ymax+1)); //Aléatoire y entre 0 -> ymax inlus

            // Créer un point et ajout en tete 
            CellPoint* nouveauPoint = (CellPoint*)malloc(sizeof(CellPoint));
            nouveauPoint->x = x;
            nouveauPoint->y = y;
            nouveauPoint->suiv = C->chaines->points;
            C->chaines->points = nouveauPoint;
        }
    }
    return C;
}

int main(int argc, char** argv){
    printf("Attention, ce programme prend plusieurs heures (environ 10 heures) a s'executer. Voulez vous continuer?\n[1: Oui / 0: Non]\n");
    
    char input[256];
    fgets(input, sizeof(input), stdin);
    int rep;

    // Si on ne veut pas continuer le programe avec le choix 0 ou n'importe quelle de chiffre sauf 1
    if ((sscanf(input,"%d",&rep) != 1) || rep == 0)
    {
        printf("Arret du programme\n");
        return 0;
    }
    
    clock_t debut1,fin1;
    double tempsListe, tempsHachage, tempsArbre;

    FILE* f = fopen("time.txt","w");
    if (f == NULL)
    {
        printf("Erreur ouverture fichier timedata.txt\n");
        return 1;
    }
    int i = 0;
    for (int nbChaines = 500; nbChaines <= 5000; nbChaines+=500)
    {
        Chaines* C = generationAleatoire(nbChaines,100,5000,5000);

        // Liste
        printf("========\n");
        printf("DebutListe\n");
        // Mesurer le temps d'execution de liste chainée
        debut1 = clock();
        Reseau* r1 = reconstitueReseauListe(C);
        fin1 = clock();
        printf("FinListe\n");
        tempsListe = (double)(fin1-debut1) / CLOCKS_PER_SEC;
        printf("========\n");
        // Mesurer le temps d'execution de tableau de hachage
        
        printf("DebutHachage\n");
        debut1 = clock();
        Reseau* r2 = reconstitueReseauHachage(C,TAILLEHACHAGE);
        fin1 = clock();
        printf("FinHachage\n");
        tempsHachage = (double)(fin1-debut1) / CLOCKS_PER_SEC;
        printf("========\n");

        // Mesurer le temps d'execution d'arbre
        printf("DebutArbre\n");
        debut1 = clock();
        Reseau* r3 = reconstitueReseauArbre(C);
        fin1 = clock();
        printf("FinArbre\n");
        tempsArbre = (double)(fin1-debut1) / CLOCKS_PER_SEC;
        printf("========\n");

        // Écrire le temps calcul dans le fichier timedata.txt
        fprintf(f,"%lf %lf %lf\n",tempsListe,tempsHachage,tempsArbre);

        // Imprimer les étapes effectués
        printf("Fini Etape: %d/10\n",++i);

        //Libération des réseau
        libererReseau(r1);
        libererReseau(r2);
        libererReseau(r3);
        //Libération la chaine
        libererChaines(C);
    }
    fclose(f);

    printf("Après avoir effectué, la commande pour représenter le graphiquement est suivante: \n");
    printf("gnuplot RepresentationTempsCalcul.gnu\n");
    return 0;
}