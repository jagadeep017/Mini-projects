OBJ=$(patsubst %.c,%.o,$(wildcard *.c))
run : a.out
	./a.out
a.out : $(OBJ)
	gcc -o $@ $^
clean :
	rm *.o *.out
