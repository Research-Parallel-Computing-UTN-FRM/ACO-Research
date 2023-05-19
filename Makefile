CC = gcc
SRC = src

SOURCES = main.c system.c initialize.c util.c
OBJECTS = $(SOURCES:%.c=%.o)

TARGET = target
NAME = implementation
EXE = $(TARGET)/$(NAME)

MAKE_DIR_P = mkdir -p 


build: create_target_dir compile link clean_objects

compile: $(SOURCES)
	$(CC) -c  $(SOURCES)

link: $(OBJECTS)
	$(CC) -o $(EXE) $(OBJECTS)

create_target_dir: 
	$(MAKE_DIR_P) $(TARGET)

main.o: main.c main.h
system.o: system.c system.h
initialize.o: initialize.c initialize.h
util.o: util.c util.h


%.o: %.c

clean_objects: $(OBJECTS)
	rm -f $(OBJECTS)

.PHONY: clean
clean: clean_objects
	rm -rf $(TARGET)    # Remove the executable file
	

