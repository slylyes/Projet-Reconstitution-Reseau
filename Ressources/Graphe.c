#include <stdio.h>
#include <stdlib.h>
#include "Reseau.h"
#include "Chaine.h"
#include "Graphe.h"
#include "Struct_File.h"

Graphe* creerGraphe(Reseau* R){
    if (R == NULL)
    {
        printf("Le réseau n'existe pas\n");
        return NULL;
    }
    Graphe* graphe = (Graphe*)malloc(sizeof(Graphe));

    graphe->nbsom = R->nbNoeuds; //Nombre de sommets
    graphe->gamma = R->gamma;
    graphe->T_som = (Sommet**)malloc(graphe->nbsom * sizeof(Sommet*));


    //Créer les sommets à partir des noeuds du réseau
    CellNoeud* currentNoeud = R->noeuds;
    int i = 0;
    while (currentNoeud != NULL)
    {
        Sommet *S = (Sommet*)malloc(sizeof(Sommet));
        S->num = currentNoeud->nd->num; // Numéro du sommet
        S->x = currentNoeud->nd->x; // CoorX
        S->y = currentNoeud->nd->y; // CoorY
        S->L_voisin = NULL; //Initialiser la liste des voisins

        graphe->T_som[i++] = S; // Ajouter au tableau des sommets

        currentNoeud = currentNoeud->suiv;
    }
    
    //Créer les aretes entre les sommets
    for (int j = 0; j < graphe->nbsom; j++)
    {
        Sommet* S = graphe->T_som[j];
        CellNoeud* voisin = R->noeuds->nd->voisins;

        while (voisin != NULL)
        {
            if (S->num < voisin->nd->num)
            {
                //Créer une arete
                Cellule_arete* CellA = (Cellule_arete*)malloc(sizeof(Cellule_arete));
                Arete* a = (Arete*)malloc(sizeof(Arete));

                CellA->a = a;
                CellA->suiv = S->L_voisin; // Ajout à la liste des voisins
                S->L_voisin = CellA; //Mise à jour de la liste des voisins
                a->u = S->num; // Numéro du sommet u
                a->v = voisin->nd->num; //Numéro du sommet v
            }
            voisin = voisin->suiv;
        }
    }

    // Créer les commodités
    graphe->nbcommod = nbCommodites(R);
    Commod* commod = (Commod*)malloc(graphe->nbcommod * sizeof(Commod));
    graphe->T_commod = commod;

    CellCommodite* currentCommod = R->commodites;
    i = 0; //indice de tableau T_commod
    while (currentCommod != NULL)
    {
        graphe->T_commod[i].e1 = currentCommod->extrA->num; //extr1
        graphe->T_commod[i].e2 = currentCommod->extrB->num; //extr2
        currentCommod = currentCommod->suiv;
        i++; 
    }
    
    return graphe;
}

void libererGraphe(Graphe* G){
    if (G == NULL)
    {
        printf("Graphe est déjà libéré\n");
        return;
    }

    if (G->T_commod != NULL)
    {
        free(G->T_commod);
        G->T_commod = NULL;
    }
    
    // Libérer les sommets et les arêtes
    for (int i = 0; i < G->nbsom; i++)
    {
        Sommet* S = G->T_som[i];

        //Liberer les aretes pour chaque sommet
        Cellule_arete* current = S->L_voisin;
        while (current != NULL)
        {
            Cellule_arete* tmp = current;
            current = current->suiv;
            if (tmp->a != NULL)
            {
                free(tmp->a);  // Libérer l'arête
            }
            free(tmp);  // Libérer la cellule de l'arête
        }
        free(S);  // Libérer le sommet lui-même
    }

    // Libérer le tableau des sommets
    free(G->T_som);
    // Libérer le graphe lui-même
    free(G);
}

int parcoursEnLargeur(Graphe* graphe, int debut, int fin){
    if (graphe == NULL)
    {
        return -1; // Le graphe est invalide
    }
    
    if (debut < 0 || debut>=graphe->nbsom || fin < 0 || fin >= graphe->nbsom)
    {
        return -2; // Indices de départ ou de fin invalides
    }
    
    if (debut == fin)
    {
        return 0; // Si start et end sont les mêmes, pas besoin de parcourir des arêtes
    }
    
    // Créer une file pour parcours en largeur
    S_file* queue = creerFile();
    enfile(queue,debut);

    // Tableau pour stocker les distances
    int* distances = (int*)malloc(graphe->nbsom * sizeof(int));
    for (int i = 0; i < graphe->nbsom; i++)
    {
        distances[i] = -1;  // Initialiser les distances à -1 (non visité)
    }
    distances[debut] = 0;   // Distance du point de départ

    //Parcours en largeur
    while (!estFileVide(queue))
    {
        int current = defile(queue);  // Récupérer le premier élément de la file

        // Parcourir les voisins du sommet actuel
        Cellule_arete* voisin = graphe->T_som[current]->L_voisin;
        while (voisin != NULL)
        {
            int voisinNum = (voisin->a->u == current) ? voisin->a->v : voisin->a->u; // Trouver le voisin

            if (distances[voisinNum] == -1) // Si non visité
            {
                distances[voisinNum] = distances[current] + 1; // Mettre à jour la distance
                enfile(queue,voisinNum); // Ajouter le voisin à la file

                if (voisinNum == fin) // Si c'est le sommet final
                {
                    int distance = distances[voisinNum];
                    libererFile(queue);
                    free(distances);
                    return distance;
                }
                
            }
            voisin = voisin->suiv;
        }
    }
    
    libererFile(queue);
    free(distances);

    return -1;  // Si aucun chemin n'a été trouvé
}

// Ajoute un élément en tete de la liste
void ajouteElement(ListeNode **tete, int value){
    ListeNode* newNode = (ListeNode*)malloc(sizeof(ListeNode));
    newNode->val = value;
    newNode->suiv = *tete;
    *tete = newNode;
}


void libererListe(ListeNode* tete){
    if (tete == NULL)
    {
        printf("la liste node est déjà libéré\n");
        return;
    }
    while (tete != NULL)
    {
        ListeNode* tmp = tete;
        tete = tete->suiv;
        free(tmp);
    }
}

// Parcours en largueur avec prédécesseurs pour trouver le chemin
ListeNode* parcoursEnLargeurListe(Graphe* graphe, int debut, int fin){
    if (graphe == NULL)
    {
        return NULL; // Le graphe est invalide
    }

    if (debut < 0 || debut>=graphe->nbsom || fin < 0 || fin >= graphe->nbsom)
    {
        return NULL; // Indices de départ ou de fin invalides
    }
    
    if (debut == fin)
    {
        ListeNode* res = NULL;
        ajouteElement(&res, debut);
        return res; // Si debut et fin sont les mêmes sommets
    }

    S_file * queue = creerFile();
    enfile(queue,debut);

    // Tableau pour stocker les prédécesseurs
    int* predecesseur = (int*)malloc(graphe->nbsom * sizeof(int));
    for (int i = 0; i < graphe->nbsom; i++)
    {
        predecesseur[i] = -1; //-1 indique qu'il n'y a pas de prédécesseur (non visité) Faux
    }
    
    predecesseur[debut]= debut; // le point de départ est son propre prédécesseur

    // Parcours en largueur pour trouver le chemin
    int trouver = 0; // non visité (Faux)
    while (!estFileVide(queue) && !trouver)
    {
        int currentSommet = defile(queue); // Récupérer le premier élément de la file et supprimez-le de la queue

        Cellule_arete* voisins = graphe->T_som[currentSommet]->L_voisin;
        while (voisins != NULL)
        {
            int voisin = (voisins->a->u == currentSommet) ? voisins->a->v : voisins->a->u;
            if (predecesseur[voisin] == -1) // Si le sommet non visité
            {
                predecesseur[voisin] = currentSommet; // mettre à jour le prédécesseur
                enfile(queue, voisin); // Ajouter à la file

                if (voisin == fin) // Si c'est le sommet final
                {
                    trouver = 1;
                    break;
                }
            }
            voisins = voisins->suiv;
        }
    }
    
    // Reconstituer le chemin en utilisant les prédécesseurs
    ListeNode* path = NULL;
    if (trouver == 1) // Si déjà visité
    {
        int currentS = fin;
        while (currentS != debut)
        {
            ajouteElement(&path,currentS);
            currentS = predecesseur[currentS]; // Aller au prédécesseur
        }
        ajouteElement(&path, debut); // Ajouter le point de départ
    }
    
    //Libération de la mémoire
    libererFile(queue);
    free(predecesseur);

    return path;
}

// Compte le nombre de chaînes passant par chaque arête
void compterChainesParArete(Graphe* graphe, ListeNode* chemin, int** matriceComptage){
    if (graphe == NULL)
    {
        printf("Il est impossible de compter des chaines car le graphe est null\n");
        return;
    }
    
    ListeNode* current = chemin;
    while (current != NULL && current->suiv != NULL)
    {
        int u = current->val;
        int v = current->suiv->val;

        // Indices dans l'ordre croissant pour une arête {u, v}
        int min = (u < v) ? u : v;
        int max = (u > v) ? u : v;

        matriceComptage[min][max]++; // Incrémente le compteur
        current = current->suiv;
    }
}

int reorganiseReseau(Reseau* R){
    if (R == NULL)
    {
        printf("Return 0 car il n'existe pas de réseau pour reorganiser du réseau\n");
        return 0; // Retourne faux s'il n'y a pas de réseau
    }
    
    // Créer le graphe correspondant au réseau
    Graphe* graphe = creerGraphe(R);
    if (graphe == NULL)
    {
        printf("Return 0 car il n'existe pas de graphe crée\n");
        return 0;
    }
    
    int nombreSom = graphe->nbsom; // Nombre de sommets
    int gamma = R->gamma;

    // Matrice de comptage des arêtes
    int **matriceComptage = (int**)malloc(nombreSom * sizeof(int*));
    for (int i = 0; i < nombreSom; i++)
    {
        matriceComptage[i] = (int*)calloc(nombreSom,sizeof(int)); //Initialiser à zero
    }

    ListeNode* chemin = NULL;
    // Parcourir les commodités et trouver les plus courts chemins
    for (int j = 0; j < graphe->nbcommod; j++)
    {
        int u = graphe->T_commod[j].e1;
        int v = graphe->T_commod[j].e2;

        // Obtenir le plus court chemin entre u et v
        chemin = parcoursEnLargeurListe(graphe,u,v);

        // Compter les chaînes passant par chaque arête
        compterChainesParArete(graphe,chemin,matriceComptage);

        // Libérer la liste après utilisation
    }
    
    // Vérifier si le nombre de chaînes par arête est inférieur à gamma
    int valide = 1; // 1 pour vrai, 0 pour faux
    for (int ligne = 0; ligne < nombreSom; ligne++)
    {
        for (int colonne = 0; colonne < nombreSom; colonne++)
        {
            if (matriceComptage[ligne][colonne] > gamma)
            {
                valide = 0; // Trop de chaines passent par cette arete
                break;
            }
            
        }
        if (valide == 0)
        {
            break; // Sortir si une arete dépasse la limite
        }
    }
    
    // Libérer la matrice de comptage
    for (int k = 0; k < nombreSom; k++)
    {
        free(matriceComptage[k]);
    }
    free(matriceComptage);
    
    //Libérer la Liste Node
    libererListe(chemin); 

    // Libérer le graphe
    libererGraphe(graphe);

    // Retourne 1 si toutes les arêtes respectent la contrainte, sinon 0
    return valide;
}


