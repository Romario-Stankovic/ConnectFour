#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*
int RandomSeed(){

    return 0;
}
*/
int main(){
    char A[6][7];

    for(int i=0; i<6; i++){
        for (int j=0; j<7; j++){
            A[i][j] = ' ';
        }
    }

    for(int i=0; i<6; i++){
        printf("+---+---+---+---+---+---+---+\n");
        printf("|");
        for(int j=0; j<7; j++){
            printf(" %c ", A[i][j]);
            printf("|");
        }
        printf("\n");
    }
    printf("+---+---+---+---+---+---+---+\n");

/*
    RandomSeed();
*/
}

/*
void ListAllSaves(){
    system("cls");
    printf("\t --ALL SAVES-- \n");
    DIR *directory;
    struct dirent *directory_entry;
    directory = opendir(SaveFolderName);
    if(directory != NULL){
        while ((directory_entry = readdir(directory)) != NULL) {
            if( (strcmp(directory_entry->d_name, ".") != 0) && (strcmp(directory_entry->d_name, "..") != 0) ){
                char subdir_name[50] = "\0";
                DIR *subidr;
                struct dirent *subdir_entry;
                strcat(subdir_name, SaveFolderName);
                strcat(subdir_name, "\\");
                strcat(subdir_name, directory_entry->d_name);
                subidr = opendir(subdir_name);
                if(subidr != NULL){
                    printf("-- %s\n", directory_entry->d_name);
                    while ((subdir_entry = readdir(subidr)) != NULL) {
                        if( (strcmp(subdir_entry->d_name, ".") != 0) && (strcmp(subdir_entry->d_name, "..") != 0) ){
                            printf("\t%s\n", subdir_entry->d_name);
                        }
                    }
                    closedir(subidr);
                }
            }
        }
        closedir(directory);
    }else{
        system("cls");
        printf("There is no save data\n");
    }
    system("pause");
}

void ListSaveByPlayer(){
    char dir_name[50] = "\0";
    char name[40];
    system("cls");
    printf("\t --SAVES BY PLAYER-- \n");
    printf("Enter player name: ");
    scanf("%s", &name);
    strcat(dir_name, SaveFolderName);
    strcat(dir_name, "\\");
    strcat(dir_name, name);

    DIR *directory;
    struct dirent *directory_entry;
    directory = opendir(dir_name);
    if(directory != NULL){
        system("cls");
        printf("All saves by %s:\n", name);
        while ((directory_entry = readdir(directory)) != NULL){
            if ((strcmp(directory_entry->d_name, ".") != 0) && (strcmp(directory_entry->d_name, "..") != 0)){
                printf("\t%s\n", directory_entry->d_name);
            }
        }
        closedir(directory);
    }else{
        system("cls");
        printf("%s does not exist\n", name);
    }
    
    system("pause");
}
*/

/*
else if(check == 1) {
            printf("%s wins!\n", _p1name);
            system("pause");
            NewGame(_p1name, _p2name);
            return check;
        }else if(check == 2){
            printf("%s wins!\n", _p2name);
            system("pause");
            NewGame(_p1name, _p2name);
            return 1;
        }else if(check == 3){
            printf("Stalemate\n");
            system("pause");
            NewGame(_p1name, _p2name);
            return 1;
        }
*/