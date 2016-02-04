all: gen_constructor gen_push_tuple gen_ref_tuple

gen_push_tuple: push_tuple_generator.py
	python push_tuple_generator.py > ../include/kaguya/gen/push_tuple.inl

gen_ref_tuple: ref_tuple_generator.py
	python ref_tuple_generator.py > ../include/kaguya/gen/ref_tuple.inl

clean:

.PHONY: all clean
