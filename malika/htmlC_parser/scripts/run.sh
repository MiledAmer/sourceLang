#!/bin/bash

# Check if at least one argument is provided
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <input_file> [output_file]"
    exit 1
fi

# Get the input file
input_file=$1

# Get the output file if provided
if [ "$#" -ge 2 ]; then
    output_file=$2
fi

# Check if transpiler.exe exists in the current directory
if [ ! -f "./transpiler.exe" ]; then
    echo "Error: transpiler.exe not found!"
    exit 1
fi

# Run transpiler.exe with or without output redirection based on $output_file
if [ -n "$output_file" ]; then
    ./transpiler.exe < "$input_file" > "$output_file"
else
    ./transpiler.exe < "$input_file"
fi

# Check if the transpiler ran successfully
if [ $? -eq 0 ]; then
    echo "end"
else
    echo "Error during transpilation."
fi
