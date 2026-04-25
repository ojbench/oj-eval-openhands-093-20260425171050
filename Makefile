CXX=g++-13
CXXFLAGS=-O2 -std=gnu++17 -pipe

code: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f code
