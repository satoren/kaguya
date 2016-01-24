
def generate_args(out,arg_num,name):
	if arg_num > 0:
		out.write(name + "1")
		for i in range (1,arg_num):
			out.write(","+name + str(i+1))

def generate_template(out,arg_num):
	out.write("template<")
	if arg_num > 0:
		generate_args(out,arg_num,"typename T")
	out.write(">\n")

def generate_fun_args(out,arg_num):
	for i in range (arg_num):
		if i != 0:
			out.write(",")
		out.write("T" + str(i+1)+"* t" + str(i+1) + "= 0")

def generate_push_tuple(out,arg_num):
	generate_template(out,arg_num)
	out.write("struct lua_type_traits<standard::tuple<")
	generate_args(out,arg_num,"T")
	out.write("> >\n{\n")
	out.write("  static int push(lua_State* l, const standard::tuple<")
	generate_args(out,arg_num,"T")
	out.write(">& v)\n")
	out.write("  {\n")
	out.write("    int count =0;\n")
	for i in range (arg_num):
		out.write("    count += lua_type_traits<T"+str(i+1)+">::push(l,standard::get<"+str(i)+">(v));\n")
	out.write("    return count;\n")
	out.write("  }\n")
	out.write("};\n")

if __name__ == "__main__":
	import sys
	sys.stdout.write('//generated header by ' + __file__ + "\n")
	for i in range(10):
		generate_push_tuple(sys.stdout,i)
