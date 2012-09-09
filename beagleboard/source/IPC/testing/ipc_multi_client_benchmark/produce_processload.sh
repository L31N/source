#!/bin/bash

for((i = 0; i < 100; i++))
do
	./produce_processload /tmp/ipc_testing/ipc_mserver.uds &
done
