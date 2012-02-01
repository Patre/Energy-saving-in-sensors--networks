#!/bin/sh
# 
# Authors : Elyes Ben Hamida and Guillaume Chelius
#
# Script for running a batch of simulation for :
#    *) a given xml config file    (option -f)
#    *) a list of xml config files (option -d)
# 
# Example 1 : wsnet-run-simulation -f cbr.xml log/ 100
# Example 2 : wsnet-run-simulation -d config/ log/ 100
# 

usage() 
{
    echo "Script usage: "
    echo "    "$1" -f config-file  output-directory  simulation-number"
    echo "    "$1" -d directory-config-file  output-directory  simulation-number"
    echo ""
    exit
}

if [ $# -ne 4 ]
then
    usage $0
fi


if [ "$1" = "-f" ]
then
    if [ ! -e $2 ]
    then
	echo "error: config file "$2" not found !"
	echo ""
	usage $0
    fi
elif [ "$1" = "-d" ]
then
    if [ ! -d $2 ]
    then
	echo "error: config file directory "$2" not found !"
	echo ""
	usage $0
    fi
else
    echo "error : undefined option ("$1") !"
    echo ""
    usage $0
fi
    

if [ ! -d $3 ]
then
    echo "error: directory "$3" not found !"
    echo ""
    usage $0
fi

if [ ! "$(echo $4 | grep "^[ [:digit:] ]*$")" ]
then
	echo "error: argument 4 ("$4") should be an integer !"
	echo ""
	usage $0
fi

WSNET=wsnet

if [ -n "$WSNET_EXEC" ];
then
        WSNET="$WSNET_EXEC"
fi

if [ -z "$(which $WSNET)" ];
then
        echo "Error: '"$WSNET"' binary file does not exist ! "
	echo ""
	echo "Please set the WSNET_EXEC env variable with the correct WSNet binary filename (example: export WSNET_EXEC=wsnet)."
	echo ""
	exit
fi

CURRENT_DIR=$(pwd)
LOG_DIR=$3
LOG_DIR_PREFIX=simulation

FULL_LOG_FILENAME=log.data
FULL_ERROR_LOG_FILENAME=log-error.data

test -d "$LOG_DIR" || mkdir $LOG_DIR

if [ "$1" = "-f" ]
then

    echo "Simulating "$2"..."

    CONFIG_FILE=$2

    k=0

    while [ $k -lt $4 ] ; do
    
	echo "   simulation number: "$k
	
	test -d "$LOG_DIR/$LOG_DIR_PREFIX-$k" || mkdir $LOG_DIR/$LOG_DIR_PREFIX-$k
	cd $LOG_DIR/$LOG_DIR_PREFIX-$k
	
	$WSNET -c $CURRENT_DIR/$CONFIG_FILE -S $k > $FULL_LOG_FILENAME 2> $FULL_ERROR_LOG_FILENAME
	
	cd $CURRENT_DIR
	
	k=$(($k+1))
	
    done
    
elif [ "$1" = "-d" ]
then     

    CONFIG_DIR=$2

    for i in $(ls $CONFIG_DIR | sort -n);
    do
	echo "Simulating "$i"..."
	CONFIG_FILE=$i

	k=0

	test -d "$LOG_DIR/$CONFIG_FILE" || mkdir $LOG_DIR/$CONFIG_FILE
	
	while [ $k -lt $4 ] ; do
    
	    echo "   simulation number: "$k
	    
	    test -d "$LOG_DIR/$CONFIG_FILE/$LOG_DIR_PREFIX-$k" || mkdir $LOG_DIR/$CONFIG_FILE/$LOG_DIR_PREFIX-$k
	    cd $LOG_DIR/$CONFIG_FILE/$LOG_DIR_PREFIX-$k
	
	    $WSNET -c $CURRENT_DIR/$CONFIG_DIR/$CONFIG_FILE -S $k > $FULL_LOG_FILENAME 2> $FULL_ERROR_LOG_FILENAME
	
	    cd $CURRENT_DIR
	
	    k=$(($k+1))
	    
	done
	
    done


else 
    echo "Nothing to do !?"
    echo ""

fi

echo "Done !"
echo ""
