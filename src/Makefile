#
#   Если хочешь знать, как это работает, загляни в
#       https://habr.com/ru/post/155201/.
#   Если этого мало, отведай https://stackoverflow.com/questions/2734719/
#       how-to-compile-a-static-library-in-linux
#

CC=gcc
CFLAGS=-Wall -Wextra -Werror
LDFLAGS=
SOURCES=$(wildcard s21_*.c)
TEST_SOURCES=unit_tests.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=
CLIBS=-lm $(shell pkg-config --cflags --libs check)


all: clean s21_string.a cpplint


%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@


s21_string.a: $(OBJECTS) ./s21_string.h
	ar -rcs $@ $^


gcov_report: test
	geninfo ./*.gcda
	genhtml ./*.info --output-directory ./
	####geninfo,genhtml --- входят в состав gcov. Выводят профилировачную
	####    инфу в консоль


./unit_tests.out: $(SOURCES) $(TEST_SOURCES)
	$(CC) $(CFLAGS) \
		-fprofile-arcs -ftest-coverage -g \
		$^ \
		-o ./unit_tests.out \
		$(CLIBS)
		####...-fprofile-arcs -ftest-coverage --- это для gcov-теста
		####...-g --- for debug info


test: ./unit_tests.out
	./unit_tests.out; exit 0


clean:
	find ./ \( -iname "*.gcda" \
		-or -iname "*.gcov" \
		-or -iname "*.gcno" \
		-or -iname "*.out" \
		-or -iname "*.o" \
		-or -iname "*.a" \
		-or -iname "*.html" \
		-or -iname "*.info" \
		-or -iname "*.css" \
		-or -iname "*.png" \
	\) -delete

	rm -rf ./tests/src/ \
		./.DS_Store/ \
		./src/ \
		./usr/ \
		./unit_tests.out.dSYM/ \
        ./gcc_extra_test_logs.txt \


extra_test: $(SOURCES) ./will_be_cutted/extra_unit_tests.c
	$(CC) \
		-fprofile-arcs -ftest-coverage \
		$^ \
		-o ./sce_extra_unit_tests.out \
		$(CLIBS) 2> ./gcc_extra_test_logs.txt
		####...-fprofile-arcs -ftest-coverage --- это для gcov-теста
		####
	./sce_extra_unit_tests.out


sprintf_test: ./will_be_cutted/sprintf_test.c $(SOURCES)
	$(CC) $^ -lm -o ./sprintf_test.out
	./sprintf_test.out
	

cpplint:
	cp ../materials/linters/CPPLINT.cfg ./
	python3 ../materials/linters/cpplint.py --extensions=c \
        ./*.c \
    	./*.h


valgrind: ./unit_tests.out
	valgrind \
		-s \
		--track-origins=yes \
		--show-leak-kinds=all \
		./unit_tests.out; exit 0;
	#valgrind \
    #   -s \
	#	--leak-check=full \
	#	--leak-resolution=med \
	#	--log-file=RESULT_VALGRIND.txt \
	#	./unit_tests.out
	#valgrind \
	#	--show-leak-kinds=all \
	#	--track-origins=no \
	#	--log-file=RESULT_VALGRIND.txt \
	#	-s \
	#	./unit_tests.out
		#--leak-check=full --- "each individual leak will be shown in detail"
		#--show-leak-kinds=all --- Show all of "definite, indirect, possible,
		#   reachable" leak kinds in the "full" report.
		#--track-origins=yes --- Favor useful output over speed. This tracks
		#   the origins of uninitialized values, which could be very useful for
		#   memory errors. Consider turning off if Valgrind is unacceptably
		#   slow.
		#--verbose: Can tell you about unusual behavior of your program. Repeat
		#   for more verbosity.
		#--log-file: Write to a file. Useful when output exceeds terminal space.


cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem $(SOURCES)

