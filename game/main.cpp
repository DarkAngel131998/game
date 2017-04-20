#include <iostream>
#include <SDL.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int board[2][2];
int check[2][2];
int coutNearMine(int x, int y);
void randomMine(const int m,const int n,const int k);
bool win(const int numberSlot, int numberCout);
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Duy's minesweeper game";
SDL_Window* window ;
SDL_Surface* windowSurface ;
const char *PLOT_IMAGE[10] = {"freq_0.bmp","freq_1.bmp", "flag.bmp","flag.bmp","flag.bmp","flag.bmp","flag.bmp","flag.bmp","flag.bmp","flag.bmp"};
void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Surface* &windowSurface);
void quitSDL(SDL_Window* window, SDL_Surface* windowSurface);
int mouseleft(int x, int y,int score);
void waitUntilKeyPressed();


int main(int argc, char* argv[]) {
    initSDL(window,windowSurface);
    SDL_Surface* normal = NULL ;
    normal = SDL_LoadBMP("normal.bmp");
    for(int i=200;i<=240;i+=40){
        for(int j=200;j<=240;j+=40){
         SDL_Rect location;
            location.x = i;
            location.y = j;
            SDL_BlitSurface(normal,NULL,windowSurface, &location);
        }
    }
    SDL_UpdateWindowSurface(window);
    int k=1,dead=0, score=0;
    int numberSlot=2*2-1;
    randomMine(2, 2, 1);
     while (numberSlot != score) {
            SDL_Event e;
        SDL_Delay(10);
        if ( SDL_WaitEvent(&e) == 0) continue;
        if (e.type == SDL_QUIT) break;
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) break;
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT)
                if(200<= e.button.x && e.button.x<= 280 && e.button.y >=200 && e.button.y <=280)
                score = mouseleft(e.button.x,e.button.y, score);

        }
    }

    waitUntilKeyPressed();
    return 0;
}
void randomMine(const int m,const int n,const int k) {
    int numberOfBoom = k;
    int x, y;
    while(numberOfBoom) {
        srand(time(0));
        x = rand() % m;
        y = rand() % n;
        srand(time(0));
        if(board[x][y]==0) {
            numberOfBoom--;
        }
        board[x][y] = -1;
    }
}
int coutNearMine(int x , int y) {
    int i, j, number=0;
    if(board[x][y] == -1 ){
        board[x][y] == -1;
        return 0;
    }
    for(i=y-1;i<=y+1;i++) {
        for(j=x-1;j<=x+1;j++) {
           if(board[i][j] == -1) number++;
        }
    }
    if(number==0) number=9;
    board[x][y]=number;
}
int mouseleft(int x,int y,int score){
    SDL_Rect filled_rect;
    filled_rect.x = (x/40)*40;
    filled_rect.y = (y/40)*40;
    x = x/40 -5;
    y = y/40 -5;
    if (check[x][y] % 2 !=0){
        SDL_Surface* flag = SDL_LoadBMP("flag.bmp");
        SDL_BlitSurface(flag,NULL,windowSurface, &filled_rect);
    }
    else{
        coutNearMine(x,y);
        if(1<=board[x][y] && board[x][y] <= 9){
            SDL_Surface* status = SDL_LoadBMP(PLOT_IMAGE[board[x][y]]);
            SDL_BlitSurface(status,NULL,windowSurface,&filled_rect);
            score++;
        }
        else{
            for(x=0;x<=1;x++){
                for(y=0;y<=1;y++){
                    coutNearMine(x,y);
                    filled_rect.x = 40*(x+5);
                    filled_rect.y = 40*(y+5);
                    score = 3;
                    if(1<=board[x][y] && board[x][y]<=9){
                        SDL_Surface* loadfullstatus = SDL_LoadBMP(PLOT_IMAGE[board[x][y]]);
                        SDL_BlitSurface(loadfullstatus,NULL,windowSurface,&filled_rect);
                    }
                    else {
                        SDL_Surface* loadfullbom = SDL_LoadBMP("bomb.bmp");
                        SDL_BlitSurface(loadfullbom,NULL,windowSurface,&filled_rect);
                    }
                }
            }
        }

    }
    SDL_UpdateWindowSurface(window);
    return score;


}

bool win(const int numberSlot, int numberCout) {
    if (numberCout==numberSlot) return 1;
    else return 0;
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
    if (windowSurface == nullptr) logSDLError(std::cout, "CreateRenderer", true);

}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}
//**************************************************************

