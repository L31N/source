#!/bin/bash

# syntax: bbcopy file

user="robotics";
host="bbone0";

echo "";

if [ $# -le 0 ]
then
    echo "to less arguments: $#";
    echo -e "Usage: $0 file_to_copy\n";
    exit -1;
fi

# check file type and write to specific directory on BeagleBoard or localhost

retval=0;
filename=$(echo $1 | sed -e 's/.*[/]//');

if [[ "$filename" == *".so" ]]
then
    echo "copy shared library \"$1\" to \"$user@$host:/bbusr/lib/$filename\"";
    scp "$1" "$user@$host:/bbusr/lib/";
    retval=$?;
    echo "copy shared library \"$1\" to \"/bbusr/lib/$filename\"";
    cp "$1" "/bbusr/lib/";
    retval+=$?;
elif [[ "$filename" == *".a" ]]
then
    #echo "copy static library \"$1\" to \"$user@$host:/bbusr/lib/$1\"";
    #scp "$1" "$user@$host:/bbusr/lib/";
    echo "copy static library \"$1\" to \"/bbusr/lib/$filename\"";
    cp "$1" "/bbusr/lib/";
    retval=$?;
else
    echo "copy binary file \"$1\" to \"$user@$host:/bbusr/bin/$filename\"";
    scp "$1" "$user@$host:/bbusr/bin/";
    retval=$?;
fi

echo "";

exit $retval;
