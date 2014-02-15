#!/bin/bash

numberWith=$(find ./beagleboard/source ./avr/source -type f | grep -E -x "(.*\.cpp|.*\.h|.*\.hpp|.*\.conf|.*\.c)" | grep -v "/can-lib/" | xargs -L 1 cat | wc -l)
numberChars=$(find ./beagleboard/source ./avr/source -type f | grep -E -x "(.*\.cpp|.*\.h|.*\.hpp|.*\.conf|.*\.c)" | grep -v "/can-lib/" | xargs -L 1 cat | wc -m)
numberMax=$(find ./beagleboard/source ./avr/source -type f | grep -E -x "(.*\.cpp|.*\.h|.*\.hpp|.*\.conf|.*\.c)" | grep -v "/can-lib/" | xargs -L 1 cat | wc -L)
numberWithout=$(find ./beagleboard/source ./avr/source -type f | grep -E -x "(.*\.cpp|.*\.h|.*\.hpp|.*\.conf|.*\.c)" | grep -v "/can-lib/" | xargs -L 1 cat | grep -E .+ | wc -l)
echo ""
echo $numberWith lines including newlines found
echo $numberWithout lines excluded newlines found
echo $(($numberWith-$numberWithout)) newlines found
echo $numberChars characters found
echo The longest line is $numberMax characters long
echo ""
