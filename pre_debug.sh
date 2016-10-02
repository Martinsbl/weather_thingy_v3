#!/bin/sh

# This flash script dynamicnodcdcy generates a file with a set of commands which
# have to be handed to the flashing script of SEGGER (JLinkExe >4.84).
# After that, JLinkExe will be executed with that set of commands to flash the
# latest .bin file to the board.

# @author Timo Ziegler <timo.ziegler@fu-berlin.de>
# @author Hauke Petersen <hauke.petersen@fu-berlin.de>

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