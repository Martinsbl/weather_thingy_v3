#!/bin/sh

# This flash script dynamicnodcdcy generates a file with a set of commands which
# have to be handed to the flashing script of SEGGER (JLinkExe >4.84).
# After that, JLinkExe will be executed with that set of commands to flash the
# latest .bin file to the board.

# @author Timo Ziegler <timo.ziegler@fu-berlin.de>
# @author Hauke Petersen <hauke.petersen@fu-berlin.de>

BINDIR=$1
HEXFILE=$2
SDFILE=$3

# setup JLink command file
echo "device nrf52" >> $BINDIR/burn.seg
echo "speed 50000" >> $BINDIR/burn.seg
echo "w4 4001e504 2" >> $BINDIR/burn.seg
echo "w4 4001e50C 1" >> $BINDIR/burn.seg
echo "sleep 100" >> $BINDIR/burn.seg
echo "r" >> $BINDIR/burn.seg
echo "loadfile $SDFILE" >> $BINDIR/burn.seg
echo "loadfile $HEXFILE" >> $BINDIR/burn.seg
echo "sleep 100" >> $BINDIR/burn.seg
echo "r" >> $BINDIR/burn.seg
echo "g" >> $BINDIR/burn.seg
echo "exit" >> $BINDIR/burn.seg
echo "" >> $BINDIR/burn.seg

# flash new binary to the board
JLinkExe < $BINDIR/burn.seg

# clean up unused flash file
rm $BINDIR/burn.seg