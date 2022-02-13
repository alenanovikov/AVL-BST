CXX = g++
CPPFLAGS = -Wall -g 

all: scheduling

rsa: scheduling.cpp
	$(CXX) $(CPPFLAGS) $^ -o $@

clean: 
	rm scheduling