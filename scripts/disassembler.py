from argparse import ArgumentParser
from csv import DictReader

parser = ArgumentParser()
parser.add_argument("bytecode", help="The bytecode file to disassemble.")

args = parser.parse_args()

OPCODES = [opcode for opcode in DictReader(
    open("opcodes.csv", "r", encoding='utf-8'))]


def get_opcode_by_id(id):
    return OPCODES[id]


with open(args.bytecode, "rb") as fp:
    while True:
        opcode = int.from_bytes(fp.read(1))

        args = [str(int.from_bytes(fp.read(1)))
                for n in range(int(OPCODES[opcode]['argument_bytes']))]

        print(f"{OPCODES[opcode]['opcode']} {' '.join(args)}")

        if OPCODES[opcode]['opcode'] == 'halt':
            break
