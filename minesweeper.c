#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "conio.h"
#define MAX 10

// background color
#define KNRM  "\x1B[0m"
#define BRED  "\x1B[41m"
#define BGRN  "\x1B[42m"
#define BYEL  "\x1B[43m"
#define BBLU  "\x1B[44m"
#define BMAG  "\x1B[45m"
#define BCYN  "\x1B[46m"
#define BWHT  "\x1B[47m"
// text color
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


// global variables
typedef enum {input,flag,check}mode; 
// game table
unsigned char table_array[MAX][MAX];
// location of cursor
int coord_x=0, coord_y=0;
// flag: input mode = 0, flag mode = 1, check mode = 2
mode game_mode=input;

/*This is a recursive function which uncovers blank cells while they are adjacent*/
int uncover_blank_cell(int row, int column) {
    int value, rows[8], columns[8], i;
    bool isError = table_array[row][column] != 0;

    if(isError)
        return 0; // error

    table_array[row][column] += 10; // uncover current cell

    // Get position of adjacent cells of current cell
    rows[0] = row - 1;
    columns[0] = column + 1;
    rows[1] = row;
    columns[1] = column + 1;
    rows[2] = row + 1;
    columns[2] = column + 1;
    rows[3] = row - 1;
    columns[3] = column;
    rows[4] = row + 1;
    columns[4] = column;
    rows[5] = row - 1;
    columns[5] = column - 1;
    rows[6] = row;
    columns[6] = column - 1;
    rows[7] = row + 1;
    columns[7] = column - 1;

    for(i = 0; i < 8; i++) {
        value = table_array[rows[i]][columns[i]];
        bool isPositiveIndex = (rows[i] >= 0 && rows[i] < MAX);
        bool isInBounds = (columns[i] >= 0 && columns[i] < MAX);
        if( isPositiveIndex && isInBounds ) {	// to prevent negative index and out of bounds
           bool isBetween0And8 = (value > 0 && value <= 8);
           bool isZero = (value == 0);
           if(isBetween0And8)
                table_array[rows[i]][columns[i]] += 10;										// it is a cell with 1-8 number so we need to uncover
            else if(isZero)
                uncover_blank_cell(rows[i], columns[i]);
        }

    }

    return 1; // success!
}

void print_table() {
    // clear screen
    system("clear");

    int i, j, value;
    for(i = 0; i < MAX; i++) {
        for(j = 0; j < MAX; j++) {
            if(coord_x == j && coord_y == i) {
                if(game_mode == flag) {
                    printf("|%sF%s",BMAG,KNRM);
                    continue;
                } else if(game_mode == check) {
                    printf("|%sC%s",BMAG,KNRM);
                    continue;
                }

            }
            value = table_array[i][j];

            if((value >= 0 && value <= 8) || value == 0 || value == 99)
                printf("|X");
            else if(value == 10) // clean area
                printf("|%s%d%s",KCYN, value - 10,KNRM);
            else if(value == 11) // the number of near mine is 1
                printf("|%s%d%s",KYEL, value - 10,KNRM);
            else if(value > 11 && value <= 18) // the number of near mine is greater than 1
                printf("|%s%d%s",KRED, value - 10,KNRM);
            else if((value >= 20 && value <= 28) || value == 100)
                printf("|%sF%s",KGRN,KNRM);
            else
                printf("ERROR"); // test purposes

        }
        printf("|\n");
    }

    printf("cell values: 'X' unknown, '%s0%s' no mines close, '1-8' number of near mines, '%sF%s' flag in cell\n",KCYN,KNRM,KGRN,KNRM);

    switch(game_mode){
    case input:
	printf("f (put/remove Flag in cell), c (Check cell), n (New game), q (Exit game): ");
	break;
    case flag:
	printf("Enter (select to put/remove Flag in cell), q (Exit selection): ");
	break;
    case check:
	printf("Enter (select to check cell), q (Exit selection): ");
	break;
    default:
	printf("Enter correct character!\n");
    }

}



int main(int argc, char *argv[]) {

    char ch;
    int nMines; // the number of the remaining mines
    int i,j,row,column,value, rows[8], columns[8];

    bool isZeroMine = (nMines == 0);

new_game:
    // the number of mines
    nMines = 10;
    if(argc == 2) {
        nMines = atoi(argv[1]);
    }
    srand (time(NULL));						// random seed
    // setting cursor
    coord_y = 0;
    // set all cells to 0
    for(i = 0; i < 10; i++)
        for(j = 0; j < 10; j++)
            table_array[i][j] = 0;

    for(i = 0; i < nMines; i++) {
        /* initialize random seed: */

        row = rand() % 10;					// it generates a integer in the range 0 to 9
        column = rand() % 10;

        // put mines
        if(table_array[row][column] != 99) {
            table_array[row][column] = 99;

            // Get position of adjacent cells of current cell
            rows[0] = row - 1;
            columns[0] = column + 1;
            rows[1] = row;
            columns[1] = column + 1;
            rows[2] = row + 1;
            columns[2] = column + 1;
            rows[3] = row - 1;
            columns[3] = column;
            rows[4] = row + 1;
            columns[4] = column;
            rows[5] = row - 1;
            columns[5] = column - 1;
            rows[6] = row;
            columns[6] = column - 1;
            rows[7] = row + 1;
            columns[7] = column - 1;

            for(j = 0; j < 8; j++) {
                value = table_array[rows[j]][columns[j]];
                if( (rows[j] >= 0 && rows[j] < MAX) && (columns[j] >= 0 && columns[j] < MAX) ) {	// to prevent negative index and out of bounds
                    if(value != 99)		// to prevent remove mines
                        table_array[rows[j]][columns[j]] += 1;	// sums 1 to each adjacent cell
                }
            }

        } else { // to make sure that there are the properly number of mines in table
            i--;
            continue;
        }
    }

    


    while(!isZeroMine) {			// when nMines becomes 0 you will win the game
        print_table();

        ch = getch();
	 // cursor direction
        char direction;
        switch (ch) {

        // flag
        case 'f':
        case 'F':


flag_mode:
            game_mode = flag;
            do {
                print_table();
                direction = getch();
                // arrow direction
                if(direction == '8') {
                    // up
                    coord_y = (MAX + --coord_y) % MAX;
                } else if(direction == '2') {
                    // down
                    coord_y = ++coord_y % MAX;
                } else if(direction == '4') {
                    coord_x = (MAX + --coord_x) % MAX;
                } else if(direction == '6') {
                    coord_x = ++coord_x % MAX;
                } else if(direction == 'c' || direction == 'C') {
                    goto check_mode;




                } else if(direction == '\n') {
                    value = table_array[coord_y][coord_x];

                    if (value == 99) {				// mine case
                        table_array[coord_y][coord_x] += 1;
                        nMines -= 1;				// mine found
                    } else if(value >= 0 && value <= 8) {	// number of mines case (the next cell is a mine)
                        table_array[coord_y][coord_x] += 20;
                    } else if(value >= 20 && value <= 28) {
                        table_array[coord_y][coord_x] -= 20;
                    }

                    if(nMines == 0)
                        break;
                }
            } while (direction != 'q' && direction != 'Q');
            game_mode = input;

            break;

        // check cell
        case 'c':
        case 'C':

check_mode:
            game_mode = check;
            do {
                print_table();
                direction = getch();

                // arrow direction
                if(direction == '8') {
                    // up
                    coord_y = (MAX + --coord_y) % MAX;
                } else if(direction == '2') {
                    // down
                    coord_y = ++coord_y % MAX;
                } else if(direction == '4') {
                    coord_x = (MAX + --coord_x) % MAX;
                } else if(direction == '6') {
                    coord_x = ++coord_x % MAX;
                } else if(direction == 'f' || direction == 'F') {
                    goto flag_mode;
                }

                else if(direction == '\n') {
                    value = table_array[coord_y][coord_x];
                    if(value == 0)						// blank case
                        uncover_blank_cell(coord_y, coord_x);
                    else if(value == 99)				// mine case
                        goto end_of_game;
                    else if(value > 0 && value <= 8)	// number case (the next cell is a mine)
                        table_array[coord_y][coord_x] += 10;

                    //	break;
                }
            } while (direction != 'q' && direction != 'Q');
            game_mode = input;

            break;



        // jump to a new game
        case 'n':
        case 'N':
            goto new_game;
            break;

        // exit
        case 'q':
        case 'Q':
            goto end_of_game;

        default:
            break;
        }
    }

end_of_game:
    game_mode = input;
    print_table();
    printf("\nGAME OVER\n");
    
    if(isZeroMine)
        printf("you won!!!!\n");

    else
        printf("BOOM! you LOOSE!\n");

    do {
        printf("Are you sure to exit? (y or n)? ");
        ch = getch();
        putchar('\n');
        if(ch == 'y' || ch == 'Y') {
            break;
        } else if(ch == 'n' || ch == 'N') {
            goto new_game;
        }
        printf("Please answer y or n\n");
    } while(1);
    printf("See you next time!\n");

    fflush(stdin);

    return 0;
}
