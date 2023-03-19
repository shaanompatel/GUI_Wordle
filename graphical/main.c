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
				 'x','y','z',};
char grid[5][6];
int color_grid[5][6];
int rowCoord = 15;
int colCoord = 0;

SDL_Color color = {225, 225, 225};
SDL_Window* window;
SDL_Renderer* Renderer;
TTF_Font* font;

void renderText(char* let);
void drawGrid();
void removeEntry();
void removeLine();
void initGrids();

int main(int argc, char** argv){
	initGrids();
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();	

	window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	Renderer = SDL_CreateRenderer(window, -1, 0);
	font = TTF_OpenFont("arial.ttf", 90);

	drawGrid();
	// a is 97, z is 122
	
	int Running = 1;
	SDL_Event Event = {0};
	
	int row;
	int col;

	while(Running) {
		while(SDL_PollEvent(&Event)) {
			switch(Event.type) {
				case SDL_QUIT: { Running = 0; }
				case SDL_KEYDOWN: { 
					if (Event.key.keysym.sym >= 97 && Event.key.keysym.sym <= 122){
						if (col < 5){
							grid[row][col] = alpha[Event.key.keysym.sym-97];
							col++;
						}
						// add letter to current index of array
						// draw grid
						drawGrid();
					}
					switch(Event.key.keysym.sym) {
						case SDLK_ESCAPE: { Running = 0; }
						case SDLK_RETURN: {
							if (row < 6){
								printf("Enter Pressed\n");
								row++;
								colCoord += 105;
								rowCoord = 15;
							}
						}
						case SDLK_BACKSPACE: {
							removeEntry();
						}
						break;
					}
				}
				break;
			}
		}

		//enter things happening here

		// Display rendered content
		SDL_RenderPresent(Renderer);
	}
	
	SDL_DestroyWindow(window);
	//SDL_FreeSurface(surface);
	//SDL_DestroyTexture(message);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();	
	return 0;
}

void renderText(char* let){
	font = TTF_OpenFont("arial.ttf", 90);
	
	SDL_Surface* surface = TTF_RenderText_Solid(font, let, color);
	SDL_Texture* message = SDL_CreateTextureFromSurface(Renderer, surface);
	

	SDL_QueryTexture(message, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { rowCoord, colCoord, texW, texH };
	
	SDL_RenderCopy(Renderer, message, NULL, &dstrect);
	
	rowCoord += 109;
	
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(message);

}

//call render text for each char in array of strings
//call render grid each time enter is pressed, going 

void drawGrid(){
	SDL_SetRenderDrawColor(Renderer, 30, 30, 30, 255);
	SDL_RenderClear(Renderer);
	
	SDL_SetRenderDrawColor(Renderer, 50, 50, 50, 255);

	int x = 0;
	int y = 0;
	int xpos = 5;
	int ypos = 5;

	for (y = 0; y < 6; y++){
		for (x = 0; x < 5; x++){
			SDL_RenderFillRect(Renderer, &(SDL_Rect){xpos, ypos, BOX_WIDTH, BOX_HEIGHT});
			char temp[1];
			temp[0] = grid[y][x];
			renderText(temp);
			xpos += BOX_WIDTH + 5;		
		}
		ypos += BOX_HEIGHT + 5;
		xpos = 5;
	}
}

void removeEntry(){

}

void removeLine(){

}

void initGrids(){
	int i;
	int j; 
	for(i = 0; i < 6; i++){
		for (j = 0; j < 5; j++){
			//grid[i][j] = ' ';
			color_grid[i][j] = 0;
		}
	}



}