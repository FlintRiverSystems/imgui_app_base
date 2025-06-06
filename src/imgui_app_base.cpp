
#include "imgui_app_base.h"

using namespace std;

bool imgui_app_base::init (std::string_view title, int width, int height)
{
	#if 0
	auto driver_count = SDL_GetNumVideoDrivers();
	if (driver_count > 0)
	{
		cout << "Available Drivers: " << endl;
		for (int i=0; i<driver_count; ++i)
		{
			cout << SDL_GetVideoDriver(i) << endl;
		}
	}
	else
	{
		cout << "No Video Drivers" << endl;
	}
	#endif

	#ifdef __EMSCRIPTEN__
	EmscriptenFullscreenStrategy s;
	memset(&s, 0, sizeof(s));
	s.scaleMode = EMSCRIPTEN_FULLSCREEN_SCALE_DEFAULT;
	s.canvasResolutionScaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_NONE;
	s.filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT;
	//s.canvasResizedCallback = on_canvassize_changed;
	emscripten_enter_soft_fullscreen("canvas", &s);
	#endif	

	// Setup SDL
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
	{
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return false;
	}

	//std::string video_driver(SDL_GetCurrentVideoDriver());
	//cout << "SDL initialized with driver: " << video_driver << endl;

	// Setup window
	Uint32 flags = SDL_WINDOW_OPENGL;
	flags |= SDL_WINDOW_RESIZABLE;
	flags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;

	#ifdef __EMSCRIPTEN__
	flags |= SDL_WINDOW_MAXIMIZED;
	//flags |= SDL_WINDOW_ALLOW_HIGHDPI;
	#endif	

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(flags);
	_window = SDL_CreateWindow("imgui example", 1280, 720, window_flags);

	// Setup SDL_Renderer instance
	_renderer = SDL_CreateRenderer(_window, nullptr);
	if (_renderer == NULL)
	{
		SDL_Log("Error creating SDL_Renderer!");
		return false;
	}
	//SDL_RendererInfo info;
	//SDL_GetRendererInfo(renderer, &info);
	//SDL_Log("Current SDL_Renderer: %s", info.name);

	SDL_SetRenderVSync(_renderer, SDL_RENDERER_VSYNC_ADAPTIVE);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForSDLRenderer(_window, _renderer);
	ImGui_ImplSDLRenderer3_Init(_renderer);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	return true;
}

void imgui_app_base::run()
{
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(wasm_update, this, 0, true);
#endif

	_done = false;

	while(!_done)
	{
		_render_update();
	}

	_cleanup();
}

#ifdef __EMSCRIPTEN__
void imgui_app_base::wasm_update(void* arg)
{
	auto self = (imgui_app_base*)arg;
	self->_render_update();
}
#endif

void imgui_app_base::_render_update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL3_ProcessEvent(&event);
		if (event.type == SDL_EVENT_QUIT)
			_done = true;
		#ifndef __EMSCRIPTEN__
		//if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(_window))
		//	_done = true;
		#endif
	}

	// Start the Dear ImGui frame
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	_render_ui();

	// Rendering
	ImGui::Render();
	SDL_SetRenderDrawColor(_renderer, (Uint8)(_bg_color.x * 255), (Uint8)(_bg_color.y * 255), (Uint8)(_bg_color.z * 255), (Uint8)(_bg_color.w * 255));
	SDL_RenderClear(_renderer);

	_render_before_imgui();

	//SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	//SDL_RenderDrawRect(_renderer, new SDL_Rect{ 1,2,30,40 });

	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), _renderer);

	_render_after_imgui();

	SDL_RenderPresent(_renderer);
}

void imgui_app_base::_render_ui()
{
	ImGui::Begin("My Window");
	ImGui::Text("Hello from My Window!");
	ImGui::End();
}

void imgui_app_base::_cleanup()
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
	ImPlot::DestroyContext();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}
