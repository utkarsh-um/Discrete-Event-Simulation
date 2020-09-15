default:qSim
obj/qsim.o:src/qsim.c
	@gcc -c src/qsim.c -I ./include -o obj/qsim.o

bin/qSim:obj/qsim.o
	@gcc -o bin/qSim obj/qsim.o -lm

clean:
	@rm ./data.txt -f
	@rm output/* -f
	@rm obj/* -f
	@rm bin/* -f

qSim:bin/qSim

.Phony : qSim clean