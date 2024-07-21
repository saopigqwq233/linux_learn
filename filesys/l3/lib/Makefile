static-lib=libmymath.a

$(static-lib):Add.o Div.o Mul.o Sub.o
	ar -rc $@ $^

%.o:%.c
	gcc -c $<

.PHONY:output
output:
	mkdir -p mymath_lib/include
	mkdir -p mymath_lib/lib 
	cp -f *.h mymath_lib/include
	cp -f *.a mymath_lib/lib

.PHONY:clean
clean:
	rm -rf *.o mymath_lib
