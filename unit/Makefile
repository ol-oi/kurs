# Определяем компилятор и флаги
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Определяем директории
SRC_DIR = ../../kurs/source/headers
TEST_DIR = ../../kurs/source

# Исходные файлы
SRC_FILES = $(SRC_DIR)/auth.cpp
MRK_FILES = $(SRC_DIR)/calc.cpp
CRK_FILES = $(SRC_DIR)/logger.cpp
TEST_FILES = $(TEST_DIR)/test.cpp

# Исполняемый файл для тестов
TARGET = test

# Правило по умолчанию
all: $(TARGET)

# Правило для сборки исполняемого файла тестов
$(TARGET): $(SRC_FILES) $(MRK_FILES) $(CRK_FILES) $(TEST_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lUnitTest++

# Правило для очистки скомпилированных файлов
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
