#!/bin/bash

P=( 1 2 4 8 )
N=( 100 $((10**6)) $((10**9)) )

for proc in "${P[@]}"
do 
	for interval in "${N[@]}"
	do
		increment=$(($interval / 20))
		for i in {1..20}
		do
			end=$(($increment * i))
			read TIME <<< $(./parallel 2 $end)
			echo "$proc,$interval,$end,${TIME}" >> result.txt
		done
	done
done
