#!/bin/sh

FLASH_DEVICE=$1
BINDIR=./

# setup JLink command file
echo "device $FLASH_DEVICE" >> $BINDIR/pre_debug.seg
echo "speed 10000" >> $BINDIR/pre_debug.seg
echo "sleep 1000" >> $BINDIR/burn.seg
echo "if SWD" >> $BINDIR/pre_debug.seg
echo "sleep 1000" >> $BINDIR/burn.seg
echo "r" >> $BINDIR/pre_debug.seg
echo "h" >> $BINDIR/pre_debug.seg
echo "exit" >> $BINDIR/pre_debug.seg

# Reset and halt chip
JLinkExe < $BINDIR/pre_debug.seg

# Clean up unused flash file
rm $BINDIR/pre_debug.seg

# Run JLink GDB Server
JLinkGDBServer -device ${FLASH_DEVICE} -if swd -speed 10000
