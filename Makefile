
## simple MAKEFILE to build and test multiple-file-executables

## Sample call: make

# print built-in make rules by saying:
# make -p > make.db

# linking libraries looked up by pkg-config
LDLIBS = 

.PHONY: progs tests clean

tests: progs
	$(info Running tests...)
	./main | c++filt -t
	
	

progs:	main	

main: src/main.cpp
	g++ -o $@ $^ $(LDLIBS) 


clean:
	rm -f *.exe *.nc
