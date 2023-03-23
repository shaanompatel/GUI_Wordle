#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define MAX_LINES 12971
#define MAX_LEN 7
#define MAX_LINES_ANS 2309


const int BOX_WIDTH = 100;
const int BOX_HEIGHT = 100;
const int KEY_HEIGHT = 50;
const int KEY_WIDTH = 32;
const int SCREEN_WIDTH = 535;
const int SCREEN_HEIGHT = 780;

int texW = 100;
int texH = 100;
char alpha[26] = {'a', 'b', 'c', 'd','e', 'f','g', 
				'h','i', 'j','k', 'l','m', 'n','o',
				 'p','q', 'r','s', 't','u', 'v','w', 
				 'x','y','z'};
int keyboard[26];
char grid[10][10];
int color_grid[5][6];
char valid[MAX_LINES][MAX_LEN];
char answers[MAX_LINES_ANS][MAX_LEN];
char* ANSWER;
bool won = false;


// 0 is light gray
// 1 is dark ray
// 2 is red
// 3 is yellow
// 4 is green

SDL_Color color = {225, 225, 225};
SDL_Window* window;
SDL_Renderer* Renderer;
TTF_Font* font;

void renderText(char* let, int r, int c);
void drawGrid();
void initGrids();
void printGrid();
char* genAnswer();
int genRandom();
bool evaluate_row(int row);
void color_row(int row);
int find(char a);
bool contains(char *ans, char guess);
int numAppearance(char a, char *ans);
void drawKeyboard();
void finishGame();


int main(int argc, char** argv){
    ANSWER = genAnswer();
	//printf("%s\n", ANSWER);
	
	initGrids();
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();	

	window = SDL_CreateWindow( "Wordle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	Renderer = SDL_CreateRenderer(window, -1, 0);
	font = TTF_OpenFont("arial.ttf", 90);


	drawGrid();
	drawKeyboard();
	
	int Running = 1;
	SDL_Event Event = {0};
	
	int row = 0;
	int col = 0;
	int key;
	while(Running) {
		while(SDL_PollEvent(&Event)) {
			switch(Event.type) {
				case SDL_QUIT: { Running = 0; }
				case SDL_KEYDOWN: { 
					key = Event.key.keysym.sym;
					if (key >= 97 && key <= 122){
						if (col < 5){
							grid[row][col] = alpha[key-97];
							color_grid[row][col] = 1;
							col++;
						}
					} else if (key == SDLK_RETURN) {
						if (col == 5 && row < 5){
							if (evaluate_row(row)){
								row++;
								col = 0;
								drawKeyboard();
							}
						} else if (row == 5){
							if (evaluate_row(row)){	
								if (!won){
									finishGame();
									Sleep(2000);
									Running = 0;
								}
							}
						}
						if (won){
							finishGame();
							Sleep(2000);
							Running = 0;
						}

					} else if (key == SDLK_BACKSPACE){
						if (col > 0){
							col -= 1;
							grid[row][col] = ' ';
							color_grid[row][col] = 0;
						}
					} else if (key == SDLK_ESCAPE){
						Running = 0;
					}
					drawGrid();
					drawKeyboard();
				}
				break;
			}
		}
		// Display rendered content
		SDL_RenderPresent(Renderer);
	}

	
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();	
	return 0;
}

void renderText(char* let, int row, int col){
	font = TTF_OpenFont("arial.ttf", 90);
	
	SDL_Surface* surface = TTF_RenderText_Solid(font, let, color);
	SDL_Texture* message = SDL_CreateTextureFromSurface(Renderer, surface);
	
	int colCoord = (col*105) + 20;
	int rowCoord = row*105;

	SDL_QueryTexture(message, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { colCoord, rowCoord, texW, texH };
	
	SDL_RenderCopy(Renderer, message, NULL, &dstrect);
	
	
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(message);

}


//call render text for each char in array of strings
//call render grid each time enter is pressed, going 

void drawGrid(){
	SDL_SetRenderDrawColor(Renderer, 30, 30, 30, 255);
	SDL_RenderClear(Renderer);
	
	int r = 0;
	int c = 0;
	int xpos = 9;
	int ypos = 5;

	for (r = 0; r < 6; r++){
		for (c = 0; c < 5; c++){
			switch(color_grid[r][c]) {
				case 0:{
					SDL_SetRenderDrawColor(Renderer, 50, 50, 50, 255);
					break;
				}
				case 1:{
					SDL_SetRenderDrawColor(Renderer, 100, 100, 100, 255);
					break;
				}
				case 2:{
					SDL_SetRenderDrawColor(Renderer, 150, 0, 0, 255);
					break;
				}
				case 3:{
					SDL_SetRenderDrawColor(Renderer, 194, 206, 24, 255);
					break;
				}
				case 4:{
					SDL_SetRenderDrawColor(Renderer, 0, 150, 0, 255);
					break;
				}
				break;
			}
			SDL_RenderFillRect(Renderer, &(SDL_Rect){xpos, ypos, BOX_WIDTH, BOX_HEIGHT});
			char temp[2];
			temp[0] = grid[r][c];
			temp[1] = '\0';
			renderText(temp, r, c);
			xpos += BOX_WIDTH + 5;		
		}
		ypos += BOX_HEIGHT + 5;
		xpos = 9;
	}
}


//traverse through the alphabet
//for each letter in the alphabet, check it is in the grid
//if it is, note its position and find its color
//add the color int to the current index 
//only change the color if the new one that is found is greater than
//the one that is currently there

void drawKeyboard(){
	int i = 0;
	int j;
	int z;
	char let;
	int xpos = 27;
	int ypos = 650;
	font = TTF_OpenFont("arial.ttf", 20);
	char temp[2];
	
	SDL_SetRenderDrawColor(Renderer, 50, 50, 50, 255);

	for (z = 0; z < 2; z++){
		for (j = 0; j < 13; j++){
			let = alpha[i];
			temp[0] = let;
			
			switch(keyboard[i]) {
				case 0:{
					SDL_SetRenderDrawColor(Renderer, 50, 50, 50, 255);
					break;
				}
				case 1:{
					SDL_SetRenderDrawColor(Renderer, 100, 100, 100, 255);
					break;
				}
				case 2:{
					SDL_SetRenderDrawColor(Renderer, 150, 0, 0, 255);
					break;
				}
				case 3:{
					SDL_SetRenderDrawColor(Renderer, 194, 206, 24, 255);
					break;
				}
				case 4:{
					SDL_SetRenderDrawColor(Renderer, 0, 150, 0, 255);
					break;
				}
				break;
			}
			
			
			temp[1] = '\0';
			SDL_Surface* surface = TTF_RenderText_Solid(font, temp, color);
			SDL_Texture* message = SDL_CreateTextureFromSurface(Renderer, surface);
			SDL_RenderFillRect(Renderer, &(SDL_Rect){xpos, ypos, KEY_WIDTH, KEY_HEIGHT});
			SDL_QueryTexture(message, NULL, NULL, &texW, &texH);
			SDL_Rect dstrect = { xpos+10, ypos+20, texW, texH };
			SDL_RenderCopy(Renderer, message, NULL, &dstrect);
			SDL_FreeSurface(surface);
			SDL_DestroyTexture(message);
			xpos += KEY_WIDTH + 5;
			i++;
		}
		ypos += 55;
		xpos = 27;
	}
	
}

void initGrids(){
	int i;
	int j; 
	for(i = 0; i < 6; i++){
		for (j = 0; j < 5; j++){
			grid[i][j] = ' ';
			color_grid[i][j] = 0;
		}
	}
}


char* genAnswer(){
    FILE *fpointer = fopen("valid.txt", "r");
    int line = 0;
    while(line <= MAX_LINES && !feof(fpointer)){
        if (fgets(valid[line], MAX_LEN, fpointer)!=NULL){
            valid[line][5] = '\0';
            line++;
        }
    }
    fclose(fpointer);

	FILE *fpointerAns = fopen("answer.txt", "r");
    line = 0;
    while(line <= MAX_LINES_ANS && !feof(fpointerAns)){
        if (fgets(answers[line], MAX_LEN, fpointerAns)!=NULL){
            answers[line][5] = '\0';
            line++;
        }
    }
    fclose(fpointerAns);
    return(answers[genRandom()]);
}

int genRandom(){
    srand(time(NULL)); 
    int num = (rand() % (MAX_LINES_ANS - 0 + 1));
    return num;
}

bool evaluate_row(int row){
	int i;
	bool correct = false;
    for(i=0;i<(sizeof valid / sizeof valid[0]);i++){
		if (strcmp(valid[i], grid[row]) == 0){
           correct = true;
        }
    }

	if (strcmp(ANSWER, grid[row]) == 0){
		won = true;
	}
	
	if(correct){
		color_row(row);
		return true; 
	}
	return false; 
}

void color_row(int row){
	int index = 0;
    char checked[6];

    int i;
    for(i=0;i<5;i++){
        if (grid[row][i] == ANSWER[i]){
			color_grid[row][i] = 4;
            checked[index] = grid[row][i];
			keyboard[find(grid[row][i])] = 4;
            index++;
        }
    }
   
    for(i=0;i<5;i++){
        if (contains(ANSWER, grid[row][i])){
            if (numAppearance(grid[row][i], ANSWER) > numAppearance(grid[row][i], checked)){
                if (color_grid[row][i] != 4){
                    color_grid[row][i] = 3;
                    checked[index] = grid[row][i];
					keyboard[find(grid[row][i])] = 3;
                    index++;
                }
            }
        }
    }
	
	for (i=0;i<5;i++){
		if (color_grid[row][i] != 3 && color_grid[row][i] != 4){
			color_grid[row][i] = 2;
			keyboard[find(grid[row][i])] = 2;

		}
	}
}

int find(char a){
	int i;
	for (i=0;i< (sizeof alpha / sizeof alpha[0]);i++){
		if (a == alpha[i]){
			return i;
		}
	}
	return -1;
}

bool contains(char *ans, char guess){
    int i;
    for(i=0;i<5;i++){
        if (ans[i] == guess){
            return true;
        }
    }
    return false;
}

int numAppearance(char a, char *ans){
    int i;
    int count = 0;
    for(i=0;i<5;i++){
        if (ans[i] == a){
            count++;
        }
    }
    return count;
}

void finishGame(){
	SDL_RenderClear(Renderer);					
	drawGrid();
	drawKeyboard();
	SDL_SetRenderDrawColor(Renderer, 30, 30, 30, 255);
	SDL_RenderFillRect(Renderer, &(SDL_Rect){65, 165, 400, 300});

	font = TTF_OpenFont("arial.ttf", 20);
	int xcoord;

	char output[100];
	if(won){
		strcpy(output, "Congrats, you guessed correctly!");
		xcoord = 120;
	} else {
		strcpy(output, "You lost. The word was ");
		strcat(output, ANSWER);
		strcat(output, ".");
		xcoord = 135;
	}
	
	SDL_Surface* surface = TTF_RenderText_Solid(font, output, color);
	SDL_Texture* message = SDL_CreateTextureFromSurface(Renderer, surface);
	

	SDL_QueryTexture(message, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = {xcoord, 200, texW, texH };
	
	SDL_RenderCopy(Renderer, message, NULL, &dstrect);
	
	
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(message);
	SDL_RenderPresent(Renderer);


}

// debug

void printGrid(){
	int i;
	int j; 
	for(i = 0; i < 6; i++){
		for (j = 0; j < 5; j++){
			printf("%c", grid[i][j]);
		}
		printf("\n");
	}
}
