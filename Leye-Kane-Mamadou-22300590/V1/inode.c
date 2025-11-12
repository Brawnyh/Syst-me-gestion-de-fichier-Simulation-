/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 1
 * Fichier : inode.c
 * Module de gestion des inodes.
 **/
#include "inode.h"
#include "bloc.h"
#include <stdlib.h>
// Nombre maximal de blocs dans un inode
#define NB_BLOCS_DIRECTS 10

// Définition d'un inode
struct sInode
{
  // Numéro de l'inode
  unsigned int numero;
  // Le type du fichier : ordinaire, répertoire ou autre
  natureFichier type;
  // La taille en octets du fichier
  long taille;
  // Les adresses directes vers les blocs (NB_BLOCS_DIRECTS au maximum)
  tBloc blocDonnees[NB_BLOCS_DIRECTS];
  // Les dates : dernier accès à l'inode, dernière modification du fichier
  // et de l'inode
  time_t dateDerAcces, dateDerModif, dateDerModifInode;
};

/* V1
 * Crée et retourne un inode.
 * Entrées : numéro de l'inode et le type de fichier qui y est associé
 * Retour : l'inode créé ou NULL en cas de problème
 */
tInode CreerInode(int numInode, natureFichier type) {
  tInode Inode; 
  Inode=(tInode) malloc(sizeof(struct sInode));
  if (Inode==NULL){
    fprintf(stderr,"CreerInode:Probléme de creation\n");
    return NULL;
  }
  Inode ->numero=numInode; //equivaut à (*Inode).numero=numInode
  Inode ->type = type;
  Inode ->taille=0;
  for (int i = 0; i < NB_BLOCS_DIRECTS; i++) {
    Inode->blocDonnees[i] = NULL;
  }
  //on initialise le temps
  Inode->dateDerAcces=0; 
  Inode->dateDerModif=0;
  Inode->dateDerModifInode=0;
  return Inode;

}

/* V1
 * Détruit un inode.
 * Entrée : l'inode à détruire
 * Retour : aucun
 */
void DetruireInode(tInode *pInode) {
    int i;
    tInode inode;

    if (pInode == NULL) return;
    inode = *pInode;
    if (inode == NULL) return;

    for (i = 0; i < NB_BLOCS_DIRECTS; i++) {
        if (inode->blocDonnees[i] != NULL) {
            DetruireBloc(&(inode->blocDonnees[i]));
        }
    }

    free(inode);
    *pInode = NULL;
}

/* V1
 * Récupère la date de dernier accès à un inode.
 * Entrée : l'inode pour lequel on souhaite connaître la date de dernier accès
 * Retour : la date de dernier accès à l'inode
 */
time_t DateDerAcces(tInode inode) {
  return inode->dateDerAcces;
}

/* V1
 * Récupère la date de dernière modification d'un inode.
 * Entrée : l'inode pour lequel on souhaite connaître la date de dernière modification
 * Retour : la date de dernière modification de l'inode
 */
time_t DateDerModif(tInode inode) {
  // A COMPLETER
  return inode->dateDerModif;
}

/* V1
 * Récupère la date de dernière modification d'u fichier associé à un inode.
 * Entrée : l'inode pour lequel on souhaite connaître la date de dernière modification du fichier associé
 * Retour : la date de dernière modification du fichier associé à l'inode
 */
time_t DateDerModifFichier(tInode inode) {
  // A COMPLETER
  return inode->dateDerModifInode;
}

/* V1
 * Récupère le numéro d'un inode.
 * Entrée : l'inode pour lequel on souhaite connaître le numéro
 * Retour : le numéro de l'inode
 */
unsigned int Numero(tInode inode) {
  // A COMPLETER
  return inode->numero;
}

/* V1
 * Récupère la taille en octets du fichier associé à un inode.
 * Entrée : l'inode pour lequel on souhaite connaître la taille
 * Retour : la taille en octets du fichier associé à l'inode
 */
long Taille(tInode inode) {
  // A COMPLETER
  return inode->taille;
}

/* V1
 * Récupère le type du fichier associé à un inode.
 * Entrée : l'inode pour lequel on souhaite connaître le tyep de fichier associé
 * Retour : le type du fichier associé à l'inode
 */
natureFichier Type(tInode inode) {
  // A COMPLETER
  return inode->type;
}

/* V1
 * Affiche les informations d'un inode
 * Entrée : l'inode dont on souhaite afficher les informations
 * Retour : aucun
 */
void AfficherInode(tInode inode) {
  // A COMPLETER
  printf("Inode %u\n", (*inode).numero);
  printf("Type : %d\n", (int)(*inode).type);
  printf("Taille : %ld octets\n", (*inode).taille);
  printf("Blocs de donnees :\n");
  for (int i = 0; i < NB_BLOCS_DIRECTS; i++) {
    if (inode->blocDonnees[i] != NULL){
      printf("  Bloc %d : alloue\n", i);
    }else{
      printf("  Bloc %d : vide\n", i);
    }
            
    }

}

/* V1
 * Copie dans un inode les taille octets situés à l’adresse contenu.
 * Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
 * Entrées : l'inode, l'adresse de la zone à recopier et sa taille en octets
 * Retour : le nombre d'octets effectivement écrits dans l'inode ou -1 en cas d'erreur
 */
long LireDonneesInode1bloc(tInode inode, unsigned char *contenu, long taille) {
  // A COMPLETER
  if (inode==NULL) return -1;
  if((inode->blocDonnees[0]==NULL)){
    inode->blocDonnees[0]=CreerBloc();
    if (inode->blocDonnees[0]==NULL){
      return -1;
    }
  }

  long nb=EcrireContenuBloc(inode->blocDonnees[0],contenu,taille);
  if (nb>=0){
    inode->taille=nb;
    inode->dateDerModif=time(NULL);
  }

  return nb;
}

/* V1
 * Copie à l'adresse contenu les taille octets stockés dans un inode.
 * Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
 * Entrées : l'inode, l'adresse de la zone où recopier et la taille en octets de l'inode
 * Retour : le nombre d'octets effectivement lus dans l'inode ou -1 en cas d'erreur
 */
long EcrireDonneesInode1bloc(tInode inode, unsigned char *contenu, long taille) {
  // A 
  
  if (inode == NULL) return -1;
  if (inode->blocDonnees[0] == NULL) return -1;  // aucun bloc

  long nb = LireContenuBloc(inode->blocDonnees[0], contenu, taille);

  if (nb >= 0){
    inode->dateDerAcces = time(NULL);
  } 
  return nb;
}
