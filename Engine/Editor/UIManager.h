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
#include "../tools/event/Event.h"
#include "../tools/util/Singleton.h"

enum class UIStyle
{
	CLASSIC_STYLE,
	DARK_STYLE,
	LIGHT_STYLE,
	DUNE_DARK_STYLE,
	ALTERNATIVE_DARK_STYLE
};

class UIManager :public Singleton<UIManager>
{
	friend class Singleton<UIManager>;
public:
	

	void Init(SDL_Window* window, UIStyle style, const std::string& glslVersion);
	void Destroy();

	void ApplyStyle(UIStyle style);
	void EnableDocking(bool enableDock);

	void PreUpdate();
	void Update();
	void PostUpdate();

	Tools::Event<int> testEvent;

protected:
	UIManager() = default;
	//~UIManager() = default;
	void _OnTestEvent(int testNum);

private:
	bool mDockingState = false;
	SDL_GLContext mGlContext;
	
};