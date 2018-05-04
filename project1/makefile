CC=gcc
CFLAGS=-Wall -pedantic

FILES= main.c cleaning.c cleaning.h
OUT_EXE = cleaning

build: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $(OUT_EXE)

run: cleaning
	./cleaning

clean: 
	-rm -f $(OUT_EXE)

uninstall:
	-rm -f $(FILES)
	-rm -f $(OUT_EXE)

