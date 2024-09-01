import random
from csv import DictReader
from argparse import ArgumentParser
import uuid
import os

COMMANDS = [
    "mkdir fuzzing.local/%s",
    "python3 scripts/fuzzer.py -o fuzzing.local/%s/bin.saw > /dev/null",
    "./saw fuzzing.local/%s/bin.saw > fuzzing.local/%s/std.out 2> fuzzing.local/%s/std.err"
]
OPCODES = [opcode for opcode in DictReader(
    open("opcodes.csv", "r", encoding='utf-8'))]


parser = ArgumentParser()

parser.add_argument(
    "-m", "--mode", choices=["single", "multi"], default="single", help="The mode to use")
parser.add_argument(
    "-o", "--out", help="The file to write to in single mode.", default="fuzzed.saw")
parser.add_argument("-i", "--instructions",
                    help="The max number of instructions to generate in single mode.", default=200)
parser.add_argument(
    "-c", "--count", help="The number of fuzzing tests to perform in multi mode.", default=200)

args = parser.parse_args()

if args.mode == "single":
    with open(args.out, "wb") as fp:
        for i in range(random.randint(0, args.instructions)):
            # len(OPCODES) - 1: We don't want 'halt' being generated.
            selected_opcode = random.randrange(0, len(OPCODES) - 1)
            fp.write(bytes([selected_opcode]))
            print(
                f"Selected: {selected_opcode} - {OPCODES[selected_opcode]['opcode']}")
        fp.write(bytes([len(OPCODES) - 1]))
elif args.mode == "multi":
    for i in range(args.count):
        id = str(uuid.uuid4()).replace("-", "")[:20]

        for cmd in COMMANDS:
            os.system(cmd.replace("%s", id))
