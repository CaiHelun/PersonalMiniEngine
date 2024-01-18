#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include "SDL.h"
#include <iostream>

#undef main

void HandleResizeEvent(SDL_Window* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);


    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    static const int width = 800;
    static const int height = 600;

    SDL_Window* window
        = SDL_CreateWindow("SDLWindow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window)
    {
        std::cout << "Window Init Failed!" << std::endl;
        SDL_Quit();
        return -1;
    }
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1);


    bool quit = false;
    for (; !quit;) 
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_WINDOWEVENT:
                {
                    switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_CLOSE:
                        {
                            if (event.window.windowID == SDL_GetWindowID(window))
                                quit = true;
                            break;
                        }
                        case SDL_WINDOWEVENT_RESIZED:
                            //HandleResizeEvent(window, event.window.data1, event.window.data2);
                            glViewport(0, 0, event.window.data1, event.window.data2);
                            break;
                        default:
                            break;
                    }
                    break;
                }
                default:
            	break;
            }
        }


        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}