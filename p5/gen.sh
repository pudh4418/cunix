#!/bin/bash

export LC_ALL=C

for ((i=0;i<1000;i=i+1)) do
	printf '%d,%s,%s,%d\n' $((RANDOM * 32768 + RANDOM)) $(tr -cd '0-9A-Za-z' < /dev/urandom | head -c $((RANDOM % 63 + 1))) $(tr -cd '0-9A-Za-z' < /dev/urandom | head -c 8) $((RANDOM % 118 + 10)) 
done

exit 0

