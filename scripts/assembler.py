import csv
from argparse import ArgumentParser

OPCODES = [opcode for opcode in csv.DictReader(open("opcodes.csv", "r", encoding='utf-8'))]

def get_opcode(name: str):
    for i, opcode in enumerate(OPCODES):
        if opcode['opcode'].lower() == name.lower():
            return i
    raise SyntaxError("Cannot find opcode '%s'!" % name)

parser = ArgumentParser()
parser.add_argument("-i", "--input-file", help="The .sawasm file to assemble.", default="main.sawasm")
parser.add_argument("-o", "--output-file", help="The output file", default="main.saw")

args = parser.parse_args()

with open(args.input_file, "r", encoding='utf-8') as source_fp:
    with open(args.output_file, "wb") as fp:
        lines = [line.strip() for line in source_fp.readlines()]
        out = []

        for line in lines:
            if line.startswith("#"): # support for comments
                continue
            if len(line.strip()) == 0: # whitespace
                continue
            spl = line.split(" ")

            opcode = spl[0]
            out.append(get_opcode(opcode))

            for arg in spl[1:]:
                out.append(int(arg))

        fp.write(bytes(out))