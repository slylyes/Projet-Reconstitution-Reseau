#ifndef _GRAPHE_H_

#define _GRAPHE_H_
#include "Reseau.h"
#include "Struct_File.h"

typedef struct
{
    int u,v;        /* Numeros des sommets extremite */
}Arete;

typedef struct cellule_arete
{
    Arete *a;       /*Pointeur sur l'arete*/
    struct cellule_arete* suiv;
} Cellule_arete;

typedef struct {
    int num;        /* Numero du sommet (le meme que dans T_som) */ 
    double x, y;
    Cellule_arete* L_voisin;    /* Liste chainee des voisins */
}Sommet;

typedef struct{ 
    int e1,e2;
} Commod;

typedef struct listeNode
{
    int val;
    struct listeNode* suiv;
} ListeNode;


typedef struct
{
    int nbsom;            /*Nombre de sommets*/
    Sommet** T_som;        /* Tableau de pointeurs sur sommets*/
    int gamma;
    int nbcommod;           /*Nombre de commodites*/
    Commod* T_commod;       /*Tableau des commodites*/
} Graphe;

Graphe* creerGraphe(Reseau* R);
void libererGraphe(Graphe* G);
int parcoursEnLargeur(Graphe* graphe, int debut, int fin);
void ajouteElement(ListeNode **tete, int value);
void libererListe(ListeNode* tete);
ListeNode* parcoursEnLargeurListe(Graphe* graphe, int debut, int fin);
void compterChainesParArete(Graphe* graphe, ListeNode* chemin, int** matriceComptage);
int reorganiseReseau(Reseau* R);
#endif