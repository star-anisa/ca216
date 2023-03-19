#// Anisa Hoxha, 21413586
#/* 
#I declare that this material, which I now submit for assessment,
#is entirely my own work and has not been taken from the work of others
#save and to the extent that such work 
#has been cited and acknowledged within the text of my work.
#*/

#//////////////////////////////////////////////////

CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g
SRC_DIR=../src
BIN_DIR=../bin

$(BIN_DIR)/myshell: $(SRC_DIR)/myshell.c $(SRC_DIR)/utilities.c $(SRC_DIR)/myshell.h
	$(CC) $(CFLAGS) $^ -o $@


.PHONY: clean

clean:
	rm -f $(BIN_DIR)/myshell

#Code adapted from https://makefiletutorial.com/