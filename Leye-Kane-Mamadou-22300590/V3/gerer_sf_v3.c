#include "bloc.h"
#include "inode.h"
#include "sf.h"
#include <stdio.h>



int main(void) {

    char nomDisque[30];
    char nomFichier[100];
    char sauvegarde[100];

    printf("Nom du disque pour le SF : ");
    scanf("%s", nomDisque);

    printf("Nom du fichier (sur disque) a importer dans le SF : ");
    scanf("%s", nomFichier);

    printf("Nom du fichier de sauvegarde du SF : ");
    scanf("%s", sauvegarde);

    printf("\n=== CREATION SF ===\n");
    tSF sf = CreerSF(nomDisque);

    if (!sf) {
        printf("Erreur creation du SF\n");
        return 1;
    }

    printf("\n=== ECRITURE FICHIER DANS LE SF  ===\n");
    long nb = EcrireFichierSF(sf, nomFichier, ORDINAIRE);

    if (nb < 0) {
        printf("Erreur ecriture fichier dans SF\n");
        DetruireSF(&sf);
        return 1;
    }

    printf("Octets ecrits : %ld\n", nb);

    printf("\n=== AFFICHAGE SF ===\n");
    AfficherSF(sf);

    printf("\n=== SAUVEGARDE SF ===\n");
    if (SauvegarderSF(sf, sauvegarde) == 0){
        printf("Sauvegarde reussie\n");
    }else{
        printf("Erreur sauvegarde\n");
    }
        
    
        

    printf("\n=== DESTRUCTION SF ===\n");
    DetruireSF(&sf);

    printf("\n=== CHARGEMENT SF ===\n");
    if (ChargerSF(&sf, sauvegarde) == 0){
        printf("Chargement reussi\n");
    }else{
        printf("Erreur chargement\n");
    }

    printf("\n=== AFFICHAGE SF RESTAURE ===\n");
    AfficherSF(sf);

    printf("\n=== DESTRUCTION FINALE ===\n");
    DetruireSF(&sf);

    return 0;
}