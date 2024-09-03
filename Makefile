CFLAGS := -Iinclude -D SAW_DEBUG_MODE

saw: include/opcode.h src/**.c include/**.h
	@${CC} src/**.c -o saw ${CFLAGS} -g

include/opcode.h: opcodes.csv scripts/opcode_gen.py
	@python3 scripts/opcode_gen.py