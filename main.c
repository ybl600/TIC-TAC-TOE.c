 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Standard ANSI Colors
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define CYN   "\x1B[36m"
#define RESET "\x1B[0m"

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

char board[3][3];

void resetBoard() {
    char start = '1';
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = start++;
}

void displayBoard(int p1S, int p2S, char p2Name[]) {
    system(CLEAR);
    printf("\n      TIC-TAC-TOE\n");
    printf("    -----------------\n");

    // Highlighted Scores: P1 is Red, P2/AI is Cyan
    printf("    " RED "P1: %d" RESET "  |  " CYN "%s: %d" RESET "\n\n", p1S, p2Name, p2S);

    for (int i = 0; i < 3; i++) {
        printf("\t ");
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 'X')
                printf(RED "X" RESET);
            else if (board[i][j] == 'O')
                printf(CYN "O" RESET);
            else
                printf("%c", board[i][j]);

            if (j < 2) printf(" | ");
        }
        printf("\n");
        if (i < 2) printf("\t---|---|---\n");
    }
    printf("\n");
}

char checkWinner() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) return board[i][0];
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) return board[0][i];
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) return board[0][0];
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) return board[0][2];

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != 'X' && board[i][j] != 'O') return 'N';
    return 'D';
}

// Easy AI - Random moves
int aiMoveEasy() {
    int available[9], count = 0;
    for (int i = 1; i <= 9; i++) {
        int r = (i-1)/3, c = (i-1)%3;
        if (board[r][c] != 'X' && board[r][c] != 'O') {
            available[count++] = i;
        }
    }
    if (count > 0) return available[rand() % count];
    return -1;
}

// Medium AI - Tries to block and win
int aiMoveMedium() {
    // Try to win
    for (int i = 1; i <= 9; i++) {
        int r = (i-1)/3, c = (i-1)%3;
        if (board[r][c] != 'X' && board[r][c] != 'O') {
            char temp = board[r][c];
            board[r][c] = 'O';
            if (checkWinner() == 'O') {
                board[r][c] = temp;
                return i;
            }
            board[r][c] = temp;
        }
    }
    
    // Try to block player
    for (int i = 1; i <= 9; i++) {
        int r = (i-1)/3, c = (i-1)%3;
        if (board[r][c] != 'X' && board[r][c] != 'O') {
            char temp = board[r][c];
            board[r][c] = 'X';
            if (checkWinner() == 'X') {
                board[r][c] = temp;
                return i;
            }
            board[r][c] = temp;
        }
    }
    
    // Take center if available
    if (board[1][1] != 'X' && board[1][1] != 'O') return 5;
    
    // Random move if nothing else
    return aiMoveEasy();
}

// Hard AI - Strategic but not perfect
int aiMoveHard() {
    // Try to win
    for (int i = 1; i <= 9; i++) {
        int r = (i-1)/3, c = (i-1)%3;
        if (board[r][c] != 'X' && board[r][c] != 'O') {
            char temp = board[r][c];
            board[r][c] = 'O';
            if (checkWinner() == 'O') {
                board[r][c] = temp;
                return i;
            }
            board[r][c] = temp;
        }
    }
    
    // Try to block player
    for (int i = 1; i <= 9; i++) {
        int r = (i-1)/3, c = (i-1)%3;
        if (board[r][c] != 'X' && board[r][c] != 'O') {
            char temp = board[r][c];
            board[r][c] = 'X';
            if (checkWinner() == 'X') {
                board[r][c] = temp;
                return i;
            }
            board[r][c] = temp;
        }
    }
    
    // Take center
    if (board[1][1] != 'X' && board[1][1] != 'O') return 5;
    
    // Take corners
    int corners[] = {1, 3, 7, 9};
     for (int i = 0; i < 4; i++) {
        int r = (corners[i]-1)/3, c = (corners[i]-1)%3;
        if (board[r][c] != 'X' && board[r][c] != 'O') return corners[i];
    }
    
    return aiMoveEasy();
}

int main() {
    int p1Score = 0, p2Score = 0, mode, difficulty = 1;
    char playAgain, p2Name[20] = "P2";
    srand(time(NULL));

    printf("--- GAME SETTINGS ---\n");
    printf("1. Player vs Player\n2. Player vs Computer\nSelect Mode: ");
    scanf("%d", &mode);

    if (mode == 2) {
        strcpy(p2Name, "AI");
        printf("\n--- AI DIFFICULTY ---\n");
        printf("1. Easy (Random moves)\n");
        printf("2. Medium (Basic strategy)\n");
        printf("3. Hard (Advanced strategy)\n");
        printf("Select Difficulty (1-3): ");
        scanf("%d", &difficulty);
    }

    do {
        resetBoard();
        int turn = 1;
        char status = 'N';

        while (status == 'N') {
            displayBoard(p1Score, p2Score, p2Name);
            int choice = -1;

            if (mode == 2 && turn == 2) {
                // AI delay
                for(long i=0; i<200000000; i++);
                
                // Choose AI based on difficulty
                if (difficulty == 1) {
                    choice = aiMoveEasy();
                } else if (difficulty == 2) {
                    choice = aiMoveMedium();
                } else {
                    choice = aiMoveHard();
                }
            } else {
                printf("Player %d (%s%c" RESET "), enter (1-9): ",
                        turn, (turn == 1 ? RED : CYN), (turn == 1 ? 'X' : 'O'));

                if (scanf("%d", &choice) != 1) {
                    while(getchar() != '\n'); 
                    continue;
                }
            }

            int r = (choice - 1) / 3, c = (choice - 1) % 3;
            if (choice >= 1 && choice <= 9 && board[r][c] != 'X' && board[r][c] != 'O') {
                board[r][c] = (turn == 1) ? 'X' : 'O';
                status = checkWinner();
                if (status == 'N') turn = (turn == 1) ? 2 : 1;
            }
        }

        displayBoard(p1Score, p2Score, p2Name);

        if (status == 'D') {
            printf(YEL "Game Draw!\n" RESET);
        } else {
            printf(GRN "==> Player %c Wins! <==\n" RESET, status);
            (status == 'X') ? p1Score++ : p2Score++;
        }

        printf("\nPlay again? (y/n): ");
        scanf(" %c", &playAgain);
    } while (playAgain == 'y' || playAgain == 'Y');

    printf("\nFinal Scores: " RED "P1: %d" RESET " | " CYN "%s: %d" RESET "\n", p1Score, p2Name, p2Score);
    printf("Thanks for playing!\n");

    return 0;
}
