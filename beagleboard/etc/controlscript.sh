#!/bin/bash

while true
do
    if [ps -A | grep ipc_mserver = '']
    then
        /etc/init.d/ipc_mserver start
    fi
done



