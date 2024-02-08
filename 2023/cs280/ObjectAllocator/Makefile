CCFLAGS=-g -Werror -Wall -Wextra -Wconversion -std=c++14 -pedantic -Wold-style-cast
SRC=driver.cpp ObjectAllocator.cpp PRNG.cpp
#Basically takes all the src files, and appends a .o to the end of them ex driver.cpp = driver.cpp.o
OBJS:= $(SRC:%=%.o)
target=gcc0.exe



# Compiles whatever executable is in the target variable
# : $(OBJS) says that this depends on all the .o files to be compiled
${target}: $(OBJS)
# $@ means the name of the target
	@echo "[Linking] $@"
	@clang++ $(OBJS) -o $@

# This is called a pattern, it tells make how to turn any .cpp file into a .cpp.o file
# it only compiles .cpp files THAT HAVE BEEN MODIFIED (aka, saves compile time)
%.cpp.o : %.cpp
# $@ means name of the target, aka what we are compiling
	@echo "[Compiling] $@"
# compiles with the flags provided, along with the -c flag which says to not link
# $< is the first dependency name, in this case, whatever .cpp file we are compiling
	@clang++ $(CCFLAGS) -c $< -o $@

clean:
	@rm $(OBJS)
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21:
	@echo "Running test $@"
	@./$(target) $@ > myout_$@.txt
	diff myout_$@.txt expected_out$@.txt
