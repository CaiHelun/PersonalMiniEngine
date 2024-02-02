#pragma once

#if defined(__cplusplus)
extern "C"
{
#endif

#include <SDL.h>

#if defined(__cplusplus)
};
#endif

#include <string>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

enum class UIStyle
{
	CLASSIC_STYLE,
	DARK_STYLE,
	LIGHT_STYLE,
	DUNE_DARK_STYLE,
	ALTERNATIVE_DARK_STYLE
};

class UIManager
{
public:
	UIManager(SDL_Window* window, UIStyle style, const std::string& glslVersion);
	~UIManager();

	void ApplyStyle(UIStyle style);
	void EnableDocking(bool enableDock);

	void PreUpdate();
	void Update();
	void PostUpdate();

protected:
private:
	bool mDockingState = false;
	SDL_GLContext mGlContext;
};