#include "kaguya/kaguya.hpp"

	class SetGet
	{
	public:
		SetGet() :_i(0.0) {}
		void set(double i)
		{
			_i = i;
		}
		double get()const
		{
			return _i;
		}
	private:
		double _i;
	};
	
int main()
{
		kaguya::State state; state.openlibs();
		state["SetGet"].setClass(kaguya::ClassMetatable<SetGet>()
			.addConstructor()
			.addMember("set", &SetGet::set)
			.addMember("get", &SetGet::get)
			);
		state("local N = 10\n"
"local average = 0\n"
"local times = 1000000\n"
"for i = 0, N do\n"
"local getset = SetGet.new()\n"
//"script={}\n"
"local start = os.clock()\n"
"for i=1,times do\n"
//"script.n = i\n"
//"if(script.n ~= i)then\n"
"getset:set(i)\n"
"if(getset:get() ~= i)then\n"
"error('error')\n"
"end\n"
"end\n"
"local dt = os.clock()-start\n"
"average = average + dt\n"
"end\n"
"print('average time:',average/N)");
}