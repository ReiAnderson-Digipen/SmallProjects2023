CCFLAGS=-g -Werror -Wall -Wextra -Wconversion -std=c++14 -pedantic -Wold-style-cast
SRC=driver.cpp ObjectAllocator.cpp PRNG.cpp
OBJS:= $(SRC:%=%.o)
target=gcc0.exe

${target}: $(OBJS)
	@echo "[Linking] $@"
	@clang++ $(OBJS) -o $@
%.cpp.o : %.cpp
	@echo "[Compiling] $@"
	@clang++ $(CCFLAGS) -c $< -o $@
clean:
	@rm $(OBJS)
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21:
	@echo "Running test $@"
	@./$(target) $@ > myout_$@.txt
	diff myout_$@.txt expected_out$@
