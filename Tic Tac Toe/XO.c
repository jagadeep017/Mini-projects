#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct games
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


int check(struct games *game){
    int i=0,j=0;

    for(i=0;i<game->size;i++){
            if((game->arr[i][0]=='X' && game->arr[i][1]=='X' && game->arr[i][2]=='X')||(game->arr[i][0]=='O' && game->arr[i][1]=='O' && game->arr[i][2]=='O')){
                game->winner[0][0]=i;
                game->winner[0][1]=0;
                game->winner[1][0]=i;
                game->winner[1][1]=1;
                game->winner[2][0]=i;
                game->winner[2][1]=2;
                return 1;
            }
            if((game->arr[0][i]=='X' && game->arr[1][i]=='X' && game->arr[2][i]=='X')||(game->arr[0][i]=='O' && game->arr[1][i]=='O' && game->arr[2][i]=='O')){
                game->winner[0][0]=0;
                game->winner[0][1]=i;
                game->winner[1][0]=1;
                game->winner[1][1]=i;
                game->winner[2][0]=2;
                game->winner[2][1]=i;
                return 1;
            }
    }

    if((game->arr[0][0]=='X' && game->arr[1][1]=='X' && game->arr[2][2]=='X')||(game->arr[0][0]=='O' && game->arr[1][1]=='O' && game->arr[2][2]=='O')){
        game->winner[0][0]=0;
        game->winner[0][1]=0;
        game->winner[1][0]=1;
        game->winner[1][1]=1;
        game->winner[2][0]=2;
        game->winner[2][1]=2;
        return 1;
    }
    if((game->arr[0][2]=='X' && game->arr[1][1]=='X' && game->arr[2][0]=='X')||(game->arr[0][2]=='O' && game->arr[1][1]=='O' && game->arr[2][0]=='O')){
        game->winner[0][0]=0;
        game->winner[0][1]=2;
        game->winner[1][0]=1;
        game->winner[1][1]=1;
        game->winner[2][0]=2;
        game->winner[2][1]=0;
        return 1;
    }
    return 0;
}

void print(struct games *game){
    short int temp=game->offset;
    short int flag=0;

    printf("|~~~|~~~|~~~|\n");
    for(int i=0;i<game->size;i++){
        for(int j=0;j<game->size;j++){
            for(int k=0;k<3;k++){
                if(i==game->winner[k][0]&&j==game->winner[k][1]){
                    printf("|-%c-",game->arr[i][j]);
                    flag=1;
                }
            }
            if(flag){
                flag=0;
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

    struct games game={3,{"X","O"},'O','\0',{' ',' ',' ',' ',' ',' ',' ',' ',' '},{30,30,30,30,30,30},0,0};

    printf("welcome to tik-tac-toe\n");
    printf("Enter the name of player 1 : ");
    scanf("%s",game.player[0]);
    printf("Enter the name of player 2 : ");
    scanf("%s",game.player[1]);
    printf("player %s please choose X or O\n",game.player[0]);
    scanf(" %c",&game.current);
    if(game.current>='a'&&game.current<='z'){
        game.current=game.current-32;
    }
    while (game.current!='X' && game.current!='O')
    {
        system("clear");
        printf("invalid input\n");
        printf("player %s please choose X or O\n",game.player[0]); 
        scanf("%c",&game.current);
        if(game.current>='a'&&game.current<='z'){
        game.current=game.current-32;
    }
    }
    getchar();
    printf("press enter to continue\n");
    getchar();
    sleep(1);
    while(check(&game)==0){



        system("clear");
        printf("%s's turn(%c)\n",game.player[game.cnt%2],game.current);
        print(&game);
        game.choice=getchar();
        while(game.choice=='\n'||game.choice==' '||game.choice=='\t'){
            game.offset++;
            if(game.offset==9){
                game.offset=0;
            }
            system("clear");
            printf("%s's turn(%c)\n",game.player[game.cnt%2],game.current);
            print(&game);
            game.choice=getchar();
        }
        if(game.choice>='a'&&game.choice<='z'){
            game.choice=game.choice-32;
        }
        if(game.choice==game.current){
            if(game.arr[game.offset/game.size][game.offset%game.size]=='X'||game.arr[game.offset/game.size][game.offset%game.size]=='O'){
                printf("space is already taken by %c\n",game.arr[game.offset/game.size][game.offset%game.size]);
                game.offset=-1;
                sleep(1);
                continue;
            }
            game.arr[game.offset/game.size][game.offset%game.size]=game.choice;
        }
        else if((game.choice==game.current+9||game.choice==game.current-9)&&(game.choice!='X'+9&&game.choice!='O'-9)){
            printf("the turn is %c's \n",game.current);
            sleep(1);
            continue;
        }
        else{
            system("clear");
            printf("invalid input\n");
            printf("press enter to continue\n");
            sleep(1);
            game.offset=0;
            continue;
        }
        if(game.current=='X'){
            game.current='O';
        }
        else{
            game.current='X';
        }
        if(game.cnt==8){
            system("clear");
            printf("Game ended with DRAW\n");
            print(&game);
            return 0;
        }
        game.cnt++;
    }
    if(game.current=='X'){
        game.current='O';
    }
    else{
        game.current='X';
    }
    game.cnt++;
    system("clear");
    printf("Game ended with %s(%c)'s win\n",game.player[game.cnt%2],game.current);
    game.offset=30;
    print(&game);
    return 0;
}
