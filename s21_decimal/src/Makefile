CC           = gcc
CFLAGS       = -std=c11 -Wall -Werror -Wextra
LIBS         = -lcheck
LFLAGS       = -fprofile-arcs -ftest-coverage
#OS          = $(shell uname -s)
SOURCES      = $(wildcard s21_*.c)
TEST_SOURCES = unit_tests.c
OBJECTS      = $(SOURCES:.c=.o)
CLIBS        = -lm $(shell pkg-config --cflags --libs check)


all: s21_decimal.a test check

s21_decimal.a: obj
	ar -rcs s21_decimal.a $(OBJECTS)

obj: $(SOURCES) s21_decimal.h
	$(CC) $(CFLAGS) -c $(SOURCES)

test: s21_decimal.a
	@$(CC) $(CFLAGS) $(TEST_SOURCES) s21_decimal.a $(CLIBS) -o unit_tests.out
	@./unit_tests.out; exit 0

gcov_report: s21_decimal.a
	@$(CC) $(CFLAGS) $(LFLAGS) $(TEST_SOURCES) $(SOURCES) $(CLIBS) \
        -o unit_tests.out
	@./unit_tests.out; exit 0
	@lcov -t "test" -o unit_tests.info -c -d .
	@genhtml -o report unit_tests.info
	@open report/index.html

check:
	@#cp ../materials/linters/cpplint.py .
	@#cp ../materials/linters/CPPLINT.cfg .
	@#echo "========CPPLINT========"
	@#python3 cpplint.py --extensions=c *.c
	@cp ../materials/linters/.clang-format ./
	@clang-format -n ./*.c
	@rm -f ./.clang-format

rebuild: clean all

clean:
	@rm -rf *.o *.a *.out unit_tests.out ./report *.gcda *.gcno *.info \
        CPPLINT.cfg cpplint.py
