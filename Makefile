CXX			= g++
CXXFLAGS	= \
	-I/usr/local/boost_1_44_0 \
	-g \
	-D_BOOSCHEME_DEBUG_

TARGET	= a.out
OBJS	= test.o environment.o output_port.o input_port.o utils.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $^

clean:
	rm -f $(TARGET) *.o *~
