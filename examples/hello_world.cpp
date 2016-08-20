#include <iostream>
#include <kaguya/kaguya.hpp>

void hello_cpp_world()
{
	std::cout << "hello cpp world" << std::endl;
}

int main()
{
  using namespace kaguya;
  State state;
  state.dostring("print('hello lua world')");

  state["hello_cpp_world"] = &hello_cpp_world;//bind function
  state.dostring("hello_cpp_world()");

#ifdef WIN32
  system("PAUSE");
#endif
}
