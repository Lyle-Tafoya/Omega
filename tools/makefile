# The files in this directory are only useful to people who want to do
# development of omega - decrypt will take any of the encrypted files in
# ../omegalib and turn it into plain ascii, crypt will change the plain
# ascii file back into the encrypted format

CC = gcc

all: crypt decrypt

crypt: crypt.c
	$(CC) $(CFLAGS) crypt.c -o crypt

decrypt: decrypt.c
	$(CC) $(CFLAGS) decrypt.c -o decrypt

clean:
	rm -f crypt decrypt
