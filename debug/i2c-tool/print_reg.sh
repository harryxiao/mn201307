#!/bin/sh

#for((i=1;i<=10;i++));
#do
#    i2cget -y 0 0x5d $i
#done

# ds90ub954
for addr in 0x0 0x1 0x2 0x3 0x4 0x5 0xc 0x14 0x35
do
	i2cget -y 0 0x5d $addr
done
