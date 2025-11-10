#include <stdio.h>
#include "bloc.c"
#include "inode.c" //tiene que ser el .h

int main(void){
    tInode inode;
    tBloc bloc;
    unsigned char c;
    //int x;

    printf("---Test bloc---\n");
    printf("Introduit le contenu du bloc:");
    scanf("%c",&c);
    bloc=CreerBloc();
    EcrireContenuBloc(bloc,&c,1);
    LireContenuBloc(bloc,&c,1);

    //tbloc compile,sanitizers a tester et make a configurer

    printf("Fin Test");
    printf("---Test Inode---");
    inode=CreerInode(0,ORDINAIRE);
    printf("=========Inode=========[%d]\n",Numero(inode));
    printf("type: %d \n",Type(inode));
    printf("taille: %ld Octets\n",Taille(inode));
    printf("date dernier accès : %ld\n",DateDerAcces(inode));
    printf("date dernière modification : %ld\n", DateDerModif(inode));
    printf("date dernière modification inode : %ld\n",DateDerModifFichier(inode));
    //printf("Données:\n %d \n",AfficherInode(inode)); à finir 

    
    return 0;
}