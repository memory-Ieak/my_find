CFLAGS = -std=c99 -Werror -pedantic -Wall -Wextra
CPPFLAGS = -Isrc/ -Isrc/lexer -Isrc/parser -Isrc/ast -Isrc/utils -Isrc/action -Isrc/eval

ADDRESS_SANITIZER = -fsanitize=address

OBJS = src/lexer/lexer.o src/lexer/token.o src/parser/parser.o src/action/action.o src/eval/eval.o src/ast/ast.o src/utils/utils.o src/utils/vector.o src/my_find.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(ADDRESS_SANITIZER) $^ -o myfind

testsuite: 
	./tests/testsuite.sh

check: all testsuite clean

clean:
	@rm $(OBJS) myfind

.PHONY: clean