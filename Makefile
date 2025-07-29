CXX	= g++
CXXFLAGS	= -std=c++17 -I/opt/X11/include
LDFLAGS	= -L/opt/X11/lib -lX11

SRCS	= $(wildcard *.cc)
OBJS	 = $(SRCS:.cc=.o)
TARGET = raiinet

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
