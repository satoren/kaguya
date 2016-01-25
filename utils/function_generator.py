from io import StringIO

def args(out,arg_num,name,argoffset=0):
	if arg_num > 0:
		for i in range (argoffset,arg_num):
			if i > argoffset:
				out.write(',')
			out.write(name + str(i+1))

def get_call(out,arg_num,offset=0):
	for i in range (offset + 1,arg_num + 1):
		out.write('    typename lua_type_traits<T'+str(i) +'>::get_type t' +str(i) +' = lua_type_traits<T'+ str(i) + '>::get(state,' + str(i) + ');\n')

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
			out.write('!lua_type_traits<T'+ str(i) + '>::strictCheckType(state,' + str(i) + ')')
		out.write('){return false;}\n')
		out.write('  }else{\n')
		out.write('      if(')
		for i in range (offset+1,arg_num+1):
			if i>offset+1:
				out.write('||')
			out.write('!lua_type_traits<T'+ str(i) + '>::checkType(state,' + str(i) + ')')
		out.write('){return false;}\n')
		out.write('  }\n')
	out.write('    return true;\n')
	out.write('  }\n')

def arg_typenames(arg_num):
	result ='  virtual std::string argumentTypeNames(){\n'
	result +='    std::string result;\n'
	for i in range (1,arg_num+1):
		result +='    result+='
		if i>1:
			result +='std::string(",")+'
		result +='typeid(T'+ str(i)+').name();\n'
	result +='    return result;\n'
	result +='  }\n'
	return result


def generate(out,classname,basename,template_arg,func_type,checktype_fn,invoke_code,typename_fn):
	if len(template_arg) > 0:
		out.write('template'+template_arg + '\n')
	out.write('struct ' + classname + ':' + basename)
	out.write('{\n')
	#typedef  function and add member
	out.write('  typedef ' + func_type.replace('#FUNC_NAME#','func_type') + ';\n')
	out.write('  func_type func_;\n')
	#constructor
	out.write('  ' + classname + '(func_type fun):func_(fun){}\n')

	out.write(checktype_fn)

	out.write('  virtual int invoke(lua_State *state)\n')
	out.write('  {\n')
	out.write(invoke_code)
	out.write('  }\n')

	out.write(typename_fn)

	out.write('};\n')

	#typedef factory method
	if len(template_arg) > 0:
		out.write('template'+template_arg + '\n')
	out.write('inline base_ptr_ create(' + func_type.replace('#FUNC_NAME#','fun')+')\n')
	out.write('{\n')

	out.write('  typedef ' + classname + template_arg.replace('typename ','') + ' InvokerType;\n')
	out.write('  return base_ptr_(new InvokerType(fun));\n')
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
	invoke_code += '    return lua_type_traits<Ret>::push(state,standard::forward<Ret>(r));\n'

	generate(out,classname,basename,template_str(arg_num),function_type,checkType_str(arg_num),invoke_code,arg_typenames(arg_num))


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

	generate(out,classname,basename,void_template_str(arg_num),function_type,checkType_str(arg_num),invoke_code,arg_typenames(arg_num))

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
	invoke_code += '    return lua_type_traits<Ret>::push(state,standard::forward<Ret>(r));\n'

	generate(out,classname,basename,template_str(arg_num),function_type,checkType_str(arg_num),invoke_code,arg_typenames(arg_num))


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

	generate(out,classname,basename,void_template_str(arg_num),function_type,checkType_str(arg_num),invoke_code,arg_typenames(arg_num))

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

	invoke_code +='  '+funattr+' T1* ptr = lua_type_traits<'+ funattr +' T1*>::get(state, 1);\n'
	invoke_code +='  if(!ptr){return 0;}\n'
	invoke_code +='    Ret r = (ptr->*func_)('
	if with_state:
		invoke_code += 'state'+ (',' if arg_num>0 else '')
	invoke_code +=args_str(arg_num+1,'t',1)
	invoke_code +=');\n'
	invoke_code +='    return lua_type_traits<Ret>::push(state,standard::forward<Ret>(r));\n'

	checktype_fun = checkType_str(arg_num+1,1,'if(lua_type_traits<'+ funattr +' T1*>::get(state, 1) == 0){return false;}\n')

	generate(out,classname,basename,template_str(arg_num+ 1),function_type,checktype_fun,invoke_code,arg_typenames(arg_num))

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

	invoke_code +='  '+funattr+' T1* ptr = lua_type_traits<' + funattr + ' T1*>::get(state, 1);\n'

	invoke_code +='  if(!ptr){return 0;}\n'
	invoke_code +='    (ptr->*func_)('
	if with_state:
		invoke_code += 'state'+ (',' if arg_num>0 else '')
	invoke_code +=args_str(arg_num+1,'t',1)
	invoke_code +=');\n'
	invoke_code +='    return 0;\n'

	checktype_fun = checkType_str(arg_num+1,1,'if(lua_type_traits<'+ funattr +' T1*>::get(state, 1) == 0){return false;}\n')

	generate(out,classname,basename,void_template_str(arg_num+ 1),function_type,checktype_fun,invoke_code,arg_typenames(arg_num))


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
	out.write('  typedef ObjectWrapper<CLASS> wrapper_type;\n')
	out.write('  void *storage = lua_newuserdata(state, sizeof(wrapper_type));\n')
	out.write('    new(storage) wrapper_type(')
	if arg_num > 0:
		args(out,arg_num,'t')
	out.write(');\n')
	out.write('    class_userdata::setmetatable<CLASS>(state);\n')
	out.write('    return 1;\n')
	out.write('  }\n')
	out.write(arg_typenames(arg_num))
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
