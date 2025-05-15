#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * This file contains the implementation of the Tic Tac Toe game.
 * 
 * 
 * The game allows two players to play Tic Tac Toe on a 3x3 grid.
 * Players take turns to place their marks (X or O) on the grid.
 * The game checks for a win condition or a draw after each move.
 * 
 * Functions in this file:
 * - Function to display the game board.
 * - Function to check for win conditions.
 * - Function to check for a draw.
 * - Main function to run the game loop.
 */

struct games    //structure to store the game data
{
    short int size;
    char player[2][11];
    char current;
    char choice;
    char arr[3][3];
    short int winner[3][2];
    short int offset;
    short int cnt;
};


int check(struct games *game){  //function to check the winner return 1 if winner is found else 0
    int i=0,j=0;

    for(i=0;i<game->size;i++){
        if((game->arr[i][0]=='X' && game->arr[i][1]=='X' && game->arr[i][2]=='X')||(game->arr[i][0]=='O' && game->arr[i][1]=='O' && game->arr[i][2]=='O')){
            game->winner[0][0]=i;       //storing the winner's position
            game->winner[0][1]=0;
            game->winner[1][0]=i;
            game->winner[1][1]=1;
            game->winner[2][0]=i;
            game->winner[2][1]=2;
            return 1;
        }
        if((game->arr[0][i]=='X' && game->arr[1][i]=='X' && game->arr[2][i]=='X')||(game->arr[0][i]=='O' && game->arr[1][i]=='O' && game->arr[2][i]=='O')){
            game->winner[0][0]=0;       //storing the winner's position
            game->winner[0][1]=i;
            game->winner[1][0]=1;
            game->winner[1][1]=i;
            game->winner[2][0]=2;
            game->winner[2][1]=i;
            return 1;
        }
    }

    if((game->arr[0][0]=='X' && game->arr[1][1]=='X' && game->arr[2][2]=='X')||(game->arr[0][0]=='O' && game->arr[1][1]=='O' && game->arr[2][2]=='O')){
        game->winner[0][0]=0;       //storing the winner's position
        game->winner[0][1]=0;
        game->winner[1][0]=1;
        game->winner[1][1]=1;
        game->winner[2][0]=2;
        game->winner[2][1]=2;
        return 1;
    }
    if((game->arr[0][2]=='X' && game->arr[1][1]=='X' && game->arr[2][0]=='X')||(game->arr[0][2]=='O' && game->arr[1][1]=='O' && game->arr[2][0]=='O')){
        game->winner[0][0]=0;       //storing the winner's position
        game->winner[0][1]=2;
        game->winner[1][0]=1;
        game->winner[1][1]=1;
        game->winner[2][0]=2;
        game->winner[2][1]=0;
        return 1;
    }
    return 0;
}

//function to print the board takes the address of game structure as input
void print(struct games *game){
    short int temp = game->offset;        //variable to store the offset
    char index = 0;
    printf("|~~~|~~~|~~~|\n");
    for(int i=0;i<game->size;i++){
        for(int j=0;j<game->size;j++){
            if(i == game->winner[index][0] && j == game->winner[index][1]){
                printf("|\033[0;32m-%c-\033[0m",game->arr[i][j]);           //print winning blocks with green highlighting
                index++;
                continue;
            }
            if(j==temp){
                printf("| %c_",game->arr[i][j]);
            }
            else{
                printf("| %c ",game->arr[i][j]);
            }
        }
        printf("|\n");
        printf("|~~~|~~~|~~~|\n");
        temp-=game->size;
    }
}

int main() {

    struct games game={3,{"X","O"},'O','\0',{' ',' ',' ',' ',' ',' ',' ',' ',' '},{-1,-1,-1,-1,-1,-1},0,0}; //initializing the game data

    printf("welcome to tik-tac-toe\n");                 //welcome message
    printf("Enter the name of player 1 : ");        //input player names
    scanf("%s",game.player[0]);
    printf("Enter the name of player 2 : ");
    scanf("%s",game.player[1]);
    printf("player %s please choose X or O\n",game.player[0]);      //ask player1 to choose X or O
    scanf(" %c",&game.current);
    if(game.current>='a'&&game.current<='z'){       //converting the input to uppercase
        game.current=game.current-32;
    }
    while (game.current!='X' && game.current!='O')  //checking the input
    {
        system("clear");
        printf("invalid input\n");                  //if input is invalid
        printf("player %s please choose X or O\n",game.player[0]);
        scanf("%c",&game.current);                  //taking input again
        if(game.current>='a'&&game.current<='z'){
        game.current=game.current-32;
    }
    }
    getchar();
    printf("press enter to continue\n");
    getchar();
    while(check(&game)==0){                //checking the winner
        system("clear");
        printf("Use ENTER key to traverse the board\n");
        printf("%s's turn(%c)\n",game.player[game.cnt%2],game.current);
        print(&game);
        game.choice=getchar();                  //taking the input
        while(game.choice=='\n'||game.choice==' '||game.choice=='\t'){  //if the input is a whitespace
            game.offset++;                    //increment the offset
            if(game.offset==9){             //if the offset is 9 reset it to 0
                game.offset=0;
            }
            system("clear");
            printf("%s's turn(%c)\n",game.player[game.cnt%2],game.current);
            print(&game);
            game.choice=getchar();
        }
        if(game.choice>='a'&&game.choice<='z'){     //converting the input to uppercase
            game.choice=game.choice-32;
        }
        if(game.choice==game.current){              //if the input is valid
            if(game.arr[game.offset/game.size][game.offset%game.size]=='X'||game.arr[game.offset/game.size][game.offset%game.size]=='O'){   //if the space is already taken
                printf("space is already taken by %c\n",game.arr[game.offset/game.size][game.offset%game.size]);
                game.offset=-1;
                sleep(1);
                continue;
            }
            game.arr[game.offset/game.size][game.offset%game.size]=game.choice; //store the input in the array
        }
        else if((game.choice==game.current+9||game.choice==game.current-9)&&(game.choice!='X'+9&&game.choice!='O'-9)){  //if player enters the opposite character
            printf("the turn is %c's \n",game.current);     //print the current player
            sleep(1);
            continue;
        }
        else{                                        //if the input is invalid
            system("clear");
            printf("invalid input\n");
            printf("press enter to continue\n");
            sleep(1);
            game.offset=0;
            continue;
        }
        if(game.current=='X'){                //changing the player turn
            game.current='O';
        }
        else{
            game.current='X';
        }
        if(game.cnt==8){                    //if the game is draw just print the board and exit
            if(check(&game)){               //check if there is a victory at the end
                game.cnt++;
                break;
            }
            system("clear");
            printf("Game ended with DRAW\n");
            print(&game);
            return 0;                       //exit the game
        }
        game.cnt++;                     //increment the move count
    }
    if(game.current=='X'){              //changing back player to declare the winner
        game.current='O';
    }
    else{
        game.current='X';
    }
    game.cnt++;
    system("clear");
    printf("Game ended with %s(%c)'s win\n",game.player[game.cnt%2],game.current);  //print the winner
    print(&game);               //print the board
    return 0;               //exit the program
}
