# Based on Makefile provided by CPeck
#
# # CC = gcc
# # CFLAGS = -Wall -O0 -fopenmp
# #

CC = g++
CFLAGS = -Wall -ggdb
#CFLAGS = -framework OpenGL -framework GLUT
LIBS = -lm -lGL -lGLU -lglut

ifeq ($(wildcard /etc/bccd-revision),)      # not the BCCD
  TARGETS =  firework
  else                                        # the BCCD
 TARGETS =  firework
endif

all: $(TARGETS)

firework : firework.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean :
	rm -f firework
