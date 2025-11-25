CFLAGS := 

.PHONY: run clean

saw: src/**.c src/opcodes.{c,h}
	@${CC} src/**.c -o saw ${CFLAGS}

src/opcodes.{c,h}: src/opcodes/**
	python3 scripts/gen_opcodes.py

code.saw: scripts/compiler.py
	python3 scripts/compiler.py

run: code.saw saw
	@./saw code.saw

clean:
	rm ./saw