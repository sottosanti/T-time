CC=gcc
CFLAGS=-g -Wall

t-time: %: %.c 
	$(CC) $(CFLAGS) $< -o $@ -lcurl

clean:
	$(RM) t-time