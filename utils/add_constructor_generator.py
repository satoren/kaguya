
def generate_args(out,arg_num,name):
	if arg_num > 0:
		out.write(name + "1")
		for i in range (1,arg_num):
			out.write(","+name + str(i+1))

def generate_template(out,arg_num):
	if arg_num > 0:
		out.write("template<")
		generate_args(out,arg_num,"typename T")
		out.write(">\n")

def generate_fun_args(out,arg_num):
	for i in range (arg_num):
		if i != 0:
			out.write(",")
		out.write("types::type_tag<T" + str(i+1)+">* t" + str(i+1) + "= 0")

def generate_add_constructor_function(out,arg_num):
	generate_template(out,arg_num)
	out.write("ClassMetatable& addConstructor" + "(")
	generate_fun_args(out,arg_num)
	out.write(")\n")
	out.write("{\n")

	out.write("  functor_type fun(new kaguya::nativefunction::constructor_caller" + str(arg_num)+"<class_type")
	if arg_num > 0:
		out.write(",")
		generate_args(out,arg_num,"T")
	out.write(">());\n")
	out.write("  function_map_[\"new\"].push_back(fun);\n")
	out.write("return *this;\n")

	out.write("}\n")


if __name__ == "__main__":
	import sys
	sys.stdout.write('//generated header by ' + __file__ + "\n")
	for i in range(10):
		generate_add_constructor_function(sys.stdout,i)
