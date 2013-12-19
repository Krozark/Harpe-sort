
#.cpp.o:
#	$(CC) $(FLAGS) -o "$@" -c "$<" 

all: calc_score.cpp
	$(CC) $(DEFINES) -fPIC -o "calc_score.o" -c "$<" 
	gcc -shared -Wl,-soname,$(TOP)/$(SHARED) -o $(TOP)/$(SHARED) "calc_score.o"


clean:
	@rm -f *.o
