# saw

stack-based bytecode virtual machine

## Usage

```bash
# Install requirements
sudo apt install build-essential python3

# Clone this repository
git clone https://github.com/michael-m-2983/saw-vm.git

# Create a bytecode file
echo "nop" >> main.sawasm
echo "halt" >> main.sawasm
python3 scripts/assembler.py -o main.saw main.sawasm

# Build the VM
make

# Run the bytecode file
./saw main.saw
```
