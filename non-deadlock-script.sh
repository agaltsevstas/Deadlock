#!/bin/bash
    
printf "********* Test-Case Non-Deadlock Script Running *********\n"

# Check command line argument entered correct or not
file="Deadlock/Test.cpp"

printf "[+] Checking file: "
if [ -f "$file" ]; then
    echo "file exist"
else
    echo "file does not exist!"
    exit 1
fi

exe="Test"
if [ ! -f "$exe" ]; then

    printf "[+] Start compiling: "
    make=$(g++ -std=c++20 -o "$exe" "$file")
    # Check the compiling process success or not to exit
    if [ -n "$make" ]; then
        echo "compiling failed!"
        exit 2
    else
        echo "successfully compiled"
    fi
fi

printf "[+] Running Deadlock: "
argument="false"
run=$(./"$exe" "$argument")

echo "********* Script End succesfully *********"
exit 0
