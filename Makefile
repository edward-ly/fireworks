# Based on Makefile provided by CPeck
#
# # CC = gcc
# # CFLAGS = -Wall -O0 -fopenmp
# #

CC = g++
CFLAGS = -Wall -ggdb
#CFLAGS = -framework OpenGL -framework GLUT
LIBS = -lm -lGL -lGLU -lglut
EXPENDABLES = firework.o

ifeq ($(wildcard /etc/bccd-revision),)      # not the BCCD
  EXECUTABLES =  firework
  else                                        # the BCCD
 EXECUTABLES =  firework
endif

firework: demo.cpp firework.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

firework.o: firework.h firework.cpp
	$(CC) $(CFLAGS) -c firework.cpp

clean:
	rm -f $(EXPENDABLES)

distclean:
	rm -f $(EXPENDABLES) $(EXECUTABLES)

all:
	rm -f $(EXPENDABLES) $(EXECUTABLES)
	@make $(EXECUTABLES)
