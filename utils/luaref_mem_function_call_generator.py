
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
	out.write('fun_evaluator operator()(')
	generate_fun_args(out,arg_num)
	out.write(')\n')
	out.write('{\n')
	out.write('  return fun_(self_')
	for i in range (arg_num):
		out.write('  ,t' + str(i+1))
	out.write(');\n')
	out.write('}\n')

if __name__ == "__main__":
	import sys
	sys.stdout.write('//generated header by ' + __file__ + "\n")
	for i in range(10):
		generate(sys.stdout,i)
