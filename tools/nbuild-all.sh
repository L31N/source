#!/bin/bash

# Fill in the needed information ...
# the build order will follow the order of the entries ...

###############################################################################

# project_to_build[n]='...'     name the code::blocks project file (*.cbp -- without an path !!!)
# project_directory[n]='...'    name the project directory (absolute path)
# library_directory[n]='...'    name the destination folder for the library file if available else 'false'
# include_directory[n]='...'    name the destination folder for the header files if available else 'false'
# binary_directory[n]='...'     name the destination folder for the executable if available else 'false'

# ipc_mserver_boost
project_to_build[0]='ipc_mserver_boost.cbp';
#project_to_build[0]='false';
project_directory[0]='/bbusr/source/IPC/ipc_mserver_boost/';
library_directory[0]='false';
include_directory[0]='false';
binary_directory[0]='/bbusr/bin/';
target_name[0]='Release';

# bbvector
project_to_build[1]='bbvector.cbp';
#project_to_build[1]='false';
project_directory[1]='/bbusr/source/vectorClass/';
library_directory[1]='/bbusr/lib/';
include_directory[1]='/bbusr/include/';
binary_directory[1]='false';
target_name[1]='Release';

# bbipc
project_to_build[2]='bbipc.cbp';
#project_to_build[2]='false';
project_directory[2]='/bbusr/source/IPC/bbipc/';
library_directory[2]='/bbusr/lib/';
include_directory[2]='/bbusr/include/ipc/';
binary_directory[2]='false';
target_name[2]='Release';

# bbdebug
project_to_build[3]='bbdebug.cbp';
#project_to_build[3]='false';
project_directory[3]='/bbusr/source/debug/bbdebug/';
library_directory[3]='/bbusr/lib/';
include_directory[3]='/bbusr/include/debug/';
binary_directory[3]='false';
target_name[3]='Release';

# debug-user
project_to_build[4]='debug-user.cbp';
#project_to_build[4]='false';
project_directory[4]='/bbusr/source/debug/debug-user/';
library_directory[4]='false';
include_directory[4]='false';
binary_directory[4]='/bbusr/bin/';
target_name[4]='Release';

# bbcan
project_to_build[5]='bbcan.cbp';
#project_to_build[5]='false';
project_directory[5]='/bbusr/source/CAN/bbcan/';
library_directory[5]='/bbusr/lib/';
include_directory[5]='/bbusr/include/can/';
binary_directory[5]='false';
target_name[5]='Release';

# spi_mserver
project_to_build[6]='spi_mserver.cbp';
#project_to_build[6]='false';
project_directory[6]='/bbusr/source/spi_mserver/';
library_directory[6]='false';
include_directory[6]='false';
binary_directory[6]='/bbusr/bin/';
target_name[6]='Release';

# bbsensor
project_to_build[7]='bbsensor.cbp';
#project_to_build[7]='false';
project_directory[7]='/bbusr/source/sensorAPI/';
library_directory[7]='/bbusr/lib/';
include_directory[7]='/bbusr/include/sensor/';
binary_directory[7]='false';
target_name[7]='Release';

# bbmotion
project_to_build[8]='bbmotion.cbp';
#project_to_build[8]='false';
project_directory[8]='/bbusr/source/motionControl/bbmotion/';
library_directory[8]='/bbusr/lib/';
include_directory[8]='/bbusr/include/motion/';
binary_directory[8]='false';
target_name[8]='Release';

# motionControl
project_to_build[9]='motionControl.cbp';
#project_to_build[9]='false';
project_directory[9]='/bbusr/source/motionControl/server/';
library_directory[9]='false';
include_directory[9]='false';
binary_directory[9]='/bbusr/bin/';
target_name[9]='Release';

#remoteControlServer
project_to_build[10]='remoteControlServer.cbp';
project_directory[10]='/bbusr/source/remoteControlServer/';
library_directory[10]='false';
include_directory[10]='false';
binary_directory[10]='/bbusr/bin/';
target_name[10]='Release';


# ..........................


build_type='intelligent';
#build_type='rebuild';

###############################################################################

if [ $# -gt 0 ]
then
    if [ "$1" == "--help" ]
    then
        echo "";
        echo "Usage: $0 [OPTION]";
        echo "Builds the bb-sourcetree.";
        echo "";
        echo "Options:";
        echo -e "-r, --rebuild\t\tmake a new and clean rebuild instead of an intelligent build.";
        echo -e "-i, --intelligent\tonly build non-existing outputfiles.\n";
        exit 0;
    elif [ "$1" == "-r" ] || [ "$1" == "--rebuild" ]
    then
        build_type='rebuild';
    elif [ "$1" == "-i" ] || [ "$1" == "--intelligent" ]
    then
        build_type='intelligent';
    else
        echo -e "\ninvalid option used: \"$1\"\n";
        exit 3;
    fi
fi

echo "This script will build the whole beagleboard-sourcetree";
echo -e "#######################################################\n";

echo -e "Following projects will be builded:\n";
for (( i=0; i<${#project_to_build[@]}; i++ )) do
    [ "${project_to_build[$i]}" != "false" ] && echo -e "[$i]:\t${project_to_build[$i]}";
done
echo "";

if [ "$build_type" == "rebuild" ]
then
    echo -e "WARNING: rebuild enabled -> all old object-, library- and binary- files will be deleted !!!\n";
fi

read -p "Do you want to continue [Y/n]?" choice;
case "$choice" in
    y|Y )
        echo -e "yes\ncontinue ...\n";
        ;;

    n|N )
        echo -e "no\nexit now ...\n";
        exit 0;
        ;;

    * )
        echo -e "invalid\nexit now ...\n";
        exit 1;
        ;;
esac

# check weather native-build is installed ...
command -v '/usr/bin/native-build' > /dev/null 2>&1 || echo -e "could not find /usr/bin/native-build ...\nexiting now ...\n";
command -v '/usr/bin/native-build' > /dev/null 2>&1 || exit 2;

# start building-process here ...
for (( j=0; j<${#project_to_build[@]}; j++ )) do
    if [ "${project_to_build[$j]}" != "false" ]
        then

        echo -e "\naccessing directory ${project_directory[$j]} for \"${project_to_build[$j]}\"";

        cd "${project_directory[$j]}";

        # check weather the bin/$target_name directory already exists ...
        [ -d "bin/${target_name[$j]}" ] || mkdir -p "bin/${target_name[$j]}";

        # check for rebuild and delete old files ...
        if [ "$build_type" == "rebuild" ]
        then
            rm -v *.o;

            if [ "${binary_directory[$j]}" != "false" ]
            then
                rm -v "bin/${target_name[$j]}/${project_to_build[$j]%????}";
            fi

            if [ "${library_directory[$j]}" != "false" ]
            then
                rm -v *.a;
            fi
        fi

        # the build process self ...
        if /usr/bin/native-build "${project_to_build[$j]}" "${target_name[$j]}"
            then
            echo "${project_to_build[$j]} built successfully ...";
        else
            echo -e "an error occured while building ${project_to_build[$j]} ...\n";
            echo -e "aborting\n"
            exit 5;
        fi

        # copy all the built stuff to the right directories ...
        if [ "${library_directory[$j]}" != "false" ]
        then
            echo -n "copy ";
            cp -v "lib${project_to_build[$j]%????}.a" "${library_directory[$j]}";

            if [ "${include_directory[$j]}" != "false" ]
            then
                echo -n "copy ";
                cp -v *.h "${include_directory[$j]}";

            fi
        elif [ "${binary_directory[$j]}" != "false" ]
        then
            echo -n "copy ";
            cp -v "bin/${target_name[$j]}/${project_to_build[$j]%????}" "${binary_directory[$j]}";
        fi
    fi

done

echo -e "\n************************************\n";
