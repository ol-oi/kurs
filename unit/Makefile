CXX = g++
CXXFLAGS = -std=c++11 -I/usr/include -I/usr/local/include
LDFLAGS = -lssl -lcrypto -lUnitTest++

SRCS_SERVER = ../../servak/source/headers/server.cpp
SRCS_SERV = ../../servak/source/headers/calc.cpp
SRCS_TEST = ../../servak/unit/test.cpp

OBJS_SERVER = $(SRCS_SERVER:.cpp=.o) $(SRCS_SERV:.cpp=.o) 
OBJS_TEST = $(SRCS_TEST:.cpp=.o)

TARGET_SERVER = vcalc_server
TARGET_TEST = test

all: $(TARGET_SERVER) $(TARGET_TEST)

$(TARGET_SERVER): $(OBJS_SERVER)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(TARGET_TEST): $(OBJS_TEST)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS_SERVER) $(OBJS_TEST) $(TARGET_SERVER) $(TARGET_TEST)
