CXX = g++
CXXFLAGS = -Wall -std=c++17

SRCDIR = .
OBJDIR = obj
LIBRARY = libabel.a
TARGET = ap rr

CPP_FILES = $(wildcard $(SRCDIR)/*.cpp)
OBJ_FILES = $(CPP_FILES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

LIB_OBJ_FILES = $(filter-out $(OBJDIR)/ap.o $(OBJDIR)/rr.o, $(OBJ_FILES))

$(LIBRARY): $(LIB_OBJ_FILES)
	@echo "Creating static library $(LIBRARY)"
	ar rcs $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJDIR)/ap.o $(LIBRARY)
	@echo "Creating executable $(TARGET)"
	$(CXX) $(CXXFLAGS) $(OBJDIR)/ap.o -L. -label -o $@

$(OBJDIR)/ap.o: $(SRCDIR)/ap.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/rr.o: $(SRCDIR)/rr.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJDIR) $(LIBRARY) $(TARGET)

.PHONY: clean
