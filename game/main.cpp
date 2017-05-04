//********************************************************************************************************************

// MineSweeper
#include <iostream>
#include <SDL.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 680;
const string WINDOW_TITLE = "Duy's Minesweeper Game";
SDL_Window* window ;
SDL_Surface* windowSurface ;
const char* number_bmp[10] = {"n0.bmp","n1.bmp", "n2.bmp","n3.bmp","n4.bmp","n5.bmp","n6.bmp","n7.bmp","n8.bmp","n0.bmp"};
void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Surface* &windowSurface);
void quitSDL(SDL_Window* window, SDL_Surface* windowSurface);
int board[6][6];
int checkflag[6][6];
int opened[6][6];
void randomMine(const int m,const int n,const int k);
int coutNearMine(int x, int y);
int openNear(int x, int y, int score);
int mouseleft(int x, int y,int score);
void mouseright(int x,int y);



int main(int argc, char* argv[]) {
    bool playagain = true;
    //make SDL and start the game
    initSDL(window,windowSurface);
    SDL_Surface* background = NULL;
    background = SDL_LoadBMP("start.bmp");
    SDL_BlitSurface(background,NULL,windowSurface,NULL);
    SDL_UpdateWindowSurface(window);
    while (true) {
        SDL_Event e;
        if ( SDL_WaitEvent(&e) == 0) continue;
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) return 0;
        if (e.type == SDL_QUIT){
            quitSDL(window,windowSurface);
            return 0;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if(306<= e.button.x && e.button.x<= 415 && e.button.y >=300 && e.button.y <=357) break;// play
            if(306<= e.button.x && e.button.x<= 415 && e.button.y >=396 && e.button.y <=450) {//quit
                quitSDL(window,windowSurface);
                return 0;
            }
        }
    }
    // make the board
    while(playagain){
        for(int i=0;i<=5;i++){
            for(int j=0;j<=5;j++){
                board[i][j] = 0;
                checkflag[i][j] = 0;
                opened[i][j]=0;
            }
        }
        background = SDL_LoadBMP("background.bmp");
        SDL_BlitSurface(background,NULL,windowSurface,NULL);
        srand(time(0));
        int score=0 ;
        int numberSlot=6*6-9;
        randomMine(6, 6, 9);
        SDL_Surface* normal = NULL ;
        normal = SDL_LoadBMP("normal.bmp");
        for(int i=252;i<=462;i+=42){
            for(int j=252;j<=462;j+=42){
                SDL_Rect location;
                location.x = i;
                location.y = j;
                SDL_BlitSurface(normal,NULL,windowSurface, &location);
            }
        }
        SDL_UpdateWindowSurface(window);
        //begin play game
        while (numberSlot != score && score!= -1) {
            SDL_Event e;
            SDL_Delay(10);
            if ( SDL_WaitEvent(&e) == 0) continue;
            if (e.type == SDL_QUIT) return 0;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) return 0;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if(252<= e.button.x && e.button.x<= 502 && e.button.y >=252 && e.button.y <=502)
                    if (e.button.button == SDL_BUTTON_LEFT){
                        score = mouseleft(e.button.x,e.button.y, score);
                        cout << score << " ";
                    }
                    else mouseright(e.button.x,e.button.y);
            }
        }
        //check win or loose and end game
        SDL_Delay(500);
        if(score == -1){
            background = SDL_LoadBMP("loose.bmp");
            SDL_BlitSurface(background,NULL,windowSurface,NULL);
        }
        else {
            background = SDL_LoadBMP("Win.bmp");
            SDL_BlitSurface(background,NULL,windowSurface,NULL);
        }
        SDL_UpdateWindowSurface(window);
        while (true) {
        SDL_Event e;
        if ( SDL_WaitEvent(&e) == 0) continue;
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) return 0;
        if (e.type == SDL_QUIT){
            quitSDL(window,windowSurface);
            return 0;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if(322<= e.button.x && e.button.x<= 470 && e.button.y >=332 && e.button.y <=378) {
                    playagain = true;
                    break;
            }
            if(322<= e.button.x && e.button.x<= 470 && e.button.y >=405 && e.button.y <=442) {
                playagain = false;
                break;
            }
        }
    }
    }
    quitSDL(window,windowSurface);
    return 0;
}

void randomMine(const int m,const int n,const int k) {
    int numberOfBoom = k;
    int x, y;
    while(numberOfBoom) {
        x = rand() % m;
        y = rand() % n;
        if(board[x][y]==0) {
            numberOfBoom--;
        }
        board[x][y] = -1;
    }
}
int countNearMine(int x , int y) {
    int i, j, number=0;
    if(board[x][y] == -1 ){
        board[x][y] == -1;
        return -1;
    }
    for(i=-1;i<2;i++) {
        for(j=-1;j<2;j++) {
           if(x+i>=0 && x+i<=5 && y+j>=0 && y+j<=5){
                if(board[x+i][y+j] == -1) number+=1;
           }
        }
    }
    if(number == 0) number = 9;
    return number;
}
int openNear(int x,int y,int score){
    opened[x][y]= 1;
    SDL_Rect filled_rect1;
    filled_rect1.x = 42*(y+6);
    filled_rect1.y = 42*(x+6);
    board[x][y] = countNearMine(x,y);
    if(1<=board[x][y] && board[x][y] <= 9 ){
        SDL_Surface* status1 = SDL_LoadBMP(number_bmp[board[x][y]]);
        SDL_BlitSurface(status1,NULL,windowSurface,&filled_rect1);
        score ++;
    }
    SDL_UpdateWindowSurface(window);
    if(board[x][y] == 9){
        for(int i = -1;i < 2;i++){
            for ( int j = -1;j < 2;j++){
                if(i != 0 || j != 0){
                    if(x+i>=0 && x+i <= 5 && y+j>=0 && y+j<=5){
                        if(opened[x+i][y+j] == 0){
                            score = openNear(x+i,y+j,score);
                        }
                    }
                }
            }
        }
    }
    return score;
}
int mouseleft(int x,int y,int score){
    SDL_Rect filled_rect;
    filled_rect.x = (x/42)*42;
    filled_rect.y = (y/42)*42;
    x = (x/42) - 6;
    y = (y/42) - 6;
    if (checkflag[y][x] %2 == 0){
        board[y][x] = countNearMine(y,x);
        if (board[y][x] == 9 && opened[y][x] == 0){
            score = openNear(y,x,score);
        }
        if(1<=board[y][x] && board[y][x] <= 8 && opened[y][x]==0){
            opened[y][x] = 1;
            SDL_Surface* status = SDL_LoadBMP(number_bmp[board[y][x]]);
            SDL_BlitSurface(status,NULL,windowSurface,&filled_rect);
            score++;
        }
        if (board[y][x]== -1){
            for(x=0;x<=5;x++){
                for(y=0;y<=5;y++){
                    board[x][y] = countNearMine(x,y);
                    filled_rect.x = 42*(y+6) ;
                    filled_rect.y = 42*(x+6) ;
                    if(1<=board[x][y] && board[x][y]<=9){
                        SDL_Surface* loadfullstatus = SDL_LoadBMP(number_bmp[board[x][y]]);
                        SDL_BlitSurface(loadfullstatus,NULL,windowSurface,&filled_rect);
                    }
                    else {
                        SDL_Surface* loadfullbom = SDL_LoadBMP("bomb.bmp");
                        SDL_BlitSurface(loadfullbom,NULL,windowSurface,&filled_rect);
                    }
                }
            }
            score = -1 ;
        }

    }
    SDL_UpdateWindowSurface(window);
    return score;


}
void mouseright(int x,int y){
    SDL_Rect flag;
    flag.x = (x/42)*42;
    flag.y = (y/42)*42;
    x = x/42 - 6;
    y = y/42 - 6;
    checkflag[y][x]++;
    if(checkflag[y][x] %2 !=0 ){
        if(board[y][x]==0 || board[y][x]== -1){
            SDL_Surface* flag1 = SDL_LoadBMP("flag.bmp");
            SDL_BlitSurface(flag1, NULL , windowSurface , &flag);
        }
    }
    else {
        if(board[y][x] ==0 || board[y][x] == -1 ){
            SDL_Surface* normal = SDL_LoadBMP("normal.bmp");
            SDL_BlitSurface(normal,NULL,windowSurface,&flag);
        }
    }
    SDL_UpdateWindowSurface(window);
}


void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Surface* &windowSurface)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);


    windowSurface = SDL_GetWindowSurface(window);
    if (windowSurface == nullptr) logSDLError(std::cout, "CreateWindowSurface", true);

}

void quitSDL(SDL_Window* window, SDL_Surface* windowSurface)
{
	SDL_FreeSurface(windowSurface);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


//***************************************************************************************************************************************

