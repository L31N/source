#!/bin/bash

while [true]
do
    if [/bin/ps -A | /usr/bin/grep ipc_mserver = ''] /etc/init.d/ipc_mserver start
done



