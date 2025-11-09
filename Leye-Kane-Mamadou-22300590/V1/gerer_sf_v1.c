#include <stdio.h>
#include "bloc.h"
#include "inode.h"
int main(void){
    tInode inode;
    printf("---Test bloc---");


    printf("Fin Test");
    printf("---Test Inode---");
    printf("=========Inode=========[%d]\n",Numero(inode));
    printf("type: %d \n",Type(inode));
    printf("taille: %d Octets\n",Taille(inode));
    printf("date dernier accès : %d\n",DateDerAcces(inode));
    printf("date dernière modification : %d\n", DateDerModif(inode));
    printf("date dernière modification inode : %d\n",DateDerModifFichier(inode));
    //printf("Données:\n %d \n",AfficherInode(inode)); à finir 
    
    return 0;
}