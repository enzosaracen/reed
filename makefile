CC = gcc
CF = -Wall -Wextra -g
HF = src/raid.h
OF = raid
OBJ = src/galois.o \
      src/disk.o \
      src/reed.o \
      src/cmd.o \

%.o:	%.c $(HF)
	$(CC) -c $< -o $@ $(CF)

$(OF):	$(OBJ) $(HF)
	$(CC) $(OBJ) -o $(OF) $(CF)

.PHONY: clean

clean:
	rm -f $(OBJ) $(OF)
