#!/usr/bin/env python
import sys
from ScriptingBridge import SBApplication

if sys.stdin.isatty():
    print "usage: pebble logs 2>&1 | ./peynote.py"
    exit(1)

keynote = SBApplication.applicationWithBundleIdentifier_("com.apple.iWork.Keynote")
deckset = SBApplication.applicationWithBundleIdentifier_("com.unsignedinteger.Deckset")

echo = True
while True:
    # use readline() to work around strange buffer problems with pebble command
    line = sys.stdin.readline()
    if len(line) == 0:
        exit(0)

    deckset_doc = deckset.documents()[0] if deckset and deckset.isRunning() else None

    if echo:
        print line.strip()

    if "Displaying logs" in line:
        print "Connected to watch!"
        print "Start the peynote app on your Pebble and use UP/DOWN to go previous/next slide in Keynote."
        print "Press CTRL+C to quit."
        echo = False
    elif "UP" in line:
        if keynote and keynote.isRunning(): keynote.showPrevious()
        if deckset_doc: deckset_doc.setSlideIndex_(deckset_doc.slideIndex()-1)
    elif "DOWN" in line:
        if keynote and keynote.isRunning(): keynote.showNext()
        if deckset_doc: deckset_doc.setSlideIndex_(deckset_doc.slideIndex()+1)
