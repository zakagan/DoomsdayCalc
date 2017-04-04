#~ Doomsday Makefile ~#

### Definitions Section ###

# Name of the executable to create
EXECUTABLE = DoomsdayCalc
SOURCE_DIR = src
OBJECT_DIR = build

# The command to run for the C++ compiler and linker
CC = clang++

# Basic compiler arguments
CFLAGS = -g -c -Wall
	
# Basic linker arguments
LDFLAGS = -g

FILES := $(wildcard $(SOURCE_DIR)/*.cpp)
OBJ   := $(patsubst $(SOURCE_DIR)/%.cpp, $(OBJECT_DIR)/%.o, $(FILES))

### Rules section
	
all: setup $(EXECUTABLE)

print-%: ; @echo $*=$($*)

rebuild: clean all

setup: | $(OBJECT_DIR)	

$(OBJECT_DIR):                    # Create the object_files directory only if it does not exist. 
	mkdir -p $@	

$(OBJ): $(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ $<

$(EXECUTABLE): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -rf $(OBJECT_DIR) $(EXECUTABLE)

