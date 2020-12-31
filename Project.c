#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Defines----------------------------------------------------------------------
#define BOARDX 7
#define BOARDY 6
#define MATCH_SIZE 4

#define PLAYER1_CHECKER 'X'
#define PLAYER2_CHECKER 'O'
#define WINNER_HIGHLIGHT 'Y'

//Functions---------------------------------------------------------------------

//Menu
void MainMenu();
void LoadGameMenu();
void ListSavesByPlayer();
void DrawBoardWithID();
void LoadGame();

void StartGame(int _loadedGame);
void ExitGame(int _code);

//Game Logic
int GameLoop();
int CheckBoardState();
int CheckForMatches(int _row, int _column, int _rowPattern, int _columnPattern);
int AddChecker(int _column, int _checker);

void ClearBoard();
void DisplayBoard();
void FillInMatch(int _row, int _column, int _rowPattern, int _columnPattern);

//Other
void SaveGame();
int LoadData(int _mode, int _id, char *name);

//Game Data---------------------------------------------------------------------

int Board[BOARDY][BOARDX];
char Player1Name[20];
char Player2Name[20];
int CurrentPlayer = 0;

int main() {

    MainMenu();

    return 0;
}

//Menues

void MainMenu() {
    //Player input
    int choice;
    while (1) {
        //Display menu (Repeat if input does not match)
        system("cls");
        printf("\t --MAIN MENU-- \n");
        printf("1) New Game\n");
        printf("2) Load Game\n");
        printf("3) Exit Game\n");
        printf("Choose an option: ");
        //Get player input
        scanf("%d", &choice);
        //Check player input
        switch (choice) {
            case 1:
                //Start a game
                StartGame(0);
                break;
            case 2:
                //Open Load menu
                LoadGameMenu();
                break;
            case 3: 
                //Exit the program
                ExitGame(0);
                break;
        }
    }
}

void LoadGameMenu() {
    //Player input
    int choice;
    //Display menu (Repeat if input does not match)
    while (1) {
        system("cls");
        printf("\t --LOAD GAME-- \n");
        printf("1) List all saved games\n");
        printf("2) List all saved games for a particular player\n");
        printf("3) Show the board of one of the saved games\n");
        printf("4) Load a game\n");
        printf("5) Return to main menu\n");
        printf("Choose an option: ");
        //Get player input
        scanf("%d", &choice);
        //Check player input
        switch (choice) {
            case 1:
                //Load data and display all saves
                system("cls");
                printf("\t --ALL SAVES--\n");
                if(!LoadData(1, 0, NULL)){
                    //If we don't have saves, display a message
                    printf("There are no saves\n");
                }
                system("pause");
                break;
            case 2:
                //Display saves for one player
                ListSavesByPlayer();
                break;
            case 3:
                //Draw a board for a gmae
                DrawBoardWithID();
                break;
            case 4:
                //Load a game
                LoadGame();
                break;
            case 5:
                return;
                break;
        }
    }
}

void ListSavesByPlayer() {
    //Player input
    char name[20];
    //Display message
    system("cls");
    printf("\t --LIST SAVES WITH-- \n");
    printf("Enter a name: ");
    //Get player input
    scanf("%s", name);
    system("cls");
    printf("\t --LIST SAVES WITH %s-- \n", name);
    //Load data and display all saves for a player
    if (!LoadData(2, 0, name)) {
        //If we can't find the player, display a message
        printf("There are no saves with said player\n");
    }
    system("pause");
}

void DrawBoardWithID() {
    //Player input
    int id;
    //Display message
    system("cls");
    printf("\t --DRAW BOARD--\n");
    printf("Enter the ID of the save: ");
    //Get player input
    scanf("%d", &id);
    system("cls");
    printf("\t --DRAW BOARD %d--\n", id);
    //Load data and display a board
    if (!LoadData(3, id, NULL)) {
        //If we can't find the save, display a message
        printf("There are no saves with said ID\n");
    }
    system("pause");
}

void LoadGame() {
    //Player input
    int id;
    //Display message
    system("cls");
    printf("\t --LOAD GAME--\n");
    printf("Enter the ID of the save: ");
    //Get player input
    scanf("%d", &id);
    system("cls");
    printf("\t --LOAD GAME--\n");
    //Load data and Start the game
    if (!LoadData(0, id, NULL)) {
        //If we can't find the save, display a message
        printf("There are no saves with said ID\n");
        system("pause");
    } else {
        StartGame(1);
    }
}

void StartGame(int _loadedGame) {
    //Game result
    int result = 0;
    //Player input
    int choice;
    system("cls");
    //If we did not load a game, ask the player to enter player names and set the first player
    if (!_loadedGame) {
        printf("\t --NEW GAME--\n");
        printf("Enter the name of Player 1: ");
        scanf("%s", Player1Name);
        printf("Enter the name of Player 2: ");
        scanf("%s", Player2Name);
        CurrentPlayer = 0;
    }
    //Start the game loop and repeat until the game result is not 0 (exit)
    do {
        if (!_loadedGame) {
            //If we did not load the game, clear the board of any previous data and set the first player
            ClearBoard(Board);
            CurrentPlayer = 0;
        } else {
            //If we loaded a game, do nothing (because the data is loaded from the save)
            // And set _loadedGame to 0 for the next do iteration
            _loadedGame = 0;
        }
        
        //Get the result form the GameLoop
        result = GameLoop();
        //Check the result of the game
        if (result == 1) {
            //If the result is 1, Player 1 won the game
            printf("%s wins!\n", Player1Name);
            system("pause");
        } else if (result == 2) {
            //If the result is 2, Player2 won the game
            printf("%s wins!\n", Player2Name);
            system("pause");
        } else if (result == 3) {
            //If the result is 3, None of the players won
            printf("No one won :(\n");
            system("pause");
        } else {
            //If we get anything other than 1,2 or 3, break out of the loop
            break;
        }
        
        //After the game has ended and we got a result, ask the player what to do next
        do {
            //Print out a message
            system("cls");
            printf("What do you want to do next?\n");
            printf("1) Play Again\n");
            printf("2) Return to Main Menu\n");
            //Get player input
            scanf("%d", &choice);
        } while (choice != 1 && choice != 2);

        //Check player choice
        if (choice == 1) {
            //If the player choose 1, repeat the game loop
            continue;
        } else {
            //If the player choose 0, set the result to 0 and stop the game loop
            result = 0;
        }
    } while (result != 0);
}

void ExitGame(int _code) {
    //Print out a message
    printf("Program terminated with exit code: %d", _code);
    //Terminate the program with exit code
    exit(_code);
}

//Game Logic

int GameLoop() {
    //Player input
    int choice;
    //Board check
    int check = 0;
    while (1) {
        system("cls");
        //Check the board state
        check = CheckBoardState();
        //Display the board
        DisplayBoard();
        if (check == 0) {
            //If check is 0, which means no winner is determined, ask the players for input
            // Display current player and a message
            printf("%s is now playing...\n", CurrentPlayer ? Player2Name : Player1Name);
            printf("Select a column (1-%d) or 0 to save: ", BOARDX);
            //Get player input
            scanf("%d", &choice);
            //Check player input
            if (choice > 0 && choice <= BOARDX) {
                //If the input is in range of the board width, allow the input
                if (AddChecker(choice, CurrentPlayer + 1)) {
                    //If we successfully added a checker, change the player
                    CurrentPlayer = !CurrentPlayer;
                } else {
                    //If we did not add a checker, display that the column is full
                    printf("Column %d is full, please select a different column\n", choice);
                    system("pause");
                }
            } else if (choice == 0) {
                //If the input is 0, that means the current player saves the game
                SaveGame();
                system("pause");
            } else {
                //If the input was not 0, nor in range of the board, the player made an invalid choice
                printf("Invalid choice\n");
                system("pause");
            }
        } else {
            //If our check returns anything other than 0, that means the game came to an end and pass that value back to StartGame
            return check;
        }
    }
    return 0;
}

int CheckBoardState() {
    //Check the board for matches
    int winner = 0;
    // Horizontal (-) check
    for (int i = BOARDY - 1; i >= 0; i--) {
        for (int j = 0; j <= BOARDX - MATCH_SIZE; j++) {
            //Check if there are any matches
            if (CheckForMatches(i, j, 0, 1)) {
                //If we got a match return the winner and mark the winning match
                winner = Board[i][j];
                FillInMatch(i, j, 0, 1);
                return winner;
            }
            //Go to next element
        }
    }

    // Vertical (|) check
    for (int i = BOARDY - 1; i >= (MATCH_SIZE - 1); i--) {
        for (int j = 0; j < BOARDX; j++) {
            //Check if there are any matches
            if (CheckForMatches(i, j, -1, 0)) {
                //If we got a match return the winner and mark the winning match
                winner = Board[i][j];
                FillInMatch(i, j, -1, 0);
                return winner;
            }
            //Go to next element
        }
    }

    // Diagonal (/) check
    for (int i = BOARDY - 1; i >= (MATCH_SIZE - 1); i--) {
        for (int j = 0; j <= BOARDX - MATCH_SIZE; j++) {
            //Check if there are any matches
            if (CheckForMatches(i, j, -1, 1)) {
                //If we got a match return the winner and mark the winning match
                winner = Board[i][j];
                FillInMatch(i, j, -1, 1);
                return winner;
            }
            //Go to next element
        }
    }

    // Diagonal (\) check
    for (int i = BOARDY - 1; i >= (MATCH_SIZE - 1); i--) {
        for (int j = BOARDX - 1; j >= (MATCH_SIZE - 1); j--) {
            //Check if there are any matches
            if (CheckForMatches(i, j, -1, -1)) {
                //If we got a match return the winner and mark the winning match
                winner = Board[i][j];
                FillInMatch(i, j, -1, -1);
                return winner;
            }
            //Go to next element
        }
    }

    //If none of the loops matched, check if the board is filled up
    int checkerCount = 0;
    for (int i = 0; i < BOARDY; i++) {
        for (int j = 0; j < BOARDX; j++) {
            if (Board[i][j] == 1 || Board[i][j] == 2) {
                //Count the amount of checkers in the board
                checkerCount++;
            }
        }
    }

    if (checkerCount == BOARDX * BOARDY) {
        //If the amount of checkers in the board matches the amount of elements the board has
        //Return 3 which means none of the players won the game
        winner = 3;
    }else{
        winner = 0;
    }
    //If none of the players won and the board is not filled up, return 0 which means that the game carries on
    return winner;
}

int CheckForMatches(int _row, int _column, int _rowPattern, int _columnPattern) {
    //Count the matches
    int matches = 0;
    //Loop through elements following a pattern and find if any of them match
    for (int i = 0; i < MATCH_SIZE; i++) {
        //Check if any any of the elements are 0
        if (Board[_row + _rowPattern * i][_column + _columnPattern * i] == 0) {
            //If they are, skip this check since we know that 0 is a empty field
            return 0;
        } else if (Board[_row + _rowPattern * i][_column + _columnPattern * i] == Board[_row][_column]) {
            //If the element matches the first element, count the match
            matches++;
        }else{
            //If we don't have a 0 and an element does not match the starting one, skip since we know there is no winner
            return 0;
        }
    }
    if (matches == MATCH_SIZE) {
        //If we matched MATCH_SIZE of elements, return 1 meaning there is a winner
        return 1;
    }
    //If we didn't match MATCH_SIZE, return 0 meaning we did not find any winners
    return 0;
}

int AddChecker(int _column, int _checker) {
    //Count for amount of checkers in the column
    int fillCheck = 0;
    //Loop through elements vertically
    for (int i = BOARDY - 1; i >= 0; i--) {
        if (Board[i][_column - 1] == 0) {
            //If we find an empty slot, add a checker
            Board[i][_column - 1] = _checker;
            //Return 1 meaning we successfully added a checker
            return 1;
        } else {
            //If we did not find an empty slot, count how many elements we have
            fillCheck++;
        }
    }

    if (fillCheck == BOARDY) {
        //If we have the same amount of elements as the height of the board, return 0
        //meaning that we can't add a checker
        return 0;
    }
}

void ClearBoard() {
    //Loop through the elements and set all of them to 0
    for (int i = 0; i < BOARDY; i++) {
        for (int j = 0; j < BOARDX; j++) {
            Board[i][j] = 0;
        }
    }
}

void DisplayBoard() {
    //Loop through rows
    for (int i = 0; i < BOARDY; i++) {
        //Draw a line
        printf("+");
        for (int k = 0; k < BOARDX; k++) {
            printf("---+");
        }
        printf("\n");

        //Start a column
        printf("|");
        //Loop through columns
        for (int j = 0; j < BOARDX; j++) {
            //Draw the elements
            if (Board[i][j] != 0) {
                //Draw checker if the element is not empty
                if(Board[i][j] == 1){
                    printf(" %c ", PLAYER1_CHECKER);
                }else if(Board[i][j] == 2){
                    printf(" %c ", PLAYER2_CHECKER);
                }else if(Board[i][j] == 3){
                    printf(" %c ", WINNER_HIGHLIGHT);
                }
            } else {
                //Draw a empty slot if the element is empty
                printf("   ");
            }
            //End a column
            printf("|");
        }
        //Go to new line
        printf("\n");
    }

    //When every element is drawn, draw the bottom line
    printf("+");
    for (int k = 0; k < BOARDX; k++) {
        printf("---+");
    }
    printf("\n");
}

void FillInMatch(int _row, int _column, int _rowPattern, int _columnPattern) {
    //Based on the starting element and pattern, loop throught the match size and set 
    //every element to Y, marking the winner match
    for (int i = 0; i < MATCH_SIZE; i++) {
        Board[_row + _rowPattern * i][_column + _columnPattern * i] = 3;
    }
}

//Other

void SaveGame() {
    //Last saved ID in the save file
    int lastID = 0;
    //string in which we will store data with max size of 256 characters
    char line[256];
    //Pointer to our file
    FILE *file;

    //Open the file for reading only
    file = fopen("result.txt", "r");
    if (file) {
        //If we have a file, read through its lines, storing the data into line[]
        //and allowing a max of 256 character to be read from each line
        while (fgets(line, sizeof(line), file)) {
            //Get the first token from the file (ID)
            char *token = strtok(line, ",");
            //Convert that token from string to int
            lastID = atoi(token);
        }
        //Close the file after reading through it
        fclose(file);
    } else {
        //If we do not have a file, then the last ID is 0
        lastID = 0;
    }

    //Open the file for appending only
    file = fopen("result.txt", "a");
    if (file) {
        //If we have a file, write a line in it containtng our listID + 1,
        //both player names, current player and board data which is stored in sequence
        fprintf(file, "%d,%s,%s,%d", lastID + 1, Player1Name, Player2Name, CurrentPlayer);
        for (int i = 0; i < BOARDY; i++) {
            for (int j = 0; j < BOARDX; j++) {
                fprintf(file, ",%d", Board[i][j]);
            }
        }
        //After we write everything to in the line, add a line break character at the end
        //and close the file
        fprintf(file, "\n");
        fclose(file);
    }
    //Display a message to the players with the ID of the saved game
    printf("Game saved with ID: %d\n", lastID + 1);
}

int LoadData(int _mode, int _id, char *_name) {
    //Modes
    // 0 - Load Data and Start Game
    // 1 - Display all saves
    // 2 - Display saves with a specific player
    // 3 - Display Board with ID

    //Pointer to our file
    FILE *file;
    //Open file for reading
    file = fopen("result.txt", "r");
    if (file) {
        //If we have a file, create some temp variables
        int id;
        int displayedSaves = 0;
        int emptySlots = 0;
        int tokenIndex = 0;
        char line[256];
        char *token;
        //Read through file lines, storing the data into line[] and allow a max of 256 characters to be read
        while (fgets(line, sizeof(line), file)) {
            //Clear any previous data we might have when reading a new line
            ClearBoard();
            strcpy(Player1Name, "");
            strcpy(Player2Name, "");
            //Set the token index so we know which element are we reading
            tokenIndex = 0;
            //Split the line into tokens
            token = strtok(line, ",");

            while (token != NULL) {
                //While we have tokens, check its index and read into the corrent variable
                switch (tokenIndex) {
                    case 0:
                        //Read ID
                        id = atoi(token);
                        break;
                    case 1:
                        //Read player 1 name
                        strcpy(Player1Name, token);
                        break;
                    case 2:
                        //Read player 2 name
                        strcpy(Player2Name, token);
                        break;
                    case 3:
                        //Read current player
                        CurrentPlayer = atoi(token);
                        break;
                    default:
                        //Read board data (convert sequence to 2d array)
                        Board[(tokenIndex - 4) / BOARDX][(tokenIndex - 4) % BOARDX] = atoi(token);
                        break;
                }
                //Go to next token
                token = strtok(NULL, ",");
                tokenIndex++;
            }
            //When we read through all of the tokens, count how many empty slots we have in our board (if we need to display them)
            emptySlots = 0;
            for (int i = 0; i < BOARDY; i++) {
                for (int j = 0; j < BOARDX; j++) {
                    if (Board[i][j] == 0) {
                        emptySlots++;
                    }
                }
            }
            //Check in which mode did we call this function
            switch (_mode) {
                case 0:
                    //Mode 0 returns 1 if the current loaded save matches our search
                    if (id == _id) {
                        fclose(file);
                        return 1;
                    }
                    break;
                case 1:
                    //Mode 1 prints out all the saves we have and counts how many it printed out
                    printf("%d,%s,%s,%d\n", id, Player1Name, Player2Name, emptySlots);
                    displayedSaves++;
                    break;
                case 2:
                    //Mode 2 check if any of the loaded players match with the search, and prints out only those saves
                    //It also counts how many it printed out
                    if (strcmp(_name, Player1Name) == 0 || strcmp(_name, Player2Name) == 0) {
                        printf("%d,%s,%s,%d\n", id, Player1Name, Player2Name, emptySlots);
                        displayedSaves++;
                    }
                    break;
                case 3:
                    //Mode 3 check if any of the saves match the requested ID, draws the board of that save and returns 1
                    //meaning the save matched our search
                    if (id == _id) {
                        fclose(file);
                        printf("'%c' - %s\n", PLAYER1_CHECKER, Player1Name);
                        printf("'%c' - %s\n", PLAYER2_CHECKER, Player2Name);
                        DisplayBoard();
                        return 1;
                    }
                    break;
            }
        }
        //When we looped through all of the lines, close the file
        fclose(file);
        //If we displayed any messages, return 1 meaning that got results
        if (displayedSaves != 0) {
            return 1;
        }
        //If we did print out any saves, return 0 meaning we have no results
        return 0;
    } else {
        //If we don't have a file, print out a message and return 0 meaning we have no results
        printf("Save file does not exist\n");
        return 0;
    }
}