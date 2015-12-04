#include <iostream>
#include <cassert>

#include "kaguya/kaguya.hpp"

#include "GLFW/glfw3.h"

#include "u8u32.hpp"

using namespace kaguya;
namespace
{
	//typedef void(*GLFWerrorfun)(int, const char*);
	namespace global_callbacks
	{
		LuaRef error_callback_fn;
		void error_callback_function(int code, const char* message)
		{
			if (error_callback_fn)
			{
				error_callback_fn(code, message);
			}
		};
		void SetErrorCallback(LuaRef function)
		{
			if (function.type() == LuaRef::TYPE_FUNCTION)
			{
				error_callback_fn = function;
				glfwSetErrorCallback(&error_callback_function);
			}
			else
			{
				error_callback_fn = LuaRef();
			}
		}


		//typedef void(*GLFWmonitorfun)(GLFWmonitor*, int);
		// *  @param[in] event One of `GLFW_CONNECTED` or `GLFW_DISCONNECTED`.
		LuaRef monitor_callback_fn;
		void monitor_callback_function(GLFWmonitor* monitor, int status)
		{
			if (monitor_callback_fn)
			{
				monitor_callback_fn(monitor, status);
			}
		}
		void SetMonitorCallback(LuaRef function)
		{
			if (function.type() == LuaRef::TYPE_FUNCTION)
			{
				monitor_callback_fn = function;
				glfwSetMonitorCallback(&monitor_callback_function);
			}
			else
			{
				monitor_callback_fn = LuaRef();
			}
		}

	}

	struct Window
	{
		Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
		{
			window_ = glfwCreateWindow(width, height, title, monitor, share);
			glfwSetWindowUserPointer(window_, this);
		}

		~Window()
		{
			glfwSetWindowUserPointer(window_, 0);
			glfwDestroyWindow(window_);
		}
		GLFWwindow* pointer() { return window_; }


		LuaRef SetWindowPosCallback(LuaRef function)
		{
			return setCallback(function, window_pos_callback_fn_, glfwSetWindowPosCallback, pos_callback_function);
		}
		LuaRef SetWindowSizeCallback(LuaRef function)
		{
			return setCallback(function, window_size_callback_fn_, glfwSetWindowSizeCallback, size_callback_function);
		}
		LuaRef SetWindowCloseCallback(LuaRef function)
		{
			return setCallback(function, window_close_callback_fn_, glfwSetWindowCloseCallback, close_callback_function);
		}
		LuaRef SetWindowRefreshCallback(LuaRef function)
		{
			return setCallback(function, window_refresh_callback_fn_, glfwSetWindowRefreshCallback, refresh_callback_function);
		}
		LuaRef SetWindowFocusCallback(LuaRef function)
		{
			return setCallback(function, window_focus_callback_fn_, glfwSetWindowFocusCallback, focus_callback_function);
		}
		LuaRef SetWindowIconifyCallback(LuaRef function)
		{
			return setCallback(function, window_iconify_callback_fn_, glfwSetWindowIconifyCallback, iconify_callback_function);
		}
		LuaRef SetFramebufferSizeCallback(LuaRef function)
		{
			return setCallback(function, framebuffer_size_callback_fn_, glfwSetFramebufferSizeCallback, framebuffer_size_callback_function);
		}

		LuaRef SetKeyCallback(LuaRef function)
		{
			return setCallback(function, key_callback_fn_, glfwSetKeyCallback, key_callback_function);
		}

		LuaRef SetCharCallback(LuaRef function)
		{
			return setCallback(function, char_callback_fn_, glfwSetCharCallback, char_callback_function);
		}



		LuaRef SetCharModsCallback(LuaRef function)
		{
			return setCallback(function, charmods_callback_fn_, glfwSetCharModsCallback, charmods_callback_function);
		}
		LuaRef SetMouseButtonCallback(LuaRef function)
		{
			return setCallback(function, mouse_button_callback_fn_, glfwSetMouseButtonCallback, mouse_button_callback_function);
		}
		LuaRef SetCursorPosCallback(LuaRef function)
		{
			return setCallback(function, cursor_pos_callback_fn_, glfwSetCursorPosCallback, cursor_pos_callback_function);
		}
		LuaRef SetCursorEnterCallback(LuaRef function)
		{
			return setCallback(function, cursor_enter_callback_fn_, glfwSetCursorEnterCallback, cursor_enter_callback_function);
		}
		LuaRef SetScrollCallback(LuaRef function)
		{
			return setCallback(function, scroll_callback_fn_, glfwSetScrollCallback, scroll_callback_function);
		}
		LuaRef SetDropCallback(LuaRef function)
		{
			return setCallback(function, drop_callback_fn_, glfwSetDropCallback, drop_callback_function);
		}
	private:
		template<typename GLFWFUN, typename CALLBACKFUN>
		LuaRef setCallback(const LuaRef& function, LuaRef& to, GLFWFUN glfwfun, CALLBACKFUN callback)
		{
			LuaRef prev = to;
			if (function.type() == LuaRef::TYPE_FUNCTION)
			{
				to = function;
				glfwfun(window_, callback);
			}
			else
			{
				to = LuaRef();
			}
			return prev;
		}


		static void pos_callback_function(GLFWwindow* window, int width, int height)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			if (w->window_pos_callback_fn_)
			{
				w->window_pos_callback_fn_(width, height);
			}
		};
		static void size_callback_function(GLFWwindow* window, int width, int height)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			if (w->window_size_callback_fn_)
			{
				w->window_size_callback_fn_(width, height);
			}
		};
		static void close_callback_function(GLFWwindow* window)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			if (w->window_close_callback_fn_)
			{
				w->window_close_callback_fn_();
			}
		}
		static void refresh_callback_function(GLFWwindow* window)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			if (w->window_refresh_callback_fn_)
			{
				w->window_refresh_callback_fn_();
			}
		}
		static void focus_callback_function(GLFWwindow* window, int focus)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			if (w->window_focus_callback_fn_)
			{
				w->window_focus_callback_fn_(focus == GLFW_TRUE);
			}
		}
		static void iconify_callback_function(GLFWwindow* window, int iconify)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			if (w->window_iconify_callback_fn_)
			{
				w->window_iconify_callback_fn_(iconify == GLFW_TRUE);
			}
		}
		static void framebuffer_size_callback_function(GLFWwindow* window, int width, int height)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			if (w->framebuffer_size_callback_fn_)
			{
				w->framebuffer_size_callback_fn_(width, height);
			}
		}

		static void key_callback_function(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			if (w->key_callback_fn_)
			{
				w->key_callback_fn_(key, scancode, action, mods);
			}
		}

		static void char_callback_function(GLFWwindow* window, unsigned int chr)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			if (w->char_callback_fn_)
			{
				utfconv::u32string u32str; u32str.push_back(chr);
				w->char_callback_fn_(utfconv::to_utf8(u32str));
			}
		}

		static void charmods_callback_function(GLFWwindow* window, unsigned int chr, int mods)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			if (w->charmods_callback_fn_)
			{
				utfconv::u32string u32str; u32str.push_back(chr);
				w->charmods_callback_fn_(utfconv::to_utf8(u32str), mods);
			}
		}

		static void mouse_button_callback_function(GLFWwindow* window, int button, int action, int mods)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			if (w->mouse_button_callback_fn_)
			{
				w->mouse_button_callback_fn_(button, action, mods);
			}
		}
		static void cursor_pos_callback_function(GLFWwindow* window, double x, double y)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			if (w->cursor_pos_callback_fn_)
			{
				w->cursor_pos_callback_fn_(x, y);
			}
		}

		static void cursor_enter_callback_function(GLFWwindow* window, int enter)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			if (w->cursor_enter_callback_fn_)
			{
				w->cursor_enter_callback_fn_(enter == GLFW_TRUE);
			}
		}
		static void scroll_callback_function(GLFWwindow* window, double xoffset, double yoffset)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			if (w->scroll_callback_fn_)
			{
				w->scroll_callback_fn_(xoffset, yoffset);
			}
		}

		static void drop_callback_function(GLFWwindow* window, int count, const char** path)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			if (w->drop_callback_fn_)
			{
				std::vector < std::string > path;
				path.reserve(count);
				for (int i = 0; i < count; ++i)
				{
					path.push_back(path[i]);
				}
				w->drop_callback_fn_(path);
			}
		}

		GLFWwindow* window_;
		LuaRef window_pos_callback_fn_;
		LuaRef window_size_callback_fn_;
		LuaRef window_close_callback_fn_;
		LuaRef window_refresh_callback_fn_;
		LuaRef window_focus_callback_fn_;
		LuaRef window_iconify_callback_fn_;
		LuaRef framebuffer_size_callback_fn_;
		LuaRef key_callback_fn_;
		LuaRef char_callback_fn_;
		LuaRef charmods_callback_fn_;
		LuaRef mouse_button_callback_fn_;
		LuaRef cursor_pos_callback_fn_;
		LuaRef cursor_enter_callback_fn_;
		LuaRef scroll_callback_fn_;
		LuaRef drop_callback_fn_;

	};

	GLFWwindow* CreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
	{
		Window* w = new Window(width, height, title, monitor, share);
		return w->pointer();
	}


	void DestroyWindow(GLFWwindow* window)
	{
		Window* w = (Window*)glfwGetWindowUserPointer(window);
		delete w;
	}

	namespace WindowCallbacks
	{
		LuaRef SetWindowPosCallback(GLFWwindow* window, LuaRef function)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			return w->SetWindowPosCallback(function);
		}

		LuaRef SetWindowSizeCallback(GLFWwindow* window, LuaRef function)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			return w->SetWindowSizeCallback(function);
		}
		LuaRef SetWindowCloseCallback(GLFWwindow* window, LuaRef function)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			return w->SetWindowCloseCallback(function);
		}

		LuaRef SetWindowRefreshCallback(GLFWwindow* window, LuaRef function)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			return w->SetWindowRefreshCallback(function);
		}

		LuaRef SetWindowFocusCallback(GLFWwindow* window, LuaRef function)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			return w->SetWindowFocusCallback(function);
		}
		LuaRef SetWindowIconifyCallback(GLFWwindow* window, LuaRef function)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			return w->SetWindowIconifyCallback(function);
		}
		LuaRef SetFramebufferSizeCallback(GLFWwindow* window, LuaRef function)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			return w->SetFramebufferSizeCallback(function);
		}

		LuaRef SetKeyCallback(GLFWwindow* window, LuaRef function)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			return w->SetKeyCallback(function);
		}
		LuaRef SetCharCallback(GLFWwindow* window, LuaRef function)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			return w->SetCharCallback(function);
		}


		LuaRef SetCharModsCallback(GLFWwindow* window, LuaRef function)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			return w->SetCharModsCallback(function);
		}
		LuaRef SetMouseButtonCallback(GLFWwindow* window, LuaRef function)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			return w->SetMouseButtonCallback(function);
		}
		LuaRef SetCursorPosCallback(GLFWwindow* window, LuaRef function)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			return w->SetCursorPosCallback(function);
		}
		LuaRef SetCursorEnterCallback(GLFWwindow* window, LuaRef function)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			return w->SetCursorEnterCallback(function);
		}
		LuaRef SetScrollCallback(GLFWwindow* window, LuaRef function)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			return w->SetScrollCallback(function);
		}
		LuaRef SetDropCallback(GLFWwindow* window, LuaRef function)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			return w->SetDropCallback(function);
		}
	}

}

extern "C"
#ifdef _MSC_VER
__declspec(dllexport)
#endif
int luaopen_luaglfw(lua_State *L)
{
	State state(L);

	LuaRef luaglfw = state.newLib();

	//fixme can not direct register callback function
#define GLFW_LUA_REGFUNC(FNAME) luaglfw[#FNAME + 4] = function(&FNAME)
	GLFW_LUA_REGFUNC(glfwInit);
	GLFW_LUA_REGFUNC(glfwTerminate);
	GLFW_LUA_REGFUNC(glfwGetVersion);
	GLFW_LUA_REGFUNC(glfwGetVersionString);

	//	GLFW_LUA_REGFUNC(glfwSetErrorCallback);
	luaglfw["SetErrorCallback"] = function(&global_callbacks::SetErrorCallback);


	GLFW_LUA_REGFUNC(glfwGetMonitors);
	GLFW_LUA_REGFUNC(glfwGetPrimaryMonitor);
	GLFW_LUA_REGFUNC(glfwGetMonitorPos);
	GLFW_LUA_REGFUNC(glfwGetMonitorPhysicalSize);
	GLFW_LUA_REGFUNC(glfwGetMonitorName);

	//	GLFW_LUA_REGFUNC(glfwSetMonitorCallback);
	luaglfw["SetMonitorCallback"] = function(&global_callbacks::SetMonitorCallback);

	luaglfw["GetVideoModes"] = function([](GLFWmonitor* monitor) {
		int count = 0;
		const GLFWvidmode* mode = glfwGetVideoModes(monitor, &count);
		std::vector<GLFWvidmode> result;
		for (int i = 0; i < count; ++i)
		{
			result.push_back(mode[i]);
		}
		return result;
	});

	luaglfw["vidmode"].setClass(ClassMetatable<GLFWvidmode>()
		.addMember("width", &GLFWvidmode::width)
		.addMember("height", &GLFWvidmode::height)
		.addMember("redBits", &GLFWvidmode::redBits)
		.addMember("greenBits", &GLFWvidmode::greenBits)
		.addMember("blueBits", &GLFWvidmode::blueBits)
		.addMember("refreshRate", &GLFWvidmode::refreshRate)
		);

	luaglfw["GetVideoMode"] = function([](GLFWmonitor* monitor) {
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		return *mode;
	});

	GLFW_LUA_REGFUNC(glfwSetGamma);
	//	GLFW_LUA_REGFUNC(glfwGetGammaRamp);
	GLFW_LUA_REGFUNC(glfwSetGammaRamp);
	GLFW_LUA_REGFUNC(glfwDefaultWindowHints);
	GLFW_LUA_REGFUNC(glfwWindowHint);


	luaglfw["CreateWindow"] = function(&CreateWindow);
	luaglfw["DestroyWindow"] = function(&DestroyWindow);

	GLFW_LUA_REGFUNC(glfwSetWindowShouldClose);
	GLFW_LUA_REGFUNC(glfwWindowShouldClose);
	GLFW_LUA_REGFUNC(glfwSetWindowTitle);
	GLFW_LUA_REGFUNC(glfwGetWindowPos);

	GLFW_LUA_REGFUNC(glfwSetWindowPos);
	GLFW_LUA_REGFUNC(glfwSetWindowSize);

	luaglfw["GetWindowSize"] = function([](GLFWwindow* window) {
		int width = 0;
		int height = 0;
		glfwGetWindowSize(window, &width, &height);
		return standard::tuple<int, int>(width, height);
	});
	luaglfw["GetFramebufferSize"] = function([](GLFWwindow* window) {
		int width = 0;
		int height = 0;
		glfwGetFramebufferSize(window, &width, &height);
		return standard::tuple<int, int>(width, height);
	});
	luaglfw["GetWindowFrameSize"] = function([](GLFWwindow* window) {
		int left = 0;
		int right = 0;
		int top = 0;
		int bottom = 0;
		glfwGetWindowFrameSize(window, &left, &right, &top, &bottom);
		return standard::tuple<int, int, int, int>(left, right, top, bottom);
	});


	GLFW_LUA_REGFUNC(glfwIconifyWindow);
	GLFW_LUA_REGFUNC(glfwRestoreWindow);
	GLFW_LUA_REGFUNC(glfwShowWindow);
	GLFW_LUA_REGFUNC(glfwHideWindow);
	GLFW_LUA_REGFUNC(glfwGetWindowMonitor);
	GLFW_LUA_REGFUNC(glfwGetWindowAttrib);


	luaglfw["SetWindowPosCallback"] = function(&WindowCallbacks::SetWindowPosCallback);
	luaglfw["SetWindowSizeCallback"] = function(&WindowCallbacks::SetWindowSizeCallback);
	luaglfw["SetWindowCloseCallback"] = function(&WindowCallbacks::SetWindowCloseCallback);
	luaglfw["SetWindowRefreshCallback"] = function(&WindowCallbacks::SetWindowRefreshCallback);
	luaglfw["SetWindowFocusCallback"] = function(&WindowCallbacks::SetWindowFocusCallback);
	luaglfw["SetWindowIconifyCallback"] = function(&WindowCallbacks::SetWindowIconifyCallback);
	luaglfw["SetFramebufferSizeCallback"] = function(&WindowCallbacks::SetFramebufferSizeCallback);


	GLFW_LUA_REGFUNC(glfwPollEvents);
	GLFW_LUA_REGFUNC(glfwWaitEvents);
	GLFW_LUA_REGFUNC(glfwPostEmptyEvent);
	GLFW_LUA_REGFUNC(glfwGetInputMode);
	GLFW_LUA_REGFUNC(glfwSetInputMode);



	GLFW_LUA_REGFUNC(glfwGetKey);
	GLFW_LUA_REGFUNC(glfwGetMouseButton);
	GLFW_LUA_REGFUNC(glfwGetCursorPos);
	GLFW_LUA_REGFUNC(glfwSetCursorPos);


	GLFW_LUA_REGFUNC(glfwCreateCursor);
	GLFW_LUA_REGFUNC(glfwCreateStandardCursor);
	GLFW_LUA_REGFUNC(glfwDestroyCursor);
	GLFW_LUA_REGFUNC(glfwSetCursor);

	luaglfw["SetKeyCallback"] = function(&WindowCallbacks::SetKeyCallback);
	luaglfw["SetCharCallback"] = function(&WindowCallbacks::SetCharCallback);
	luaglfw["SetCharModsCallback"] = function(&WindowCallbacks::SetCharModsCallback);
	luaglfw["SetMouseButtonCallback"] = function(&WindowCallbacks::SetMouseButtonCallback);
	luaglfw["SetCursorPosCallback"] = function(&WindowCallbacks::SetCursorPosCallback);
	luaglfw["SetCursorEnterCallback"] = function(&WindowCallbacks::SetCursorEnterCallback);
	luaglfw["SetScrollCallback"] = function(&WindowCallbacks::SetScrollCallback);
	luaglfw["SetDropCallback"] = function(&WindowCallbacks::SetDropCallback);


	//	GLFW_LUA_REGFUNC(glfwSetCharModsCallback);
	//	GLFW_LUA_REGFUNC(glfwSetMouseButtonCallback);
	//	GLFW_LUA_REGFUNC(glfwSetCursorPosCallback);
	//	GLFW_LUA_REGFUNC(glfwSetCursorEnterCallback);
	//	GLFW_LUA_REGFUNC(glfwSetScrollCallback);
	//	GLFW_LUA_REGFUNC(glfwSetDropCallback);


	GLFW_LUA_REGFUNC(glfwJoystickPresent);

	luaglfw["GetJoystickAxes"] = function([](int joy) {
		int count = 0;
		const float* axes = glfwGetJoystickAxes(joy, &count);
		std::vector<float> result;
		for (int i = 0; i < count; ++i)
		{
			result.push_back(axes[i]);
		}
		return result;
	});
	luaglfw["GetJoystickButtons"] = function([](int joy) {
		int count = 0;
		const unsigned char* buttons = glfwGetJoystickButtons(joy, &count);
		std::vector<float> result;
		for (int i = 0; i < count; ++i)
		{
			result.push_back(buttons[i]);
		}
		return result;
	});
	GLFW_LUA_REGFUNC(glfwGetJoystickName);

	GLFW_LUA_REGFUNC(glfwSetClipboardString);
	GLFW_LUA_REGFUNC(glfwGetClipboardString);
	GLFW_LUA_REGFUNC(glfwGetTime);
	GLFW_LUA_REGFUNC(glfwSetTime);

	GLFW_LUA_REGFUNC(glfwMakeContextCurrent);
	GLFW_LUA_REGFUNC(glfwGetCurrentContext);
	GLFW_LUA_REGFUNC(glfwSwapBuffers);
	GLFW_LUA_REGFUNC(glfwSwapInterval);
	GLFW_LUA_REGFUNC(glfwExtensionSupported);
	//	GLFW_LUA_REGFUNC(glfwGetProcAddress);//procaddress cant use in lua

	luaglfw["VERSION_MAJOR"] = 3;
	luaglfw["VERSION_MINOR"] = 1;
	luaglfw["VERSION_REVISION"] = 2;
	luaglfw["RELEASE"] = 0;
	luaglfw["PRESS"] = 1;
	luaglfw["REPEAT"] = 2;
	luaglfw["KEY_UNKNOWN"] = -1;
	luaglfw["KEY_SPACE"] = 32;
	luaglfw["KEY_APOSTROPHE"] = 39;
	luaglfw["KEY_COMMA"] = 44;
	luaglfw["KEY_MINUS"] = 45;
	luaglfw["KEY_PERIOD"] = 46;
	luaglfw["KEY_SLASH"] = 47;
	luaglfw["KEY_0"] = 48;
	luaglfw["KEY_1"] = 49;
	luaglfw["KEY_2"] = 50;
	luaglfw["KEY_3"] = 51;
	luaglfw["KEY_4"] = 52;
	luaglfw["KEY_5"] = 53;
	luaglfw["KEY_6"] = 54;
	luaglfw["KEY_7"] = 55;
	luaglfw["KEY_8"] = 56;
	luaglfw["KEY_9"] = 57;
	luaglfw["KEY_SEMICOLON"] = 59;
	luaglfw["KEY_EQUAL"] = 61;
	luaglfw["KEY_A"] = 65;
	luaglfw["KEY_B"] = 66;
	luaglfw["KEY_C"] = 67;
	luaglfw["KEY_D"] = 68;
	luaglfw["KEY_E"] = 69;
	luaglfw["KEY_F"] = 70;
	luaglfw["KEY_G"] = 71;
	luaglfw["KEY_H"] = 72;
	luaglfw["KEY_I"] = 73;
	luaglfw["KEY_J"] = 74;
	luaglfw["KEY_K"] = 75;
	luaglfw["KEY_L"] = 76;
	luaglfw["KEY_M"] = 77;
	luaglfw["KEY_N"] = 78;
	luaglfw["KEY_O"] = 79;
	luaglfw["KEY_P"] = 80;
	luaglfw["KEY_Q"] = 81;
	luaglfw["KEY_R"] = 82;
	luaglfw["KEY_S"] = 83;
	luaglfw["KEY_T"] = 84;
	luaglfw["KEY_U"] = 85;
	luaglfw["KEY_V"] = 86;
	luaglfw["KEY_W"] = 87;
	luaglfw["KEY_X"] = 88;
	luaglfw["KEY_Y"] = 89;
	luaglfw["KEY_Z"] = 90;
	luaglfw["KEY_LEFT_BRACKET"] = 91;
	luaglfw["KEY_BACKSLASH"] = 92;
	luaglfw["KEY_RIGHT_BRACKET"] = 93;
	luaglfw["KEY_GRAVE_ACCENT"] = 96;
	luaglfw["KEY_WORLD_1"] = 161;
	luaglfw["KEY_WORLD_2"] = 162;
	luaglfw["KEY_ESCAPE"] = 256;
	luaglfw["KEY_ENTER"] = 257;
	luaglfw["KEY_TAB"] = 258;
	luaglfw["KEY_BACKSPACE"] = 259;
	luaglfw["KEY_INSERT"] = 260;
	luaglfw["KEY_DELETE"] = 261;
	luaglfw["KEY_RIGHT"] = 262;
	luaglfw["KEY_LEFT"] = 263;
	luaglfw["KEY_DOWN"] = 264;
	luaglfw["KEY_UP"] = 265;
	luaglfw["KEY_PAGE_UP"] = 266;
	luaglfw["KEY_PAGE_DOWN"] = 267;
	luaglfw["KEY_HOME"] = 268;
	luaglfw["KEY_END"] = 269;
	luaglfw["KEY_CAPS_LOCK"] = 280;
	luaglfw["KEY_SCROLL_LOCK"] = 281;
	luaglfw["KEY_NUM_LOCK"] = 282;
	luaglfw["KEY_PRINT_SCREEN"] = 283;
	luaglfw["KEY_PAUSE"] = 284;
	luaglfw["KEY_F1"] = 290;
	luaglfw["KEY_F2"] = 291;
	luaglfw["KEY_F3"] = 292;
	luaglfw["KEY_F4"] = 293;
	luaglfw["KEY_F5"] = 294;
	luaglfw["KEY_F6"] = 295;
	luaglfw["KEY_F7"] = 296;
	luaglfw["KEY_F8"] = 297;
	luaglfw["KEY_F9"] = 298;
	luaglfw["KEY_F10"] = 299;
	luaglfw["KEY_F11"] = 300;
	luaglfw["KEY_F12"] = 301;
	luaglfw["KEY_F13"] = 302;
	luaglfw["KEY_F14"] = 303;
	luaglfw["KEY_F15"] = 304;
	luaglfw["KEY_F16"] = 305;
	luaglfw["KEY_F17"] = 306;
	luaglfw["KEY_F18"] = 307;
	luaglfw["KEY_F19"] = 308;
	luaglfw["KEY_F20"] = 309;
	luaglfw["KEY_F21"] = 310;
	luaglfw["KEY_F22"] = 311;
	luaglfw["KEY_F23"] = 312;
	luaglfw["KEY_F24"] = 313;
	luaglfw["KEY_F25"] = 314;
	luaglfw["KEY_KP_0"] = 320;
	luaglfw["KEY_KP_1"] = 321;
	luaglfw["KEY_KP_2"] = 322;
	luaglfw["KEY_KP_3"] = 323;
	luaglfw["KEY_KP_4"] = 324;
	luaglfw["KEY_KP_5"] = 325;
	luaglfw["KEY_KP_6"] = 326;
	luaglfw["KEY_KP_7"] = 327;
	luaglfw["KEY_KP_8"] = 328;
	luaglfw["KEY_KP_9"] = 329;
	luaglfw["KEY_KP_DECIMAL"] = 330;
	luaglfw["KEY_KP_DIVIDE"] = 331;
	luaglfw["KEY_KP_MULTIPLY"] = 332;
	luaglfw["KEY_KP_SUBTRACT"] = 333;
	luaglfw["KEY_KP_ADD"] = 334;
	luaglfw["KEY_KP_ENTER"] = 335;
	luaglfw["KEY_KP_EQUAL"] = 336;
	luaglfw["KEY_LEFT_SHIFT"] = 340;
	luaglfw["KEY_LEFT_CONTROL"] = 341;
	luaglfw["KEY_LEFT_ALT"] = 342;
	luaglfw["KEY_LEFT_SUPER"] = 343;
	luaglfw["KEY_RIGHT_SHIFT"] = 344;
	luaglfw["KEY_RIGHT_CONTROL"] = 345;
	luaglfw["KEY_RIGHT_ALT"] = 346;
	luaglfw["KEY_RIGHT_SUPER"] = 347;
	luaglfw["KEY_MENU"] = 348;
	luaglfw["KEY_LAST"] = luaglfw["KEY_MENU"];
	luaglfw["MOD_SHIFT"] = 0x0001;
	luaglfw["MOD_CONTROL"] = 0x0002;
	luaglfw["MOD_ALT"] = 0x0004;
	luaglfw["MOD_SUPER"] = 0x0008;
	luaglfw["MOUSE_BUTTON_1"] = 0;
	luaglfw["MOUSE_BUTTON_2"] = 1;
	luaglfw["MOUSE_BUTTON_3"] = 2;
	luaglfw["MOUSE_BUTTON_4"] = 3;
	luaglfw["MOUSE_BUTTON_5"] = 4;
	luaglfw["MOUSE_BUTTON_6"] = 5;
	luaglfw["MOUSE_BUTTON_7"] = 6;
	luaglfw["MOUSE_BUTTON_8"] = 7;
	luaglfw["MOUSE_BUTTON_LAST"] = luaglfw["MOUSE_BUTTON_8"];
	luaglfw["MOUSE_BUTTON_LEFT"] = luaglfw["MOUSE_BUTTON_1"];
	luaglfw["MOUSE_BUTTON_RIGHT"] = luaglfw["MOUSE_BUTTON_2"];
	luaglfw["MOUSE_BUTTON_MIDDLE"] = luaglfw["MOUSE_BUTTON_3"];
	luaglfw["JOYSTICK_1"] = 0;
	luaglfw["JOYSTICK_2"] = 1;
	luaglfw["JOYSTICK_3"] = 2;
	luaglfw["JOYSTICK_4"] = 3;
	luaglfw["JOYSTICK_5"] = 4;
	luaglfw["JOYSTICK_6"] = 5;
	luaglfw["JOYSTICK_7"] = 6;
	luaglfw["JOYSTICK_8"] = 7;
	luaglfw["JOYSTICK_9"] = 8;
	luaglfw["JOYSTICK_10"] = 9;
	luaglfw["JOYSTICK_11"] = 10;
	luaglfw["JOYSTICK_12"] = 11;
	luaglfw["JOYSTICK_13"] = 12;
	luaglfw["JOYSTICK_14"] = 13;
	luaglfw["JOYSTICK_15"] = 14;
	luaglfw["JOYSTICK_16"] = 15;
	luaglfw["JOYSTICK_LAST"] = luaglfw["JOYSTICK_16"];
	luaglfw["NOT_INITIALIZED"] = 0x00010001;
	luaglfw["NO_CURRENT_CONTEXT"] = 0x00010002;
	luaglfw["INVALID_ENUM"] = 0x00010003;
	luaglfw["INVALID_VALUE"] = 0x00010004;
	luaglfw["OUT_OF_MEMORY"] = 0x00010005;
	luaglfw["API_UNAVAILABLE"] = 0x00010006;
	luaglfw["VERSION_UNAVAILABLE"] = 0x00010007;
	luaglfw["PLATFORM_ERROR"] = 0x00010008;
	luaglfw["FORMAT_UNAVAILABLE"] = 0x00010009;
	luaglfw["FOCUSED"] = 0x00020001;
	luaglfw["ICONIFIED"] = 0x00020002;
	luaglfw["RESIZABLE"] = 0x00020003;
	luaglfw["VISIBLE"] = 0x00020004;
	luaglfw["DECORATED"] = 0x00020005;
	luaglfw["AUTO_ICONIFY"] = 0x00020006;
	luaglfw["FLOATING"] = 0x00020007;
	luaglfw["RED_BITS"] = 0x00021001;
	luaglfw["GREEN_BITS"] = 0x00021002;
	luaglfw["BLUE_BITS"] = 0x00021003;
	luaglfw["ALPHA_BITS"] = 0x00021004;
	luaglfw["DEPTH_BITS"] = 0x00021005;
	luaglfw["STENCIL_BITS"] = 0x00021006;
	luaglfw["ACCUM_RED_BITS"] = 0x00021007;
	luaglfw["ACCUM_GREEN_BITS"] = 0x00021008;
	luaglfw["ACCUM_BLUE_BITS"] = 0x00021009;
	luaglfw["ACCUM_ALPHA_BITS"] = 0x0002100a;
	luaglfw["AUX_BUFFERS"] = 0x0002100b;
	luaglfw["STEREO"] = 0x0002100c;
	luaglfw["SAMPLES"] = 0x0002100d;
	luaglfw["SRGB_CAPABLE"] = 0x0002100e;
	luaglfw["REFRESH_RATE"] = 0x0002100f;
	luaglfw["DOUBLEBUFFER"] = 0x00021010;
	luaglfw["CLIENT_API"] = 0x00022001;
	luaglfw["CONTEXT_VERSION_MAJOR"] = 0x00022002;
	luaglfw["CONTEXT_VERSION_MINOR"] = 0x00022003;
	luaglfw["CONTEXT_REVISION"] = 0x00022004;
	luaglfw["CONTEXT_ROBUSTNESS"] = 0x00022005;
	luaglfw["OPENGL_FORWARD_COMPAT"] = 0x00022006;
	luaglfw["OPENGL_DEBUG_CONTEXT"] = 0x00022007;
	luaglfw["OPENGL_PROFILE"] = 0x00022008;
	luaglfw["CONTEXT_RELEASE_BEHAVIOR"] = 0x00022009;
	luaglfw["OPENGL_API"] = 0x00030001;
	luaglfw["OPENGL_ES_API"] = 0x00030002;
	luaglfw["NO_ROBUSTNESS"] = 0;
	luaglfw["NO_RESET_NOTIFICATION"] = 0x00031001;
	luaglfw["LOSE_CONTEXT_ON_RESET"] = 0x00031002;
	luaglfw["OPENGL_ANY_PROFILE"] = 0;
	luaglfw["OPENGL_CORE_PROFILE"] = 0x00032001;
	luaglfw["OPENGL_COMPAT_PROFILE"] = 0x00032002;
	luaglfw["CURSOR"] = 0x00033001;
	luaglfw["STICKY_KEYS"] = 0x00033002;
	luaglfw["STICKY_MOUSE_BUTTONS"] = 0x00033003;
	luaglfw["CURSOR_NORMAL"] = 0x00034001;
	luaglfw["CURSOR_HIDDEN"] = 0x00034002;
	luaglfw["CURSOR_DISABLED"] = 0x00034003;
	luaglfw["ANY_RELEASE_BEHAVIOR"] = 0;
	luaglfw["RELEASE_BEHAVIOR_FLUSH"] = 0x00035001;
	luaglfw["RELEASE_BEHAVIOR_NONE"] = 0x00035002;
	luaglfw["ARROW_CURSOR"] = 0x00036001;
	luaglfw["IBEAM_CURSOR"] = 0x00036002;
	luaglfw["CROSSHAIR_CURSOR"] = 0x00036003;
	luaglfw["HAND_CURSOR"] = 0x00036004;
	luaglfw["HRESIZE_CURSOR"] = 0x00036005;
	luaglfw["VRESIZE_CURSOR"] = 0x00036006;
	luaglfw["CONNECTED"] = 0x00040001;
	luaglfw["DISCONNECTED"] = 0x00040002;
	luaglfw["DONT_CARE"] = -1;

	return 1;
}
