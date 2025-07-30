CXX	= g++
CXXFLAGS	= -std=c++17 -I/opt/X11/include -fsanitize=address -fno-omit-frame-pointer -g
LDFLAGS	= -L/opt/X11/lib -lX11 -fsanitize=address

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
