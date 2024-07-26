#!/bin/bash

# Generate Makefiles
premake5 gmake2 > /dev/null 2>&1

# Function to run a configuration 4 times
run_config() {
    config=$1
    output_dir=$2
    make config=$config
    for i in {1..4}; do
        ./$output_dir/main/main
        make clean > /dev/null 2>&1
    done
}

# Run Release configuration 4 times
run_config "release" "bin/Release-linux-x86_64"

# Run ReleaseSIMD configuration 4 times
run_config "releasesimd" "bin/ReleaseSIMD-linux-x86_64"

# Run ReleaseCSIMD configuration 4 times
run_config "releasecsimd" "bin/ReleaseCSIMD-linux-x86_64"

# Clean up
make clean > /dev/null 2>&1
