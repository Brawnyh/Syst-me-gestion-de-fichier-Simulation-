#include "inode.h"
#include "bloc.h"
#include "sf.h"
#include <stdio.h>
#include <stdlib.h>

int main(void){
    
    printf("=== TEST Ecrire1BlocFichierSF ===\n");

    char nomDisque[30];
    char nomFichier[100];

    printf("Nom du disque : ");
    scanf("%s", nomDisque);

    printf("Nom du fichier (sur disque) a ecrire dans le SF : ");
    scanf("%s", nomFichier);

    // creer le systeme de fichiers
    tSF sf = CreerSF(nomDisque);
    if (sf == NULL){
        printf("Erreur creation SF.\n");
        return 1;
    }

    printf("SF cree.\n");

    long nb = Ecrire1BlocFichierSF(sf, nomFichier, ORDINAIRE);

    if (nb < 0){
        printf("Erreur ecriture fichier dans SF.\n");
        DetruireSF(&sf);
        return 1;
    }

    printf("Octets ecrits dans le SF : %ld\n", nb);

    printf("\n=== Contenu du systeme de fichiers ===\n");
    AfficherSF(sf);

    printf("\nDestruction du SF...\n");
    DetruireSF(&sf);

    if (sf == NULL)
        printf("SF detruit correctement.\n");
    else
        printf("Erreur destruction SF.\n");

    return 0;
}