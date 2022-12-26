#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h> 

#define ROW 10
#define COLUMN 10
#define MAXSTACK 500
#define B(piece) "\033[34m"#piece"\033[0m"
#define R(piece) "\033[31m"#piece"\033[0m"
#define CROSS "　"

int i, j;
char *array[ROW][COLUMN];//The board array.
int xi, yi;//The piece player want to move.
int xj, yj;//The destination of piece movement.
int yt1, yt2;//The input that has not been transform into the real array form.
int xistack[MAXSTACK];
int xjstack[MAXSTACK];
int yistack[MAXSTACK];
int yjstack[MAXSTACK];
char *wstack[MAXSTACK];
int xitop=-1;
int xjtop=-1;
int yitop=-1;
int yjtop=-1;
int wtop=-1;
int inp1, inp2, inp3, inp4;
int func;
char *t;
char text;
int opt;
extern char *optarg; 
char filename;
char *tmp;

bool isStandard = 1;//Whether it breaks the rule, 0 means it does.
bool gameOverSign = 1;//The game ends when it becomes 0.
bool restart = 0;//Whether to restart the game.

struct Node {
    char data;
    struct Node* next;
};

FILE *ChessManual;

void BoardBuilding();
void PrintBoard();
int RedOrBlue(int x, int y);
void redMove();
void blueMove();
void isGameOver();
void rulesOfAllKindsOfChessPieces();
void MovingThePiece();
void NumberOrNot();
void xipush(int data); 
int xipop();
void xjpush(int data);
int xjpop();
void yipush(int data); 
int yipop();
void yjpush(int data); 
int yjpop();
void wpush(char *data);
char *wpop();
int isEmpty();
void regret();
void filewrite();
void rewrite();
void translating(int i, int j);
void fileread();
void manual();


//TODO: getopt -n -s -l
int main(int argc, char *argv[])
{   
    //程式執行時，引數決定 1)開新局 “–n new_game_file_name” 或 2)載入棋譜 “–l old_game_file_name”。
    while((opt = getopt(argc, argv, "n:l:")) != -1) 
    { 
        switch(opt) 
        { 
            case 'n':
                
                BoardBuilding();
                PrintBoard();
                ChessManual = fopen(optarg, "w+");
                filewrite();
                //Beginning of the game.
                int turn = -1;
                while (gameOverSign) {
                    isStandard = 1;
                    turn *= (-1);
                    switch (turn) {
                        case 1:
                            blueMove();
                            turn = (restart) ? (turn*-1) : turn;
                            break;
                        case -1:
                            redMove();
                            turn = (restart) ? (turn*-1) : turn;
                            break;
                    }
                    isGameOver();
                    
                }
                printf("Game over!\n");
                fclose(ChessManual);
                break;

            case 'l': 
                system("clear");
                ChessManual = fopen(optarg, "r");
                manual();
                fclose(ChessManual);
                break;

            case '?': 
                printf("C'mon dude, it's just n and l. What's wrong with you?");
                break; 
        } 
    } 

    
    // printf("Enter '1' to open a new game.\nEnter '2' to open the manual.\n");
    // scanf("%d", &func);

    // if(func == 1){
    
    //     int i, j;
    //     BoardBuilding();
    //     PrintBoard();
    //     ChessManual = fopen("filename", "w+");
    //     filewrite();
    //     //Beginning of the game.
    //     int turn = -1;
    //     while (gameOverSign) {
    //         isStandard = 1;
    //         turn *= (-1);
    //         switch (turn) {
    //             case 1:
    //                 blueMove();
    //                 turn = (restart) ? (turn*-1) : turn;
    //                 break;
    //             case -1:
    //                 redMove();
    //                 turn = (restart) ? (turn*-1) : turn;
    //                 break;
    //         }
    //         isGameOver();
            
    //     }
    //     printf("Game over!\n");
    //     fclose(ChessManual);
    // }

    // else if(func == 2){
    //     system("clear");
    //     ChessManual = fopen("filename", "r");
    //     manual();
    //     fclose(ChessManual);
    // }

    // else{
    //     printf("C'mon dude, it's just 1 and 2. What's wrong with you?");
    // }

    return 0;
}

//Define the initial status of the board.
void BoardBuilding()
{
    for (int i = 0; i < ROW; i ++) {
        for (int j = 0; j < COLUMN ; j ++) {
            array[i][j] = "　";
        }
        printf("\n");
    }

    //Define the numbers on the board.
    array[0][0] = "９";
    array[0][1] = "８";
    array[0][2] = "７";
    array[0][3] = "６";
    array[0][4] = "５";
    array[0][5] = "４";
    array[0][6] = "３";
    array[0][7] = "２";
    array[0][8] = "１";

    array[1][9] = "１";
    array[2][9] = "２";
    array[3][9] = "３";
    array[4][9] = "４";
    array[5][9] = "５";
    array[6][9] = "６";
    array[7][9] = "７";
    array[8][9] = "８";
    array[9][9] = "９";

    array[0][9] = "－";

    //Define the initial places of the red pieces.
    array[1][0] = array[1][8] = R(香);
    array[1][1] = array[1][7] = R(桂);
    array[1][2] = array[1][6] = R(銀);
    array[1][3] = array[1][5] = R(金);
    array[1][4] = R(王);
    array[2][1] = R(飛);
    array[2][7] = R(角);
    array[3][0] = array[3][1] = array[3][2] = array[3][3] = array[3][4] = array[3][5] = array[3][6] = array[3][7] = array[3][8] =R(步);
    
    //Define the initial places of the blue pieces.
    array[9][0] = array[9][8] = B(香);
    array[9][1] = array[9][7] = B(桂);
    array[9][2] = array[9][6] = B(銀);
    array[9][3] = array[9][5] = B(金);
    array[9][4] = B(王);
    array[8][1] = B(角);
    array[8][7] = B(飛);
    array[7][0] = array[7][1] = array[7][2] = array[7][3] = array[7][4] = array[7][5] = array[7][6] = array[7][7] = array[7][8] =B(步);
}

void PrintBoard()
{
	system("clear");
    
    for (int i = 0; i < ROW ;  i ++) {
        for (int j = 0; j < COLUMN ; j ++) {
            printf("%s", array[i][j]);
        }
        printf("\n");
    }

}

//Determine whether it's a red one or a blue one.
int RedOrBlue(int x, int y)
{
    if (array[x][y] == R(香) || array[x][y] == R(桂) || array[x][y] == R(銀) || array[x][y] == R(金) || array[x][y] == R(王) || array[x][y] == R(角) || array[x][y] == R(飛) || array[x][y] == R(步))
    {
        return  1;
    }
    else if (array[x][y] == B(香) || array[x][y] == B(桂) || array[x][y] == B(銀) || array[x][y] == B(金) || array[x][y] == B(王) || array[x][y] == B(角) || array[x][y] == B(飛) || array[x][y] == B(步))
    {
        return -1;
    }
    else
        return 0;
}

//Red's round progress.
void redMove()
{
    if (restart) {
        printf("\nBreaks the rule. Please re-enter\n");
        restart = 0;
    }
    printf("\n\033[31m[RED]\033[0m Please enter the piece you want to move:\n");
    scanf("%d",&inp1);    
  
    if(inp1 != 0 && inp1 != -1)
    {   
        scanf(" %d",&inp2);
        yi = 9 - inp1;
        xi = inp2;

        while(RedOrBlue(xi, yi) != 1)
        {           
            printf("\nBreaks the rule. Please re-enter\n");
            printf("\n\033[31m[RED]\033[0m Please enter the piece you want to move:\n");
            scanf("%d",&inp1);
            if(inp1 != 0 && inp1 != -1)
            {   
                scanf(" %d",&inp2);
                yi = 9 - inp1;
                xi = inp2;

            }else{
                if(inp1 == 0)
                {
                    regret();
                    rewrite();
                    filewrite();
                }
                if(inp1 == -1)
                {
                    filewrite();
                    gameOverSign = 0;
                }
            }
        }

        printf("\033[31m[RED]\033[0m Please enter the place you want to put the piece onto:\n");
        scanf("%d %d",&inp3,&inp4);

        yj = 9 - inp3;
        xj = inp4;

        rulesOfAllKindsOfChessPieces();
        
        if(!restart){
            xipush(xi);
            yipush(yi);
            xjpush(xj);
            yjpush(yj);
            wpush(array[xj][yj]);
            
            MovingThePiece();
            PrintBoard();
            filewrite();
        }
    }
    else{
        if(inp1 == 0)
        {
            regret();
            rewrite();
            filewrite();
        }
        if(inp1 == -1)
        {
            filewrite();
            gameOverSign = 0;
        }
    }
}

//Blue's round progress.
void blueMove()
{
    if (restart) {
        printf("\nBreaks the rule. Please re-enter\n");
        restart = 0;
    }
    printf("\n\033[34m[Blue]\033[0m Please enter the piece you want to move:\n");
    scanf("%d",&inp1);

    if(inp1 != 0 && inp1 != -1) 
    {   
        scanf(" %d",&inp2);
        yi = 9 - inp1;
        xi = inp2;

        while(RedOrBlue(xi, yi) != -1)
        {   
            printf("\nBreaks the rule. Please re-enter\n");
            printf("\n\033[34m[Blue]\033[0m Please enter the piece you want to move:\n");
            scanf("%d",&inp1);

            if(inp1 != 0 && inp1 != -1)
            {   
                scanf(" %d",&inp2);
                yi = 9 - inp1;
                xi = inp2;

            }else{
                if(inp1 == 0)
                {
                    regret();
                    rewrite();
                    filewrite();
                }
                if(inp1 == -1)
                {   
                    filewrite();
                    gameOverSign = 0;
                }
            }
        }

        printf("\033[34m[Blue]\033[0m Please enter the place you want to put the piece onto:\n");
        scanf("%d %d",&inp3,&inp4);
        
        yj = 9 - inp3;
        xj = inp4;

        rulesOfAllKindsOfChessPieces();

        if(!restart){
            xipush(xi);
            yipush(yi);
            xjpush(xj);
            yjpush(yj);
            wpush(array[xj][yj]);

            MovingThePiece();
            PrintBoard();
            filewrite();
        }
    }
    else{
        if(inp1 == 0)
        {
            regret();
            rewrite();
            filewrite();
        }
        if(inp1 == -1)
        {
            filewrite();
            gameOverSign = 0;
        }
    }
}

//Determine wheher the game is over.
void isGameOver()
{
    bool sign_r = 0;
    bool sign_b = 0;
    for (int i = 0; i < ROW; i ++) {
        for (int j = 0; j < COLUMN; j ++) {
            if (array[i][j] == R(王)) {
                sign_r = 1;
            }
            else if (array[i][j] == B(王))
            {
                sign_b = 1;
            }
        }
    }
    if ((sign_r == 0)||(sign_b == 0)) {
        gameOverSign = 0;
    }
}

//Define the ways each kind of piece move.
void rulesOfAllKindsOfChessPieces(){
    
    //R（王）
    if (array[xi][yi] == R(王))
    {   
        NumberOrNot(xj, yj);

        if ((RedOrBlue(xj, yj) != 1) && ( ((xj-xi)*(xj-xi)+(yj-yi)*(yj-yi)) <= 2))
        {
            restart = 0;
        }
        else
        {
            restart = 1;
        }
    }

    //B（王）
    else if (array[xi][yi] == B(王))
    {   
        NumberOrNot(xj, yj);

        if ((RedOrBlue(xj, yj) != -1) && ( ((xj-xi)*(xj-xi)+(yj-yi)*(yj-yi)) <= 2))
        {
            restart = 0;
        }
        else
        {
            restart = 1;
        }
    }
    
    //R（金）
    else if (array[xi][yi] == R(金))
    {   
        NumberOrNot(xj, yj);

        if(xj == xi-1 && abs(yj-yi) == 1 )
            isStandard = 0;

        if(abs(xj-xi) > 1 || abs(yj-yi) > 1)
            isStandard = 0;

        if (isStandard && (RedOrBlue(xj, yj) != 1))
        {
            restart = 0;
        }
        else
        {
            restart = 1;
        }
    }

    //B（金）
    else if (array[xi][yi] == B(金))
    {   
        NumberOrNot(xj, yj);

        if(xj == xi+1 && abs(yj-yi) == 1 )
            isStandard = 0;

        if(abs(xj-xi) > 1 || abs(yj-yi) > 1)
            isStandard = 0;

        if (isStandard && (RedOrBlue(xj, yj) != 1))
        {
            restart = 0;
        }
        else
        {
            restart = 1;
        }
    }

    //R（銀）
    else if (array[xi][yi] == R(銀))
    {   
        NumberOrNot(xj, yj);

        if(xj == xi)
            isStandard = 0;

        if((xj == xi-1) && (yj == yi))
            isStandard = 0;

        if(abs(xj-xi) > 1 || abs(yj-yi) > 1)
            isStandard = 0;

        if (isStandard && (RedOrBlue(xj, yj) != 1))
        {
            restart = 0;
        }
        else
        {
            restart = 1;
        }
    }

    //B（銀）
    else if (array[xi][yi] == B(銀))
    {   
        NumberOrNot(xj, yj);

        if(xj == xi)
            isStandard = 0;
        
        if((xj == xi+1) && (yj == yi))
            isStandard = 0;

        if(abs(xj-xi) > 1 || abs(yj-yi) > 1)
            isStandard = 0;

        if (isStandard && (RedOrBlue(xj, yj) != -1))
        {
            restart = 0;
        }
        else
        {
            restart = 1;
        }
    }

    //R（桂）
    else if (array[xi][yi] == R(桂))
    {   
        NumberOrNot(xj, yj);

        if(xj-xi != 2 || abs(yj-yi) != 1)
            isStandard = 0;

        if (isStandard && (RedOrBlue(xj, yj) != 1))
        {
            restart = 0;
        }
        else
        {
            restart = 1;
        }
    }

    //B（桂）
    else if (array[xi][yi] == B(桂))
    {   
        NumberOrNot(xj, yj);

        if(xi-xj != 2 || abs(yj-yi) != 1)
            isStandard = 0;

        if (isStandard && (RedOrBlue(xj, yj) != -1))
        {
            restart = 0;
        }
        else
        {
            restart = 1;
        }
    }

    //R（香）
    else if (array[xi][yi] == R(香))
    {   
        NumberOrNot(xj, yj);

        if (yi == yj)//Column coordinate stays the same.
        {
            for (int i = xi+1; i < xj; i ++)
            {
                if (array[i][yi] != CROSS)
                    isStandard = 0;//If there's a piece between the initial place and the destination, then it breaks the rule.
            }
        }

        if(yj != yi)
            isStandard = 0;
        
        if(xj < xi)
            isStandard = 0;

        if ( isStandard && (RedOrBlue(xj, yj) != 1))
        {
            restart = 0;
        }
        else
        {
            restart = 1;
        }
    }

    //B（香）
    else if (array[xi][yi] == B(香))
    {   
        NumberOrNot(xj, yj);

        if (yi == yj)//Column coordinate stays the same.
        {
            for (int i = xi-1; i > xj; i --)
            {
                if (array[i][yi] != CROSS)
                    isStandard = 0;//If there's a piece between the initial place and the destination, then it breaks the rule.
            }
        }

        if(yj != yi)
            isStandard = 0;
        
        if(xj > xi)
            isStandard = 0;

        if ( isStandard && (RedOrBlue(xj, yj) != -1))
        {
            restart = 0;
        }
        else
        {
            restart = 1;
        }
    }
    
    //R（飛）
    else if (array[xi][yi] == R(飛))
    {   
        NumberOrNot(xj, yj);

        if (yi == yj)//Column coordinate stays the same.
        {
            for (int i = xi+1; i < xj; i ++)
            {
                if (array[i][yi] != CROSS)
                    isStandard = 0;//If there's a piece between the initial place and the destination, then it breaks the rule.
            }
            for (int i = xi-1; i > xj; i --)
            {
                if (array[i][yi] != CROSS)
                    isStandard = 0;
            }
        }
        else if (xi == xj)//Row coordinate stays the same.
        {
            for (int i = yi+1; i < yj; i ++)
                if (array[xi][i] != CROSS)
                    isStandard = 0;
            for (int i = yi-1; i > yj; i --)
                if (array[xi][i] != CROSS)
                    isStandard = 0;
        }
        
        if ((xi == xj || yi == yj)&& isStandard && (RedOrBlue(xj, yj) != 1))
        {
            restart = 0;
        }
        else
        {
            restart = 1;
        }
    }

    //B（飛）
    else if (array[xi][yi] == B(飛))
    {
        NumberOrNot(xj, yj);

        if (yi == yj)//Column coordinate stays the same.
        {
            for (int i = xi+1; i < xj; i ++)
            {
                if (array[i][yi] != CROSS)
                    isStandard = 0;//If there's a piece between the initial place and the destination, then it breaks the rule.
            }
            for (int i = xi-1; i > xj; i --)
            {
                if (array[i][yi] != CROSS)
                    isStandard = 0;
            }
        }
        else if (xi == xj)//Row coordinate stays the same.
        {
            for (int i = yi+1; i < yj; i ++)
            {
                if (array[xi][i] != CROSS)
                    isStandard = 0;
            }

            for (int i = yi-1; i > yj; i --)
            {
                if (array[xi][i] != CROSS)
                    isStandard = 0;
            }
        }
        if ((xi == xj || yi == yj)&& isStandard && RedOrBlue(xj, yj) != -1)
        {
            restart = 0;
        }
        else
        {
            restart = 1;
        }
    }

    //R（角）
    else if (array[xi][yi] == R(角))
    {   
        NumberOrNot(xj, yj);

        int j=yi;

        if(xj == xi || yj ==yi)
            isStandard = 0;

        else if(xj > xi && yj > yi)
        {
            for (int i = xi+1; i < xj; i ++)
            {
                j=j+1;
                if (array[i][j] != CROSS)
                    isStandard = 0;
            }
        }

        else if(xj > xi && yj < yi)
        {
            for (int i = xi+1; i < xj; i ++)
            {
                j=j-1;
                if (array[i][j] != CROSS)
                    isStandard = 0;
            }
        }

        else if(xj < xi && yj > yi)
        {
            for (int i = xi-1; i > xj; i --)
            {
                j=j+1;
                if (array[i][j] != CROSS)
                    isStandard = 0;
            }
        }

        else if(xj < xi && yj < yi)
        {
            for (int i = xi-1; i > xj; i --)
            {
                j=j-1;
                if (array[i][j] != CROSS)
                    isStandard = 0;
            }
        }

        if (isStandard && RedOrBlue(xj, yj) != 1)
        {
            restart = 0;
        }
        else
        {
            restart = 1;
        }
    }

    //B（角）
    else if (array[xi][yi] == B(角))
    {   
        NumberOrNot(xj, yj);

        int j=yi;

        if(xj == xi || yj ==yi)
            isStandard = 0;

        else if(xj > xi && yj > yi)
        {
            for (int i = xi+1; i < xj; i ++)
            {
                j=j+1;
                if (array[i][j] != CROSS)
                    isStandard = 0;
            }
        }

        else if(xj > xi && yj < yi)
        {
            for (int i = xi+1; i < xj; i ++)
            {
                j=j-1;
                if (array[i][j] != CROSS)
                    isStandard = 0;
            }
        }

        else if(xj < xi && yj > yi)
        {
            for (int i = xi-1; i > xj; i --)
            {
                j=j+1;
                if (array[i][j] != CROSS)
                    isStandard = 0;
            }
        }

        else if(xj < xi && yj < yi)
        {
            for (int i = xi-1; i > xj; i --)
            {
                j=j-1;
                if (array[i][j] != CROSS)
                    isStandard = 0;
            }
        }

        if (isStandard && RedOrBlue(xj, yj) != -1)
        {
            restart = 0;
        }
        else
        {
            restart = 1;
        }
    }

    //R（步）
    else if (array[xi][yi] == R(步))
    {   
        NumberOrNot(xj, yj);

        if(xj == xi || xj < xi || xj-xi > 1 || yj != yi)
            isStandard = 0;

        if (isStandard && (RedOrBlue(xj, yj) != 1))
        {
            restart = 0;
        }
        else
        {
            restart = 1;
        }

    }

    //B（步）
    else if (array[xi][yi] == B(步))
    {   
        NumberOrNot(xj, yj);

        if(xj == xi ||  xj > xi || xi-xj > 1 || yj != yi)
            isStandard = 0;

        if (isStandard && (RedOrBlue(xj, yj) != -1))
        {
            restart = 0;
        }
        else
        {
            restart = 1;
        }
    }


    else {
        restart = 1;
    }
}

//Moving the selected piece to its destination.
void MovingThePiece()
{
    //R（王）
    if (array[xi][yi] == R(王))
    {   
        array[xi][yi] = CROSS;
        array[xj][yj] = R(王);
    }

    //B（王）
    else if (array[xi][yi] == B(王))
    {   
        array[xi][yi] = CROSS;
        array[xj][yj] = B(王);
    }
    
    //R（金）
    else if (array[xi][yi] == R(金))
    {   
        array[xi][yi] = CROSS;
        array[xj][yj] = R(金);
    }

    //B（金）
    else if (array[xi][yi] == B(金))
    {   
        array[xi][yi] = CROSS;
        array[xj][yj] = B(金);
    }

    //R（銀）
    else if (array[xi][yi] == R(銀))
    {   
        array[xi][yi] = CROSS;
        array[xj][yj] = R(銀);
    }

    //B（銀）
    else if (array[xi][yi] == B(銀))
    {   
        array[xi][yi] = CROSS;
        array[xj][yj] = B(銀);
    }

    //R（桂）
    else if (array[xi][yi] == R(桂))
    {   
        array[xi][yi] = CROSS;
        array[xj][yj] = R(桂);
    }

    //B（桂）
    else if (array[xi][yi] == B(桂))
    {   
        array[xi][yi] = CROSS;
        array[xj][yj] = B(桂);
    }

    //R（香）
    else if (array[xi][yi] == R(香))
    {   
        array[xi][yi] = CROSS;
        array[xj][yj] = R(香);
    }

    //B（香）
    else if (array[xi][yi] == B(香))
    {   
        array[xi][yi] = CROSS;
        array[xj][yj] = B(香);
    }
    
    //R（飛）
    else if (array[xi][yi] == R(飛))
    {   
        array[xi][yi] = CROSS;
        array[xj][yj] = R(飛);
    }

    //B（飛）
    else if (array[xi][yi] == B(飛))
    {
        array[xi][yi] = CROSS;
        array[xj][yj] = B(飛);
    }

    //R（角）
    else if (array[xi][yi] == R(角))
    {   
        array[xi][yi] = CROSS;
        array[xj][yj] = R(角);
    }

    //B（角）
    else if (array[xi][yi] == B(角))
    {   
        array[xi][yi] = CROSS;
        array[xj][yj] = B(角);
    }

    //R（步）
    else if (array[xi][yi] == R(步))
    {
        array[xi][yi] = CROSS;
        array[xj][yj] = R(步);
    }

    //B（步）
    else if (array[xi][yi] == B(步))
    {   
        array[xi][yi] = CROSS;
        array[xj][yj] = B(步);
    }

    else {
        restart = 1;
    }
}

//Determine whether the destination is a border.
void NumberOrNot(int c, int d)
{
    if(array[c][d] == "１" || array[c][d] == "２" || array[c][d] == "３" || array[c][d] == "４" || array[c][d] == "５" || array[c][d] == "６" | array[c][d] == "７" || array[c][d] == "８" || array[c][d] == "９" || array[c][d] == "－")
        isStandard = 0;
}


void xipush(int data){
	if(xitop>=MAXSTACK){
		printf("Stack is full, no more data can be added.\n");	
	}else{
		xitop++;
		xistack[xitop]=data;
	}
}

int xipop(){
        int data;
        data=xistack[xitop];
        xitop--;
        return data;
}

void xjpush(int data){
	if(xjtop>=MAXSTACK){
		printf("Stack is full, no more data can be added.\n");	
	}else{
		xjtop++;
		xjstack[xjtop]=data;
	}
}

int xjpop(){
        int data;
        data=xjstack[xjtop];
        xjtop--;
        return data;
}

void yipush(int data){
	if(yitop>=MAXSTACK){
		printf("Stack is full, no more data can be added.\n");	
	}else{
		yitop++;
		yistack[yitop]=data;
	}
}

int yipop(){
        int data;
        data=yistack[yitop];
        yitop--;
        return data;
}

void yjpush(int data){
	if(yjtop>=MAXSTACK){
		printf("Stack is full, no more data can be added.\n");	
	}else{
		yjtop++;
		yjstack[yjtop]=data;
	}
}

int yjpop(){
        int data;
        data=yjstack[yjtop];
        yjtop--;
        return data;
}

void wpush(char *data){
	if(wtop>=MAXSTACK){
		printf("Stack is full, no more data can be added.\n");	
	}else{
		wtop++;
		wstack[wtop]=data;
	}
}

char *wpop(){
        char *data;
        data=wstack[wtop];
        wtop--;
        return data;
}

//Determine whether the stack is empty.
int isEmpty(){
	if(xitop==-1){
		return 1; 
	}else{
		return 0;
	}

    if(xjtop==-1){
		return 1; 
	}else{
		return 0;
	}

    if(yitop==-1){
		return 1; 
	}else{
		return 0;
	}

    if(yjtop==-1){
		return 1; 
	}else{
		return 0;
	}

    if(wtop==-1){
		return 1; 
	}else{
		return 0;
	}
}  

//Undo progress.
void regret()
{   
    int x1, x2, y1, y2;
    char *w;
    if(isEmpty() == 0)
    {
        x1 = xipop();
        x2 = xjpop();
        y1 = yipop();
        y2 = yjpop();
        w = wpop();
        array[x1][y1] = array[x2][y2];
        array[x2][y2] = w;
        PrintBoard();
        printf("\nJust operated undo.\n");
    }else{
        printf("\nCan't undo yet!\n");
        restart = 1;
    }
}

void filewrite()
{
    for (int i = 1; i < ROW; i ++) {
        for (int j = 0; j < COLUMN-1 ; j ++) {
            translating(i, j);
            fprintf(ChessManual, "%s", t);
        }
        fprintf(ChessManual, "/");
    }
    fprintf(ChessManual, "\n");
}

void rewrite()
{
    fprintf(ChessManual, "\r");
}


void translating(int i, int j)
{   
        if(array[i][j]==R(香))t="l";
        if(array[i][j]==B(香))t="L";
        if(array[i][j]==R(桂))t="n";
        if(array[i][j]==B(桂))t="N";
        if(array[i][j]==R(銀))t="s";
        if(array[i][j]==B(銀))t="S";
        if(array[i][j]==R(金))t="g";
        if(array[i][j]==B(金))t="G";
        if(array[i][j]==R(王))t="k";
        if(array[i][j]==B(王))t="K";
        if(array[i][j]==R(飛))t="r";
        if(array[i][j]==B(飛))t="R";
        if(array[i][j]==R(角))t="b";
        if(array[i][j]==B(角))t="B";
        if(array[i][j]==R(步))t="p";
        if(array[i][j]==B(步))t="P";
        if(array[i][j]==CROSS)t=" ";
}

void translatingback(char tr)
{   
        if(tr=='l')tmp = R(香);
        if(tr=='L')tmp = B(香);
        if(tr=='n')tmp = R(桂);
        if(tr=='N')tmp = B(桂);
        if(tr=='s')tmp = R(銀);
        if(tr=='S')tmp = B(銀);
        if(tr=='g')tmp = R(金);
        if(tr=='G')tmp = B(金);
        if(tr=='k')tmp = R(王);
        if(tr=='K')tmp = B(王);
        if(tr=='r')tmp = R(飛);
        if(tr=='R')tmp = B(飛);
        if(tr=='b')tmp = R(角);
        if(tr=='B')tmp = B(角);
        if(tr=='p')tmp = R(步);
        if(tr=='P')tmp = B(步);
        if(tr==' ')tmp = CROSS;
        if(tr=='/')tmp = "\n";
}

void fileread()
{
    while(!feof(ChessManual)){
        fscanf(ChessManual, "%c", &text);
        printf("%c", text);
    }
    
}

void manual()
{
    struct Node* head = NULL;
    char forb;
    int i = 0;
    int n = 0;

    for(i = n ; i < n+91 ; i++)
    {   
        head = (struct Node*)malloc(sizeof(struct Node));
        fscanf(ChessManual, "%c", &text);
        head->data = text;
        translatingback(text);
        printf("%s", tmp);
        head = head->next;
    }

    while(1)
    {
        printf("\nEnter \'f\' to go to the next round.\nEnter \'b\' to go to the previous round.\n");
        scanf("\n%c", &forb);

        if(forb == 'f')
        {
            system("clear");
            for(i = 0 ; i < 91 ; i++)
            {   
                // fseek(ChessManual, 91, SEEK_CUR);
                head = (struct Node*)malloc(sizeof(struct Node));
                fscanf(ChessManual, "%c", &text);
                head->data = text;
                translatingback(text);
                printf("%s", tmp);
                head = head->next;
            }
        }

        else if(forb == 'b')
        {   
            system("clear");
            fseek(ChessManual, -182, SEEK_CUR);
            for(i = 0 ; i < 91 ; i++)
            {
                
                head = (struct Node*)malloc(sizeof(struct Node));
                fscanf(ChessManual, "%c", &text);
                head->data = text;
                translatingback(text);
                printf("%s", tmp);
                head = head->next;
            }
        }

        else if(forb == 'x')
        {
            break;
        }

        else{
            printf("error\n");
        }
    }
}


 
// This function prints contents of linked list starting
// from the given node
void printList(struct Node* n)
{
    while (n != NULL) {
        printf(" %c ", n->data);
        n = n->next;
    }
}
 

