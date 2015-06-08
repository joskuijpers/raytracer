## Makefile for the routeplanner project

# name of executable output file in $(BINDIR)
TARGET		= raytrace.out

CC			= g++
# flags for gcc compiler
CFLAGS		+= -Wall -std=gnu++11 -g

# flags for gcc linker
# LDFLAGS		= -Wall -framework OpenGL -framework GLUT
LDFLAGS		= -Wall -Wextra

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LDFLAGS += -framework OpenGL -framework GLUT
else
	CFLAGS += -mmacosx-version-min=10.8
	LDFLAGS += -IGL -IGLU -lglew32 -lopengl32 -lWs2_32 -lole32 -lcomctl32 -lgdi32 -lcomdlg32 -luuid
endif

# directories for source, object and binary file(s)
SRCDIR		= src
OBJDIR		= obj
BINDIR		= bin

SOURCES		:= $(wildcard $(SRCDIR)/*.cpp)
INCLUDES	:= $(wildcard $(SRCDIR)/*.h)
OBJECTS 	:= $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEPENDENCIES:= $(wildcard $(OBJDIR)/*.d)

default: $(BINDIR)/$(TARGET)

# link all .o files together
$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/$(TARGET) $(LDFLAGS) $(OBJECTS)

# compile and generate dependency (.d) file for each .c file
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(SRCDIR)/*.h
	@mkdir -p $(DEPDIR) $(OBJDIR)
	@gcc -MM $(CFLAGS) $(SRCDIR)/$*.cpp > $(OBJDIR)/$*.d
	$(CC) -c $(CFLAGS) $(SRCDIR)/$*.cpp -o $(OBJDIR)/$*.o

debug:
	@make clean && CFLAGS="-DDEBUG" make

# remove object and dependency files
clean:
	rm -rf $(BINDIR) $(OBJDIR)

# run the program with given arguments
run: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET) 1 3 11 5

# create archive with source code
release:
	tar -czvf routeplanner.tgz --exclude=*.tgz --exclude=*.sh --exclude=bin --exclude=obj --exclude=.* *

# install binary to /usr/local/bin
install: $(BINDIR)/$(TARGET)
	sudo cp $(BINDIR)/$(TARGET) /usr/local/bin/routeplanner
