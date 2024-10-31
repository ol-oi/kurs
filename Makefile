# Определяем компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -lpthread -lssl -lcrypto

# Указываем целевые исполняемые файлы
TARGETS = server client

# Указываем исходные файлы
SRCS_SERVER = server.cpp
SRCS_CLIENT = client.cpp

# Указываем объектные файлы
OBJS_SERVER = $(SRCS_SERVER:.cpp=.o)
OBJS_CLIENT = $(SRCS_CLIENT:.cpp=.o)

# Правило по умолчанию
all: $(TARGETS)

# Правило для сборки сервера
server: $(OBJS_SERVER)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Правило для сборки клиента
client: $(OBJS_CLIENT)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Правило для сборки объектных файлов
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для очистки скомпилированных файлов
clean:
	rm -f $(OBJS_SERVER) $(OBJS_CLIENT) $(TARGETS)

# Правило для справки
.PHONY: all clean
