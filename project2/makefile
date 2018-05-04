CC=gcc
CFLAGS1= -Wall -pedantic
CFLAGS2 = -pthread -Wall -pedantic

FILES1 = fifoclient.c fifoclient.h
OUT1 = fifoclient
FILES2 =  fifoserver.c fifoserver.h
OUT2 = fifoserver

build:
	$(CC) $(CFLAGS1) $(FILES1) -o $(OUT1)
	$(CC) $(CFLAGS2) $(FILES2) -o $(OUT2)

clean: 
	-rm -f $(OUT1)
	-rm -f $(OUT2)

uninstall:
	-rm -f $(OUT1)
	-rm -f $(FILES1)
	-rm -f $(OUT2)
	-rm -f $(FILES2)


