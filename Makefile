
all:
	mpic++ proj.cpp

run:
	mpirun -oversubscribe -np 14 a.out "ABCDEFGH"

zip:
	zip xkanto14.zip proj.cpp test.sh xkanto14.pdf