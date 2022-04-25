
all:
	mpic++ proj.cpp

run:
	mpirun -oversubscribe -np 8 a.out "ABCDEFG"

zip:
	zip xkanto14.zip proj.cpp test.sh xkanto14.pdf