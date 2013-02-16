#!/bin/bash

echo ""
echo "####################"
echo "removing cbp2mak ..."
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


rm /usr/bin/cbp2mak
rm /usr/bin/cbp2mak.unix.mak
rm /usr/lib/txml.so

echo ""
echo "#########################"
echo "removal complete ..."
echo "created by Soctronic"
echo ""
