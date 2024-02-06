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

#undef main
#define STB_IMAGE_IMPLEMENTATION
#include "Image/stb_image.h"

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
	UIManager* uiManager = new UIManager(window, UIStyle::ALTERNATIVE_DARK_STYLE, "#version 330");

	if (!gladLoadGL())
	{
        std::cout << "glad Load Failed!" << std::endl;
		return -1;
	}
    std::string vertexPath = "../Engine/Shader/VertexShader.glsl";
    std::string fragPath = "../Engine/Shader/FragShader.glsl";
    Shader shader(vertexPath.c_str(), fragPath.c_str());
	Shader lightShader("../Engine/Shader/LightBoxVert.glsl", "../Engine/Shader/LightBoxFrag.glsl");
	Camera* camera = new Camera(glm::vec3(.0f, .0f, 5.0f));

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	}; 
	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	int textureWidth, textureHeight, channel;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* textureData = stbi_load("../Engine/asset/container2.png", &textureWidth, &textureHeight, &channel, 0);
	if (!textureData)
	{
		std::cout << "Image Load Failed!" << std::endl;
		return -1;
	}
	
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(textureData);
	
	unsigned char* textureData2 = stbi_load("../Engine/asset/container2_specular.png", &textureWidth, &textureHeight, &channel, 0);
	if (!textureData2)
	{
		std::cout << "Image Load Failed!" << std::endl;
		return -1;
	}
	unsigned int specularMap;
	glGenTextures(1, &specularMap);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData2);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(textureData2);

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	lightShader.UseShaderProgram();
	shader.UseShaderProgram();
    
    glEnable(GL_DEPTH_TEST);

    glm::mat4 model(1.0f);
	glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 proj(1.0f);
    proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

	//这个要放在useprogram后面不然不生效，踩坑记录
	shader.SetUniformInt("material.diffuse", 0);
	shader.SetUniformInt("material.specular", 1);

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::vec3 lightDir(.0f, .0f, -1.0f);
    bool quit = false;
    for (; !quit;)
    {
        float currentFrame = (float)SDL_GetTicks() / 1000.0f;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		uiManager->PreUpdate();

        glClearColor(.0f, .0f, .0f, .0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.UseShaderProgram();
		view = camera->GetViewMatrix();
		shader.SetUniformMat4("view", view);
		shader.SetUniformMat4("proj", proj);
		//shader.SetUniformMat4("model", model);
		shader.SetUniformVec3("viewPos", camera->mCameraPosition);
		shader.SetUniformFloat("material.shininess", 32.0f);

		shader.SetUniformVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		shader.SetUniformVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		shader.SetUniformVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		shader.SetUniformVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		shader.SetUniformVec3("pointLight.position", lightPos);
		shader.SetUniformVec3("pointLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		shader.SetUniformVec3("pointLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		shader.SetUniformVec3("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.SetUniformFloat("pointLight.constant", 1.0f);
		shader.SetUniformFloat("pointLight.linear", 0.09f);
		shader.SetUniformFloat("pointLight.quadratic", 0.032f);

		shader.SetUniformVec3("spotLight.position", camera->mCameraPosition);
		shader.SetUniformVec3("spotLight.direction", camera->mCameraFront);
		shader.SetUniformVec3("spotLight.ambient", glm::vec3(0.2f));
		shader.SetUniformVec3("spotLight.diffuse", glm::vec3(0.5f));
		shader.SetUniformVec3("spotLight.specular", glm::vec3(1.0f));
		shader.SetUniformFloat("spotLight.constant", 1.0f);
		shader.SetUniformFloat("spotLight.linear", 0.09f);
		shader.SetUniformFloat("spotLight.quadratic", 0.032f);
		shader.SetUniformFloat("spotLight.cutOff", cos(glm::radians(12.5f)));
		shader.SetUniformFloat("spotLight.outerCutOff", cos(glm::radians(17.5f)));

		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 curModel(.0f);
			curModel = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			curModel = glm::rotate(curModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.SetUniformMat4("model", curModel);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		lightShader.UseShaderProgram();
		glm::mat4 lightModel(1.0f);
		lightModel = glm::translate(model, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		lightShader.SetUniformMat4("view", view);
		lightShader.SetUniformMat4("proj", proj);
		lightShader.SetUniformMat4("model", lightModel);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		uiManager->PostUpdate();
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
            ImGui_ImplSDL2_ProcessEvent(&event);
            ProcessEventInScene(camera, event);
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                quit = true;
            break;
		}
        SDL_GL_SwapWindow(window);
    }

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
    SDL_DestroyWindow(window);
    SDL_Quit();
	delete uiManager;
	delete camera;
	camera = nullptr;
	uiManager = nullptr;
    
    return 0;
}