#!/bin/bash
while true
do
    prozesstest=`/bin/ps -A | /bin/grep ipc_mserver`
    if  [ $prozesstest = `` ]
    then
              service ipc_mserver start
    fi
done

