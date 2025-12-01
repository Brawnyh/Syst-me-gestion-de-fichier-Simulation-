#include "sf.h"
#include "inode.h"
#include "repertoire.h"
#include "bloc.h"

int main(void)
{
    char nomDisque[30];
    char nomFichier[100];
    int choix;

    printf("Nom du disque : ");
    scanf("%s", nomDisque);

    tSF sf = CreerSF(nomDisque);
    if (sf == NULL) {
        printf("Erreur creation SF.\n");
        return 1;
    }

    do {
        printf("\n=== MENU ===\n");
        printf("1. Ecrire un fichier dans le SF\n");
        printf("2. Lister (Ls simple)\n");
        printf("3. Lister (Ls detail)\n");
        printf("0. Quitter\n");
        scanf("%d", &choix);

        if (choix == 1) {
            printf("Nom du fichier à importer : ");
            scanf("%s", nomFichier);
            long n = EcrireFichierSF(sf, nomFichier, ORDINAIRE);
            if (n < 0) printf("Erreur.\n");
            else printf("Fichier importé (%ld octets).\n", n);
        }
        else if (choix == 2) {
            Ls(sf, false);
        }
        else if (choix == 3) {
            Ls(sf, true);
        }
    } while (choix != 0);

    return 0;
}
