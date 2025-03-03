CXX = g++
CXXFLAGS = -Wall -std=c++17

SRCDIR = .
OBJDIR = obj
LIBRARY = libabel.a
TARGET = ap

CPP_FILES = $(wildcard $(SRCDIR)/*.cpp)
OBJ_FILES = $(CPP_FILES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Exclude ap.o from the library
LIB_OBJ_FILES = $(filter-out $(OBJDIR)/ap.o, $(OBJ_FILES))

# Rule to create the static library
$(LIBRARY): $(LIB_OBJ_FILES)
	@echo "Creating static library $(LIBRARY)"
	ar rcs $@ $^

# Rule to compile each .cpp file into an .o file
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to build the final executable
$(TARGET): $(OBJDIR)/ap.o $(LIBRARY)
	@echo "Creating executable $(TARGET)"
	$(CXX) $(CXXFLAGS) $(OBJDIR)/ap.o -L. -label -o $@

# Ensure ap.o is built separately
$(OBJDIR)/ap.o: $(SRCDIR)/ap.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJDIR) $(LIBRARY) $(TARGET)

.PHONY: clean
