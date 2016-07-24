#include <iostream>
#include <kaguya/kaguya.hpp>

#define KAGUYA_DYNAMIC_LIB
#include "kaguya/another_binding_api.hpp"

void hello()
{
	std::cout << "hello cpp world" << std::endl;
}


KAGUYA_BINDINGS(hello_lua_module_experimental)
{
	using namespace kaguya;
	using kaguya::function;

	function("hello", &hello);
}
