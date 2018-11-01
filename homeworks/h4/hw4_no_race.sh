#!/bin/bash
if [ ! -f "1.txt" ]; then
	echo 0 > 1.txt
fi
for ((i=0;i<1000;++i)); do
	(
	flock 3
	num=$(tail -1 ex3.txt)
	let "num=num+1"
	echo $num >> ex3.txt
	)3<>1.txt
done
