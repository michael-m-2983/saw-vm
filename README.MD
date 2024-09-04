# saw

stack-based bytecode virtual machine

## Usage

```bash
# Install requirements
sudo apt install build-essential python3 # Debian, Ubuntu, etc.
sudo pacman -S make gcc python3 # Arch-based distros

# Clone this repository
git clone https://github.com/michael-m-2983/saw-vm.git

# Create a bytecode file
echo "nop" >> main.sawasm
echo "halt" >> main.sawasm
python3 scripts/assembler.py

# Build the VM
make

# Start the VM
./saw main.saw

# If the debugger is enabled, run all of the commands
# If it is not enabled this is not needed.
>>> N
```
