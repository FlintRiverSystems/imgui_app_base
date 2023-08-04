
#pragma once

class imgui_app_base
{
public:
	imgui_app_base() {}
	virtual ~imgui_app_base() {}

	virtual bool init (std::string_view title, int width, int height);

	virtual void run();

	#ifdef __EMSCRIPTEN__
	static void wasm_update(void* arg);
	#endif

protected:

	virtual void _render_update();
	virtual void _render_ui();
	virtual void _cleanup();

	SDL_Window* _window;
	SDL_Renderer* _renderer;

	bool _done = false;
    ImVec4 _bg_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

};

