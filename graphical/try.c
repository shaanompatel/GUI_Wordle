#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
	
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window *Window = 
        SDL_CreateWindow("Title",
                         SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED,
                         640, 640, 0);
						 
    // Create a renderer
	
    SDL_Renderer* Renderer = SDL_CreateRenderer(Window, -1, 0);
    
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
        
        // Render to a backbuffer
        
        SDL_SetRenderDrawColor(Renderer, 30, 30, 30, 255);
        SDL_RenderClear(Renderer);
        
        SDL_SetRenderDrawColor(Renderer, 0, 0, 50, 255);
        SDL_RenderFillRect(Renderer, &(SDL_Rect){64, 64, 64, 64});
        
        // Display rendered content
        
        SDL_RenderPresent(Renderer);
    }
    
    SDL_Quit();
    
	return 0;
}