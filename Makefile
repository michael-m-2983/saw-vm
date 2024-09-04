CFLAGS := -Iinclude -lm
CFLAGS_DEV := -D SAW_DEBUG_MODE -Wall -g
CFLAGS_BUILD := -O2 -s

saw: include/opcode.h src/**.c include/**.h
	@${CC} src/**.c -o saw ${CFLAGS} ${CFLAGS_DEV}

.PHONY: build

build: include/opcode.h src/**.c include/**.h
	${CC} src/**.c -o saw ${CFLAGS} ${CFLAGS_BUILD}

include/opcode.h: opcodes.csv scripts/opcode_gen.py
	@python3 scripts/opcode_gen.py