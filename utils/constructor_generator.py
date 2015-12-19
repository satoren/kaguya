def generate_args(out,arg_num,name):
	if arg_num > 0:
		out.write(name + "1")
		for i in range (1,arg_num):
			out.write(","+name + str(i+1))

def generate_fun_args(out,arg_num):
	for i in range (arg_num):
		out.write(","+"T" + str(i+1)+" t" + str(i+1))

def generate_template(out,arg_num):
	out.write("template<typename CLASS")
	if arg_num > 0:
		out.write(",")
		generate_args(out,arg_num,"typename T")
	out.write(">\n")


def generate_constructor_function(out,arg_num):
	generate_template(out,arg_num)
	out.write("CLASS* constructor(void* pointer")
	generate_fun_args(out,arg_num)
	out.write(")\n")
	out.write("{\n")
	out.write("  CLASS* ptr = new(pointer) CLASS(")
	generate_args(out,arg_num,"t")

	out.write(");\n")
	out.write("  return ptr;\n")
	out.write("}\n")

if __name__ == "__main__":
	import sys
	sys.stdout.write('//generated header by ' + __file__ + "\n")
	for i in range(10):
		generate_constructor_function(sys.stdout,i)
