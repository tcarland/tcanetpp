#!/bin/bash
#
#   TNMS init script 
#
#   The script is used for starting, stopping, and verifying TNMS services.
#
#

PNAME=${0##*\/}
VERSION="1.14"
AUTHOR="tcarland@gmail.com"

SYSHOME=""
CURDIR=`dirname $0`


if [ "$CURDIR" == "." ]; then
    CURDIR=${PWD}
fi

if [ -n "$TNMS_HOME" ] && [ -d $TNMS_HOME ]; then
    SYSHOME="$TNMS_HOME"
fi
if [ -z "$SYSHOME" ] && [ -d $TNMS_PREFIX ]; then
    SYSHOME="$TNMS_PREFIX"
fi
if [ -z "$SYSHOME" ]; then
    SYSHOME="$CURDIR"
fi

echo ""
echo "$PNAME (v${VERSION}):"
echo "   Home: $SYSHOME"
echo ""


CONFIGDIR="$SYSHOME/etc"


if [ -z "$RC_TNMS_BASHRC" ]; then
    if [ -e $CONFIGDIR/tnmsrc ]; then
        echo "Using rc file from $CONFIGDIR"
        source $CONFIGDIR/tnmsrc
    elif [ -e $HOME/etc/tnmsrc ]; then
        echo "Using rc file from $HOME/tnms/etc"
        source $HOME/etc/tnmsrc
    elif [ -e $CURDIR/tnmsrc ]; then
        echo "Using rc file from $CURDIR"
        source $CURDIR/tnmsrc
    else
        echo "Failed to locate rc file"
        exit 1
    fi
fi



if [ -z "$RC_TNMS_FUNCTIONS" ] && [ -e ${SYSHOME}/bin/tnms_functions.sh ]; then
    source ${SYSHOME}/bin/tnms_functions.sh
fi
if [ -z "$RC_TNMS_FUNCTIONS" ] && [ -e ./bin/tnms_functions.sh ]; then
    source ./bin/tnms_functions.sh
fi
if [ -z "$RC_TNMS_FUNCTIONS" ] && [ -e ./etc/tnms_functions.sh ]; then
    source ./etc/tnms_functions.sh
fi

if [ -z "$RC_TNMS_FUNCTIONS" ]; then
    echo "Failed to locate tnms_functions.sh"
    exit 1
fi



BINDIR="$SYSHOME/sbin"
RUNDIR="$SYSHOME/run"
LOGDIR="$SYSHOME/logs"

export INITERRLOG="$SYSHOME/tmp/init_error.log"
export PROCESS_STATUS_FILE="$SYSHOME/tmp/init_status.log"
echo -n "PROCESS_STATUS_CODE=" >> $PROCESS_STATUS_FILE
INITERR=0



usage()
{
    echo ""
    echo "Usage: $PNAME {start|stop|restart|info} [service] [key]"
    echo ""
    echo "  service = service name to start"
    echo "  key     = process identifier "
    echo ""
    echo "  Note: 'restart' will only restart services determined not currently "
    echo "  running. To force the restart of an individual process, stop|start "
    echo "  targets must be provided."
    echo ""
    return 1
}


start_services()
{
    local proc=$1
    local key=$2
    local retval=0

    echo "Starting services..."

    get_process_list $proc $key

    retval=$?
    if [ $retval -eq 0 ]; then
        echo "No services found"
        return 0
    fi

    if [ -n "$proc" ] && [ -n "$key" ]; then
        echo "Starting service $proc for $key "

        get_config_list $proc $key

        retval=$?
        if [ $retval -eq 0 ]; then
            echo "  Config not found for $proc $key" >> $INITERRLOG
            return 0
        else
            
            start_process $proc $key $CONFIGLIST

        fi
    else
            
        for svc in $PROCLIST; do
            echo "Starting processes for service: $svc"

            get_config_list $svc
            
            retval=$?

            if [ $retval -eq 0 ]; then
                echo "  Config not found for $proc $key" >> $INITERRLOG
                return 0
            else
                
                start_all_processes $svc

                retval=$?

                if [ $retval -gt 0 ]; then
                    echo "  Error starting $retval processes for $svc" >> $INITERRLOG
                fi
            fi
        done
    fi
    
    return 1
}


stop_services()
{
    local proc=$1
    local key=$2
    local retval=

    echo "Stopping services..."

    get_process_list $proc $key

    retval=$?

    if [ $retval -eq 0 ]; then
        echo "  No services found"
        return 0
    fi

    if [ -n "$proc" ] && [ -n "$key" ]; then
        echo "Stopping service $proc for $key "

        get_config_list $proc $key

        retval=$?

        if [ $retval -eq 0 ]; then
            echo "Config not found for $proc $key"
            return 0
        else
            
            stop_process $proc $key $CONFIGLIST

        fi
    else
            
        for svc in $PROCLIST; do
            echo "Stopping processes for service: $svc"

            get_config_list $svc
            
            retval=$?

            if [ $retval -eq 0 ]; then
                echo "  Config not found for $svc"
                return 0
            else
                
                stop_all_processes $svc

                retval=$?

                if [ $retval -gt 0 ]; then
                    echo "  Error stopping $retval processes for $svc" >> $INITERRLOG
                fi

            fi
        done
    fi
    
    return 1
}


restart_services()
{
    local proc=$1
    local key=$2
    local retval=
    local result=0

    get_process_list $proc $key

    retval=$?

    if [ $retval -eq 0 ]; then
        echo "  No processes found" >> $INITERRLOG
        return 0
    fi

    if [ -n "$proc" ] && [ -n "$key" ]; then
        echo "Restarting service $proc for $key "

        get_config_list $proc $key

        retval=$?

        if [ $retval -eq 0 ]; then
            echo "  Config not found for $proc $key" >> $INITERRLOG
            return 0
        else
            
            restart_process $proc $key $CONFIGLIST

            retval=$?

            if [ $retval -eq 0 ]; then
                echo "Error restarting service $proc $key" >> $INITERRLOG
                return 0
            fi
        fi

    else
            
        for svc in $PROCLIST; do
            echo "Restarting any stopped processes for service: $svc"

            get_config_list $svc
            
            retval=$?

            if [ $retval -eq 0 ]; then
                echo "  Config not found for $svc" >> $INITERRLOG
                return 0
            else
                
                restart_all_processes $svc

                retval=$?

                if [ $retval -gt 0 ]; then
                    echo "  Restarted $retval processes for $svc" 
                    ((result += 1))
                fi
            fi
        done
    fi

    if [ $result -gt 0 ]; then
        return 0
    fi

    return 1
}


info_services()
{
    local proc=$1
    local key=$2
    local retval=
    local result=

    result=0

    echo "Verifying services..."

    get_process_list $proc $key

    retval=$?

    if [ $retval -eq 0 ]; then
        echo "  No services found" >> $INITERRLOG
        return 0
    fi

    if [ -n "$proc" ] && [ -n "$key" ]; then
        echo "Checking service $proc for $key "

        check_process $proc $key

        retval=$?

        if [ $retval -eq 0 ]; then
	    source $PROCESS_STATUS_FILE
            echo "  Process $proc $key:  [ ${PROCESS_STATUS_CODE} ]" >> $INITERRLOG
	    echo "  Process $proc $key:  [ ${PROCESS_STATUS_CODE} ]"
            result=1
        else
            echo "  Process $proc $key:   [ OK ]"
        fi

    else
        for svc in $PROCLIST; do
            echo "Checking processes for service: $svc"

            get_config_list $svc
            
            retval=$?

            if [ $retval -eq 0 ]; then
                echo "  Configs not found for $svc" >> $INITERRLOG
                return 0
            else
                
                verify_processes $svc

                retval=$?

                result=`expr $result + $retval`
            fi
        done
    fi

    if [ $result -gt 0 ]; then
        echo "Services not running: $result" >> $INITERRLOG
        return 0
    else
        echo "All services running"
    fi

    return 1
}


retval=

case "$1" in
    'start')
        start_services $2 $3
        ;;
    'stop')
        stop_services $2 $3
        ;;
    'restart')
        restart_services $2 $3
        ;;
    'info'|'status')
        info_services $2 $3
        ;;
    *)
        usage
esac


retval=$?

if [ $retval -eq 0 ]; then
    if [ -n "${EMAIL_ON_ERROR}" ] && [ ${EMAIL_ON_ERROR} == 1 ]; then
        send_errlog $INITERRLOG
    else
	echo
	echo "Error Summary:"
        cat $INITERRLOG 
    fi
fi

rm -f $INITERRLOG
rm -f $PROCESS_STATUS_FILE

exit
