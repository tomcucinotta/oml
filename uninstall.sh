#!/bin/bash

if [ "$EUID" -ne 0 ] || [ "$#" -ne 0 ]; then    
    echo "USAGE: sudo $0"
    exit
fi

set -x
rm /usr/local/lib/liboml.so
rm /usr/local/include/oml_*.h
