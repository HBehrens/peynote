#!/usr/bin/env python
import sys
from ScriptingBridge import SBApplication

if sys.stdin.isatty():
    print "usage: pebble logs 2>&1 | ./peynote.py"
    exit(1)

keynote = SBApplication.applicationWithBundleIdentifier_("com.apple.iWork.Keynote")

echo = True
while True:
    # use readline() to work around strange buffer problems with pebble command
    line = sys.stdin.readline()
    if len(line) == 0:
        exit(0)

    if echo:
        print line.strip()

    if "Displaying logs" in line:
        print "Connected to watch!"
        print "Start the peynote app on your Pebble and use UP/DOWN to go previous/next slide in Keynote."
        print "Press CTRL+C to quit."
        echo = False
    elif "UP" in line:
        keynote.showPrevious()
    elif "DOWN" in line:
        keynote.showNext()
