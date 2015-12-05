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
		Window(int width, int height, const char* title)
		{
			window_ = glfwCreateWindow(width, height, title, 0, 0);
			glfwSetWindowUserPointer(window_, this);
		}
		Window(int width, int height, const char* title, GLFWmonitor* monitor, Window* share)
		{
			window_ = glfwCreateWindow(width, height, title, monitor, share->window_);
			glfwSetWindowUserPointer(window_, this);
		}
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
		GLFWwindow* native_pointer() { return window_; }


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
		return w->native_pointer();
	}
	void DestroyWindow(GLFWwindow* window)
	{
		Window* w = (Window*)glfwGetWindowUserPointer(window);
		delete w;
	}
	standard::tuple<int, int> GetWindowSize(GLFWwindow* window)
	{
		int width = 0;
		int height = 0;
		glfwGetWindowSize(window, &width, &height);
		return standard::tuple<int, int>(width, height);	
	}
	standard::tuple<int, int> GetFramebufferSize(GLFWwindow* window)
	{
		int width = 0;
		int height = 0;
		glfwGetFramebufferSize(window, &width, &height);
		return standard::tuple<int, int>(width, height);
	}
	standard::tuple<int, int, int, int> GetWindowFrameSize(GLFWwindow* window)
	{
		int left = 0;
		int right = 0;
		int top = 0;
		int bottom = 0;
		glfwGetWindowFrameSize(window, &left, &right, &top, &bottom);
		return standard::tuple<int, int, int, int>(left, right, top, bottom);
	}

	standard::tuple<int, int> GetWindowPos(GLFWwindow* window)
	{
		int xpos = 0;
		int ypos = 0;
		glfwGetWindowPos(window, &xpos, &ypos);
		return standard::tuple<int, int>(xpos, ypos);
	}

	standard::tuple<double, double> GetCursorPos(GLFWwindow* window)
	{
		double xpos = 0;
		double ypos = 0;
		glfwGetCursorPos(window, &xpos, &ypos);
		return standard::tuple<double, double>(xpos, ypos);
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
	
	luaglfw["GetMonitors"] = function([]() {
		int count = 0;
		GLFWmonitor** mon = glfwGetMonitors( &count);
		std::vector<GLFWmonitor*> result;
		for (int i = 0; i < count; ++i)
		{
			result.push_back(mon[i]);
		}
		return result;
	});
	GLFW_LUA_REGFUNC(glfwGetPrimaryMonitor);


	GLFW_LUA_REGFUNC(glfwGetMonitorPos);

	luaglfw["GetMonitorPos"] = function([](GLFWmonitor* monitor) {
		int xpos = 0;
		int ypos = 0;
		glfwGetMonitorPos(monitor,&xpos,&ypos);
		return standard::tuple<int, int>(xpos, ypos);
	});

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
	
	luaglfw["GetWindowPos"] = function(GetWindowPos);

	GLFW_LUA_REGFUNC(glfwSetWindowPos);
	GLFW_LUA_REGFUNC(glfwSetWindowSize);

	luaglfw["GetWindowSize"] = function(&GetWindowSize);
	luaglfw["GetFramebufferSize"] = function(&GetFramebufferSize);
	luaglfw["GetWindowFrameSize"] = function(GetWindowFrameSize);


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
	GLFW_LUA_REGFUNC(GetCursorPos);
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

#define GLFW_LUA_REGDEF(NAME) luaglfw[#NAME + 5] = NAME
	GLFW_LUA_REGDEF(GLFW_VERSION_MAJOR);
	GLFW_LUA_REGDEF(GLFW_VERSION_MINOR);
	GLFW_LUA_REGDEF(GLFW_VERSION_REVISION);
	GLFW_LUA_REGDEF(GLFW_RELEASE);
	GLFW_LUA_REGDEF(GLFW_PRESS);
	GLFW_LUA_REGDEF(GLFW_REPEAT);
	GLFW_LUA_REGDEF(GLFW_KEY_UNKNOWN);
	GLFW_LUA_REGDEF(GLFW_KEY_SPACE);
	GLFW_LUA_REGDEF(GLFW_KEY_APOSTROPHE);
	GLFW_LUA_REGDEF(GLFW_KEY_COMMA);
	GLFW_LUA_REGDEF(GLFW_KEY_MINUS);
	GLFW_LUA_REGDEF(GLFW_KEY_PERIOD);
	GLFW_LUA_REGDEF(GLFW_KEY_SLASH);
	GLFW_LUA_REGDEF(GLFW_KEY_0);
	GLFW_LUA_REGDEF(GLFW_KEY_1);
	GLFW_LUA_REGDEF(GLFW_KEY_2);
	GLFW_LUA_REGDEF(GLFW_KEY_3);
	GLFW_LUA_REGDEF(GLFW_KEY_4);
	GLFW_LUA_REGDEF(GLFW_KEY_5);
	GLFW_LUA_REGDEF(GLFW_KEY_6);
	GLFW_LUA_REGDEF(GLFW_KEY_7);
	GLFW_LUA_REGDEF(GLFW_KEY_8);
	GLFW_LUA_REGDEF(GLFW_KEY_9);
	GLFW_LUA_REGDEF(GLFW_KEY_SEMICOLON);
	GLFW_LUA_REGDEF(GLFW_KEY_EQUAL);
	GLFW_LUA_REGDEF(GLFW_KEY_A);
	GLFW_LUA_REGDEF(GLFW_KEY_B);
	GLFW_LUA_REGDEF(GLFW_KEY_C);
	GLFW_LUA_REGDEF(GLFW_KEY_D);
	GLFW_LUA_REGDEF(GLFW_KEY_E);
	GLFW_LUA_REGDEF(GLFW_KEY_F);
	GLFW_LUA_REGDEF(GLFW_KEY_G);
	GLFW_LUA_REGDEF(GLFW_KEY_H);
	GLFW_LUA_REGDEF(GLFW_KEY_I);
	GLFW_LUA_REGDEF(GLFW_KEY_J);
	GLFW_LUA_REGDEF(GLFW_KEY_K);
	GLFW_LUA_REGDEF(GLFW_KEY_L);
	GLFW_LUA_REGDEF(GLFW_KEY_M);
	GLFW_LUA_REGDEF(GLFW_KEY_N);
	GLFW_LUA_REGDEF(GLFW_KEY_O);
	GLFW_LUA_REGDEF(GLFW_KEY_P);
	GLFW_LUA_REGDEF(GLFW_KEY_Q);
	GLFW_LUA_REGDEF(GLFW_KEY_R);
	GLFW_LUA_REGDEF(GLFW_KEY_S);
	GLFW_LUA_REGDEF(GLFW_KEY_T);
	GLFW_LUA_REGDEF(GLFW_KEY_U);
	GLFW_LUA_REGDEF(GLFW_KEY_V);
	GLFW_LUA_REGDEF(GLFW_KEY_W);
	GLFW_LUA_REGDEF(GLFW_KEY_X);
	GLFW_LUA_REGDEF(GLFW_KEY_Y);
	GLFW_LUA_REGDEF(GLFW_KEY_Z);
	GLFW_LUA_REGDEF(GLFW_KEY_LEFT_BRACKET);
	GLFW_LUA_REGDEF(GLFW_KEY_BACKSLASH);
	GLFW_LUA_REGDEF(GLFW_KEY_RIGHT_BRACKET);
	GLFW_LUA_REGDEF(GLFW_KEY_GRAVE_ACCENT);
	GLFW_LUA_REGDEF(GLFW_KEY_WORLD_1);
	GLFW_LUA_REGDEF(GLFW_KEY_WORLD_2);
	GLFW_LUA_REGDEF(GLFW_KEY_ESCAPE);
	GLFW_LUA_REGDEF(GLFW_KEY_ENTER);
	GLFW_LUA_REGDEF(GLFW_KEY_TAB);
	GLFW_LUA_REGDEF(GLFW_KEY_BACKSPACE);
	GLFW_LUA_REGDEF(GLFW_KEY_INSERT);
	GLFW_LUA_REGDEF(GLFW_KEY_DELETE);
	GLFW_LUA_REGDEF(GLFW_KEY_RIGHT);
	GLFW_LUA_REGDEF(GLFW_KEY_LEFT);
	GLFW_LUA_REGDEF(GLFW_KEY_DOWN);
	GLFW_LUA_REGDEF(GLFW_KEY_UP);
	GLFW_LUA_REGDEF(GLFW_KEY_PAGE_UP);
	GLFW_LUA_REGDEF(GLFW_KEY_PAGE_DOWN);
	GLFW_LUA_REGDEF(GLFW_KEY_HOME);
	GLFW_LUA_REGDEF(GLFW_KEY_END);
	GLFW_LUA_REGDEF(GLFW_KEY_CAPS_LOCK);
	GLFW_LUA_REGDEF(GLFW_KEY_SCROLL_LOCK);
	GLFW_LUA_REGDEF(GLFW_KEY_NUM_LOCK);
	GLFW_LUA_REGDEF(GLFW_KEY_PRINT_SCREEN);
	GLFW_LUA_REGDEF(GLFW_KEY_PAUSE);
	GLFW_LUA_REGDEF(GLFW_KEY_F1);
	GLFW_LUA_REGDEF(GLFW_KEY_F2);
	GLFW_LUA_REGDEF(GLFW_KEY_F3);
	GLFW_LUA_REGDEF(GLFW_KEY_F4);
	GLFW_LUA_REGDEF(GLFW_KEY_F5);
	GLFW_LUA_REGDEF(GLFW_KEY_F6);
	GLFW_LUA_REGDEF(GLFW_KEY_F7);
	GLFW_LUA_REGDEF(GLFW_KEY_F8);
	GLFW_LUA_REGDEF(GLFW_KEY_F9);
	GLFW_LUA_REGDEF(GLFW_KEY_F10);
	GLFW_LUA_REGDEF(GLFW_KEY_F11);
	GLFW_LUA_REGDEF(GLFW_KEY_F12);
	GLFW_LUA_REGDEF(GLFW_KEY_F13);
	GLFW_LUA_REGDEF(GLFW_KEY_F14);
	GLFW_LUA_REGDEF(GLFW_KEY_F15);
	GLFW_LUA_REGDEF(GLFW_KEY_F16);
	GLFW_LUA_REGDEF(GLFW_KEY_F17);
	GLFW_LUA_REGDEF(GLFW_KEY_F18);
	GLFW_LUA_REGDEF(GLFW_KEY_F19);
	GLFW_LUA_REGDEF(GLFW_KEY_F20);
	GLFW_LUA_REGDEF(GLFW_KEY_F21);
	GLFW_LUA_REGDEF(GLFW_KEY_F22);
	GLFW_LUA_REGDEF(GLFW_KEY_F23);
	GLFW_LUA_REGDEF(GLFW_KEY_F24);
	GLFW_LUA_REGDEF(GLFW_KEY_F25);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_0);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_1);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_2);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_3);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_4);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_5);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_6);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_7);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_8);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_9);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_DECIMAL);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_DIVIDE);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_MULTIPLY);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_SUBTRACT);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_ADD);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_ENTER);
	GLFW_LUA_REGDEF(GLFW_KEY_KP_EQUAL);
	GLFW_LUA_REGDEF(GLFW_KEY_LEFT_SHIFT);
	GLFW_LUA_REGDEF(GLFW_KEY_LEFT_CONTROL);
	GLFW_LUA_REGDEF(GLFW_KEY_LEFT_ALT);
	GLFW_LUA_REGDEF(GLFW_KEY_LEFT_SUPER);
	GLFW_LUA_REGDEF(GLFW_KEY_RIGHT_SHIFT);
	GLFW_LUA_REGDEF(GLFW_KEY_RIGHT_CONTROL);
	GLFW_LUA_REGDEF(GLFW_KEY_RIGHT_ALT);
	GLFW_LUA_REGDEF(GLFW_KEY_RIGHT_SUPER);
	GLFW_LUA_REGDEF(GLFW_KEY_MENU);
	GLFW_LUA_REGDEF(GLFW_KEY_LAST);
	GLFW_LUA_REGDEF(GLFW_MOD_SHIFT);
	GLFW_LUA_REGDEF(GLFW_MOD_CONTROL);
	GLFW_LUA_REGDEF(GLFW_MOD_ALT);
	GLFW_LUA_REGDEF(GLFW_MOD_SUPER);
	GLFW_LUA_REGDEF(GLFW_MOUSE_BUTTON_1);
	GLFW_LUA_REGDEF(GLFW_MOUSE_BUTTON_2);
	GLFW_LUA_REGDEF(GLFW_MOUSE_BUTTON_3);
	GLFW_LUA_REGDEF(GLFW_MOUSE_BUTTON_4);
	GLFW_LUA_REGDEF(GLFW_MOUSE_BUTTON_5);
	GLFW_LUA_REGDEF(GLFW_MOUSE_BUTTON_6);
	GLFW_LUA_REGDEF(GLFW_MOUSE_BUTTON_7);
	GLFW_LUA_REGDEF(GLFW_MOUSE_BUTTON_8);
	GLFW_LUA_REGDEF(GLFW_MOUSE_BUTTON_LAST);
	GLFW_LUA_REGDEF(GLFW_MOUSE_BUTTON_LEFT);
	GLFW_LUA_REGDEF(GLFW_MOUSE_BUTTON_RIGHT);
	GLFW_LUA_REGDEF(GLFW_MOUSE_BUTTON_MIDDLE);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_1);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_2);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_3);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_4);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_5);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_6);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_7);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_8);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_9);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_10);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_11);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_12);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_13);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_14);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_15);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_16);
	GLFW_LUA_REGDEF(GLFW_JOYSTICK_LAST);
	GLFW_LUA_REGDEF(GLFW_NOT_INITIALIZED);
	GLFW_LUA_REGDEF(GLFW_NO_CURRENT_CONTEXT);
	GLFW_LUA_REGDEF(GLFW_INVALID_ENUM);
	GLFW_LUA_REGDEF(GLFW_INVALID_VALUE);
	GLFW_LUA_REGDEF(GLFW_OUT_OF_MEMORY);
	GLFW_LUA_REGDEF(GLFW_API_UNAVAILABLE);
	GLFW_LUA_REGDEF(GLFW_VERSION_UNAVAILABLE);
	GLFW_LUA_REGDEF(GLFW_PLATFORM_ERROR);
	GLFW_LUA_REGDEF(GLFW_FORMAT_UNAVAILABLE);
	GLFW_LUA_REGDEF(GLFW_FOCUSED);
	GLFW_LUA_REGDEF(GLFW_ICONIFIED);
	GLFW_LUA_REGDEF(GLFW_RESIZABLE);
	GLFW_LUA_REGDEF(GLFW_VISIBLE);
	GLFW_LUA_REGDEF(GLFW_DECORATED);
	GLFW_LUA_REGDEF(GLFW_AUTO_ICONIFY);
	GLFW_LUA_REGDEF(GLFW_FLOATING);
	GLFW_LUA_REGDEF(GLFW_RED_BITS);
	GLFW_LUA_REGDEF(GLFW_GREEN_BITS);
	GLFW_LUA_REGDEF(GLFW_BLUE_BITS);
	GLFW_LUA_REGDEF(GLFW_ALPHA_BITS);
	GLFW_LUA_REGDEF(GLFW_DEPTH_BITS);
	GLFW_LUA_REGDEF(GLFW_STENCIL_BITS);
	GLFW_LUA_REGDEF(GLFW_ACCUM_RED_BITS);
	GLFW_LUA_REGDEF(GLFW_ACCUM_GREEN_BITS);
	GLFW_LUA_REGDEF(GLFW_ACCUM_BLUE_BITS);
	GLFW_LUA_REGDEF(GLFW_ACCUM_ALPHA_BITS);
	GLFW_LUA_REGDEF(GLFW_AUX_BUFFERS);
	GLFW_LUA_REGDEF(GLFW_STEREO);
	GLFW_LUA_REGDEF(GLFW_SAMPLES);
	GLFW_LUA_REGDEF(GLFW_SRGB_CAPABLE);
	GLFW_LUA_REGDEF(GLFW_REFRESH_RATE);
	GLFW_LUA_REGDEF(GLFW_DOUBLEBUFFER);
	GLFW_LUA_REGDEF(GLFW_CLIENT_API);
	GLFW_LUA_REGDEF(GLFW_CONTEXT_VERSION_MAJOR);
	GLFW_LUA_REGDEF(GLFW_CONTEXT_VERSION_MINOR);
	GLFW_LUA_REGDEF(GLFW_CONTEXT_REVISION);
	GLFW_LUA_REGDEF(GLFW_CONTEXT_ROBUSTNESS);
	GLFW_LUA_REGDEF(GLFW_OPENGL_FORWARD_COMPAT);
	GLFW_LUA_REGDEF(GLFW_OPENGL_DEBUG_CONTEXT);
	GLFW_LUA_REGDEF(GLFW_OPENGL_PROFILE);
	GLFW_LUA_REGDEF(GLFW_CONTEXT_RELEASE_BEHAVIOR);
	GLFW_LUA_REGDEF(GLFW_OPENGL_API);
	GLFW_LUA_REGDEF(GLFW_OPENGL_ES_API);
	GLFW_LUA_REGDEF(GLFW_NO_ROBUSTNESS);
	GLFW_LUA_REGDEF(GLFW_NO_RESET_NOTIFICATION);
	GLFW_LUA_REGDEF(GLFW_LOSE_CONTEXT_ON_RESET);
	GLFW_LUA_REGDEF(GLFW_OPENGL_ANY_PROFILE);
	GLFW_LUA_REGDEF(GLFW_OPENGL_CORE_PROFILE);
	GLFW_LUA_REGDEF(GLFW_OPENGL_COMPAT_PROFILE);
	GLFW_LUA_REGDEF(GLFW_CURSOR);
	GLFW_LUA_REGDEF(GLFW_STICKY_KEYS);
	GLFW_LUA_REGDEF(GLFW_STICKY_MOUSE_BUTTONS);
	GLFW_LUA_REGDEF(GLFW_CURSOR_NORMAL);
	GLFW_LUA_REGDEF(GLFW_CURSOR_HIDDEN);
	GLFW_LUA_REGDEF(GLFW_CURSOR_DISABLED);
	GLFW_LUA_REGDEF(GLFW_ANY_RELEASE_BEHAVIOR);
	GLFW_LUA_REGDEF(GLFW_RELEASE_BEHAVIOR_FLUSH);
	GLFW_LUA_REGDEF(GLFW_RELEASE_BEHAVIOR_NONE);
	GLFW_LUA_REGDEF(GLFW_ARROW_CURSOR);
	GLFW_LUA_REGDEF(GLFW_IBEAM_CURSOR);
	GLFW_LUA_REGDEF(GLFW_CROSSHAIR_CURSOR);
	GLFW_LUA_REGDEF(GLFW_HAND_CURSOR);
	GLFW_LUA_REGDEF(GLFW_HRESIZE_CURSOR);
	GLFW_LUA_REGDEF(GLFW_VRESIZE_CURSOR);
	GLFW_LUA_REGDEF(GLFW_CONNECTED);
	GLFW_LUA_REGDEF(GLFW_DISCONNECTED);
	GLFW_LUA_REGDEF(GLFW_DONT_CARE);

	luaglfw["window"].setClass(ClassMetatable<MetaPointerWrapper<GLFWwindow> >()
		.addStaticMember("new", [](int w, int h, const char* title) {return CreateWindow(w, h, title, 0, 0); })
		.addStaticMember("new", &CreateWindow)
		.addStaticMember("__gc", &DestroyWindow)
		.addStaticMember("iconify", &glfwIconifyWindow)
		.addStaticMember("restore", &glfwRestoreWindow)
		.addStaticMember("show", &glfwShowWindow)
		.addStaticMember("hide", &glfwHideWindow)
		.addStaticMember("getMonitor", &glfwGetWindowMonitor)
		.addStaticMember("getAttrib", &glfwGetWindowAttrib)
		.addStaticMember("makeContextCurrent", &glfwMakeContextCurrent)
		.addStaticMember("setWindowShouldClose", &glfwSetWindowShouldClose)
		.addStaticMember("windowShouldClose", &glfwWindowShouldClose)
		.addStaticMember("swapBuffers", &glfwSwapBuffers)
		.addStaticMember("getWindowSize", &GetWindowSize)
		.addStaticMember("getFramebufferSize", &GetFramebufferSize)
		.addStaticMember("getWindowFrameSize", &GetWindowFrameSize)

		.addStaticMember("setClipboardString", &glfwSetClipboardString)
		.addStaticMember("getWindowPos", &GetWindowPos)

		.addStaticMember("setWindowTitle", &glfwSetWindowTitle)
		.addStaticMember("setInputMode", &glfwSetInputMode)

		.addStaticMember("getInputMode", &glfwGetInputMode)
		.addStaticMember("setInputMode", &glfwSetInputMode)
		.addStaticMember("getKey", &glfwGetKey)
		.addStaticMember("getMouseButton", &glfwGetMouseButton)
		.addStaticMember("getCursorPos", &GetCursorPos)
		.addStaticMember("setCursorPos", &glfwSetCursorPos)

		.addStaticMember("setCursor", &glfwSetCursor)

		.addStaticMember("setWindowPosCallback", &WindowCallbacks::SetWindowPosCallback)
		.addStaticMember("setWindowSizeCallback", &WindowCallbacks::SetWindowSizeCallback)
		.addStaticMember("setWindowCloseCallback", &WindowCallbacks::SetWindowCloseCallback)
		.addStaticMember("setWindowRefreshCallback", &WindowCallbacks::SetWindowRefreshCallback)
		.addStaticMember("setWindowFocusCallback", &WindowCallbacks::SetWindowFocusCallback)
		.addStaticMember("setWindowIconifyCallback", &WindowCallbacks::SetWindowIconifyCallback)
		.addStaticMember("setKeyCallback", &WindowCallbacks::SetKeyCallback)
		.addStaticMember("setCharCallback", &WindowCallbacks::SetCharCallback)
		.addStaticMember("setCharModsCallback", &WindowCallbacks::SetCharModsCallback)
		.addStaticMember("setMouseButtonCallback", &WindowCallbacks::SetMouseButtonCallback)
		.addStaticMember("setCursorPosCallback", &WindowCallbacks::SetCursorPosCallback)
		.addStaticMember("setCursorEnterCallback", &WindowCallbacks::SetCursorEnterCallback)
		.addStaticMember("setScrollCallback", &WindowCallbacks::SetScrollCallback)
		.addStaticMember("setDropCallback", &WindowCallbacks::SetDropCallback)
		);
		


	luaglfw["cursor"].setClass(ClassMetatable<MetaPointerWrapper<GLFWcursor> >()
		.addStaticMember("new", &glfwCreateCursor)
		.addStaticMember("new", &glfwCreateStandardCursor)
		.addStaticMember("__gc", &glfwDestroyCursor)
		);
	return 1;
}
