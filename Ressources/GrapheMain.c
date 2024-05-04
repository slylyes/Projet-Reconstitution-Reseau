#include <stdio.h>
#include <stdlib.h>
#include "ArbreQuat.h"
#include "Hachage.h"
#include "Reseau.h"
#include "Graphe.h"
#include "Struct_File.h"


int main(int argc, char** argv){
    if (argc != 2)
    {
        printf("Erreur : Manque / Trop d'arguments!\n");
        printf("Format de syntaxte: ./GrapheMain [fileData.cha]\n");
        exit(1);
    }

    FILE *rfgr = fopen(argv[1],"r");
    if (rfgr == NULL)
    {
        printf("Erreur ouverture de fichier fichierData.cha\n");
        exit(2);
    }

    Chaines *C  = lectureChaines(rfgr);
    fclose(rfgr);

    Reseau* RA = reconstitueReseauArbre(C);
    // Graphe* G = creerGraphe(R);

    if (reorganiseReseau(RA) == 1)
    {
        printf("Instance Valide? %s\n","Oui");
    }
    else
    {
        printf("Instance Valide? %s\n","Non");
    }
    
    libererChaines(C);
    libererReseau(RA);
    
    return 0;
}