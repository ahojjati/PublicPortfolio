all: mp1_test mp1_utest

mp1_test: mp1_testcases.o
	gcc -g -o mp1_test mp1_testcases.o

mp1_utest: mp1_userspace.o mp1_testcases_u.o mp1_u.o
	gcc -g -o mp1_utest mp1_userspace.o mp1_testcases_u.o mp1_u.o

mp1_u.o: mp1.S mp1.h
#	gcc -g -Wall -c -D_USERSPACE -D_ASM -o $@ $<
	as -g -L mp1.S -o mp1_u.o

mp1_testcases.o: mp1_testcases.c mp1.h
	gcc -g -Wall -c -o $@ $<

mp1_testcases_u.o: mp1_testcases.c mp1.h
	gcc -g -Wall -c -D_USERSPACE -o $@ $<

mp1_userspace.o: mp1_userspace.c mp1.h
	gcc -g -Wall -c -D_USERSPACE -o $@ $<

%.o: %.c
	gcc -g -Wall -c -o $@ $<

clean:
	rm -f *.o *~ mp1_test mp1_utest
