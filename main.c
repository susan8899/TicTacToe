#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
int board[25] = {
       :,:,:,:,:,
       :,O,-,X,:,
       :,,-,O-,:,
       :,X,-,-,:,
       :,:,:,:,:,



    0, 1, 2, 3, 4,
    5, 6, 7, 8, 9,
    10,11,12,13,14,
    15,16,17,18,19,
    20,21,22,23,24



}
*/


/*const OHS = 1;
const EXES = 2;
const BORDER = 3;
const EMPTY = 0;
*/

enum{OHS, EXES, BORDER, EMPTY};  // enum easier way to define consts
enum{HUMANWIN, COMPWIN, DRAW};

const int Directions[4] = { 1, 5, 4, 6}; //the 4 directions you can win.
                                        //+-1 if the next directions is next to it ie. 12 to 13
                                        //+-5 if the next direction is below it ie. 12 to 17
                                        //+-4 if the next direction is one diagonal ie 12 to 16
                                        //+- 6 if the next direction is the other diagonal ie 12 to 18




int GetNumberForDirection(int startSq, const int dir, const int *board, const int us){
    int found = 0;

    while(board[startSq] != BORDER){ //while we havent hit the border
        if(board[startSq] != us){   //if the square is not on us then break so Os dont overlap
            break;
        }
        found++;
        startSq += dir;
    }

    return found;
}


int FindThreeInARow(const int *board, const int ourindex, const int us){
        int DirIndex = 0; //direction index to loop thru the direction array
        int Dir = 0;      //particular direction in the array
        int threeCount = 1; //the count up to 3. always starts at 1 because we have one O down

        for(DirIndex = 0; DirIndex < 4; ++DirIndex){ //loop thru the direction array
            Dir = Directions[DirIndex]; //take direction
            threeCount += GetNumberForDirection(ourindex + Dir, Dir, board, us); //move in that direction
            threeCount += GetNumberForDirection(ourindex + Dir * -1, Dir * -1, board, us); //move in the opposite of that direction
            if(threeCount == 3){
                break;          //found 3 so we won
            }
            threeCount = 1;
        }
        return threeCount;
}

const int ConvertTo25[9]=
{
    6,7,8,
    11,12,13,
    16,17,18
};

void InitializeBoard(int *board)
{
    int index = 0;

    for (index = 0; index <25; ++index)
    {
        board[index] = BORDER;
    }
      for (index = 0; index < 9; ++index)
    {
        board[ConvertTo25[index]] = EMPTY;
    }

}


void PrintBoard(const int *board)
{
    int index = 0;
    printf("\n\nBoard:\n\n");
    char CharPieces[] = "OX|-";  //enum{OHS, EXES, BORDER, EMPTY};

    for (index = 0; index < 9; ++index){

        //at the end of every 3 squares make a new line
        if(index != 0 && index%3==0){
            printf("\n\n");
        }
            //want to print the character pieces with only the 9 in the middle
            printf("%4c",CharPieces[board[ConvertTo25[index]]]);
    }

        printf("\n");
}



// function to ask the pointer board array if there is an empty square
int HasEmptySpot(const int *board){

        int index = 0;

        //loop through the 9 squares and if a spot is empty return 1
        //if not return 0
        for(index = 0; index < 9; ++index){
            if (board[ConvertTo25[index]] == EMPTY)
                return 1;
        }

        return 0;
}



//take in a pointer to the board, the square that it was made in and which side is making the move
void MakeMove(int *board, const int square, const side){

        //the board at square = X or O
        board[square] = side;
}

int GetComputerMove( int *board){
    int index = 0;
    int numFree = 0;
    int availabeMoves[9];
    int randomMove = 0;
    int move = 0;

    /*
    stores the free space into the array of availableMoves

    */

    //loops thru 9 squares on the board and if it is empty it loops through the numbers
    //that are free in the avaiiableMoves array
    for(index = 0; index < 9; ++index){
        if( board[ConvertTo25[index]]  == EMPTY){
            availabeMoves[numFree++] = ConvertTo25[index];
        };
    }

    //get the random move and sends back to available moves
    randomMove = (rand() % numFree);
    return availabeMoves[randomMove];

}



int GetHumanMove(const int *board){ //returns an integer which will be the move
                                    // returns integer as interal board


    char userInput[4];
    int moveOk = 0;
    int move = -1;


    //this while loops keeps running untill moveOk is not 0
    while (moveOk ==0){

        printf(" You are O's. Please enter a move from 1 to 9. The Number you enter is the space you choose");
        fgets(userInput, 3, stdin); //get input from user, takes 3 characters
        fflush(stdin); //make a call to fflush(stdin) so incase the user entered
                       // string longer than 3 characters, the program would flush
                       // out the rest of the char and not take the following 3 char as input

       // I) now we have the user inupt.
       //    first we see if string length is not = 2. because there are only 2
       //   characters to type in, the square and enter.
        if(strlen(userInput) != 2){
            printf("Invalid Entry\n");
            continue;
        }

        //II) now that there are 2 inputs, this checks if there is only 1 number
        // that was entered and not letters or 2 numbers

        // use sscanf to try to format the string to accept one int
        if( sscanf(userInput, "%d", &move) != 1 ){ //stores int to move address
            move = -1; //reset move to -1 in case something was stored to move
            printf("Invalid sscanf()\n");
            continue;
        }

        //III) test if the integer is in the right range
        if( move < 1 || move > 9){
            move = -1;
            printf("Invalid range()\n");
            continue;
        }


        // otherwise the move is succesful because they entered a number from 1-9

        //we need the ConvertTo25 on a 0-8 to be able to use the internal array
        // need to decrement move by 1 to be able to use it
        move--;  //zero indexing


        //asks if the space is empty
        if( board[ConvertTo25[move]] != EMPTY){
            move = -1;
            printf("Square not available \n");
            continue;
        }

        //if the space is abailable then the move is ok

        moveOk = 1; //causes while loop to end
    }

    printf("Making the Move...%d\n",(move+1));
    return ConvertTo25[move];

}




void RunGame()
{
    int GameOver = 0;   // 1 if game is over
    int Side = OHS;     // OHS are human side
    int LastMoveMade = 0;
    int board[25];

    InitializeBoard(&board[0]);
    PrintBoard(&board[0]);

/*  I) If its the OHS(humans) turn then we need to get move from human,
make a move on board, and change side. Otherwise we get a move from
the computer, make a move on the board, and change sides.

    II) If 3 in a row exist then Game is Over

    II) if there are no more moves, game is a draw
*/
    while(!GameOver){    //while game is running or while game = 0


            if(Side==OHS){
                //get move from human, make a move on board, and change side
                LastMoveMade = GetHumanMove(&board[0]);
                MakeMove(&board[0], LastMoveMade, Side);
                Side = EXES;

            } else{
            //get a move from the computer, make a move on the board, and change sides
                LastMoveMade = GetComputerMove(&board[0]);
                MakeMove(&board[0], LastMoveMade, Side);
                Side = OHS;
                 PrintBoard(&board[0]);
            }

            //If 3 in a row exist then Game is Over
            if(FindThreeInARow(board, LastMoveMade, Side ^ 1) == 3){ //change side with exclusive or for last move made because it switched side when your move has been made
                printf("Game over!!\n");
                GameOver = 1;
                if(Side == OHS){
                    printf("Computer Wins\n");
                } else {
                    printf("You Win\n");
                }
            }


            //if there are no more moves, game is a draw
            if(!HasEmptySpot(board)){
                printf("Game over!\n");
                GameOver = 1;
                printf("It's a draw\n");

            }
    }


    PrintBoard(&board[0]);
}


int main()
{
   srand(time(NULL)); //generate random x's or o's

   RunGame();

   return 0;


}
