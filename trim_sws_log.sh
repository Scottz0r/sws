#!/bin/bash
echo "$(tail -n 50 /home/pi/sws/sws.log)" > /home/pi/sws/sws.log
