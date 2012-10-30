#!/bin/bash

# write in all deamons here ...
deamon_count=1

deamon[0]="ipc_mserver"

# write all processes here ...
process_count=1

#process name eg. helloworld
process_name[0]="helloworld"

#process paths eg. /source/beagleboard/bin/helloworld
process_path[0]="/home/l31n/robotics.git/source/beagleboard/bin/helloworld"

# other settings here ...
logfile="./controlscript.log"

###########################################################

while true
do

    echo -----------------------------------------------------

    # -----------------------------

    for (( i=0; i<deamon_count; i++ ))
    do
        if  [ $(ps -A | grep -c ${deamon[$i]}) = 0 ]
        then
            echo "$(date): restart ${deamon[$i]}"
            echo "$(date): restart ${deamon[$i]}" >> $logfile

            #restart deamon here ...
            service ipc_mserver restart
        else echo "$(date): all deamons are running ..."
        fi
    done

    #------------------------------

    for (( i=0; i<process_count; i++ ))
    do
        if [ $(ps -A | grep -c ${process_name[$i]}) = 0 ]
        then
            echo "$(date): restart ${process_path[$i]}"
            echo "$(date): restart ${process_path[$i]}" >> $logfile

            #restart process here ...
            ${process_path[$i]} &
        else echo "$(date): all processes are running ..."
        fi
    done

    # -----------------------------

    sleep 5
done

