#!/bin/bash
export SWS_LOG_PATH=/home/pi/sws/sws.log

/home/pi/sws/sws /home/pi/sws/sws_config.txt > /home/pi/sws/sws_err.log 2>&1
