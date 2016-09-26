#!/bin/bash
script_name=auto_start_gamesvr.sh
process_exists=$(ps -ef | grep GameSvr | grep -v grep | wc -l)

if [ $process_exists -eq 0 ]; then
    echo $(date) "| GameSvr is down." 
    $(/home/charleliu/minigame/bin/GameSvr) &
    echo $(date) "| GameSvr reboot."
else
    echo $(date) "| GameSvr is up." 
fi
