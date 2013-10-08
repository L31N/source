#!/bin/bash

# syntax: bbcopy file

user="robotics";
host="bboard1";

echo "";

if [ $# -le 0 ]
then
    echo "to less arguments: $#";
    echo -e "Usage: $0 file_to_copy\n";
    exit -1;
fi

# check file type and write to specific directory on BeagleBoard or localhost

retval=0;

if [[ "$1" == *".so" ]]
then
    echo "copy shared library \"$1\" to \"$user@$host:/bbusr/lib/$1\"";
    scp "$1" "$user@$host:/bbusr/lib/";
    retval=$?;
elif [[ "$1" == *".a" ]]
then
    #echo "copy static library \"$1\" to \"$user@$host:/bbusr/lib/$1\"";
    #scp "$1" "$user@$host:/bbusr/lib/";
    echo "copy static library \"$1\" to \"/bbusr/lib/$1\"";
    cp "$1" "/bbusr/lib/";
    retval=$?;
else
    echo "copy binary file \"$1\" to \"$user@$host:/bbusr/bin/$1\"";
    scp "$1" "$user@$host:/bbusr/bin/";
    retval=$?;
fi

echo "";

exit $retval;
