#!/bin/bash
make
dmesg --clear
sudo insmod *.ko
sudo rmmod *.ko
dmesg
