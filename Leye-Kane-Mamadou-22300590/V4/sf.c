/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 4
 * Fichier : sf.c
 * Module de gestion d'un systèmes de fichiers (simulé)
 **/

#include "sf.h"
#include "bloc.h"
#include <stdlib.h>
#include "repertoire.h"

// Taille maximale du nom du SF (ou nom du disque)
#define TAILLE_NOM_DISQUE 24
#define MAX_ENTREES 8

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

// Définirion d'un système de fichiers (simplifié)
struct sSF
{
  // Le super-bloc
  tSuperBloc superBloc;
  // La liste chaînée des inodes
  struct sListeInodes listeInodes;
};

/* V3
*  Crée et retourne un super-bloc.
* Entrée : le nom du disque (ou du SF)
* Sortie : le super-bloc, ou NULL en cas de problème
*/
static tSuperBloc CreerSuperBloc(char nomDisque[]) {
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


/* V2 & V4
 * Crée un nouveau système de fichiers.
 * Entrée : nom du disque à associer au système de fichiers créé
 * Retour : le système de fichiers créé, ou NULL en cas d'erreur
 */
tSF CreerSF (char nomDisque[]){
    tSF sf = malloc(sizeof(struct sSF));
    if (sf == NULL) {
        fprintf(stderr, "CreerSF : probleme creation\n");
        return NULL;
    }

    sf->superBloc = malloc(sizeof(struct sSuperBloc));
    if (sf->superBloc == NULL) {
        fprintf(stderr, "CreerSF : probleme creation\n");
        free(sf);
        return NULL;
    }

    int i = 0;
    while (nomDisque[i] != '\0' && i < TAILLE_NOM_DISQUE) {
        sf->superBloc->nomDisque[i] = nomDisque[i];
        i++;
    }
    sf->superBloc->nomDisque[i] = '\0';
    sf->superBloc->dateDerModif = time(NULL);

    sf->listeInodes.premier = NULL;
    sf->listeInodes.nbInodes = 0;

    tInode inode0 = CreerInode(0, REPERTOIRE);
    if (inode0 == NULL) {
        fprintf(stderr, "CreerSF : probleme creation\n");
        free(sf->superBloc);
        free(sf);
        return NULL;
    }

    struct sListeInodesElement *elem = malloc(sizeof(struct sListeInodesElement));
    elem->inode = inode0;
    elem->suivant = NULL;

    sf->listeInodes.premier = elem;
    sf->listeInodes.nbInodes = 1;

    tRepertoire rep = CreerRepertoire();
    EcrireRepertoireDansInode(rep, inode0);
    DetruireRepertoire(&rep);

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
    sf->listeInodes.dernier->suivant=elt; //??
    sf->listeInodes.dernier=elt;
  }
  sf->listeInodes.nbInodes++;
  sf->superBloc->dateDerModif=time(NULL);
  
  return ecrit;

}

/* V3 & V4
 * Ecrit un fichier (d'un nombre de blocs quelconque) dans le système de fichiers.
 * Si la taille du fichier à écrire dépasse la taille maximale d'un fichier dans le SF(10 x 64 octets),
 * seuls les 640 premiers octets seront écrits dans le système de fichiers.
 * Entrées : le système de fichiers, le nom du fichier (sur disque) et son type dans le SF (simulé)
 * Sortie : le nombre d'octets effectivement écrits, -1 en cas d'erreur.
 */
long EcrireFichierSF(tSF sf, char nomFichier[], natureFichier type) {

  if (sf == NULL) return -1;
  FILE *f = fopen(nomFichier, "rb");
  if (!f) return -1;
  unsigned char buffer[640];
  long lus = fread(buffer, 1, 640, f);
  fclose(f);
  int num = sf->listeInodes.nbInodes;
  tInode inode = CreerInode(num, type);
  if (inode == NULL) return -1;
  EcrireDonneesInode(inode, buffer, lus, 0);

  struct sListeInodesElement *elem = malloc(sizeof(struct sListeInodesElement));
  elem->inode = inode;
  elem->suivant = sf->listeInodes.premier;
  sf->listeInodes.premier = elem;
  sf->listeInodes.nbInodes++;

  //mis a jour rep racine
  tInode inode0 = sf->listeInodes.premier->suivant->inode; // premier est inode créé, suivant = inode0
  tRepertoire rep = CreerRepertoire();
  LireRepertoireDepuisInode(&rep, inode0);
  EcrireEntreeRepertoire(rep, nomFichier, num);
  EcrireRepertoireDansInode(rep, inode0);
  DetruireRepertoire(&rep);
  return lus;
}

/* V3
 * Sauvegarde un système de fichiers dans un fichier (sur disque).
 * Entrées : le système de fichiers, le nom du fichier sauvegarde (sur disque)
 * Sortie : 0 en cas de succèe, -1 en cas d'erreur
 */
int SauvegarderSF(tSF sf, char nomFichier[]) {
    FILE *f = fopen(nomFichier, "wb");
    if (!f) return -1;

    fwrite(sf->superBloc, sizeof(struct sSuperBloc), 1, f);
    fwrite(&(sf->listeInodes.nbInodes), sizeof(int), 1, f);

    struct sListeInodesElement *tmp = sf->listeInodes.premier;
    while (tmp) {
        SauvegarderInode(tmp->inode, f);
        tmp = tmp->suivant;
    }

    fclose(f);
    return 0;
}

/* V3
 * Restaure le contenu d'un système de fichiers depuis un fichier sauvegarde (sur disque).
 * Entrées : le système de fichiers où restaurer, le nom du fichier sauvegarde (sur disque)
 * Sortie : 0 en cas de succèe, -1 en cas d'erreur
 */
int ChargerSF(tSF *pSF, char nomFichier[]) {
    FILE *f = fopen(nomFichier, "rb");
    if (!f) return -1;

    tSF sf = malloc(sizeof(struct sSF));
    if (!sf) return -1;

    sf->superBloc = malloc(sizeof(struct sSuperBloc));
    fread(sf->superBloc, sizeof(struct sSuperBloc), 1, f);

    sf->listeInodes.nbInodes = 0;
    sf->listeInodes.premier = NULL;
    sf->listeInodes.dernier = NULL;

    int n;
    fread(&n, sizeof(int), 1, f);

    for (int i = 0; i < n; i++) {
        tInode inode = NULL;
        ChargerInode(&inode, f);

        struct sListeInodesElement *elt = malloc(sizeof(struct sListeInodesElement));
        elt->inode = inode;
        elt->suivant = NULL;

        if (!sf->listeInodes.premier)
            sf->listeInodes.premier = elt;
        else
            sf->listeInodes.dernier->suivant = elt;

        sf->listeInodes.dernier = elt;
        sf->listeInodes.nbInodes++;
    }

    fclose(f);
    *pSF = sf;
    return 0;
}

/* V4
 * Affiche le détail du répertoire racine d'un système de fichiers.
 * Entrées : le système de fichiers,
 * et un booléen indiquant si l'affichage doit être détaillé (true) ou non (false),
 * voir l'énoncé pour les consignes d'affichage à suivre
 * Sortie : 0 en cas de succès, -1 en cas d'erreur
 */
int Ls(tSF sf, bool detail)  {

  if (sf == NULL) return -1;
  tInode inode0 = sf->listeInodes.premier->inode;
  tRepertoire rep = CreerRepertoire();
  LireRepertoireDepuisInode(&rep, inode0);

  int n = NbEntreesRepertoire(rep);
  printf("Nombre de fichiers dans le répertoire racine : %d\n", n);

  struct sEntreesRepertoire tab[MAX_ENTREES];
  EntreesContenuesDansRepertoire(rep, tab);

  if (!detail) {
    for (int i = 0; i < n; i++){
      printf("%s\n", tab[i].nomEntree);
    }
    DetruireRepertoire(&rep);
    return 0;
  }

    // affichage détaillé
  for (int i = 0; i < n; i++) {
      unsigned int num = tab[i].numeroInode;
      struct sListeInodesElement *p = sf->listeInodes.premier;
      while (p && Numero(p->inode) != num)
          p = p->suivant;

      tInode inode = p->inode;

      char *typeName =
          (Type(inode)==ORDINAIRE? "ORDINAIRE" :
            Type(inode)==REPERTOIRE? "REPERTOIRE":"AUTRE");
      time_t t2 = DateDerModif(inode);
      printf("%-3u %-12s %6ld %s %s\n",num,typeName,Taille(inode),ctime(&t2),tab[i].nomEntree);
    }

    DetruireRepertoire(&rep);
    return 0;
}
