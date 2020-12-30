#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARDX 7
#define BOARDY 6

#define PLAYER1_CHECKER 'X'
#define PLAYER2_CHECKER 'O'
#define WINNER_HIGHLIGHT 'Y'

//Menu
void MainMenu();
void LoadGameMenu();
void ListSavesByPlayer();
void DawBoardWithID();

void StartGame(int _loadedGame, char *_player1name, char *_player2name, int _currentPlayer, char _board[BOARDY][BOARDX]);
void LoadGame();
void ExitGame();

//Game Logic
int GameLoop(char *_player1name, char *_player2name, int _currentPlayer, char _board[BOARDY][BOARDX]);
int CheckBoardState(char _board[BOARDY][BOARDX]);
int AddChecker(char _board[BOARDY][BOARDX], int _column, char _checker);
void ClearBoard(char _board[BOARDY][BOARDX]);
void DisplayBoard(char _board[BOARDY][BOARDX]);
void FillInMatch(char *_field1, char *_field2, char *_field3, char *_field4);

//Other
void SaveGame(char *_p1name, char *_p2name, int _currentPlayer, char _board[BOARDY][BOARDX]);
void ListSaves(char *_name);
int DrawBoardFromSave(int _id);

int main() {
    MainMenu();

    return 0;
}

//Menues

void MainMenu() {
    int choice;
    while (1) {
        system("cls");
        printf("\t --MAIN MENU-- \n");
        printf("1) New Game\n");
        printf("2) Load Game\n");
        printf("3) Exit Game\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                StartGame(0, NULL, NULL, 0, NULL);
                break;
            case 2:
                LoadGameMenu();
                break;
            case 3:
                ExitGame(0);
                break;
        }
    }
}

void LoadGameMenu() {
    int choice;
    while (1) {
        system("cls");
        printf("\t --LOAD GAME-- \n");
        printf("1) List all saved games\n");
        printf("2) List all saved games for a particular player\n");
        printf("3) Show the board of one of the saved games\n");
        printf("4) Load a game\n");
        printf("5) Return to main menu\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                ListSaves(NULL);
                system("pause");
                break;
            case 2:
                ListSavesByPlayer();
                break;
            case 3:
                DawBoardWithID();
                break;
            case 4:
                LoadGame();
                break;
            case 5:
                return;
                break;
        }
    }
}

void ListSavesByPlayer(){
    char name[40];
    system("cls");
    printf("\t --LIST SAVES WITH-- \n");
    printf("Enter a name: ");
    scanf("%s", name);
    ListSaves(name);
    system("pause");
}

void DawBoardWithID(){
    int id;
    system("cls");
    printf("\t --DRAW BOARD--\n");
    printf("Enter the ID of the save: ");
    scanf("%d", &id);
    if(!DrawBoardFromSave(id)){
        printf("There is no save with that ID\n");
    }
    system("pause");
}

void LoadGame(){
    int id;
    system("cls");
    printf("\t --LOAD GAME-- \n");
    printf("Enter the ID of the save: ");
    scanf("%d", &id);
    FILE *file;
    file = fopen("result.txt", "r");
    if (file) {
        int i = 0;
        char line[256];
        char *token;
        char player1Name[40];
        char player2Name[40];
        char board[BOARDY][BOARDX];
        int currentPlayer = 0;
        while (fgets(line, sizeof(line), file)) {
            if (line != "\n") {
                i=0;
                token = strtok(line, ",");
                if(token != NULL && atoi(token) == id){
                    while (token != NULL) {
                        if (i == 1) {
                            strcpy(player1Name, token);
                        } else if (i == 2) {
                            strcpy(player2Name, token);
                        }else if(i==3){
                            currentPlayer = atoi(token);
                        } else if (i >= 4) {
                            board[(i-4)/BOARDX][(i-4)%BOARDX] = (char)atoi(token);
                        }
                        token = strtok(NULL, ",");
                        i++;
                    }
                    fclose(file);
                    StartGame(1, player1Name, player2Name, currentPlayer, board);
                    return;
                }
            }
        }
        fclose(file);
        system("cls");
        printf("\t --LOAD GAME--\n");
        printf("There is no game with that ID\n");
    } else {
        printf("Save file does not exist\n");
    }
    system("pause");
}

void StartGame(int _loadedGame, char *_player1name, char *_player2name, int _currentPlayer, char _board[BOARDY][BOARDX]) {
    char Board[BOARDY][BOARDX];
    char player1Name[40];
    char player2Name[40];
    int result = 0;
    int choice;
    int currentPlayer;
    system("cls");
    if (!_loadedGame) {
        printf("\t --NEW GAME--\n");
        printf("Enter the name of Player 1: ");
        scanf("%s", player1Name);
        printf("Enter the name of Player 2: ");
        scanf("%s", player2Name);
        currentPlayer = 0;
    }else{
        strcpy(player1Name, _player1name);
        strcpy(player2Name, _player2name);
        currentPlayer = _currentPlayer;
        for(int i=0; i<BOARDY; i++){
            for(int j=0; j<BOARDX; j++){
                Board[i][j] = _board[i][j];
            }
        }
    }
    do {
        if(!_loadedGame){
        ClearBoard(Board);
        currentPlayer = 0;
        }else{
            _loadedGame = 0;
        }
        result = GameLoop(player1Name, player2Name, currentPlayer, Board);
        if (result == 1) {
            printf("%s wins!\n", player1Name);
            system("pause");
        } else if (result == 2) {
            printf("%s wins!\n", player2Name);
            system("pause");
        } else if (result == 3) {
            printf("No one won :(\n");
            system("pause");
        } else {
            break;
        }

        do {
            system("cls");
            printf("What do you want to do next?\n");
            printf("1) Play Again\n");
            printf("2) Return to Main Menu\n");
            scanf("%d", &choice);
        } while (choice != 1 && choice != 2);
        if (choice == 1) {
            continue;
        } else {
            result = 0;
        }
    } while (result != 0);
}

void ExitGame(int _code) {
    printf("Program terminated with exit code: %d", _code);
    exit(_code);
}

//Game Logic

int GameLoop(char *_player1name, char *_player2name, int _currentPlayer, char _board[BOARDY][BOARDX]) {
    int currentPlayer = _currentPlayer;
    int choice;
    int check = 0;
    while (1) {
        system("cls");
        check = CheckBoardState(_board);
        DisplayBoard(_board);
        if (check == 0) {
            printf("%s is now playing...\n", currentPlayer ? _player2name : _player1name);
            printf("Select a column (1-%d) or 0 to save: ", BOARDX);
            scanf("%d", &choice);
            if (choice > 0 && choice <= BOARDX) {
                if (AddChecker(_board, choice, currentPlayer ? PLAYER2_CHECKER : PLAYER1_CHECKER)) {
                    currentPlayer = !currentPlayer;
                } else {
                    printf("Column %d is full, please select a different column\n", choice);
                    system("pause");
                }
            } else if (choice == 0) {
                SaveGame(_player1name, _player2name, currentPlayer, _board);
                system("pause");
            } else {
                printf("Invalid choice\n");
                system("pause");
            }
        } else {
            return check;
        }
    }
    return 0;
}

int CheckBoardState(char _board[BOARDY][BOARDX]) {
    int checkerCount = 0;

    // Horizontal (-) check
    for (int i = BOARDY - 1; i >= 0; i--) {
        for (int j = 0; j <= BOARDX - 4; j++) {
            if (_board[i][j] == 0 || _board[i][j + 1] == 0 || _board[i][j + 2] == 0 || _board[i][j + 3] == 0) {
                continue;
            } else if (_board[i][j] == PLAYER1_CHECKER && _board[i][j + 1] == PLAYER1_CHECKER && _board[i][j + 2] == PLAYER1_CHECKER && _board[i][j + 3] == PLAYER1_CHECKER) {
                FillInMatch(&_board[i][j], &_board[i][j + 1], &_board[i][j + 2], &_board[i][j + 3]);
                return 1;
            } else if (_board[i][j] == PLAYER2_CHECKER && _board[i][j + 1] == PLAYER2_CHECKER && _board[i][j + 2] == PLAYER2_CHECKER && _board[i][j + 3] == PLAYER2_CHECKER) {
                FillInMatch(&_board[i][j], &_board[i][j + 1], &_board[i][j + 2], &_board[i][j + 3]);
                return 2;
            }
        }
    }
    // Vertical (|) check
    for (int i = BOARDY - 1; i >= 3; i--) {
        for (int j = 0; j < BOARDX; j++) {
            if (_board[i][j] == 0 || _board[i - 1][j] == 0 || _board[i - 2][j] == 0 || _board[i - 3][j] == 0) {
                continue;
            } else if (_board[i][j] == PLAYER1_CHECKER && _board[i - 1][j] == PLAYER1_CHECKER && _board[i - 2][j] == PLAYER1_CHECKER && _board[i - 3][j] == PLAYER1_CHECKER) {
                FillInMatch(&_board[i][j], &_board[i - 1][j], &_board[i - 2][j], &_board[i - 3][j]);
                return 1;
            } else if (_board[i][j] == PLAYER2_CHECKER && _board[i - 1][j] == PLAYER2_CHECKER && _board[i - 2][j] == PLAYER2_CHECKER && _board[i - 3][j] == PLAYER2_CHECKER) {
                FillInMatch(&_board[i][j], &_board[i - 1][j], &_board[i - 2][j], &_board[i - 3][j]);
                return 2;
            }
        }
    }
    // Diagonal (/) check

    for (int i = BOARDY - 1; i >= 3; i--) {
        for (int j = 0; j <= BOARDX - 4; j++) {
            if (_board[i][j] == 0 || _board[i - 1][j + 1] == 0 || _board[i - 2][j + 2] == 0 || _board[i - 3][j + 3] == 0) {
                continue;
            } else if (_board[i][j] == PLAYER1_CHECKER && _board[i - 1][j + 1] == PLAYER1_CHECKER && _board[i - 2][j + 2] == PLAYER1_CHECKER && _board[i - 3][j + 3] == PLAYER1_CHECKER) {
                FillInMatch(&_board[i][j], &_board[i - 1][j + 1], &_board[i - 2][j + 2], &_board[i - 3][j + 3]);
                return 1;
            } else if (_board[i][j] == PLAYER2_CHECKER && _board[i - 1][j + 1] == PLAYER2_CHECKER && _board[i - 2][j + 2] == PLAYER2_CHECKER && _board[i - 3][j + 3] == PLAYER2_CHECKER) {
                FillInMatch(&_board[i][j], &_board[i - 1][j + 1], &_board[i - 2][j + 2], &_board[i - 3][j + 3]);
                return 2;
            }
        }
    }
    // Diagonal (\) check
    for (int i = BOARDY - 1; i >= 3; i--) {
        for (int j = BOARDX - 1; j >= 3; j--) {
            if (_board[i][j] == 0 || _board[i - 1][j - 1] == 0 || _board[i - 2][j - 2] == 0 || _board[i - 3][j - 3] == 0) {
                continue;
            } else if (_board[i][j] == PLAYER1_CHECKER && _board[i - 1][j - 1] == PLAYER1_CHECKER && _board[i - 2][j - 2] == PLAYER1_CHECKER && _board[i - 3][j - 3] == PLAYER1_CHECKER) {
                FillInMatch(&_board[i][j], &_board[i - 1][j - 1], &_board[i - 2][j - 2], &_board[i - 3][j - 3]);
                return 1;
            } else if (_board[i][j] == PLAYER2_CHECKER && _board[i - 1][j - 1] == PLAYER2_CHECKER && _board[i - 2][j - 2] == PLAYER2_CHECKER && _board[i - 3][j - 3] == PLAYER2_CHECKER) {
                FillInMatch(&_board[i][j], &_board[i - 1][j - 1], &_board[i - 2][j - 2], &_board[i - 3][j - 3]);
                return 2;
            }
        }
    }

    for (int i = 0; i < BOARDY; i++) {
        for (int j = 0; j < BOARDX; j++) {
            if (_board[i][j] == PLAYER1_CHECKER || _board[i][j] == PLAYER2_CHECKER) {
                checkerCount++;
            }
        }
    }

    if (checkerCount == BOARDX * BOARDY) {
        return 3;
    }

    return 0;
}

int AddChecker(char _board[BOARDY][BOARDX], int _column, char _checker) {
    int fillCheck = 0;
    for (int i = BOARDY - 1; i >= 0; i--) {
        if (_board[i][_column - 1] == 0) {
            _board[i][_column - 1] = _checker;
            return 1;
        } else {
            fillCheck++;
        }
    }
    if (fillCheck == BOARDY) {
        return 0;
    }
}

void ClearBoard(char _board[BOARDY][BOARDX]) {
    for (int i = 0; i < BOARDY; i++) {
        for (int j = 0; j < BOARDX; j++) {
            _board[i][j] = 0;
        }
    }
}

void DisplayBoard(char _board[BOARDY][BOARDX]) {
    for (int i = 0; i < BOARDY; i++) {
        printf("+");
        for (int k = 0; k < BOARDX; k++) {
            printf("---+");
        }
        printf("\n");

        printf("|");
        for (int j = 0; j < BOARDX; j++) {
            if (_board[i][j] != 0) {
                printf(" %c ", _board[i][j]);
            } else {
                printf("   ");
            }
            printf("|");
        }
        printf("\n");
    }

    printf("+");
    for (int k = 0; k < BOARDX; k++) {
        printf("---+");
    }
    printf("\n");
}

void FillInMatch(char *_field1, char *_field2, char *_field3, char *_field4) {
    *_field1 = WINNER_HIGHLIGHT;
    *_field2 = WINNER_HIGHLIGHT;
    *_field3 = WINNER_HIGHLIGHT;
    *_field4 = WINNER_HIGHLIGHT;
}

//Other

void SaveGame(char *_p1name, char *_p2name, int _currentPlayer, char _board[BOARDY][BOARDX]) {
    int lastID = 0;
    char line[256];
    FILE *file;

    file = fopen("result.txt", "r");
    if (file) {
        while (fgets(line, sizeof(line), file)) {
            if (line != "\n") {
                char *token = strtok(line, ",");
                lastID = atoi(token);
            }
        }
        fclose(file);
    }else{
        lastID = 0;
    }

    file = fopen("result.txt", "a");
    if (file) {
        fprintf(file, "%d,%s,%s,%d", lastID + 1, _p1name, _p2name, _currentPlayer);
        for (int i = 0; i < BOARDY; i++) {
            for (int j = 0; j < BOARDX; j++) {
                fprintf(file, ",%d", _board[i][j]);
            }
        }
        fprintf(file, "\n");
        fclose(file);
    }
    printf("Game saved with ID: %d\n", lastID + 1);
}

void ListSaves(char *_name) {
    system("cls");
    if(_name == NULL){
    printf("\t --LIST ALL SAVES--\n");
    }else{
        printf("\t --LIST SAVES WITH %s--\n", _name);
    }
    FILE *file;
    file = fopen("result.txt", "r");
    if (file) {
        int i = 0;
        char line[256];
        char *token;
        int ID;
        char player1Name[40];
        char player2Name[40];
        int emptySlots = 0;
        while (fgets(line, sizeof(line), file)) {
            if (line != "\n") {
                i=0;
                emptySlots = 0;
                token = strtok(line, ",");
                while (token != NULL) {
                    if(i==0){
                    ID = atoi(token);
                    }else if(i==1){
                        strcpy(player1Name, token);
                    }else if(i==2){
                        strcpy(player2Name, token);
                    }else if(i>=4){
                        if(token[0] == '0'){
                        emptySlots++;
                        }
                    }
                    token = strtok(NULL, ",");
                    i++;
                }
                if(_name == NULL){
                    printf("%d, %s, %s, %d\n",ID, player1Name, player2Name, emptySlots);
                }else{
                    if(strcmp(_name, player1Name) == 0 || strcmp(_name, player2Name) == 0){
                        printf("%d, %s, %s, %d\n",ID, player1Name, player2Name, emptySlots);
                    }
                }
            }
        }
        fclose(file);
    } else {
        printf("Save file does not exist\n");
    }
}

int DrawBoardFromSave(int _id){
    system("cls");
    printf("\t --DRAW BOARD %d--\n", _id);
    FILE *file;
    file = fopen("result.txt", "r");
    if (file) {
        int i = 0;
        char line[256];
        char *token;
        char player1Name[40];
        char player2Name[40];
        char board[BOARDY][BOARDX];
        while (fgets(line, sizeof(line), file)) {
            if (line != "\n") {
                i=0;
                token = strtok(line, ",");
                if(token != NULL && atoi(token) == _id){
                    while (token != NULL) {
                        if (i == 1) {
                            strcpy(player1Name, token);
                        } else if (i == 2) {
                            strcpy(player2Name, token);
                        } else if (i >= 4) {
                            board[(i-4)/BOARDX][(i-4)%BOARDX] = (char)atoi(token);
                        }
                        token = strtok(NULL, ",");
                        i++;
                    }
                    printf("'X' - %s\n'O' - %s\n", player1Name, player2Name);
                    DisplayBoard(board);
                    fclose(file);
                    return 1;
                }
            }
        }
        fclose(file);
        return 0;
    } else {
        printf("Save file does not exist\n");
        return 1;
    }
}