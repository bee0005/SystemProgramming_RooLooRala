#!/bin/bash

ifconfig eth0 192.168.1.3

tftp -r Btest -g 192.168.1.1 
tftp -r insins.sh -g 192.168.1.1

insmod cdc-acm.ko
insmod fnddrv.ko
insmod mleddrv.ko
insmod tlcddrv.ko
insmod oleddrv.ko
insmod keydrv.ko
insmod leddrv.ko
insmod cleddrv.ko
insmod buzzerdrv.ko
insmod dipswdrv.ko

chmod 777 Btest
chmod 777 insins.sh

