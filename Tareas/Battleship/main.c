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

//Funcion para limpiar la memoria compartida de la tabla Player (Hay 2 de estas)
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



//Funcion para posicionar los barcos
void positionBoats(pid_t pid, TableTop* Player1, TableTop* Player2){

    char *FileOfShips[] = {"AircraftCarrier.ship", "Cruiser.ship", "Destroyer.ship", "Frigate.ship", "Corvette.ship"};
    char *NameOfShips[] = {"Aircraft Carrier", "Cruiser", "Destroyer", "Frigate", "Corvette"};    //Tipos de barcos de guerra
    char *player;
    char coordinates[3] = "A1";
    char Letters[] = "ABCDE";
    char Path[50];

    TableTop* ptr;
    if (pid == 0) {
        ptr = Player1;
        player = "Player1";
    } else {
        ptr = Player2;
        player = "Player2";
    }

    char ReviewInput[3], Buff;

    int ValidFlag, InsertedFlag, Column, Row;

    for (int i = 0; i < 5; i++) {
        while (1) {
            while ( (Buff != '\n') && (Buff != EOF) ) Buff = getchar();
            ValidFlag = 0;
            InsertedFlag = 0;
            printf("Please %s, tell me the coordinates for your warships like this: 'A1'\n", player);
            printf("Coordinates for your %s: \n", NameOfShips[i]);

            fgets(coordinates, 3, stdin);

            for (int ReviewLetters = 0; ReviewLetters < 5; ReviewLetters++) {
                for (int ReviewNumbers = 1; ReviewNumbers <= 5; ReviewNumbers++) {
                    sprintf(ReviewInput, "%c%d", Letters[ReviewLetters], ReviewNumbers);
                    if (strcmp(coordinates, ReviewInput) == 0) {
                        ValidFlag = 1;
                        Column = ReviewLetters;
                        Row = ReviewNumbers;
                        break;
                    }
                }
            }
            if (ValidFlag == 0) {
                printf("Error, invalid coordinates, try again. \n");
            } else {
                if (ptr->gridForShips[Column][Row] == 1) {
                    printf("Already used those coordinates, try again. \n");
                } else {
                    ptr->gridForShips[Column][Row] = 1;
                    InsertedFlag = 1;
                    sprintf(Path, "Game/%s/%c_%d/%s", player, Letters[Column], Row, FileOfShips[i]);
                    FILE *fp = fopen(Path, "ab+");
                    printf("Done with the %s\n \n", NameOfShips[i]);
                }
            }
            if (InsertedFlag == 1) {
                break;
            }
        }



    }
}


//Main
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
            return 0;
        }
        printf("That's not a valid answer. \nPlease press '2' or '0'.\n");

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
    int WhoPlays[1];

    pipe(PipeForPlayer1);
    pipe(PipeForPlayer2);

    if (pid == 0){
        close(PipeForPlayer1[1]);
        close(PipeForPlayer2[0]);
        write(PipeForPlayer2[1], "0", 1);
    } else{
        close(PipeForPlayer2[1]);
        close(PipeForPlayer1[0]);
    }

    if (pid > 0){
        read(PipeForPlayer2[0],WhoPlays,1);
        if (WhoPlays[0] == '0'){
            positionBoats(pid, Player1, Player2);
            write(PipeForPlayer1[1], "1", 1);
        }
    }
    else{
        read(PipeForPlayer1[0],WhoPlays,1);
        if (WhoPlays[0] == '1'){
            positionBoats(pid, Player1, Player2);
            write(PipeForPlayer2[1], "0", 1);
        }
    }


    //Bueno, no funciona, ni con ningun ejemplo encontrado en google ni en moodle ni en ningun lado, pero aqui esta la funcion


    if (pid > 0) {
        positionBoats(pid, Player1, Player2);
    }

    return 0;
}
