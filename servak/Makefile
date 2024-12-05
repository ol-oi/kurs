CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
LDFLAGS = -lssl -lcrypto


SOURCES = server.cpp logger.cpp auth.cpp
HEADERS = server.h logger.h auth.h

TARGET = vcalc_server

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)


clean:
	rm -f $(TARGET)
