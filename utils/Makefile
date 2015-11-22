all: gen_native_fun gen_add_constructor_function gen_constructor gen_push_tuple gen_evaluator_function

gen_native_fun: function_generator.py
	python function_generator.py > ../include/kaguya/gen/native_function.inl

gen_add_constructor_function: add_constructor_generator.py
	python add_constructor_generator.py > ../include/kaguya/gen/add_constructor.inl

gen_constructor: constructor_generator.py
	python constructor_generator.py > ../include/kaguya/gen/constructor.inl

gen_push_tuple: push_tuple_generator.py
	python push_tuple_generator.py > ../include/kaguya/gen/push_tuple.inl

gen_evaluator_function: evaluator_function_generator.py
	python evaluator_function_generator.py > ../include/kaguya/gen/evaluator_function.inl


clean:

.PHONY: all clean