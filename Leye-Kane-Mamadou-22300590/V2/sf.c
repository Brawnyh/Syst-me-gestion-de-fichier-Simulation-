/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 2
 * Fichier : sf.c
 * Module de gestion d'un systèmes de fichiers (simulé)
 **/

#include "sf.h"
#include "bloc.h"
#include "inode.h"
#include <stdlib.h>

// Taille maximale du nom du SF (ou nom du disque)
#define TAILLE_NOM_DISQUE 24

// Définition du super-bloc
struct sSuperBloc
{
  // Le nom du disque (ou du système de fichiers) (avec le '\0')
  char nomDisque[TAILLE_NOM_DISQUE+1];
  // La date de dernière modification du système de fichiers
  time_t dateDerModif;
};

// Type représentant le super-bloc
typedef struct sSuperBloc *tSuperBloc;

// Définition de la liste chaînée des inodes
struct sListeInodes
{
  // Références vers le premier et le dernier élément de la liste chaînée
  struct sListeInodesElement *premier, *dernier;
  // Nombre d'inodes dans la liste
  int nbInodes;
};

// Définition d'un élement de la liste chaînée des inodes dans le SF
struct sListeInodesElement
{
  // L'inode proprement dit
  tInode inode;
  // L'élément suivant dans la liste chaînée
  struct sListeInodesElement *suivant;
};

// Définition d'un système de fichiers (simplifié)
struct sSF
{
  // Le super-bloc
  tSuperBloc superBloc;
  // La liste chaînée des inodes
  struct sListeInodes listeInodes;
};

/* V2
*  Crée et retourne un super-bloc.
* Fonction non publique (static)
* Entrée : le nom du disque (ou du SF)
* Sortie : le super-bloc, ou NULL en cas de problème
*/
static tSuperBloc CreerSuperBloc(char nomDisque[]) {
  // A COMPLETER
  tSuperBloc superBloc;
  int i;
  superBloc=(tSuperBloc) malloc(sizeof(struct sSuperBloc));;
  if (superBloc==NULL){
    fprintf(stderr,"Erreur de allocation de un superBloc\n");
    return NULL;
  }
  for (i = 0; i < TAILLE_NOM_DISQUE && nomDisque[i] != '\0'; i++) {
    superBloc->nomDisque[i] = nomDisque[i];
  }
  superBloc->nomDisque[i] = '\0';
  superBloc->dateDerModif = time(NULL);

  return superBloc;
}

/* V2
*  Détruit un super-bloc.
* Fonction non publique (static)
* Entrée : le super-bloc à détruire
* Sortie : aucune
*/
static void DetruireSuperBloc(tSuperBloc *pSuperBloc) {
  // A COMPLETER
  if (*pSuperBloc!=NULL && pSuperBloc!=NULL){
    free(*pSuperBloc);
    *pSuperBloc=NULL;

  }
  
}

/* V2
*  Affiche le contenu d'un super-bloc.
* Fonction non publique (static)
* Entrée : le super-bloc à afficher
* Sortie : aucune
*/
static void AfficherSuperBloc(tSuperBloc superBloc) {
  // A COMPLETER
  printf("Nom disque : %s\n", superBloc->nomDisque);
  printf("Derniere modification : %s", ctime(&(superBloc->dateDerModif)));
}

/* V2
 * Crée un nouveau système de fichiers.
 * Entrée : nom du disque à associer au système de fichiers créé
 * Retour : le système de fichiers créé, ou NULL en cas d'erreur
 */
tSF CreerSF(char nomDisque[]) {
  tSF sf;
  sf = (tSF) malloc(sizeof(struct sSF));
  if (sf == NULL) {
    fprintf(stderr, "Erreur creation SF\n");
    return NULL;
  }

  sf->superBloc = CreerSuperBloc(nomDisque);
  if (sf->superBloc == NULL) {
    free(sf);
    return NULL;
  }

  sf->listeInodes.premier = NULL;
  sf->listeInodes.dernier = NULL;
  sf->listeInodes.nbInodes = 0;

  return sf;
}

/* V2
 * Détruit un système de fichiers et libère la mémoire associée.
 * Entrée : le SF à détruire
 * Sortie : aucune
 */
void DetruireSF(tSF *pSF) {
  
  struct sListeInodesElement *tmp;

  if (pSF == NULL || *pSF == NULL)
    return;
  tmp = (*pSF)->listeInodes.premier;
  while (tmp != NULL) {

    struct sListeInodesElement *suiv = tmp->suivant;
    DetruireInode(&(tmp->inode));
    free(tmp);
    tmp = suiv;
  }

  DetruireSuperBloc(&((*pSF)->superBloc));

  free(*pSF);
  *pSF = NULL;
}


/* V2
 * Affiche les informations relative à un système de fichiers i.e;
 * le contenu du super-bloc et celui des différents inodes du SF
 * Entrée : le SF à afficher
 * Sortie : aucune
 */
void AfficherSF(tSF sf){
  printf("===== SUPER BLOC =====\n");
  AfficherSuperBloc(sf->superBloc);

  printf("\n===== LISTE INODES =====\n");

  struct sListeInodesElement *tmp = sf->listeInodes.premier;

  while (tmp != NULL) {
    AfficherInode(tmp->inode);
    printf("\n");
    tmp = tmp->suivant;
  }
}

/* V2
 * Ecrit un fichier d'un seul bloc dans le système de fichiers.
 * Entrées : le système de fichiers, le nom du fichier (sur disque) et son type dans le SF (simulé)
 * Sortie : le nombre d'octets effectivement écrits, -1 en cas d'erreur.
 */
long Ecrire1BlocFichierSF(tSF sf, char nomFichier[], natureFichier type) {
  // A COMPLETER
  if (sf==NULL){
    return -1;
  }
  FILE *f;
  f=fopen(nomFichier,"rb");

  if (f==NULL) {
    return -1;
  }

  unsigned char b[TAILLE_BLOC];
  
  long nb=fread(b,1,TAILLE_BLOC,f);
  fclose(f);

  tInode inode = CreerInode(sf->listeInodes.nbInodes + 1,type);
  if (inode==NULL){
    return -1;
  }
  //mremplire inode avec donnes du fichier
  long ecrit=LireDonneesInode1bloc(inode,b,nb);
  if (ecrit<0){
    return -1;
  }

  struct sListeInodesElement *elt =malloc(sizeof(struct sListeInodesElement));
  elt->inode=inode;
  elt->suivant=NULL;

  if(sf->listeInodes.premier==NULL){
    sf->listeInodes.premier=elt;
    sf->listeInodes.dernier=elt;
  }else{
    sf->listeInodes.dernier->suivant=elt;
    sf->listeInodes.dernier=elt;
  }
  sf->listeInodes.nbInodes++;
  sf->superBloc->dateDerModif=time(NULL);
  
  return ecrit;




}
