#!/bin/bash
    
printf "********* Test-Case Script Running *********\n"

# Check command line argument entered correct or not

printf "[+] Checking command line arguments: "
if [ $# -ne 1 ]; then
    echo "Missing arguments, usage: ./script.sh true/false"
    exit 1
else
    echo "Correct."
fi

file="Deadlock/Test.cpp"

# Check the given file exist
printf "[+] Checking file: "
if [ -f "$file" ]; then
    echo "file exist"
else
    echo "file does not exist!"
    exit 2
fi

argument="$1"

# Check bool operation
printf "[+] Checking argument: "
if [ "$argument" == 'true' ] || [ "$argument" == 'false' ]; then
    echo "argument is valid"
else
    echo "argument is invalid!"
    exit 3
fi

exe="Test"
if [ ! -f "$exe" ]; then

    printf "[+] Start compiling: "
    make=$(g++ -std=c++20 -o "$exe" "$file")
    # Check the compiling process success or not to exit
    if [ -n "$make" ]; then
        echo "compiling failed!"
        exit 4
    else
        echo "successfully compiled"
    fi
fi

printf "[+] Running Deadlock: "
run=$(./"$exe" "$argument")

echo "********* Script End succesfully *********"
exit 0
