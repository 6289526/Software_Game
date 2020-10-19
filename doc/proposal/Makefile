
CC = gcc
SRCS = main.c func1.c
HEADERS = func1.h
OBJS = main.o func1.o
LIB = -lm
TEX = platex
PDF = dvipdfmx
.SUFFIXES:
.SUFFIXES: .tex .dvi .pdf
prog: $(OBJS)
	$(CC) –o $@ $(OBJS) $(LIB)
.c.o:
	$(CC) –c $<
.tex.pdf:
	$(TEX) $<
	$(PDF) $*.dvi
clean:
	rm –rf *.aux *.dvi
