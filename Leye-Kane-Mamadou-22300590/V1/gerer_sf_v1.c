#include <stdio.h>
#include "bloc.h"
#include "inode.h"//tiene que ser el .h

//AfficherInode a utiliser au lieu de tout faire dans le main
int main(void){
    tInode inode;
    tBloc bloc;
    unsigned char texte[TAILLE_BLOC];
    unsigned char lecture[TAILLE_BLOC];
    char c;
    int taille = 0;

    printf("---Test bloc---\n");
    printf("Introduit le contenu du bloc: ");

    while (taille < TAILLE_BLOC - 1) {
        scanf("%c", &c);
        if (c == '\n') break; 
        texte[taille] = c; 
        taille++;
    }
    texte[taille] = '\0';

    bloc = CreerBloc();
    if (bloc == NULL) {
        printf("erreur creation bloc\n");
        return 1;
    }

    EcrireContenuBloc(bloc, texte, taille);
    printf("nombre de octets ecrits: %d\n", taille);

    LireContenuBloc(bloc, lecture, taille);
    lecture[taille] = '\0';
    printf("Contenu lu: %s\n", lecture);

    printf("Fin Test\n");

    printf("---Test Inode---\n");
    inode = CreerInode(1, ORDINAIRE);
    if (inode == NULL) {
        printf("erreur creation inode\n");
        return 1;
    }
    printf("inode cree\n");

    // remplir l'inode avec les données du bloc
    EcrireDonneesInode1bloc(inode, texte, taille);
    printf("inode remplit\n");
    LireDonneesInode1bloc(inode,texte,taille);
    AfficherInode(inode);
    //donnes a afficher

    DetruireInode(&inode);
    if (inode == NULL)
        printf("\nInode détruit correctement.\n");

    return 0;
}