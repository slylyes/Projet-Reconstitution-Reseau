#include <stdlib.h>
#include <stdio.h>
#include "Struct_File.h"

S_file *creerFile()
{
  S_file *new = (S_file *)malloc(sizeof(S_file));
  //Teste allocation memoire 
  if (new == NULL)
  {
    fprintf(stderr, "Erreur allocation de memoire de la file echouee\n");
    return NULL;
  }
  Init_file(new);
  return new;
}

void Init_file(S_file *f)
{
  f->tete = NULL;
  f->dernier = NULL;
}

int estFileVide(S_file *f)
{
  return f->tete == NULL;
}

void enfile(S_file *f, int donnee)
{
  Cellule_file *nouv = (Cellule_file *)malloc(sizeof(Cellule_file));
  nouv->val = donnee;
  nouv->suiv = NULL;
  if (f->tete == NULL)
    f->tete = nouv;
  else
    f->dernier->suiv = nouv;
  f->dernier = nouv;
}

int defile(S_file *f)
{
  int v = f->tete->val;
  Cellule_file *temp = f->tete;
  if (f->tete == f->dernier)
    f->dernier = NULL;
  f->tete = f->tete->suiv;
  free(temp);
  return v;
}

void libererFile(S_file *F)
{
  if (F == NULL)
  {
    return;
  }
  while (F->tete != NULL)
  {
    Cellule_file *tmp = F->tete;
    F->tete = F->tete->suiv;
    if (tmp != NULL)
    {
      free(tmp);
    }
  }
  free(F);
}


