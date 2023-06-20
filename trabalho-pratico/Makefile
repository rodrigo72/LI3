CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors -O3 -g $(shell pkg-config --cflags glib-2.0)
LIBS = -lm -lncurses $(shell pkg-config --libs glib-2.0) -I./includes 

ARGS = ./csv/ ./input/input.txt

# all .c files in the src/ directory
PRINCIPAL = programa-principal
TESTES = programa-testes

SRC_DIR = src
OBJ_DIR = obj

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# all .o files in the obj/ directory
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# .o file to ignore when linking
IGNORE_PRINCIPAL = $(OBJ_DIR)/$(PRINCIPAL).o
IGNORE_TESTES = $(OBJ_DIR)/$(TESTES).o

# target to link all .o files except the ignored .o file
$(PRINCIPAL): $(filter-out $(IGNORE_TESTES), $(OBJ_FILES))
	$(CC) $(CFLAGS) $^ -o $(PRINCIPAL) $(LIBS)
	if ! [ -d "Resultados/" ]; then mkdir Resultados/; fi
	$(MAKE) test

# recipe to compile all .c files into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)/
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

# target to create the $(OBJ_DIR)/ directory if it does not exist
$(OBJ_DIR)/:
	mkdir -p $(OBJ_DIR)/

.PHONY: clean
.PHONY: run
.PHONY: interact
.PHONY: interact-debug
.PHONY: debug 
.PHONY: mem
.PHONY: test
.PHONY: run-test

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(PRINCIPAL) $(TESTES)
	rm -f valgrind-out.txt
	if [ -d "Resultados/" ]; then rm -r "Resultados/"; fi
	clear

run: $(PRINCIPAL)
	./$(PRINCIPAL) $(ARGS)

interact: $(PRINCIPAL)
	./$(PRINCIPAL) 

interact-debug: $(PRINCIPAL)
	gdb --args $(PRINCIPAL)

debug: $(PRINCIPAL)
	gdb --args $(PRINCIPAL) $(ARGS)

time: $(PRINCIPAL)
	sh -c "time ./$(PRINCIPAL) $(ARGS)"

mem:
	valgrind --leak-check=full --track-origins=yes --log-file=valgrind-out.txt --suppressions=/usr/share/glib-2.0/valgrind/glib.supp --show-leak-kinds=all -s -- ./$(PRINCIPAL) $(ARGS)

# target to link all .o files except the ignored .o file
test: $(filter-out $(IGNORE_PRINCIPAL), $(OBJ_FILES))
	$(CC) $(CFLAGS) $^ -o $(TESTES) $(LIBS)
	if ! [ -d "Resultados/" ]; then mkdir Resultados/; fi

run-test: test
	./$(TESTES) $(ARGS)
