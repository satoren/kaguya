
def generate_args(out,arg_num,name):
	for i in range (0,arg_num):
		out.write(","+name + str(i+1))

def generate_template(out,arg_num):
	out.write("template<typename CLASS")
	generate_args(out,arg_num,"typename T")
	out.write(">\n")

def generate_fun_args(out,arg_num):
	for i in range (arg_num):
		if i != 0:
			out.write(",")
		out.write("types::typetag<T" + str(i+1)+">* t" + str(i+1) + "= 0")

def generate_add_constructor_function(out,arg_num):
	generate_template(out,arg_num)
	out.write('static FunctorType ConstructorInvoker(')
	generate_fun_args(out,arg_num)
	out.write(")\n")
	out.write("{\n")

	out.write('  return FunctorType(base_ptr_(new ConstructorInvoker'+ str(arg_num) +'<CLASS')
	generate_args(out,arg_num,"T")
	out.write('>()));')
	out.write("}\n")


if __name__ == "__main__":
	import sys
	sys.stdout.write('//generated header by ' + __file__ + "\n")
	for i in range(10):
		generate_add_constructor_function(sys.stdout,i)
