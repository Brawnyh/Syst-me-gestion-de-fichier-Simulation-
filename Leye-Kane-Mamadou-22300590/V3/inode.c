/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 3
 * Fichier : inode.c
 * Module de gestion des inodes.
 **/
#include "inode.h"
#include "bloc.h"
#include <stdlib.h>

#define NB_BLOCS_DIRECTS 10

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

/* V1 & V3
 * Affiche les informations d'un inode
 * Entrée : l'inode dont on souhaite afficher les informations
 * Retour : aucun
 */
void AfficherInode(tInode inode) {
    time_t t1 = DateDerAcces(inode);
    time_t t2 = DateDerModif(inode);
    time_t t3 = DateDerModifFichier(inode);
    if(inode==NULL){
      printf("vide");
      return;
    }
    printf("=========Inode=========[%d]\n", Numero(inode));
    printf("type: ");
    switch (Type(inode)) {
        case ORDINAIRE: printf("ORDINAIRE\n"); break;
        case REPERTOIRE: printf("REPERTOIRE\n"); break;
        case AUTRE: printf("AUTRE\n"); break;
        default: printf("INCONNU\n"); break;
    }

    printf("taille: %ld Octets utilisés\n", Taille(inode));
    printf("date dernier accès : %s", ctime(&t1));
    printf("date dernière modification : %s", ctime(&t2));
    printf("date dernière modification inode : %s", ctime(&t3));

    //affichage inode
    printf("===== Contenu de l'inode =====\n");
    long total = inode->taille;
    long pos = 0;
    while (pos < total) {
        int numBloc = pos/TAILLE_BLOC;  
        int posBloc = pos%TAILLE_BLOC;
        if (inode->blocDonnees[numBloc] == NULL) {
            printf("\n[Bloc %d inexistant]\n", numBloc);
            return;
        }
        unsigned char c = inode->blocDonnees[numBloc][posBloc];
        printf("%c", c);
        pos++;
    }

    printf("\n");
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


/* V3
 * Lit les données d'un inode avec décalage, et les stocke à une adresse donnée
 * Entrées : l'inode d'où les données sont lues, la zone où recopier ces données, la taille en octets
 * des données à lire et le décalage à appliquer (voir énoncé)
 * Sortie : le nombre d'octets effectivement lus, 0 si le décalage est au-delà de la taille
 */
long LireDonneesInode(tInode inode, unsigned char *contenu, long taille, long decalage) {
  if (inode == NULL || contenu==NULL) return -1;
  if(decalage>=inode->taille ) return 0;
  long lus=0;
  long pos=decalage;
  long fin=inode->taille;
  while(lus<taille && pos<fin){
    int numB= pos/TAILLE_BLOC;
    int posB= pos%TAILLE_BLOC;

    if (inode->blocDonnees[numB]==NULL) return -1;

    contenu[lus]=inode->blocDonnees[numB][posB];
    lus+=1;
    pos+=1;
  }

  inode->dateDerAcces=time(NULL);
  return lus;

}

/* V3
 * Ecrit dans un inode, avec décalage, ls données stockées à une adresse donnée
 * Entrées : l'inode où écrire le contenu, l'adesse de la zone depuis laquelle lire les données, la taille en octets
 * de ces données et le décalage à appliquer (voir énoncé)
 * Sortie : le nombre d'octets effectivement écrits, ou -1 en cas d'erreur
 */
long EcrireDonneesInode(tInode inode, unsigned char *contenu, long taille, long decalage) {
    if (inode == NULL || contenu == NULL) return -1;
    if (decalage < 0) return -1;
    if (taille <= 0) return 0;

    long ecrits = 0;
    long pos = decalage;
    long finPossible = (long)NB_BLOCS_DIRECTS * TAILLE_BLOC;  // limite

    if (pos >= finPossible) return -1;

    while (ecrits < taille && pos < finPossible) {
        int numBloc = pos / TAILLE_BLOC;
        int posBloc = pos % TAILLE_BLOC;

        if (numBloc >= NB_BLOCS_DIRECTS) break; 

        //creer un bloc si il n'existe pas
        if (inode->blocDonnees[numBloc] == NULL) {
            inode->blocDonnees[numBloc] = CreerBloc();
            if (inode->blocDonnees[numBloc] == NULL) {
                return -1;
            }
        }

        inode->blocDonnees[numBloc][posBloc] = contenu[ecrits];

        ecrits += 1;
        pos += 1;
    }

    if (pos > inode->taille)
        inode->taille = pos;

    inode->dateDerModif = time(NULL);
    inode->dateDerModifInode = time(NULL);

    return ecrits;
}

/* V3
 * Sauvegarde toutes les informations contenues dans un inode dans un fichier (sur disque,
 * et préalablement ouvert en écriture et en mode binaire)
 * Entrées : l'inode concerné, l'identificateur du fichier
 * Sortie : 0 en cas de succès, -1 en cas d'erreur
 */
int SauvegarderInode(tInode inode, FILE *fichier) {
    if (inode == NULL || fichier == NULL) return -1;

    if (fwrite(&(inode->numero), sizeof(unsigned int), 1, fichier) != 1) return -1;
    if (fwrite(&(inode->type), sizeof(natureFichier), 1, fichier) != 1) return -1;
    if (fwrite(&(inode->taille), sizeof(long), 1, fichier) != 1) return -1;
    if (fwrite(&(inode->dateDerAcces), sizeof(time_t), 1, fichier) != 1) return -1;
    if (fwrite(&(inode->dateDerModif), sizeof(time_t), 1, fichier) != 1) return -1;
    if (fwrite(&(inode->dateDerModifInode), sizeof(time_t), 1, fichier) != 1) return -1;

    for (int i = 0; i < NB_BLOCS_DIRECTS; i++) {
        char present;
        if ((inode->blocDonnees[i]!=NULL)){
            present=1;
        }else{
          present=0;
        }
        if (fwrite(&(inode->blocDonnees[i]), sizeof(tBloc), 1, fichier) != 1) return -1;
        if (present){
          if (fwrite(inode->blocDonnees[i],sizeof(unsigned char),TAILLE_BLOC,fichier)!=TAILLE_BLOC) return -1;

        }
    }

    return 0;
  
}

/* V3
 * Charge toutes les informations d'un inode à partir d'un fichier (sur disque,
 * et préalablement ouvert en lecture et en mode binaire)
 * Entrées : l'inode concerné, l'identificateur du fichier
 * Sortie : 0 en cas de succès, -1 en cas d'erreur
 */
int ChargerInode(tInode *pInode, FILE *fichier) {
    if (pInode == NULL || fichier == NULL) return -1;
    tInode inode = malloc(sizeof(struct sInode));
    if (inode == NULL) return -1;

    if (fread(&inode->numero, sizeof(unsigned int), 1, fichier) != 1 ||
        fread(&inode->type, sizeof(natureFichier), 1, fichier) != 1 ||
        fread(&inode->taille, sizeof(long), 1, fichier) != 1 ||
        fread(&inode->dateDerAcces, sizeof(time_t), 1, fichier) != 1 ||
        fread(&inode->dateDerModif, sizeof(time_t), 1, fichier) != 1 ||
        fread(&inode->dateDerModifInode, sizeof(time_t), 1, fichier) != 1){

          free(inode);
          return -1;
    }

    for (int i = 0; i < NB_BLOCS_DIRECTS; i++) {
        char present;
        if (fread(&present,sizeof(char),1,fichier)!=1){
          free(inode);
          return -1;
        }
        if (present){
          inode->blocDonnees[i]=CreerBloc();
          if(inode->blocDonnees[i]==NULL){
            //liberaiton bloc d'avant
            for(int j=0;j<i;j++){
              if(inode->blocDonnees[j]!=NULL){
                free(inode->blocDonnees[j]);
              }
              free(inode);
              return -1;
            }
          }
          if(fread(inode->blocDonnees[i],sizeof(unsigned char),TAILLE_BLOC,fichier)!=TAILLE_BLOC){
            //free tout
            for (int j=0;j<=i;j++){
              if(inode->blocDonnees[j]!=NULL){
                free(inode->blocDonnees[i]);
              }
              free(inode);
              return -1;
            }
          }
        }else{
          inode->blocDonnees[i]=NULL;
        }

        }
        *pInode = inode;
        return 0;
    }