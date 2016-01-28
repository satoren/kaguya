
def generate_args(out,arg_num,name):
	if arg_num > 0:
		out.write(name + '1')
		for i in range (1,arg_num):
			out.write(','+name + str(i+1))

def generate_template(out,arg_num):
	if arg_num > 0:
		out.write('template<')
		generate_args(out,arg_num,'typename T')
		out.write('>\n')

def generate_fun_args(out,arg_num):
	for i in range (arg_num):
		if i != 0:
			out.write(',')
		out.write('T' + str(i+1)+' t' + str(i+1))

def generate(out,arg_num):
	generate_template(out,arg_num)
	out.write('inline FunctionResults LuaRef::operator()(')
	generate_fun_args(out,arg_num)
	out.write(')\n')
	out.write('{\n')
	out.write('  util::ScopedSavedStack save(state_);\n')
	out.write('  push(state_);\n')
	out.write('  int typ = lua_type(state_, -1);\n')
	out.write('  lua_State* exec_state = state_;\n')
	out.write('  if(typ == TYPE_FUNCTION){}\n')
	out.write('  else if(typ == TYPE_THREAD){exec_state = get<lua_State*>();}\n')
	out.write('  else{except::typeMismatchError(exec_state, " is not function or thread");return FunctionResults();}\n')
	out.write('  int argstart = lua_gettop(exec_state);\n')
	for i in range (1,arg_num+1):
		out.write('  lua_type_traits<T' +  str(i) + '>::push(exec_state,standard::forward<T' +  str(i) + '>(t' + str(i) + '));\n')
	out.write('  int argnum = lua_gettop(exec_state) - argstart;\n')
	out.write('  return FunctionResults(funInvoke(exec_state,argnum));\n')
	out.write('}\n')

if __name__ == "__main__":
	import sys
	sys.stdout.write('//generated header by ' + __file__ + "\n")
	for i in range(10):
		generate(sys.stdout,i)
