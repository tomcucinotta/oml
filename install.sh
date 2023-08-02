#!/bin/bash

if [ "$EUID" -ne 0 ] || [ "$#" -ne 0 ]; then    
    echo "USAGE: sudo $0"
    exit
fi

set -x
cp liboml.so /usr/local/lib 
cp oml_*.h   /usr/local/include
