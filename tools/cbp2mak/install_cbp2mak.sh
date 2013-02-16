#!/bin/bash

echo ""
echo "######################"
echo "installing cbp2mak ..."
echo ""

# check for roo privileges t #
if test "`id -u`" -ne 0
then
   echo "please try with root privileges !"
   echo "exiting now ..."
   echo ""
   exit 1
fi 
#  ------------------------- #

make all

cp cbp2mak /usr/bin/
cp cbp2mak.unix.mak /usr/bin/
cp txml.so /usr/lib/

make clean

echo ""
echo "#########################"
echo "installation complete ..."
echo "created by Soctronic"
echo ""
