#!/bin/bash
string=""
for input in $@
do
	string=$string"|.*\."$input
done

string=$(echo $string | tail -c +2)
string="("$string")"

numberWith=$(find ./ -type f | grep -E -x $string | xargs -L 1 cat | wc -l)
numberChars=$(find ./ -type f | grep -E -x $string | xargs -L 1 cat | wc -m)
numberWithout=$(find ./ -type f | grep -E -x $string | xargs -L 1 cat | grep -E .+ | wc -l)
echo ""
echo $numberWith lines including newlines found
echo $numberWithout lines excluded newlines found
echo $(($numberWith-$numberWithout)) newlines found
echo $numberChars characters found
echo ""
