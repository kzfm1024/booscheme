CXX			= g++
CXXFLAGS	= \
	-I/usr/local/boost_1_44_0 \
	-g \
	-D_ENABLE_BOODEBUG_

TARGET	= a.out
OBJS	= test.o interpreter.o primitive.o closure.o procedure.o environment.o symbol.o output_port.o input_port.o utils.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $^

clean:
	rm -f $(TARGET) *.o *~
