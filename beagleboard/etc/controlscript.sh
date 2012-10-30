#!/bin/bash

while true
do
    if [/bin/ps -A | /usr/bin/grep ipc_mserver = '']
    then
        /etc/init.d/ipc_mserver start
    fi
done



