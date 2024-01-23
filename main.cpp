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
#include "imgui_impl_sdl.h"

#undef main

void HandleResizeEvent(SDL_Window* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

const char* vertexShader = R"(
    #version 330
    layout (location = 0) in vec3 aPos;

    void main()
    {
        gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);
    }
)";

const char* fragShader = R"(
    #version 330 
    out vec4 FragColor;

    void main()
    {
        FragColor=vec4(1.0f,0.5f,0.2f,1.0f);
    }
)";

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

	if (!gladLoadGL())
	{
        std::cout << "glad Load Failed!" << std::endl;
		return -1;
	}

    unsigned int VS;
    VS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VS, 1, &vertexShader, nullptr);
    glCompileShader(VS);

    int shaderCompileSuccess;
    char infoLog[512];
    glGetShaderiv(VS, GL_COMPILE_STATUS, &shaderCompileSuccess);
    if (!shaderCompileSuccess)
    {
        glGetShaderInfoLog(VS, 512, nullptr, infoLog);
        std::cout << "Error::Shader::Vertex::Compilatioion_Failed\n" << infoLog << std::endl;
    }

    unsigned int FS;
    FS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FS, 1, &fragShader, nullptr);
    glCompileShader(FS);

	glGetShaderiv(FS, GL_COMPILE_STATUS, &shaderCompileSuccess);
	if (!shaderCompileSuccess)
	{
		glGetShaderInfoLog(FS, 512, nullptr, infoLog);
		std::cout << "Error::Shader::Vertex::Compilatioion_Failed\n" << infoLog << std::endl;
	}

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, VS);
    glAttachShader(shaderProgram, FS);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderCompileSuccess);
    if (!shaderCompileSuccess)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    }

    float vertices[] = {
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
    };
	unsigned int indices[] = {
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // 4. 解绑VAO
    glBindVertexArray(0);

	glDeleteShader(VS);
	glDeleteShader(FS);

    bool quit = false;
    for (; !quit;) 
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            EventFallback eventFallback = ProcessEvent(window, event);
            switch (eventFallback)
            {
            case EventFallback::EventNone:
                break;
            case EventFallback::EventQuit:
                quit = true;
                break;
            default:
                break;
            }
        }
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        SDL_GL_SwapWindow(window);
    }

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}