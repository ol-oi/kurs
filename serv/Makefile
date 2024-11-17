# Определение переменных
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -lssl -lcrypto
SRC = main.cpp calc.cpp errors.cpp interface.cpp logger.cpp auth.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = server

# Правило по умолчанию
all: $(TARGET)

# Правило для создания исполняемого файла
$(TARGET): $(OBJ)
 $(CXX) $(OBJ) -o $@

# Правило для компиляции объектных файлов
%.o: %.cpp
 $(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для очистки
clean:
 rm -f $(OBJ) $(TARGET)

# Правило для запуска тестов (если необходимо)
run: $(TARGET)
 ./$(TARGET)

.PHONY: all clean run
