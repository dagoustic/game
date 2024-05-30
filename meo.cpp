#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow("Lucky Bomb",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    return true;
}

SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* imageTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    else {
        imageTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (imageTexture == NULL) {
            std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return imageTexture;
}

SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& message, SDL_Color color, int& width, int& height) {
    if (TTF_SizeText(font, message.c_str(), &width, &height) != 0) {
        std::cerr << "TTF_SizeText Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, message.c_str(), color);
    if (surfaceMessage == nullptr) {
        std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* textureMessage = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);

    if (textureMessage == nullptr) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    }

    return textureMessage;
}


void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture) {
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
    }
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    IMG_Quit();
    SDL_Quit();
}



int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;

    srand(time(0)); 

    unsigned int enemySpot = rand() % 6 + 1;

    if (!init(window, renderer)) {
        std::cerr << "Failed to initialize!" << std::endl;
        return -1;
    }

    texture = loadTexture("background.png", renderer);
    if (texture == NULL) {
        std::cerr << "Failed to load texture image!" << std::endl;
        close(window, renderer, texture);
        return -1;
    }
    SDL_Color color = { 0, 0, 0 };
    SDL_Texture* rockTexture1 = loadTexture("rock1.png", renderer);
    SDL_Texture* rockTexture2 = loadTexture("rock2.png", renderer);
    SDL_Texture* rockTexture3 = loadTexture("rock3.png", renderer);
    SDL_Texture* rockTexture4 = loadTexture("rock4.png", renderer);
    SDL_Texture* rockTexture5 = loadTexture("rock5.png", renderer);
    SDL_Texture* rockTexture6 = loadTexture("rock6.png", renderer);

    SDL_Texture* enemyTexture = loadTexture("enemy.png", renderer);

    SDL_Texture* finalTexture1 = rockTexture1;
    SDL_Texture* finalTexture2 = rockTexture2;
    SDL_Texture* finalTexture3 = rockTexture3;
    SDL_Texture* finalTexture4 = rockTexture4;
    SDL_Texture* finalTexture5 = rockTexture5;
    SDL_Texture* finalTexture6 = rockTexture6;

    //thu tu rock 1 den 6, tu trai sang phai, tu tren xuong duoi 
    SDL_Rect rock1 = { 70 , 390, 150, 150 };
    SDL_Rect rock2 = { 250, 305, 150, 150 };
    SDL_Rect rock3 = { 250, 445, 150, 150 };
    SDL_Rect rock4 = { 450, 300 ,150, 150 };
    SDL_Rect rock5 = { 425, 440, 150, 150 };
    SDL_Rect rock6 = { 600, 400, 150, 150 };

    TTF_Font* font = TTF_OpenFont("font.ttf", 24);

    int winningWidth = 0, winningHeight = 0;
    int losingWidth = 0, losingHeight = 0;
    SDL_Texture* winningTexture = loadTexture("win.png", renderer);
    SDL_Texture* losingTexture = loadTexture("lose.png",renderer);
    if (winningTexture == NULL or losingTexture == NULL) {
        std::cerr << "Failed to load texture image!" << std::endl;
        return -1;
    }
    bool r1 = true;
    bool r2 = true;
    bool r3 = true; 
    bool r4 = true;
    bool r5 = true;
    bool r6 = true;
    bool quit = false;
    SDL_Event e; 
    int bomb = 3;
    bool flag = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_1: 
                        if (enemySpot == 1) {
                            finalTexture1 = enemyTexture;
                            flag = true;
                        }
                        else {
                            r1 = false;
                            bomb--;
                        }
                        break;
                    case SDLK_2:
                        if (enemySpot == 2) {
                            finalTexture2 = enemyTexture;
                            flag = true;
                        }
                        else {
                            r2 = false;
                            bomb--;
                        }
                        break;
                    case SDLK_3:
                        if (enemySpot == 3) {
                            finalTexture3 = enemyTexture;
                            flag = true;
                        }
                        else {
                            r3 = false;
                            bomb--;
                        }
                        break;
                    case SDLK_4:
                        if (enemySpot == 4) {
                            finalTexture4 = enemyTexture;
                            flag = true;
                        }
                        else {
                            r4 = false;
                            bomb--;
                        }
                        break;
                    case SDLK_5:
                        if (enemySpot == 5) {
                            finalTexture5 = enemyTexture;
                            flag = true;
                        }
                        else {
                            r5 = false;
                            bomb--;
                        }
                        break;
                    case SDLK_6:
                        if (enemySpot == 6) {
                            finalTexture6 = enemyTexture;
                            flag = true;
                        }
                        else {
                            r6 = false;
                            bomb--;
                        }
                        break;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        if (r1) SDL_RenderCopy(renderer, finalTexture1, NULL, &rock1);
        if (r2) SDL_RenderCopy(renderer, finalTexture2, NULL, &rock2);
        if (r3) SDL_RenderCopy(renderer, finalTexture3, NULL, &rock3);
        if (r4) SDL_RenderCopy(renderer, finalTexture4, NULL, &rock4);
        if (r5) SDL_RenderCopy(renderer, finalTexture5, NULL, &rock5);
        if (r6) SDL_RenderCopy(renderer, finalTexture6, NULL, &rock6);
        SDL_RenderPresent(renderer);
        if (bomb == 0) {
            SDL_RenderCopy(renderer, losingTexture, NULL, NULL);
            break;
        }
        else if (flag == true) {
            SDL_RenderCopy(renderer, winningTexture, NULL, NULL);
            break;
        }
    }
    SDL_DestroyTexture(losingTexture);
    SDL_DestroyTexture(winningTexture);

    SDL_DestroyTexture(rockTexture1);
    SDL_DestroyTexture(rockTexture2);
    SDL_DestroyTexture(rockTexture3);
    SDL_DestroyTexture(rockTexture4);
    SDL_DestroyTexture(rockTexture5);
    SDL_DestroyTexture(rockTexture6);

    SDL_DestroyTexture(enemyTexture);

    SDL_DestroyTexture(finalTexture1);
    SDL_DestroyTexture(finalTexture2);
    SDL_DestroyTexture(finalTexture3);
    SDL_DestroyTexture(finalTexture4);
    SDL_DestroyTexture(finalTexture5);
    SDL_DestroyTexture(finalTexture6);
    SDL_Delay(1000);
    close(window, renderer, texture);
    return 0;
}