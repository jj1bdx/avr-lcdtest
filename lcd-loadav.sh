#!/bin/sh
# CAUTION: ^L and ^N are actual control codes
socat SYSTEM:"while true; do (sleep 5 && (uptime | sed 's/.*averages: //' | sed 's/^/Load average:/')); done" /dev/cu.usbmodem1A1221,rawer,clocal=1,ispeed=9600,ospeed=9600
