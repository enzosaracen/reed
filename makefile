CC = gcc
CF = -Wall -Wextra -g
HF = src/galois.h
OF = reed
OBJ = src/galois.o \
      src/reed.o

%.o:	%.c $(HF)
	$(CC) -c $< -o $@ $(CF)

$(OF):	$(OBJ) $(HF)
	$(CC) $(OBJ) -o $(OF) $(CF)

.PHONY: clean

clean:
	rm -f $(OBJ) $(OF)
