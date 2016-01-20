#include <iostream>
#include <cassert>

#include "kaguya/kaguya.hpp"

#include "SDL.h"


using namespace kaguya;
namespace
{
	void SetupVideoGl(LuaRef& sdl)
	{
		sdl["GL"] = NewTable();
		LuaRef gl = sdl["GL"];


#define REG_SDL_GL_ENUM(V) gl[#V + 7] = V

		REG_SDL_GL_ENUM(SDL_GL_RED_SIZE);
		REG_SDL_GL_ENUM(SDL_GL_GREEN_SIZE);
		REG_SDL_GL_ENUM(SDL_GL_BLUE_SIZE);
		REG_SDL_GL_ENUM(SDL_GL_ALPHA_SIZE);
		REG_SDL_GL_ENUM(SDL_GL_BUFFER_SIZE);
		REG_SDL_GL_ENUM(SDL_GL_DOUBLEBUFFER);
		REG_SDL_GL_ENUM(SDL_GL_DEPTH_SIZE);
		REG_SDL_GL_ENUM(SDL_GL_STENCIL_SIZE);
		REG_SDL_GL_ENUM(SDL_GL_ACCUM_RED_SIZE);
		REG_SDL_GL_ENUM(SDL_GL_ACCUM_GREEN_SIZE);
		REG_SDL_GL_ENUM(SDL_GL_ACCUM_BLUE_SIZE);
		REG_SDL_GL_ENUM(SDL_GL_ACCUM_ALPHA_SIZE);
		REG_SDL_GL_ENUM(SDL_GL_STEREO);
		REG_SDL_GL_ENUM(SDL_GL_MULTISAMPLEBUFFERS);
		REG_SDL_GL_ENUM(SDL_GL_MULTISAMPLESAMPLES);
		REG_SDL_GL_ENUM(SDL_GL_ACCELERATED_VISUAL);
		REG_SDL_GL_ENUM(SDL_GL_RETAINED_BACKING);
		REG_SDL_GL_ENUM(SDL_GL_CONTEXT_MAJOR_VERSION);
		REG_SDL_GL_ENUM(SDL_GL_CONTEXT_MINOR_VERSION);
		REG_SDL_GL_ENUM(SDL_GL_CONTEXT_EGL);
		REG_SDL_GL_ENUM(SDL_GL_CONTEXT_FLAGS);
		REG_SDL_GL_ENUM(SDL_GL_CONTEXT_PROFILE_MASK);
		REG_SDL_GL_ENUM(SDL_GL_SHARE_WITH_CURRENT_CONTEXT);
		REG_SDL_GL_ENUM(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE);
#undef REG_SDL_GL_ENUM

		gl["GetAttribute"] = function(&SDL_GL_GetAttribute);
		gl["SetAttribute"] = function(&SDL_GL_SetAttribute);
		gl["CreateContext"] = function(&SDL_GL_CreateContext);
		gl["DeleteContext"] = function(&SDL_GL_DeleteContext);

		gl["MakeCurrent"] = function(&SDL_GL_MakeCurrent);
	}


	SDL_Surface* SDL_LoadBMP_Fun(const char* file)
	{
		return SDL_LoadBMP_RW(SDL_RWFromFile(file, "rb"), 1);
	}

	void SetupVideo(LuaRef& sdl)
	{
		sdl["CreateWindow"] = function(&SDL_CreateWindow);
		sdl["DestroyWindow"] = function(&SDL_DestroyWindow);
		sdl["DisableScreenSaver"] = function(&SDL_DisableScreenSaver);
		sdl["EnableScreenSaver"] = function(&SDL_EnableScreenSaver);



		sdl["CreateRenderer"] = function(&SDL_CreateRenderer);
		sdl["DestroyRenderer"] = function(&SDL_DestroyRenderer);


		sdl["CreateTextureFromSurface"] = function(&SDL_CreateTextureFromSurface);
		sdl["DestroyTexture"] = function(&SDL_DestroyTexture);
		sdl["FreeSurface"] = function(&SDL_FreeSurface);
		sdl["LoadBMP"] = function(&SDL_LoadBMP_Fun);

		sdl["RenderClear"] = function(&SDL_RenderClear);
		sdl["RenderCopy"] = function(&SDL_RenderCopy);
		sdl["RenderPresent"] = function(&SDL_RenderPresent);

#define REG_SDL_ENUM(V) sdl[#V + 4] = int(V)

		REG_SDL_ENUM(SDL_WINDOW_FULLSCREEN);
		REG_SDL_ENUM(SDL_WINDOW_FULLSCREEN_DESKTOP);
		REG_SDL_ENUM(SDL_WINDOW_OPENGL);
		REG_SDL_ENUM(SDL_WINDOW_HIDDEN);
		REG_SDL_ENUM(SDL_WINDOW_BORDERLESS);
		REG_SDL_ENUM(SDL_WINDOW_RESIZABLE);
		REG_SDL_ENUM(SDL_WINDOW_MINIMIZED);
		REG_SDL_ENUM(SDL_WINDOW_MAXIMIZED);
		REG_SDL_ENUM(SDL_WINDOW_INPUT_GRABBED);
		REG_SDL_ENUM(SDL_WINDOW_ALLOW_HIGHDPI);

		REG_SDL_ENUM(SDL_WINDOWPOS_CENTERED);

		REG_SDL_ENUM(SDL_RENDERER_SOFTWARE);
		REG_SDL_ENUM(SDL_RENDERER_ACCELERATED);
		REG_SDL_ENUM(SDL_RENDERER_PRESENTVSYNC);
		REG_SDL_ENUM(SDL_RENDERER_TARGETTEXTURE);


#undef REG_SDL_ENUM

		SetupVideoGl(sdl);
	}

	void SetupEvent(LuaRef& sdl)
	{
		sdl["Event"].setClass(
			ClassMetatable<SDL_Event>()
			.addConstructor()
			.addMember("type", &SDL_Event::type)
			.addMember("common", &SDL_Event::common)
			.addMember("window", &SDL_Event::window)
			.addMember("key", &SDL_Event::key)
			.addMember("edit", &SDL_Event::edit)
			.addMember("text", &SDL_Event::text)
			.addMember("motion", &SDL_Event::motion)
			.addMember("button", &SDL_Event::button)
			.addMember("wheel", &SDL_Event::wheel)
			.addMember("jaxis", &SDL_Event::jaxis)
			.addMember("jball", &SDL_Event::jball)
			.addMember("jhat", &SDL_Event::jhat)
			.addMember("jbutton", &SDL_Event::jbutton)
			.addMember("jdevice", &SDL_Event::jdevice)
			.addMember("caxis", &SDL_Event::caxis)
			.addMember("cbutton", &SDL_Event::cbutton)
			.addMember("cdevice", &SDL_Event::cdevice)
			.addMember("quit", &SDL_Event::quit)
			.addMember("user", &SDL_Event::user)
			.addMember("syswm", &SDL_Event::syswm)
			.addMember("tfinger", &SDL_Event::tfinger)
			.addMember("mgesture", &SDL_Event::mgesture)
			.addMember("dgesture", &SDL_Event::dgesture)
			.addMember("drop", &SDL_Event::drop)
			);

		sdl["CommonEvent"].setClass(
			ClassMetatable<SDL_CommonEvent>()
			.addConstructor()
			.addMember("type", &SDL_CommonEvent::type)
			.addMember("timestamp", &SDL_CommonEvent::timestamp)
			);

		sdl["WindowEvent"].setClass(
			ClassMetatable<SDL_WindowEvent>()
			.addConstructor()
			.addMember("type", &SDL_WindowEvent::type)
			.addMember("timestamp", &SDL_WindowEvent::timestamp)
			.addMember("windowID", &SDL_WindowEvent::windowID)
			.addMember("event", &SDL_WindowEvent::event)
			.addMember("data1", &SDL_WindowEvent::data1)
			.addMember("data2", &SDL_WindowEvent::data2)
			);


		sdl["KeyboardEvent"].setClass(
			ClassMetatable<SDL_KeyboardEvent>()
			.addConstructor()
			.addMember("type", &SDL_KeyboardEvent::type)
			.addMember("timestamp", &SDL_KeyboardEvent::timestamp)
			.addMember("windowID", &SDL_KeyboardEvent::windowID)
			.addMember("state", &SDL_KeyboardEvent::state)
			.addMember("repeat", &SDL_KeyboardEvent::repeat)
			.addMember("keysym", &SDL_KeyboardEvent::keysym)
			);


		sdl["PollEvent"] = function(&SDL_PollEvent);
		sdl["WaitEvent"] = function(&SDL_WaitEvent);

#define REG_SDL_EVENT_ENUM(V) sdl[#V + 4] = int(V)
		REG_SDL_EVENT_ENUM(SDL_QUIT);
		REG_SDL_EVENT_ENUM(SDL_APP_TERMINATING);
		REG_SDL_EVENT_ENUM(SDL_APP_LOWMEMORY);
		REG_SDL_EVENT_ENUM(SDL_APP_WILLENTERBACKGROUND);
		REG_SDL_EVENT_ENUM(SDL_APP_DIDENTERBACKGROUND);
		REG_SDL_EVENT_ENUM(SDL_APP_WILLENTERFOREGROUND);
		REG_SDL_EVENT_ENUM(SDL_APP_DIDENTERFOREGROUND);
		REG_SDL_EVENT_ENUM(SDL_WINDOWEVENT);
		REG_SDL_EVENT_ENUM(SDL_SYSWMEVENT);
		REG_SDL_EVENT_ENUM(SDL_KEYDOWN);
		REG_SDL_EVENT_ENUM(SDL_KEYUP);
		REG_SDL_EVENT_ENUM(SDL_TEXTEDITING);
		REG_SDL_EVENT_ENUM(SDL_TEXTINPUT);
		REG_SDL_EVENT_ENUM(SDL_MOUSEMOTION);
		REG_SDL_EVENT_ENUM(SDL_MOUSEBUTTONDOWN);
		REG_SDL_EVENT_ENUM(SDL_MOUSEBUTTONUP);
		REG_SDL_EVENT_ENUM(SDL_MOUSEWHEEL);
		REG_SDL_EVENT_ENUM(SDL_JOYAXISMOTION);
		REG_SDL_EVENT_ENUM(SDL_JOYBALLMOTION);
		REG_SDL_EVENT_ENUM(SDL_JOYHATMOTION);
		REG_SDL_EVENT_ENUM(SDL_JOYBUTTONDOWN);
		REG_SDL_EVENT_ENUM(SDL_JOYBUTTONUP);
		REG_SDL_EVENT_ENUM(SDL_JOYDEVICEADDED);
		REG_SDL_EVENT_ENUM(SDL_JOYDEVICEREMOVED);
		REG_SDL_EVENT_ENUM(SDL_CONTROLLERAXISMOTION);
		REG_SDL_EVENT_ENUM(SDL_CONTROLLERBUTTONDOWN);
		REG_SDL_EVENT_ENUM(SDL_CONTROLLERBUTTONUP);
		REG_SDL_EVENT_ENUM(SDL_CONTROLLERDEVICEADDED);
		REG_SDL_EVENT_ENUM(SDL_CONTROLLERDEVICEREMOVED);
		REG_SDL_EVENT_ENUM(SDL_CONTROLLERDEVICEREMAPPED);
		REG_SDL_EVENT_ENUM(SDL_FINGERDOWN);
		REG_SDL_EVENT_ENUM(SDL_FINGERUP);
		REG_SDL_EVENT_ENUM(SDL_FINGERMOTION);
		REG_SDL_EVENT_ENUM(SDL_DOLLARGESTURE);
		REG_SDL_EVENT_ENUM(SDL_DOLLARRECORD);
		REG_SDL_EVENT_ENUM(SDL_MULTIGESTURE);
		REG_SDL_EVENT_ENUM(SDL_CLIPBOARDUPDATE);
		REG_SDL_EVENT_ENUM(SDL_DROPFILE);
		REG_SDL_EVENT_ENUM(SDL_RENDER_TARGETS_RESET);
		REG_SDL_EVENT_ENUM(SDL_USEREVENT);

#undef REG_SDL_EVENT_ENUM
	}
	void SetupInit(LuaRef& sdl)
	{
		sdl["INIT_TIMER"] = SDL_INIT_TIMER;
		sdl["INIT_AUDIO"] = SDL_INIT_AUDIO;
		sdl["INIT_VIDEO"] = SDL_INIT_VIDEO;
		sdl["INIT_JOYSTICK"] = SDL_INIT_JOYSTICK;
		sdl["INIT_GAMECONTROLLER"] = SDL_INIT_GAMECONTROLLER;
		sdl["INIT_EVENTS"] = SDL_INIT_EVENTS;
		sdl["INIT_NOPARACHUTE"] = SDL_INIT_NOPARACHUTE;
		sdl["INIT_EVERYTHING"] = SDL_INIT_EVERYTHING;


		sdl["Init"] = function(&SDL_Init);
		sdl["Quit"] = function(&SDL_Quit);


		sdl["Delay"] = function(&SDL_Delay);
	}
}

extern "C"
#ifdef _MSC_VER
__declspec(dllexport)
#endif
int luaopen_samplesdlbind(lua_State *L)
{
	State state(L);

	LuaRef sdl = state.newLib();
	SetupInit(sdl);
	SetupVideo(sdl);
	SetupEvent(sdl);

	return 1;
}
