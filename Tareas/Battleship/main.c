#include <dirent.h> // directory header
#include <stdio.h> // printf() and almost everything
#include <stdlib.h> // exit()
#include <unistd.h> // for chdir, rmdir and fork and others
#include <sys/stat.h> // for mkdir
#include <sys/types.h> //for opendir
#include <string.h>

void createTable(){
    char Path[20];
    char Letters[] = "ABCDE";
    int players = 2, size = 5;
    mkdir("Game", ACCESSPERMS);
    for (int Player = 1; Player <= players; Player++) {
        printf("Creating table for player %d. \n", Player);
        for (int Column = 0; Column < size; Column++) {
            if (Column == 0) {
                sprintf(Path, "Game/Player%d/", Player);
                mkdir(Path, ACCESSPERMS);
            }
            for (int Row = 1; Row <= size; Row++) {
                sprintf(Path, "Game/Player%d/%c_%d", Player, Letters[Column], Row);
                mkdir(Path, ACCESSPERMS);
            }
        }
    }

    printf("We are done already? \nYou are welcome... \n");
}

int main(int argc, char const *argv[]) {
    char players;

    printf("Welcome to the Battleship V2.0 \n");
    printf("How many players are going to play? \n");
    printf("2 for multiplayer. \n");
    printf("1 for going against the best IA in the world. \n");
    printf("0 for exit. \n");
    fgets(&players, 2, stdin);

    while (strcmp(&players, "2") != 0) {
        if (strcmp(&players, "0") == 0) {
            printf("Welp, byebye\n");
            return 1;
        }
        printf("That's not a valid answer. \nPlease press '2'.\n");
        strcpy(&players, "0");
        fgets(&players, 2, stdin);

    }

    printf("You have selected %c players. \n", players);



    printf("Creating the entire world of Battleship V2.1 \n");
    createTable();

    //pid_t pid;
    //pid = fork();

    return 0;
}
