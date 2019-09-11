LIB_INP = bld/string.o
LIB_DEP = -largent-middleware
LIB_OUT = bld/libargent-abstractions.so
LIB_OPT = -shared -g -O2

$(LIB_OUT): $(LIB_INP)
	gcc $(LIB_OPT) $(LIB_INP) $(LIB_DEP) -o $@


bld/%.o: src/%.c
	mkdir -p bld
	gcc -c -fPIC $(LIB_OPT) $< -o $@


clean:
	rm -rfv bld

