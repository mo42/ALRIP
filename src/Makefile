CXX = g++
CXXFLAGS = -Wall -std=c++17

SRCDIR = .
OBJDIR = obj
LIBRARY = libabel.a

CPP_FILES = $(wildcard $(SRCDIR)/*.cpp)
OBJ_FILES = $(CPP_FILES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

LIB_OBJ_FILES = $(filter-out $(OBJDIR)/ap.o $(OBJDIR)/rr.o, $(OBJ_FILES))

$(LIBRARY): $(LIB_OBJ_FILES)
	@echo "Creating static library $(LIBRARY)"
	ar rcs $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/ap.o: $(SRCDIR)/ap.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/rr.o: $(SRCDIR)/rr.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

rr: $(OBJDIR)/rr.o $(LIBRARY)
	$(CXX) $(CXXFLAGS) $(OBJDIR)/rr.o -L. -label -o $@

ap: $(OBJDIR)/ap.o $(LIBRARY)
	$(CXX) $(CXXFLAGS) $(OBJDIR)/ap.o -L. -label -o $@

clean:
	rm -rf $(OBJDIR) $(LIBRARY) rr ap

.PHONY: clean
