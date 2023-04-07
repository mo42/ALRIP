CC = clang++
CFLAGS = -std=c++20 -Weverything -Wno-c++98-compat -O3 -c

all : ap rr

# General code
math.o : math.cpp
	$(CC) $(CFLAGS) math.cpp

odd_even_test.o : odd_even_test.cpp
	$(CC) $(CFLAGS) odd_even_test.cpp

reader.o : reader.cpp
	$(CC) $(CFLAGS) reader.cpp

adapt.o : adapt.cpp
	$(CC) $(CFLAGS) adapt.cpp

# Random rotated
rr : rr.o reader.o vec.o line.o rot_square.o math.o adapt.o
	$(CC) rr.o reader.o vec.o line.o rot_square.o math.o adapt.o -o rr

rr.o : rr.cpp
	$(CC) $(CFLAGS) rr.cpp 

vec.o : vec.cpp
	$(CC) $(CFLAGS) vec.cpp

line.o : line.cpp
	$(CC) $(CFLAGS) line.cpp

rot_square.o : rot_square.cpp
	$(CC) $(CFLAGS) rot_square.cpp

# Axis parallel
ap: ap.o reader.o odd_even_test.o les.o point.o square.o rot_square.o \
		line.o corner.o box.o mesh_point.o math.o vec.o adapt.o
	$(CC) ap.o reader.o odd_even_test.o les.o point.o square.o rot_square.o \
		line.o corner.o box.o mesh_point.o math.o vec.o adapt.o -o ap

ap.o : ap.cpp
	$(CC) $(CFLAGS) ap.cpp

les.o : les.cpp
	$(CC) $(CFLAGS) les.cpp

point.o : point.cpp
	$(CC) $(CFLAGS) point.cpp

square.o : square.cpp
	$(CC) $(CFLAGS) square.cpp

corner.o : corner.cpp
	$(CC) $(CFLAGS) corner.cpp

box.o : box.cpp
	$(CC) $(CFLAGS) box.cpp

mesh_point.o : mesh_point.cpp
	$(CC) $(CFLAGS) mesh_point.cpp

clean :
	rm -f *.o

mrproper : clean
	rm -r ap rr
