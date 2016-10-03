#!/bin/sh

FLASH_DEVICE=$1
BINDIR=$2
HEXFILE=$3
SDFILE=$4

# setup JLink command file
echo "device $FLASH_DEVICE" >> $BINDIR/burn.seg
echo "speed 10000" >> $BINDIR/burn.seg
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
