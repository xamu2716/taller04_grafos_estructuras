CXX = g++
CXXFLAGS = -std=c++17

OBJS = nodo.o arista.o sistema.o main.o
TARGET = ejecutable

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
