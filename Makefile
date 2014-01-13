export CC = g++
INCPATH = -I$(CURDIR)/include 
LIBS = -lmgf -ldl
#-lpthread 
#-lmysqlcppconn 
export DEFINES = -DCOLORS 
export FLAGS = -o3 -std=c++0x $(INCPATH) $(LIBS) $(DEFINES)
export TOP = $(CURDIR)
export OBJ_DIR = $(TOP)/obj

export BASE= harpe-sort
export STATIC=$(BASE).a
export SHARED=$(BASE).so

SRC = $(wildcard *.c*)
OBJ = $(SRC:.cpp=.o) $(SRC:*.cpp=.o)


CLEANDIRS = $(SUBDIRS:%=clean-%)

.PHONY: subdirs $(SUBDIRS)
.PHONY: subdirs $(CLEANDIRS)


all: calc_score_func

calc_score_func:
	$(MAKE) -C sort

     
clean: $(CLEANDIRS)

$(CLEANDIRS): 
	$(MAKE) -C $(@:clean-%=%) clean
	@rm -f *.o
	@rm -f *.so
	@rm -f $(EXEC)

