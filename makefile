CC = gcc
CF = -Wall -Wextra -g
HF = src/rain.h
OF = rain
OBJ = src/galois.o \
      src/disk.o \
      src/rain.o \
      src/reed.o \

%.o:	%.c $(HF)
	$(CC) -c $< -o $@ $(CF)

$(OF):	$(OBJ) $(HF)
	$(CC) $(OBJ) -o $(OF) $(CF)

.PHONY: clean

clean:
	rm -f $(OBJ) $(OF)
