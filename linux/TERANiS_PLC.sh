#! /bin/sh
### BEGIN INIT INFO
# Provides:          TERANiS_PLC.sh
# Required-Start:    $start
# Required-Stop:     $shutdown
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Description:       Startet die TERANiS-PLC bei Systemstart
### END INIT INFO
# Author: Mirko Wittek

dir="/home/root/plc"
cmd="$dir/TERANiS_PLC_run"
user="root"

name=TERANiS_PLC
pid_file="/var/run/$name.pid"
stdout_log="$dir/$name.log"

get_pid() {
    cat "$pid_file"
}

is_running() {
    [ -f "$pid_file" ] && ps -p `get_pid` > /dev/null 2>&1
}

case "$1" in
    start)
    if is_running; then
        echo "Already started"
    else
        echo "Starting $name"
        cd "$dir"
        echo ----------`date` Neustart ---------- > "$stdout_log"
        if [ -z "$user" ]; then
            $cmd >> "$stdout_log" 2>&1 &
        else
            $cmd >> "$stdout_log" 2>&1 &
        fi
        echo $! > "$pid_file"
        if ! is_running; then
            echo "Unable to start, see $stdout_log"
            exit 1
        fi
    fi
    ;;
    stop)
    if is_running; then
        echo -n "Stopping $name.."
        kill `get_pid`
        for i in 1 2 3 4 5 6 7 8 9 10
        # for i in `seq 10`
        do
            if ! is_running; then
                break
            fi

            echo -n "."
            sleep 1
        done
        echo

        if is_running; then
            echo "Not stopped; may still be shutting down or shutdown may have failed"
            exit 1
        else
            echo "Stopped"
            if [ -f "$pid_file" ]; then
                rm "$pid_file"
            fi
        fi
    else
        echo "Not running"
    fi
    ;;
    restart)
    $0 stop
    if is_running; then
        echo "Unable to stop, will not attempt to start"
        exit 1
    fi
    $0 start
    ;;
    status)
    if is_running; then
        echo "Running"
    else
        echo "Stopped"
        exit 1
    fi
    ;;
    *)
    echo "Usage: $0 {start|stop|restart|status}"
    exit 1
    ;;
esac

exit 0
