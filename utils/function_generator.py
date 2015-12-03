from io import StringIO

def args(out,arg_num,name,argoffset=0):
	if arg_num > 0:
		for i in range (argoffset,arg_num):
			if i > argoffset:
				out.write(',')
			out.write(name + str(i+1))

def get_call(out,arg_num,offset=0):
	for i in range (offset + 1,arg_num + 1):
		out.write('    T'+str(i) +' t' +str(i) +' = types::get(state,' + str(i) + ',types::typetag<T'+ str(i) + '>());\n')


def strictCheckType(out,arg_num,offset=0,customcheck=""):
	out.write('  virtual bool checktype(lua_State *state,bool strictcheck){\n')
	out.write('    if(lua_gettop(state) != ' + str(arg_num) + '){return false;}\n')
	
	out.write(customcheck)
	if arg_num> offset:
		out.write('    if(strictcheck){\n')
		out.write('      if(')
		for i in range (offset+1,arg_num+1):
			if i>offset+1:
				out.write('||')
			out.write('!types::strictCheckType(state,' + str(i) + ',types::typetag<T'+ str(i) + '>())')
		out.write('){return false;}\n')
		out.write('  }else{\n')	
		out.write('      if(')
		for i in range (offset+1,arg_num+1):
			if i>offset+1:
				out.write('||')
			out.write('!types::checkType(state,' + str(i) + ',types::typetag<T'+ str(i) + '>())')
		out.write('){return false;}\n')
		out.write('  }\n')
	out.write('    return true;\n')
	out.write('  }\n')


def generate(out,classname,basename,template_arg,func_type,checktype_fun,invoke_code):
	if len(template_arg) > 0:
		out.write('template'+template_arg + '\n')
	out.write('struct ' + classname + ':' + basename)
	out.write('{\n')
	#typedef  function and add member
	out.write('  typedef ' + func_type.replace('#FUNC_NAME#','func_type') + ';\n')
	out.write('  func_type func_;\n')
	#constructor
	out.write('  ' + classname + '(func_type fun):func_(fun){}\n')
	
	out.write(checktype_fun)
	
	out.write('  virtual int invoke(lua_State *state)\n')
	out.write('  {\n')
	out.write(invoke_code)
	out.write('  }\n')
	out.write('};\n')

	#typedef factory method
	if len(template_arg) > 0:
		out.write('template'+template_arg + '\n')
	out.write('FunctorType create(' + func_type.replace('#FUNC_NAME#','fun')+')\n')
	out.write('{\n')
	
	out.write('  typedef ' + classname + template_arg.replace('typename ','') + ' InvokerType;\n')	
	out.write('  return FunctorType(new InvokerType(fun));\n')
	out.write('}\n\n\n')


def args_str(arg_num,name,argoffset=0):
	with StringIO() as output:
		args(output,arg_num,name,argoffset)
		return output.getvalue()

def template_str(arg_num):
	result = '<typename Ret'
	if arg_num > 0:
		result+=','
		result+=args_str(arg_num,'typename T')
	result+='>\n'
	return result
	
def void_template_str(arg_num):
	result = ''
	if arg_num > 0:
		result = '<'
		result+=args_str(arg_num,'typename T')
		result+='>'
	return result
	
def checkType_str(arg_num,offset=0,customcheck=""):
	with StringIO() as output:
		strictCheckType(output,arg_num,offset,customcheck)
		return output.getvalue()

def get_call_str(arg_num,offset=0):
	with StringIO() as output:
		get_call(output,arg_num,offset)
		return output.getvalue()

def standard_function(out,arg_num,with_state = False):
	basename = 'BaseInvoker'
	classname = 'StdFunInvoker' + str(arg_num)
	if with_state:
		classname = classname + "_with_state"
		
	function_type = 'standard::function<Ret('
	if with_state:
		function_type +='lua_State*'+ (',' if arg_num>0 else '')
	function_type +=args_str(arg_num,'T');
	function_type +=')> #FUNC_NAME#'

	invoke_code = get_call_str(arg_num)
	invoke_code += '    Ret r = func_('
	if with_state:
		invoke_code += 'state'+ (',' if arg_num>0 else '')			
	invoke_code += args_str(arg_num,'t')
	invoke_code += ');\n'
	invoke_code += '    return types::push(state,standard::forward<Ret>(r));\n'
	
	generate(out,classname,basename,template_str(arg_num),function_type,checkType_str(arg_num),invoke_code)


def void_standard_function(out,arg_num,with_state=False):
	basename = 'BaseInvoker'
	classname = 'VoidStdFunInvoker' + str(arg_num)

	if with_state:
		classname = classname + "_with_state"
		
	function_type = 'standard::function<void('
	if with_state:
		function_type +='lua_State*'+ (',' if arg_num>0 else '')
	function_type +=args_str(arg_num,'T');
	function_type +=')> #FUNC_NAME#'

	invoke_code = get_call_str(arg_num)
	invoke_code += '    func_('
	if with_state:
		invoke_code += 'state'+ (',' if arg_num>0 else '')			
	invoke_code += args_str(arg_num,'t')
	invoke_code += ');\n'
	invoke_code += '    return 0;\n'
	
	generate(out,classname,basename,void_template_str(arg_num),function_type,checkType_str(arg_num),invoke_code)
	
def tepmlate_function(out,arg_num,with_state=False):
	basename = 'BaseInvoker'
	classname = 'FunInvoker' + str(arg_num)

	if with_state:
		classname = classname + "_with_state"
	
	function_type = 'Ret (*#FUNC_NAME#)('
	if with_state:
		function_type +='lua_State*'+ (',' if arg_num>0 else '')
	function_type +=args_str(arg_num,'T');
	function_type +=')'

	invoke_code = get_call_str(arg_num)
	invoke_code += '    Ret r = func_('
	if with_state:
		invoke_code += 'state'+ (',' if arg_num>0 else '')			
	invoke_code += args_str(arg_num,'t')
	invoke_code += ');\n'
	invoke_code += '    return types::push(state,standard::forward<Ret>(r));\n'
	
	generate(out,classname,basename,template_str(arg_num),function_type,checkType_str(arg_num),invoke_code)


def void_tepmlate_function(out,arg_num,with_state=False):
	basename = 'BaseInvoker'
	classname = 'VoidFunInvoker' + str(arg_num)

	if with_state:
		classname = classname + "_with_state"
	
	function_type = 'void (*#FUNC_NAME#)('
	if with_state:
		function_type +='lua_State*'+ (',' if arg_num>0 else '')
	function_type +=args_str(arg_num,'T');
	function_type +=')'

	invoke_code = get_call_str(arg_num)
	invoke_code += '    func_('
	if with_state:
		invoke_code += 'state'+ (',' if arg_num>0 else '')			
	invoke_code += args_str(arg_num,'t')
	invoke_code += ');\n'
	invoke_code += '    return 0;\n'
	
	generate(out,classname,basename,void_template_str(arg_num),function_type,checkType_str(arg_num),invoke_code)

def tepmlate_mem_function(out,arg_num,funattr,with_state=False):
	basename = 'BaseInvoker'
	classname = funattr+'MemFunInvoker' + str(arg_num)

	if with_state:
		classname = classname + "_with_state"
	
	function_type = 'Ret (T1::*#FUNC_NAME#)('
	if with_state:
		function_type +='lua_State*'+ (',' if arg_num>0 else '')
	function_type += args_str(arg_num+1,'T',1);
	function_type += ') ' + funattr

	invoke_code = get_call_str(arg_num+1,1)
	
	invoke_code +='  T1* ptr = types::get(state, 1, types::typetag<T1*>());\n'
	invoke_code +='  if(!ptr){return 0;}\n'	
	invoke_code +='    Ret r = (ptr->*func_)('
	if with_state:
		invoke_code += 'state'+ (',' if arg_num>0 else '')	
	invoke_code +=args_str(arg_num+1,'t',1)
	invoke_code +=');\n'
	invoke_code +='    return types::push(state,standard::forward<Ret>(r));\n'
	
	checktype_fun = checkType_str(arg_num+1,1,'if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}\n')
	
	generate(out,classname,basename,template_str(arg_num+ 1),function_type,checktype_fun,invoke_code)

def void_tepmlate_mem_function(out,arg_num,funattr,with_state=False):
	basename = 'BaseInvoker'
	classname = funattr+'VoidMemFunInvoker' + str(arg_num)

	if with_state:
		classname = classname + "_with_state"
	
	function_type = 'void (T1::*#FUNC_NAME#)('
	if with_state:
		function_type +='lua_State*'+ (',' if arg_num>0 else '')
	function_type += args_str(arg_num+1,'T',1);
	function_type += ') ' + funattr

	invoke_code = get_call_str(arg_num+1,1)
	
	invoke_code +='  T1* ptr = types::get(state, 1, types::typetag<T1*>());\n'
	
	invoke_code +='  if(!ptr){return 0;}\n'
	invoke_code +='    (ptr->*func_)('
	if with_state:
		invoke_code += 'state'+ (',' if arg_num>0 else '')	
	invoke_code +=args_str(arg_num+1,'t',1)
	invoke_code +=');\n'
	invoke_code +='    return 0;\n'
	
	checktype_fun = checkType_str(arg_num+1,1,'if(types::get(state, 1, types::typetag<T1*>()) == 0){return false;}\n')

	generate(out,classname,basename,void_template_str(arg_num+ 1),function_type,checktype_fun,invoke_code)
	

def constructor_function(out,arg_num):
	basename = 'BaseInvoker'
	classname = 'ConstructorInvoker' + str(arg_num)

	out.write('template<typename CLASS')
	if arg_num > 0:
		out.write(',')
		args(out,arg_num,'typename T')
	out.write('>\n')
	out.write('struct ' + classname + ':' + basename)
	out.write('{\n')
	out.write('  ' + classname + '(){}\n')
	strictCheckType(out,arg_num)
	out.write('  virtual int invoke(lua_State *state)\n')
	out.write('  {\n')
	get_call(out,arg_num)
	out.write('  void *storage = lua_newuserdata(state, sizeof(CLASS));\n')
	out.write('    types::constructor<CLASS>(storage')
	if arg_num > 0:
		out.write(',')
		args(out,arg_num,'t')
	out.write(');\n')
	out.write('    luaL_setmetatable(state, types::metatableName<CLASS>().c_str());\n')
	out.write('    return 1;\n')
	out.write('  }\n')
	out.write('};\n')


if __name__ == '__main__':
	import sys
	sys.stdout.write('//generated header by ' + __file__ + "\n")
	for i in range(10):
		standard_function(sys.stdout,i)
#		standard_function(sys.stdout,i,True)
	for i in range(10):
		void_standard_function(sys.stdout,i)
#		void_standard_function(sys.stdout,i,True)
	for i in range(10):
		tepmlate_function(sys.stdout,i)
#		tepmlate_function(sys.stdout,i,True)
	for i in range(10):
		void_tepmlate_function(sys.stdout,i)
#		void_tepmlate_function(sys.stdout,i,True)
	for i in range(10):
		tepmlate_mem_function(sys.stdout,i,"")
		tepmlate_mem_function(sys.stdout,i,"const")
#		tepmlate_mem_function(sys.stdout,i,"",True)
#		tepmlate_mem_function(sys.stdout,i,"const",True)
	for i in range(10):
		void_tepmlate_mem_function(sys.stdout,i,"")
		void_tepmlate_mem_function(sys.stdout,i,"const")
#		void_tepmlate_mem_function(sys.stdout,i,"",True)
#		void_tepmlate_mem_function(sys.stdout,i,"const",True)
	for i in range(10):
		constructor_function(sys.stdout,i)
