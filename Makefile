# Based on Makefile provided by CPeck
#
# # CC = gcc
# # CFLAGS = -Wall -O0 -fopenmp
# # 

CC = g++
#CFLAGS = -framework OpenGL -framework GLUT
CFLAGS = -lm -lGL -lGLU -lglut

ifeq ($(wildcard /etc/bccd-revision),)      # not the BCCD
  TARGETS =  firework 
  else                                        # the BCCD
 TARGETS =  firework 
endif

all: $(TARGETS)

firework : firework.cpp
	$(CC) $^ -o $@ $(CFLAGS) 

clean :
	rm -f  firework

