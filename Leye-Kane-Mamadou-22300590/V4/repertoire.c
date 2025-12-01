/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 4
 * Fichier : repertoire.c
 * Module de gestion d'un répertoire d'un systèmes de fichiers (simulé)
 **/
#include "repertoire.h"
#include "inode.h"
#include <stdlib.h>
#include <stdio.h>

// Définition d'un répertoire
struct sRepertoire
{
  tEntreesRepertoire *table;
};
#define MAX_ENTREES 8
/* V4
 * Crée un nouveau répertoire.
 * Entrée : aucune
 * Sortie : le répertoire créé, ou NULL si problème
 */
tRepertoire CreerRepertoire(void)
{
  tRepertoire rep=malloc(sizeof(struct sRepertoire));

  if(rep==NULL){
    fprintf(stderr,"CreerRepertoire : probleme creation\n");
    return NULL;
  }

  rep->table=malloc( MAX_ENTREES*sizeof(tEntreesRepertoire));
  if(rep->table==NULL){
    fprintf(stderr,"CreerRepertoire : probleme creation\n");
    free(rep); 
    return NULL;
  }
  for(int i=0;i<MAX_ENTREES;i++){
      rep->table[i]=malloc(sizeof(struct sEntreesRepertoire));
      rep->table[i]->nomEntree[0]= '\0';
      rep->table[i]->numeroInode= 0;
      
  }
  return rep;
}

//fonctions auxiliaire
static void copierNom(char dest[], char src[])
{
    int i = 0;
    while (src[i] != '\0' && i < TAILLE_NOM_FICHIER) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

static int nomsEgaux(char a[], char b[])
{
    int i = 0;
    while (a[i] != '\0' || b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return 1;
}
//fin fonctions auxiliaire

static int CompterEntrees(tRepertoire rep)
{
    int c = 0;
    while (c < MAX_ENTREES && rep->table[c]->nomEntree[0] != '\0')
        c++;
    return c;
}


/* V4
 * Détruit un répertoire et libère la mémoire associée.
 * Entrée : le répertoire à détruire
 * Sortie : aucune
 */
void DetruireRepertoire(tRepertoire *pRep)
{
  if (*pRep!=NULL || pRep!=NULL) return;

  for(int i=0;i<=MAX_ENTREES;i++){
    free((*pRep)->table);
  }
  free((*pRep)->table);
  free(*pRep);
  *pRep=NULL;
}

/* V4
 * Écrit une entrée dans un répertoire.
 * Si l'entrée existe déjà dans le répertoire, le numéro d'inode associé est mis à jour.
 * Si l'entrée n'existe pas, elle est ajoutée dans le répertoire.
 * Entrées : le répertoire destination, le nom de l'entrée à écrire,
 *           le numéro d'inode associé à l'entrée
 * Retour : 0 si l'entrée est écrite avec succès, -1 en cas d'erreur
 */
int EcrireEntreeRepertoire(tRepertoire rep, char nomEntree[], unsigned int numeroInode)
{
  if (rep==NULL){
    return -1;
  }
  int n=CompterEntrees(rep);
  for(int i=0;i<n;i++){
    if(nomsEgaux(rep->table[i]->nomEntree,nomEntree)){
      rep->table[i]->numeroInode=numeroInode;
      return 0;
    }
  }
  if (n>=MAX_ENTREES) return -1;

  copierNom(rep->table[n]->nomEntree,nomEntree);
  rep->table[n]->numeroInode=numeroInode;

}

/* V4
 * Lit le contenu d'un répertoire depuis un inode.
 * Entrées : le répertoire mis à jour avec le contenu lu,
 *           l'inode source.
 * Retour : 0 si le répertoire est lu avec succès, -1 en cas d'erreur
 */
int LireRepertoireDepuisInode(tRepertoire *pRep, tInode inode)
{
    if (pRep == NULL || inode == NULL) return -1;

    unsigned char buffer[640];

    long lus = LireDonneesInode(inode, buffer, 640, 0);
    if (lus <= 0) return -1;

    int pos = 0;
    int entree = 0;

    while (pos < lus && entree < MAX_ENTREES) {
        // lecture nom
        int j = 0;
        while (pos < lus && buffer[pos] != ' ' && j < TAILLE_NOM_FICHIER) {
            (*pRep)->table[entree]->nomEntree[j++] = buffer[pos++];
        }
        (*pRep)->table[entree]->nomEntree[j] = '\0';

        if (buffer[pos] == ' ') pos++;

        // lecture num
        unsigned int num = 0;
        while (pos < lus && buffer[pos] >= '0' && buffer[pos] <= '9') {
            num = num * 10 + (buffer[pos] - '0');
            pos++;
        }
        (*pRep)->table[entree]->numeroInode = num;

        if (pos < lus && buffer[pos] == '\n') pos++;

        entree++;
    }
}

/* V4
 * Écrit un répertoire dans un inode.
 * Entrées : le répertoire source et l'inode destination
 * Sortie : 0 si le répertoire est écrit avec succès, -1 en cas d'erreur
 */
int EcrireRepertoireDansInode(tRepertoire rep, tInode inode)
{
    if (rep == NULL || inode == NULL) return -1;

    unsigned char buffer[640];  //taille max sf
    int pos = 0;

    int n = NbEntreesRepertoire(rep);

    for (int i = 0; i < n; i++) {
        // copier nom
        int j = 0;
        while (rep->table[i]->nomEntree[j] != '\0' && pos < 639) {
            buffer[pos++] = rep->table[i]->nomEntree[j];
            j++;
        }

        buffer[pos++] = ' ';

        // copie num inode
        unsigned int num = rep->table[i]->numeroInode;
        char tmp[12];
        int k = 0;

        // convertir num a text
        sprintf(tmp, "%u", num);

        while (tmp[k] != '\0' && pos < 639) {
            buffer[pos++] = tmp[k++];
        }

        buffer[pos++] = '\n';
    }

    buffer[pos] = '\0';
    if (EcrireDonneesInode(inode, buffer, pos, 0) < 0)
        return -1;

    return 0;
}

/* V4
 * Récupère les entrées contenues dans un répertoire.
 * Entrées : le répertoire source, un tableau récupérant les numéros d'inodes des entrées du rpertoire
 * Retour : le nombre d'entrées dans le répertoire
 */
int EntreesContenuesDansRepertoire(tRepertoire rep, struct sEntreesRepertoire tabNumInodes[])
{
  if (rep == NULL) return -1;

  int n = NbEntreesRepertoire(rep);

  for (int i = 0; i < n; i++) {
      //copier nom
      int j = 0;
      while (rep->table[i]->nomEntree[j] != '\0' && j < TAILLE_NOM_FICHIER) {
          tabNumInodes[i].nomEntree[j] = rep->table[i]->nomEntree[j];
          j++;
      }
      tabNumInodes[i].nomEntree[j] = '\0';
      tabNumInodes[i].numeroInode = rep->table[i]->numeroInode;
    }

    return n;
}

/* V4
 * Compte le nombre d'entrées d'un répertoire.
 * Entrée : le répertoire source
 * Retour : le nombre d'entrées du répertoire
 */
int NbEntreesRepertoire(tRepertoire rep)
{
  if (rep == NULL) return -1;
  int c = 0;
  while (c < MAX_ENTREES && rep->table[c]->nomEntree[0] != '\0'){
    c++;
  }

  return c;
}

/* V4
 * Affiche le contenu d'un répertoire.
 * Entrée : le répertoire à afficher
 * Retour : aucun
 */
void AfficherRepertoire(tRepertoire rep)
{
  if (rep == NULL) {
    printf("Répertoire vide\n");
    return;
  }

  int n = NbEntreesRepertoire(rep);

  printf("=== Répertoire ===\n");
  for (int i = 0; i < n; i++) {
      printf(" - %s  (inode %u)\n",rep->table[i]->nomEntree,rep->table[i]->numeroInode);
    }
}
