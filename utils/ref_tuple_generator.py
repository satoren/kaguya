
def gen_args(write,arg_num,format,separator=","):
	for i in range (arg_num):
		if i > 0:
			write(separator)			
		write(format.replace('#N#',str(i)))

def gen_template(write,arg_num):
	write("template<")
	gen_args(write,arg_num,"class T#N#")
	write(">\n")

def gen_ref_tuple(write,arg_num):
	gen_template(write,arg_num)
	write("struct ref_tuple"+ str(arg_num)+ "\n")
	write("{\n")
	write("  ref_tuple"+ str(arg_num)+ "(")
	gen_args(write,arg_num,"T#N#& va#N#")
	write("):")
	gen_args(write,arg_num,"v#N#(va#N#)")
	write("{}\n")
	
	gen_args(write,arg_num,"  T#N#& v#N#;","")
	
	write("\n  static const int size="+str(arg_num)+";\n")
	write("  void operator=(const Evaluator& eval)\n  {\n")
	write("    const std::vector<LuaRef>& res = eval.get_result(size);\n")
	
	gen_args(write,arg_num,"    if(#N# < res.size())v#N# = res[#N#].get<T#N#>();","\n")
	
	write("\n")
	write("  }\n")
	write("};\n\n")

def gen_tie_function(write,arg_num):
	gen_template(write,arg_num)
	write("ref_tuple"+ str(arg_num)+ "<")
	gen_args(write,arg_num,"T#N#")
	write("> tie(")
	gen_args(write,arg_num,"T#N#& va#N#")
	write(")\n{\n")
	write("  return ref_tuple"+ str(arg_num)+ "<")
	gen_args(write,arg_num,"T#N#")
	write(">(")
	gen_args(write,arg_num,"va#N#")
	write(");\n")
	write("}\n\n")

if __name__ == "__main__":
	import sys
	sys.stdout.write('//generated header by ' + __file__ + "\n")
	for i in range(1,10):
		gen_ref_tuple(sys.stdout.write,i)
	for i in range(1,10):
		gen_tie_function(sys.stdout.write,i)
