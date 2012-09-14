#!/bin/bash

for((i = 0; i < 100; i++))
do
	./produce_processload_arm /tmp/ipc_testing/ipc_mserver.uds &
done
