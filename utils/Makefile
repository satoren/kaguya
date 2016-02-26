all: gen_constructor gen_push_tuple gen_ref_tuple

gen_push_tuple: push_tuple_generator.py
	python push_tuple_generator.py > ../include/kaguya/gen/push_tuple.inl

gen_ref_tuple: ref_tuple_generator.py
	python ref_tuple_generator.py > ../include/kaguya/gen/ref_tuple.inl
    
one_header_file: generate_one_header.py
	python generate_one_header.py > ../kaguya.hpp

clean:

.PHONY: all clean
