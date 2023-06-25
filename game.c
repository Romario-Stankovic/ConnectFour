#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Defines----------------------------------------------------------------------
#define BOARD_X 7
#define BOARD_Y 6
#define MATCH_SIZE 4

#define PLAYER1_CHECKER 'X'
#define PLAYER2_CHECKER 'O'
#define WINNER_HIGHLIGHT 'Y'
#define SAVE_FILE_NAME "result.txt"

//Functions---------------------------------------------------------------------

//Menu
void mainMenu();
void loadGameMenu();
void listSavesByPlayer();
void drawBoardWithID();
void loadGame();

void startGame(bool loadGame);
void exitGame(int code);

//Game Logic
int gameLoop();
int checkBoardState();
bool checkForMatches(int row, int column, int rowPattern, int columnPattern);
bool addChecker(int column, int checker);

void clearBoard();
void displayBoard();
void fillInMatch(int row, int column, int rowPattern, int columnPattern);

//Other
void saveGame();
bool loadData(int mode, int id, char *name);

bool checkForInvalidCharacters(char *string, char invalidChar);

void flushInputBuffer();
void clearScreen();
void pause();

//Game Data---------------------------------------------------------------------

int board[BOARD_Y][BOARD_X];
char player1Name[20];
char player2Name[20];
int currentPlayer = 0;

//Program logic-----------------------------------------------------------------

int main() {
    mainMenu();

    return 0;
}

//Menues

void mainMenu() {
    //Player input
    int choice = 0;
    while (true) {
        //Reset the choice, in case that the user enters bad input
        choice = 0;
        //Display menu (Repeat if input does not match)
        clearScreen();
        printf("\t --MAIN MENU-- \n");
        printf("1) New Game\n");
        printf("2) Load Game\n");
        printf("3) Exit Game\n");
        printf("Choose an option: ");
        //Get player input
        scanf("%d", &choice);
        //clear the input buffer in case of bad input
        flushInputBuffer();
        //Check player input
        switch (choice) {
            case 1:
                //Start a game
                startGame(false);
                break;
            case 2:
                //Open Load menu
                loadGameMenu();
                break;
            case 3:
                //Exit the program
                exitGame(0);
                break;
        }
    }
}

void loadGameMenu() {
    //Player input
    int choice = 0;
    //Display menu (Repeat if input does not match)
    while (true) {
        //Reset the choice, in case that the user enters bad input
        choice = 0;
        clearScreen();
        printf("\t --LOAD GAME-- \n");
        printf("1) List all saved games\n");
        printf("2) List all saved games for a particular player\n");
        printf("3) Show the board of one of the saved games\n");
        printf("4) Load a game\n");
        printf("5) Return to main menu\n");
        printf("Choose an option: ");
        //Get player input
        scanf("%d", &choice);
        //clear the input buffer in case of bad input
        flushInputBuffer();
        //Check player input
        switch (choice) {
            case 1:
                //Load data and display all saves
                clearScreen();
                printf("\t --ALL SAVES--\n");
                if (!loadData(1, 0, NULL)) {
                    //If we don't have saves, display a message
                    printf("There are no saves\n");
                }
                pause();
                break;
            case 2:
                //Display saves for one player
                listSavesByPlayer();
                break;
            case 3:
                //Draw a board for a gmae
                drawBoardWithID();
                break;
            case 4:
                //Load a game
                loadGame();
                break;
            case 5:
                return;
                break;
        }
    }
}

void listSavesByPlayer() {
    //Player input
    char name[20];
    //Display message
    clearScreen();
    printf("\t --LIST SAVES WITH-- \n");
    printf("Enter a name: ");
    //Get player input
    scanf("%s", name);
    //clear the input buffer (in case that the user enters multiple names seperated with spaces)
    flushInputBuffer();
    clearScreen();
    printf("\t --LIST SAVES WITH %s-- \n", name);
    //Load data and display all saves for a player
    if (!loadData(2, 0, name)) {
        //If we can't find the player, display a message
        printf("There are no saves with said player\n");
    }
    pause();
}

void drawBoardWithID() {
    //Player input
    int id = 0;
    //Display message
    clearScreen();
    printf("\t --DRAW BOARD--\n");
    printf("Enter the ID of the save: ");
    //Get player input
    scanf("%d", &id);
    //clear the input buffer in case of bad input
    flushInputBuffer();
    clearScreen();
    printf("\t --DRAW BOARD %d--\n", id);
    //Load data and display a board
    if (!loadData(3, id, NULL)) {
        //If we can't find the save, display a message
        printf("There are no saves with said ID\n");
    }
    pause();
}

void loadGame() {
    //Player input
    int id = 0;
    //Display message
    clearScreen();
    printf("\t --LOAD GAME--\n");
    printf("Enter the ID of the save: ");
    //Get player input
    scanf("%d", &id);
    //clear the input buffer in case of bad input
    flushInputBuffer();
    clearScreen();
    printf("\t --LOAD GAME--\n");
    //Load data and Start the game
    if (!loadData(0, id, NULL)) {
        //If we can't find the save, display a message
        printf("There are no saves with said ID\n");
        pause();
    } else {
        startGame(true);
    }
}

void startGame(bool loadGame) {
    //Game result
    int result = 0;
    //Player input
    int choice = 0;
    clearScreen();
    //If we did not load a game, ask the player to enter player names and set the first player
    if (!loadGame) {
        //Repeat name input if the user enters an invalid character (',' since it is used in the save file to separate data)
        do {
            clearScreen();
            printf("\t --NEW GAME--\n");
            printf("Enter the name of Player 1: ");
            scanf("%s", player1Name);
            //clear the input buffer (in case that the user enters multiple names separated with spaces)
            flushInputBuffer();
        } while (checkForInvalidCharacters(player1Name, ','));
        do {
            clearScreen();
            printf("\t --NEW GAME--\n");
            printf("Enter the name of Player 2: ");
            scanf("%s", player2Name);
            //clear the input buffer (in case that the user enters multiple names separated with spaces)
            flushInputBuffer();
        } while (checkForInvalidCharacters(player2Name, ','));
    }
    //Start the game loop and repeat until the game result is not 0 (exit)
    while (true) {
        if (!loadGame) {
            //If we did not load the game, clear the board of any previous data and set the first player to Player 1
            clearBoard(board);
            currentPlayer = 0;
        } else {
            //If we loaded a game, do nothing (because the data is loaded from the save)
            // And set loadGame to false for the next do iteration
            loadGame = false;
        }

        //Get the result form the gameLoop
        result = gameLoop();
        //Check the result of the game
        if (result == 1) {
            //If the result is 1, Player 1 won the game
            printf("%s wins!\n", player1Name);
            pause();
        } else if (result == 2) {
            //If the result is 2, Player2 won the game
            printf("%s wins!\n", player2Name);
            pause();
        } else if (result == 3) {
            //If the result is 3, None of the players won
            printf("No one wins\n");
            pause();
        } else {
            //If we get anything other than 1,2 or 3, break out of the loop, returning us to the main menu
            break;
        }

        //After the game has ended and we got a result, ask the player what to do next
        do {
            //Reset the choice, in case that the user enters bad input
            choice = 0;
            //Print out a message
            clearScreen();
            printf("What do you want to do next?\n");
            printf("1) Play Again\n");
            printf("2) Return to Main Menu\n");
            printf("Choose an option: ");
            //Get player input
            scanf("%d", &choice);
            //clear the input buffer in case of bad input
            flushInputBuffer();
        } while (choice != 1 && choice != 2);

        //Check player choice
        if (choice == 1) {
            //If the player choose 1, repeat the game loop
            continue;
        } else {
            //If the player choose 0,break and return to main menu
            break;
        }
    }
}

void exitGame(int code) {
    //Print out a message
    printf("Program terminated with exit code: %d", code);
    //Terminate the program with exit code
    exit(code);
}

//Game Logic

int gameLoop() {
    //Player input
    int choice = -1;
    //Board check
    int check = 0;
    while (true) {
        clearScreen();
        //Check the board state before printing it
        check = checkBoardState();
        //Display the board
        displayBoard();
        if (check == 0) {
            //Reset the choice, in case that the user enters bad input
            choice = -1;
            //If check is 0, which means no winner is determined, ask the players for input
            // Display current player and a message
            printf("%s is now playing...\n", currentPlayer ? player2Name : player1Name);
            printf("Select a column (1-%d) or 0 to save: ", BOARD_X);
            //Get player input
            scanf("%d", &choice);
            //clear the input buffer in case of bad input
            flushInputBuffer();
            //Check player input
            if (choice > 0 && choice <= BOARD_X) {
                //If the input is in range of the board width, allow the input
                if (addChecker(choice, currentPlayer + 1)) {
                    //If we successfully added a checker, change the player
                    currentPlayer = !currentPlayer;
                } else {
                    //If we did not add a checker, display that the column is full
                    printf("Column %d is full, please select a different column\n", choice);
                    pause();
                }
            } else if (choice == 0) {
                //If the input is 0, that means the current player saves the game
                saveGame();
                pause();
            } else {
                //If the input was not 0, nor in range of the board, the player made an invalid choice
                printf("Invalid choice\n");
                pause();
            }
        } else {
            //If our check returns anything other than 0, that means the game came to an end and pass that value back to startGame
            return check;
        }
    }
    //Failsafe if something goes wrong, return 0 stopping the game
    return 0;
}

int checkBoardState() {
    //Check the board for matches
    int winner = 0;
    // Horizontal (-) check
    for (int i = BOARD_Y - 1; i >= 0; i--) {
        for (int j = 0; j <= BOARD_X - MATCH_SIZE; j++) {
            //Check if there are any matches
            if (checkForMatches(i, j, 0, 1)) {
                //If we got a match return the winner and mark the winning match
                winner = board[i][j];
                fillInMatch(i, j, 0, 1);
                return winner;
            }
            //Go to next element
        }
    }

    // Vertical (|) check
    for (int i = BOARD_Y - 1; i >= (MATCH_SIZE - 1); i--) {
        for (int j = 0; j < BOARD_X; j++) {
            //Check if there are any matches
            if (checkForMatches(i, j, -1, 0)) {
                //If we got a match return the winner and mark the winning match
                winner = board[i][j];
                fillInMatch(i, j, -1, 0);
                return winner;
            }
            //Go to next element
        }
    }

    // Diagonal (/) check
    for (int i = BOARD_Y - 1; i >= (MATCH_SIZE - 1); i--) {
        for (int j = 0; j <= BOARD_X - MATCH_SIZE; j++) {
            //Check if there are any matches
            if (checkForMatches(i, j, -1, 1)) {
                //If we got a match return the winner and mark the winning match
                winner = board[i][j];
                fillInMatch(i, j, -1, 1);
                return winner;
            }
            //Go to next element
        }
    }

    // Diagonal (\) check
    for (int i = BOARD_Y - 1; i >= (MATCH_SIZE - 1); i--) {
        for (int j = BOARD_X - 1; j >= (MATCH_SIZE - 1); j--) {
            //Check if there are any matches
            if (checkForMatches(i, j, -1, -1)) {
                //If we got a match return the winner and mark the winning match
                winner = board[i][j];
                fillInMatch(i, j, -1, -1);
                return winner;
            }
            //Go to next element
        }
    }

    //If none of the loops matched, check if the board is filled up
    int checkerCount = 0;
    for (int i = 0; i < BOARD_Y; i++) {
        for (int j = 0; j < BOARD_X; j++) {
            if (board[i][j] == 1 || board[i][j] == 2) {
                //Count the amount of checkers in the board
                checkerCount++;
            }
        }
    }

    if (checkerCount == BOARD_X * BOARD_Y) {
        //If the amount of checkers in the board matches the amount of elements the board has
        //Return 3 which means none of the players won the game
        winner = 3;
    } else {
        //If the board is not full, and we have no winners, continue the game
        winner = 0;
    }
    //return winner value
    return winner;
}

bool checkForMatches(int row, int column, int rowPattern, int columnPattern) {
    //Count the matches
    int matches = 0;
    //Loop through elements following a pattern and find if any of them match
    for (int i = 0; i < MATCH_SIZE; i++) {
        //Check if any any of the elements are 0
        if (board[row + rowPattern * i][column + columnPattern * i] == 0) {
            //If they are, skip this check since we know that 0 is a empty field
            return false;
        } else if (board[row + rowPattern * i][column + columnPattern * i] == board[row][column]) {
            //If the element matches the first element, count the match
            matches++;
        } else {
            //If we don't have a 0 and an element does not match the starting one, skip since we know there is no winner
            return false;
        }
    }
    if (matches == MATCH_SIZE) {
        //If we matched MATCH_SIZE of elements, return true meaning there is a winner
        return true;
    }
    //If we didn't match MATCH_SIZE, return false meaning we did not find any winners
    return false;
}

bool addChecker(int column, int checker) {
    //Loop through elements vertically
    for (int i = BOARD_Y - 1; i >= 0; i--) {
        if (board[i][column - 1] == 0) {
            //If we find an empty slot, add a checker
            board[i][column - 1] = checker;
            //Return true meaning we successfully added a checker
            return true;
        }
    }
    //If we did not exit the funtion with 1 during the for loop, exit with false
    //meaning that we can't add a checker in this column
    return false;
}

void clearBoard() {
    //Loop through the elements and set all of them to 0
    for (int i = 0; i < BOARD_Y; i++) {
        for (int j = 0; j < BOARD_X; j++) {
            board[i][j] = 0;
        }
    }
}

void displayBoard() {
    //Loop through rows
    for (int i = 0; i < BOARD_Y; i++) {
        //Draw a line
        printf("+");
        for (int k = 0; k < BOARD_X; k++) {
            printf("---+");
        }
        printf("\n");

        //Start a column
        printf("|");
        //Loop through columns
        for (int j = 0; j < BOARD_X; j++) {
            //Draw the elements
            switch (board[i][j]) {
                case 1:
                    printf(" %c ", PLAYER1_CHECKER);
                    break;
                case 2:
                    printf(" %c ", PLAYER2_CHECKER);
                    break;
                case 3:
                    printf(" %c ", WINNER_HIGHLIGHT);
                    break;
                default:
                    printf("   ");
                    break;
            }
            //End a column
            printf("|");
        }
        //Go to new line
        printf("\n");
        //Repeat until every cell is drawn
    }

    //When every element is drawn, draw the bottom line
    printf("+");
    for (int k = 0; k < BOARD_X; k++) {
        printf("---+");
    }
    printf("\n");
}

void fillInMatch(int row, int column, int rowPattern, int columnPattern) {
    //Based on the starting element and pattern, loop throught the match size and set
    //every element to Y, marking the winner match
    for (int i = 0; i < MATCH_SIZE; i++) {
        board[row + rowPattern * i][column + columnPattern * i] = 3;
    }
}

//Other

void saveGame() {
    //Last saved ID in the save file
    int lastID = 0;
    //string in which we will store data with max size of 256 characters
    char line[256];
    //Pointer to our file
    FILE *file;

    //Open the file for reading only
    file = fopen(SAVE_FILE_NAME, "r");
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
    file = fopen(SAVE_FILE_NAME, "a");
    if (file) {
        //If we have a file, write a line in it containtng our listID + 1,
        //both player names, current player and board data which is stored in sequence
        fprintf(file, "%d,%s,%s,%d", lastID + 1, player1Name, player2Name, currentPlayer);
        for (int i = 0; i < BOARD_Y; i++) {
            for (int j = 0; j < BOARD_X; j++) {
                fprintf(file, ",%d", board[i][j]);
            }
        }
        //After we write everything to in the line, add a line break character at the end
        //and close the file
        fprintf(file, "\n");
        fclose(file);
        //Display a message to the players with the ID of the saved game
        printf("Game saved with ID: %d\n", lastID + 1);
    } else {
        //Print a message if something is wrong
        printf("There was a problem creating a save\n");
    }
}

bool loadData(int mode, int id, char *_name) {
    //Modes
    // 0 - Load Data and Start Game
    // 1 - Display all saves
    // 2 - Display saves with a specific player
    // 3 - Display Board with ID

    //Pointer to our file
    FILE *file;
    //Open file for reading
    file = fopen(SAVE_FILE_NAME, "r");
    if (file) {
        //If we have a file, create some temp variables
        int loadedId;
        int displayedSaves = 0;
        int emptySlots = 0;
        int tokenIndex = 0;
        char line[256];
        char *token;
        //Read through file lines, storing the data into line[] and allow a max of 256 characters to be read
        while (fgets(line, sizeof(line), file)) {
            //Clear any previous data we might have when reading a new line
            loadedId = 0;
            strcpy(player1Name, "");
            strcpy(player2Name, "");
            currentPlayer = 0;
            clearBoard();
            //Set the token index so we know which element are we reading
            tokenIndex = 0;
            //Split the line into tokens
            token = strtok(line, ",");

            while (token != NULL) {
                //While we have tokens, check its index and read into the corrent variable
                switch (tokenIndex) {
                    case 0:
                        //Read ID
                        loadedId = atoi(token);
                        break;
                    case 1:
                        //Read player 1 name
                        strcpy(player1Name, token);
                        break;
                    case 2:
                        //Read player 2 name
                        strcpy(player2Name, token);
                        break;
                    case 3:
                        //Read current player
                        currentPlayer = atoi(token);
                        break;
                    default:
                        //Read board data (convert sequence to 2d array)
                        board[(tokenIndex - 4) / BOARD_X][(tokenIndex - 4) % BOARD_X] = atoi(token);
                        break;
                }
                //Go to next token
                token = strtok(NULL, ",");
                tokenIndex++;
            }

            if (tokenIndex != (BOARD_X * BOARD_Y) + 4) {
                //If data does not match the format we are looking for:
                //(ID, Player 1 Name, Player 2 Name, Current Player, Number of elements equal to board size)
                //skip this save
                continue;
            }
            // ^ This is a simple safeguard that checks if the data matches a format the programs expects it in, if not, discard the read data
            // Not a foolproof method since we are saving in a .txt file, but better anything than nothing (〜￣▽￣)〜

            //When we read through all of the tokens, count how many empty slots we have in our board (if we need to display them)
            emptySlots = 0;
            for (int i = 0; i < BOARD_Y; i++) {
                for (int j = 0; j < BOARD_X; j++) {
                    if (board[i][j] == 0) {
                        emptySlots++;
                    }
                }
            }
            //Check in which mode did we call this function
            switch (mode) {
                case 0:
                    //Mode 0 returns true if the current loaded save matches our search
                    //The data remains in the global variables, and we start a game using it
                    if (loadedId == id) {
                        fclose(file);
                        return true;
                    }
                    break;
                case 1:
                    //Mode 1 prints out all the saves we have and counts how many it printed out
                    printf("%d, %s, %s, %d\n", loadedId, player1Name, player2Name, emptySlots);
                    displayedSaves++;
                    break;
                case 2:
                    //Mode 2 check if any of the loaded players match with the search, and prints out only those saves
                    //It also counts how many it printed out
                    if (strcmp(_name, player1Name) == 0 || strcmp(_name, player2Name) == 0) {
                        printf("%d, %s, %s, %d\n", loadedId, player1Name, player2Name, emptySlots);
                        displayedSaves++;
                    }
                    break;
                case 3:
                    //Mode 3 check if any of the saves match the requested ID, draws the board of that save and returns true
                    //meaning the save matched our search
                    if (loadedId == id) {
                        fclose(file);
                        printf("'%c' - %s\n", PLAYER1_CHECKER, player1Name);
                        printf("'%c' - %s\n", PLAYER2_CHECKER, player2Name);
                        displayBoard();
                        return true;
                    }
                    break;
            }
        }
        //When we looped through all of the lines, close the file
        fclose(file);
        //If we displayed any messages, return true meaning that we got results
        if (displayedSaves != 0) {
            return true;
        }
        //If we did print out any saves, return false meaning we have no results
        return false;
    } else {
        //If we don't have a file, print out a message and return false meaning we have no results
        printf("Save file does not exist\n");
        return false;
    }
}

bool checkForInvalidCharacters(char *string, char invalidChar) {
    //This function checks each element of the string and returns true if we have an invalid character
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == invalidChar) {
            return true;
        }
    }
    //Otherwise, return false meaning we do not have invalid characters
    return false;
}

void flushInputBuffer() {
    //This function is used as a failsafe in case the user enters bad input, or multiple
    //inputs (strings) separated with spaces, which will store the next string in the buffer
    //and it will be passed to the next scanf that reads user input, which we do not want.
    int c;
    do {
        //Read the character from the input buffer (if any)
        c = getchar();
    } while (c != '\n' && c != EOF);
    //Repeat the read until we hit a new line or End Of File
}

void clearScreen() {
    //Use macros to check on which OS we are on and use the corresponding system call
    //to clear the screen. System calls are bad, macros are bad, oh well... ¯\_(ツ)_/¯
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    //Print out a message to the user
    printf("Press ENTER to continue...");
    //Flush the input buffer, this will wait for user input since we do not have any
    //(if we flushed every other input before this one, which should be the case)
    flushInputBuffer();
}

//Created by: Romario Stanković | 2020230210