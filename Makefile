CFLAGS := -Iinclude -Idyncall-1.4/dyncall -lm
CFLAGS_DEV := -D SAW_DEBUG_MODE -Wall -g
CFLAGS_BUILD := -O2 -s

.PHONY: build dyncall

saw: include/opcode.h src/**.c include/**.h dyncall
	@${CC} src/**.c dyncall-1.4/dyncall/*.o -o saw ${CFLAGS} ${CFLAGS_DEV}

dyncall:
	@cd dyncall-1.4 && ./configure && make

build: include/opcode.h src/**.c include/**.h
	${CC} src/**.c -o saw ${CFLAGS} ${CFLAGS_BUILD}

include/opcode.h: opcodes.csv scripts/opcode_gen.py
	@python3 scripts/opcode_gen.py