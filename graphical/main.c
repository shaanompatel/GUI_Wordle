#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
const int BOX_WIDTH = 100;
const int BOX_HEIGHT = 100;
const int SCREEN_WIDTH = 530;
const int SCREEN_HEIGHT = 635;


int main(int argc, char** argv){
	SDL_Window* window = NULL;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	SDL_Renderer* Renderer = SDL_CreateRenderer(window, -1, 0);

	int Running = 1;
	SDL_Event Event = {0};
	
	while(Running) {
		while(SDL_PollEvent(&Event)) {
			switch(Event.type) {
				case SDL_QUIT: { Running = 0; }
				case SDL_KEYDOWN: { 
					switch(Event.key.keysym.sym) {
						case SDLK_o: { Running = 0; }
						break;
					}
				}
				break;
			}
		}

		//enter things happening here
		SDL_SetRenderDrawColor(Renderer, 30, 30, 30, 255);
		SDL_RenderClear(Renderer);
		
		SDL_SetRenderDrawColor(Renderer, 50, 50, 50, 255);
		//SDL_RenderFillRect(Renderer, &(SDL_Rect){10, 10, BOX_WIDTH, BOX_HEIGHT});
		
		int x;
		int y;
		int xpos = 5;
		int ypos = 5;
		for (y = 0; y < 6; y++){
			for (x = 0; x < 5; x++){
				SDL_RenderFillRect(Renderer, &(SDL_Rect){xpos, ypos, BOX_WIDTH, BOX_HEIGHT});
				xpos += BOX_WIDTH + 5;		
			}
			ypos += BOX_HEIGHT + 5;
			xpos = 5;
		}
		// Display rendered content
		
		SDL_RenderPresent(Renderer);
	}
	
	SDL_DestroyWindow(window);
	SDL_Quit();	
	return 0;
}
