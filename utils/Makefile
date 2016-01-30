all: gen_native_fun gen_add_constructor_function gen_constructor gen_push_tuple gen_ref_tuple

gen_native_fun: function_generator.py
	python function_generator.py > ../include/kaguya/gen/native_function.inl

gen_add_constructor_function: add_constructor_generator.py
	python add_constructor_generator.py > ../include/kaguya/gen/add_constructor.inl

gen_constructor: constructor_generator.py
	python constructor_generator.py > ../include/kaguya/gen/constructor.inl

gen_push_tuple: push_tuple_generator.py
	python push_tuple_generator.py > ../include/kaguya/gen/push_tuple.inl

gen_ref_tuple: ref_tuple_generator.py
	python ref_tuple_generator.py > ../include/kaguya/gen/ref_tuple.inl

clean:

.PHONY: all clean
