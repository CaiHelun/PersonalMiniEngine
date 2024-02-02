#include "UIManager.h"

UIManager::UIManager(SDL_Window* window, UIStyle style, const std::string& glslVersion)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	EnableDocking(false);
	ApplyStyle(style);

	mGlContext = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, mGlContext);
	SDL_GL_SetSwapInterval(1);
	ImGui_ImplSDL2_InitForOpenGL(window, &mGlContext);
	ImGui_ImplOpenGL3_Init();
}

UIManager::~UIManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void UIManager::ApplyStyle(UIStyle style)
{
	ImGuiStyle* imguiStyle = &ImGui::GetStyle();
	switch (style)
	{
	case UIStyle::CLASSIC_STYLE:
		ImGui::StyleColorsClassic();
		break;
	case UIStyle::DARK_STYLE:
		ImGui::StyleColorsDark();
		break;
	case UIStyle::LIGHT_STYLE:
		ImGui::StyleColorsLight();
		break;
	default:
		break;
	}

	if (style == UIStyle::DUNE_DARK_STYLE)
	{
		imguiStyle->WindowPadding = ImVec2(15, 15);
		imguiStyle->WindowRounding = 5.0f;
		imguiStyle->FramePadding = ImVec2(5, 5);
		imguiStyle->FrameRounding = 4.0f;
		imguiStyle->ItemSpacing = ImVec2(12, 8);
		imguiStyle->ItemInnerSpacing = ImVec2(8, 6);
		imguiStyle->IndentSpacing = 25.0f;
		imguiStyle->ScrollbarSize = 15.0f;
		imguiStyle->ScrollbarRounding = 9.0f;
		imguiStyle->GrabMinSize = 5.0f;
		imguiStyle->GrabRounding = 3.0f;

		imguiStyle->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		imguiStyle->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		imguiStyle->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		imguiStyle->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		imguiStyle->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		imguiStyle->Colors[ImGuiCol_Border] = ImVec4(0.2f, 0.2f, 0.2f, 0.88f);
		imguiStyle->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		imguiStyle->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		imguiStyle->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		imguiStyle->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		imguiStyle->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		imguiStyle->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.3f, 0.3f, 0.3f, 0.75f);
		imguiStyle->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		imguiStyle->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		imguiStyle->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		imguiStyle->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		imguiStyle->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		imguiStyle->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		imguiStyle->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		imguiStyle->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		imguiStyle->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		imguiStyle->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		imguiStyle->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		imguiStyle->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		imguiStyle->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		imguiStyle->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		imguiStyle->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		imguiStyle->Colors[ImGuiCol_Separator] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		imguiStyle->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		imguiStyle->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		imguiStyle->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		imguiStyle->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		imguiStyle->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		imguiStyle->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		imguiStyle->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		imguiStyle->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		imguiStyle->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		imguiStyle->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		//imguiStyle->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

		imguiStyle->Colors[ImGuiCol_Tab] = imguiStyle->Colors[ImGuiCol_TabUnfocused];
	}
	else if (style == UIStyle::ALTERNATIVE_DARK_STYLE)
	{
		imguiStyle->WindowPadding = ImVec2(15, 15);
		imguiStyle->WindowRounding = 0.0f;
		imguiStyle->FramePadding = ImVec2(5, 5);
		imguiStyle->FrameRounding = 0.0f;
		imguiStyle->ItemSpacing = ImVec2(12, 8);
		imguiStyle->ItemInnerSpacing = ImVec2(8, 6);
		imguiStyle->IndentSpacing = 25.0f;
		imguiStyle->ScrollbarSize = 15.0f;
		imguiStyle->ScrollbarRounding = 0.0f;
		imguiStyle->GrabMinSize = 5.0f;
		imguiStyle->GrabRounding = 0.0f;
		imguiStyle->TabRounding = 0.0f;
		imguiStyle->ChildRounding = 0.0f;
		imguiStyle->PopupRounding = 0.0f;

		imguiStyle->WindowBorderSize = 1.0f;
		imguiStyle->FrameBorderSize = 0.0f;
		imguiStyle->PopupBorderSize = 1.0f;

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.96f, 0.96f, 0.99f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.06f, 0.06f, 0.07f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.27f, 0.27f, 0.29f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.32f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.42f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.53f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.59f, 0.59f, 0.61f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.59f, 0.59f, 0.61f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.59f, 0.59f, 0.61f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.47f, 0.39f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.44f, 0.44f, 0.47f, 0.59f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.20f, 0.20f, 0.22f, 0.39f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.44f, 0.44f, 0.47f, 0.39f);
		//colors[ImGuiCol_DockingPreview] = ImVec4(0.91f, 0.62f, 0.00f, 0.78f);
		//colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.96f, 0.96f, 0.99f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.12f, 1.00f, 0.12f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.96f, 0.96f, 0.99f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.12f, 1.00f, 0.12f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.91f, 0.62f, 0.00f, 1.00f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}
}

void UIManager::EnableDocking(bool enableDock)
{
	mDockingState = enableDock;

}

void UIManager::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
}

void UIManager::Update()
{
}

void UIManager::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
