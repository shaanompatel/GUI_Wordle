#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
const int BOX_WIDTH = 100;
const int BOX_HEIGHT = 100;
const int SCREEN_WIDTH = 530;
const int SCREEN_HEIGHT = 635;

int texW = 100;
int texH = 100;
char alpha[26] = {'a', 'b', 'c', 'd','e', 'f','g', 
				'h','i', 'j','k', 'l','m', 'n','o',
				 'p','q', 'r','s', 't','u', 'v','w', 
				 'x','y','z'};
char grid[10][10];
int color_grid[5][6];


SDL_Color color = {225, 225, 225};
SDL_Window* window;
SDL_Renderer* Renderer;
TTF_Font* font;

void renderText(char* let, int r, int c);
void drawGrid();
void initGrids();
void printGrid();

int main(int argc, char** argv){
	initGrids();
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();	

	window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	Renderer = SDL_CreateRenderer(window, -1, 0);
	font = TTF_OpenFont("arial.ttf", 90);


	drawGrid();
	
	int Running = 1;
	SDL_Event Event = {0};
	
	int row = 0;
	int col = 0;
	while(Running) {
		while(SDL_PollEvent(&Event)) {
			switch(Event.type) {
				case SDL_QUIT: { Running = 0; }
				case SDL_KEYDOWN: { 
					int key = Event.key.keysym.sym;
					if (key >= 97 && key <= 122){
						if (col < 5){
							grid[row][col] = alpha[key-97];
							printf("%c\n", grid[row][col]);
							printGrid();
							col++;
						}
						// add letter to current index of array
						// draw grid
					} else if (key == SDLK_RETURN) {
						printf("Enter Pressed\n");
						printf("%d\n", col);

						if (col == 5 && row < 5){
								row++;
								col = 0;
						}
					} else if (key == SDLK_BACKSPACE){
						if (col > 0){
								col -= 1;
								grid[row][col] = ' ';
								printGrid();
								printf("backspace pressed");
						}
					} else if (key == SDLK_ESCAPE){
						Running = 0;
					}
					drawGrid();
				}
				
				break;
			}
		}

		//enter things happening here

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
	
	int colCoord = (col*105) + 15;
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
	
	SDL_SetRenderDrawColor(Renderer, 50, 50, 50, 255);

	int r = 0;
	int c = 0;
	int xpos = 5;
	int ypos = 5;

	for (r = 0; r < 6; r++){
		for (c = 0; c < 5; c++){
			SDL_RenderFillRect(Renderer, &(SDL_Rect){xpos, ypos, BOX_WIDTH, BOX_HEIGHT});
			char temp[2];
			temp[0] = grid[r][c];
			temp[1] = '\0';
			renderText(temp, r, c);
			xpos += BOX_WIDTH + 5;		
		}
		ypos += BOX_HEIGHT + 5;
		xpos = 5;
	}
}

void initGrids(){
	int i;
	int j; 
	for(i = 0; i < 6; i++){
		for (j = 0; j < 5; j++){
			grid[i][j] = '-';
			color_grid[i][j] = 0;
		}
	}
}

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