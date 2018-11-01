#!/bin/bash
if [ ! -f "1.txt" ]; then
	echo 0 > ex3.txt
fi
for ((i=0;i<1000;++i)); do
	num=$(tail -1 ex3.txt)
	let "num=num+1"
	echo $num >> 1.txt
done
