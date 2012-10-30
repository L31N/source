#!/bin/bash
while true
do
    if  [ $(ps -A | grep -c ipc_mserver) = 0 ]
    then
              echo restart ipc_mserver
              service ipc_mserver restart
    else
        echo ipc_mserver is already running
    fi
    sleep 5
done

