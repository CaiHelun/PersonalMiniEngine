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

#undef main
#define STB_IMAGE_IMPLEMENTATION
#include "Image/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"

void HandleResizeEvent(SDL_Window* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

enum class EventFallback
{
    EventNone,
    EventQuit
};

EventFallback ProcessEvent(SDL_Window*window,SDL_Event event)
{
	switch (event.type)
	{
	case SDL_QUIT:
        return EventFallback::EventQuit;
	case SDL_WINDOWEVENT:
	{
		switch (event.window.event)
		{
		case SDL_WINDOWEVENT_CLOSE:
		{
			if (event.window.windowID == SDL_GetWindowID(window))
                return EventFallback::EventQuit;
			break;
		}
		case SDL_WINDOWEVENT_RESIZED:
			HandleResizeEvent(window, event.window.data1, event.window.data2);
			break;
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
    return EventFallback::EventNone;
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

    float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };
	unsigned int indices[] = {
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

    int imageWidth, imageHeight, imageChannels;
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
    stbi_image_free(imageData2);

    unsigned int VBO, VAO, IBO;
	glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
	glGenVertexArrays(1, &VAO);

    // 1. 绑定VAO
    glBindVertexArray(VAO);

    // 2. 把顶点数组复制到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 3. 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // 4. 解绑VAO
    glBindVertexArray(0);

	shader.UseShaderProgram();
    shader.SetUniformInt("ourTexture", 0);
    shader.SetUniformInt("ourTexture2", 1);

	/*glm::mat4 trans(1.0f);
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(.0f, .0f, 1.0f));
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));*/
    unsigned int transformLoc = glGetUniformLocation(shader.ShaderProgramID, "Transform");
    
    glBindVertexArray(VAO);

    float degree = .0f;
    bool quit = false;
    for (; !quit;) 
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        //ImGui::ShowDemoWindow();


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glm::mat4 trans(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, glm::radians(degree), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		

        ImGui::SliderFloat("Degree", &degree, -180.0f, 180.0f);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
            ImGui_ImplSDL2_ProcessEvent(&event);
            break;
		}
        SDL_GL_SwapWindow(window);
    }
    glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    
    return 0;
}