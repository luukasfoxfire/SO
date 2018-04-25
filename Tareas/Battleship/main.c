#include <dirent.h> // directory header
#include <stdio.h> // printf() and almost everything
#include <stdlib.h> // exit()
#include <unistd.h> // for chdir, rmdir and fork and others
#include <sys/stat.h> // for mkdir
#include <sys/types.h> //for opendir
#include <string.h> //for use of strings
#include <sys/mman.h> //for mmap

//Definimos la forma en la que los usuarios van a interactuar con el tablero
typedef struct TableTop {
    int gridForShips[5][5]; //una tabla para los barcos, para evitar que se repitan
    int ShipsLives[5][2];  //una tabla para saber la cantidad de barcos vivos
    int gridForAttacks[5][5];  //una tabla para los ataques, para evitar atacar la misma zona dos veces (como en el juego)
} TableTop;

//Funcion para crear la tabla con las carpetas.
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

    printf("We are done? ._. \nYou are welcome... \n");
}

void cleaningTableTops(TableTop *Player){
    for (int i = 0; i < 5; i++) {
        for (int x = 0; x < 5; x++) {
            Player->gridForShips[i][x] = 0;
            Player->gridForAttacks[i][x] = 0;
        }
        for (int x = 0; x < 2; x++) {
            Player->ShipsLives[i][x] = 0;
        }
    }
}

void positionBoats(pid_t pid){
    if (pid == 0) {
        printf("Positioning boats for Player 1\n");
    } else {
        printf("Positioning boats for Player 2\n");
    }
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
        fgets(&players, 2, stdin);

    }

    printf("You have selected %c players. \n", players);

    printf("Creating the entire world of Battleship V2.1 \n");

    createTable();

    TableTop *Player1 = (TableTop *)mmap(NULL, sizeof(TableTop), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    TableTop *Player2 = (TableTop *)mmap(NULL, sizeof(TableTop), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    cleaningTableTops(Player1);
    cleaningTableTops(Player2);

    pid_t pid;
    pid = fork();

    int PipeForPlayer1[2], PipeForPlayer2[2];

    pipe(PipeForPlayer1);
    pipe(PipeForPlayer2);

    if (pid == 0) {
        printf("Child working fine\n");
        printf("Value of A_1: %d\n", Player1->gridForShips[0][0]);
    } else {
        printf("Parent working fine too\n");
        printf("Value of A_1: %d\n", Player2->gridForShips[0][0]);
    }

    positionBoats(pid);

    return 0;
}
