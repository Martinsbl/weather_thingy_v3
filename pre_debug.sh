#!/bin/sh

FLASH_DEVICE=$1
BINDIR=./

# setup JLink command file
echo "device nrf51422_xxac" >> $BINDIR/pre_debug.seg
echo "speed 10000" >> $BINDIR/pre_debug.seg
echo "sleep 1000" >> $BINDIR/burn.seg
echo "if SWD" >> $BINDIR/pre_debug.seg
echo "sleep 1000" >> $BINDIR/burn.seg
echo "r" >> $BINDIR/pre_debug.seg
echo "h" >> $BINDIR/pre_debug.seg
echo "exit" >> $BINDIR/pre_debug.seg

# flash new binary to the board
JLinkExe < $BINDIR/pre_debug.seg

# clean up unused flash file
rm $BINDIR/pre_debug.seg

# flash new binary to the board
JLinkGDBServer -device nrf51422_xxac -if swd -speed 1000
