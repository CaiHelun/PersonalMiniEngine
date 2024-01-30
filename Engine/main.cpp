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
			camera->OnMouseRightButtonDrag(xoffset, yoffset);
		}
		else if (middleButtonDrag)
		{
			camera->OnMouseMiddleButtonDrag(xoffset, yoffset);
		}
		break;
	}
	case SDL_MOUSEWHEEL:
	{
		camera->OnMouseWheel(event.wheel.y);
		break;
	}
	default:
		break;
	}
}


int main()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

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

    ImGui_ImplSDL2_InitForOpenGL(window, &glContext);
    ImGui_ImplOpenGL3_Init();

	if (!gladLoadGL())
	{
        std::cout << "glad Load Failed!" << std::endl;
		return -1;
	}
    std::string vertexPath = "../Engine/Shader/VertexShader.vert";
    std::string fragPath = "../Engine/Shader/FragShader.frag";
    Shader shader(vertexPath.c_str(), fragPath.c_str());
	Shader lightShader("../Engine/Shader/LightBox.vert", "../Engine/Shader/LightBox.frag");
	Camera* camera = new Camera(glm::vec3(.0f, .0f, 5.0f));

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	/*int imageWidth, imageHeight, imageChannels;
	unsigned char* imageData = stbi_load("../Engine/asset/container.jpg", &imageWidth, &imageHeight, &imageChannels, 0);
	if (!imageData)
	{
		std::cout << "Image Load Failed!" << std::endl;
		return -1;
	}
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageData2 = stbi_load("../Engine/asset/awesomeface.png", &imageWidth, &imageHeight, &imageChannels, 0);
	if (!imageData2)
	{
		std::cout << "Image Load Failed!" << std::endl;
		return -1;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData2);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData2);*/

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	lightShader.UseShaderProgram();
	shader.UseShaderProgram();

    //shader.SetUniformInt("ourTexture", 0);
    //shader.SetUniformInt("ourTexture2", 1);
    
    glEnable(GL_DEPTH_TEST);

    glm::mat4 model(1.0f);
	glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 proj(1.0f);
    proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

	shader.SetUniformVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glm::vec3 lightColor;

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    bool quit = false;
    for (; !quit;)
    {
        float currentFrame = (float)SDL_GetTicks() / 1000.0f;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        glClearColor(.0f, .0f, .0f, .0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.UseShaderProgram();
		view = camera->GetViewMatrix();
		shader.SetUniformMat4("view", view);
		shader.SetUniformMat4("proj", proj);
		shader.SetUniformMat4("model", model);
		shader.SetUniformVec3("lightPos", lightPos);
		shader.SetUniformVec3("viewPos", camera->mCameraPosition);
		shader.SetUniformVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		shader.SetUniformVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		shader.SetUniformVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		shader.SetUniformFloat("material.shininess", 32.0f);

		lightColor.x = sin((float)SDL_GetTicks() / 1000.0f * 2.0f);
		lightColor.y = sin((float)SDL_GetTicks() / 1000.0f * 0.7f);
		lightColor.z = sin((float)SDL_GetTicks() / 1000.0f * 1.3f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = lightColor * glm::vec3(0.2f);

		shader.SetUniformVec3("light.position", lightPos);
		shader.SetUniformVec3("light.ambient", ambientColor);
		shader.SetUniformVec3("light.diffuse", diffuseColor);
		shader.SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightShader.UseShaderProgram();
		glm::mat4 lightModel = glm::mat4();
		lightModel = glm::translate(model, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		lightShader.SetUniformMat4("view", view);
		lightShader.SetUniformMat4("proj", proj);
		lightShader.SetUniformMat4("model", lightModel);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
	delete camera;
	camera = nullptr;
    
    return 0;
}