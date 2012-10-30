#!/bin/bash
prozesstest=`/bin/ps -A | /usr/bin/grep ipc_mserver`
if  [ $prozesstest = `` ]
then
          service ipc_mserver start
fi


