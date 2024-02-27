#include <glad.h>
#if defined(__cplusplus)
extern "C"
{
#endif

#include <SDL.h>

#if defined(__cplusplus)
};
#endif

#include <iostream>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "Shader.h"
#include "Camera.h"
#include "Editor/UIManager.h"
#include "mesh/AiModel.h"

#undef main
#define STB_IMAGE_IMPLEMENTATION
#include "Image/stb_image.h"
#include <fstream>

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
int lastX = 400, lastY = 300;
bool firstMouse = true;
bool rightButtonDrag = false;
bool middleButtonDrag = false;
bool leftButtonDrag = false;

void HandleResizeEvent(SDL_Window* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void ProcessEventInScene(Camera* camera, SDL_Event event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
	{
		switch (event.button.button)
		{
		case SDL_BUTTON_RIGHT:
			rightButtonDrag = true;
			break;
		case SDL_BUTTON_LEFT:
			leftButtonDrag = true;
			break;
		case SDL_BUTTON_MIDDLE:
			middleButtonDrag = true;
			break;
		default:
			break;
		}
		break;
	}
	case SDL_MOUSEBUTTONUP:
	{
		switch (event.button.button)
		{
		case SDL_BUTTON_RIGHT:
			rightButtonDrag = false;
			break;
		case SDL_BUTTON_LEFT:
			leftButtonDrag = false;
			break;
		case SDL_BUTTON_MIDDLE:
			middleButtonDrag = false;
			break;
		default:
			break;
		}
		break;
	}
	case SDL_MOUSEMOTION:
	{
		int curPosX = 0;
		int curPosY = 0;
		SDL_GetMouseState(&curPosX, &curPosY);
		int xoffset = curPosX - lastX;
		int yoffset = curPosY - lastY;
		lastX = curPosX;
		lastY = curPosY;
		if (rightButtonDrag)
		{
			camera->OnMouseRightButtonDrag((float)xoffset, (float)yoffset);
		}
		else if (middleButtonDrag)
		{
			camera->OnMouseMiddleButtonDrag((float)xoffset, (float)yoffset);
		}
		break;
	}
	case SDL_MOUSEWHEEL:
	{
		camera->OnMouseWheel((float)event.wheel.y);
		break;
	}
	default:
		break;
	}
}


int main()
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
	UIManager& uiManager = UIManager::GetInstance();
	uiManager.Init(window, UIStyle::CLASSIC_STYLE, "#version 330");

	if (!gladLoadGL())
	{
        std::cout << "glad Load Failed!" << std::endl;
		return -1;
	}
	Camera* camera = new Camera(glm::vec3(.0f, .0f, 3.0f));

	Shader modelShader("../Engine/Shader/model_loading_vert.glsl", "../Engine/Shader/model_loading_frag.glsl");
	AiModel aiModel("../Engine/asset/WusonOBJ.obj");
	modelShader.UseShaderProgram();

	bool firstLoad = true;
    glEnable(GL_DEPTH_TEST);
    bool quit = false;
    for (; !quit;)
    {
        float currentFrame = (float)SDL_GetTicks() / 1000.0f;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		uiManager.PreUpdate();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = camera->GetViewMatrix();
		modelShader.SetUniformMat4("projection", projection);
		modelShader.SetUniformMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		modelShader.SetUniformMat4("model", model);

		aiModel.Render(modelShader);

		uiManager.Update();
		uiManager.PostUpdate();
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			uiManager.testEvent.Invoke(10);
            ImGui_ImplSDL2_ProcessEvent(&event);
            ProcessEventInScene(camera, event);
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                quit = true;
            break;
		}
        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
	uiManager.Destroy();
	delete camera;
	camera = nullptr;
    
    return 0;
}