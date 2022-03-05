CXX = gcc
CXXFLAGS = -Wall -pedantic -Werror -Wextra -Wshadow -Wconversion -Wsign-compare -Wuninitialized -Wunused-parameter -Wunused-variable -Wunused-function -Wunused-label -Wunused-value -Wunused-but-set-variable -Wunused-result -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-label -Wno-unused-value -Wno-unused-but-set-variable -Wno-unused-result -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-label -Wno-unused-value -Wno-unused-but-set-variable -Wno-unused-result -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-label -Wno-unused-value -Wno-unused-but-set-variable -Wno-unused-result -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-label -Wno-unused-value -Wno-unused-but-set-variable -Wno-unused-result -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-label -Wno-unused-value -Wno-unused-but-set-variable -Wno-unused-result -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-label -Wno-unused-value -Wno-unused-but-set-variable -Wno-unused-result -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-label -Wno-unused-value -Wno-unused-but-set-variable -Wno-unused-result

FILES = main.c

EXEC = server

install:
	$(CXX) $(FILES) -o $(EXEC)


.PHONY: clean

clean:
	rm $(EXEC)