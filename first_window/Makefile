cc=gcc
flags=-W -Wall -Wextra -pedantic -g
libs=$(shell sdl2-config --libs)

first_window: first_window.c
	$(cc) $(flags) -o first_window first_window.c $(libs) 
