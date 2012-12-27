#!/bin/bash

number=$(find ./ -type f | grep -E -x "(.*\.cpp|.*\.h|.*\.hpp|.*\.conf|.*\.c)" | grep -v "/can-lib/" | xargs -L 1 cat | wc -l)
echo $number lines found
