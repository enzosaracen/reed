#!/bin/bash

BYTES=10

for n in $(seq 0 254); do
	head -c $BYTES /dev/random >"test/data$n"
	cp "test/data$n" "test/save$n"
	touch "test/check$n"
	data="${data}test/data$n\n"
	check="${check}test/check$n\n"
done

for n in $(seq 1 254); do
	for m in $(seq 1 $n); do
		files=$(echo -e $data | sed "$n q" | tr '\n' ' ')
		files="${files} $(echo -e $check | sed "$m q" | tr '\n' ' ')"
		./raid $n $m $files
		./raid $n $m $files $(echo -e $data | sed "$m q" | tr '\n' ' ')
		echo -ne "($n, $m): \t"
		for i in $(seq 0 $(($m - 1))); do
			if ! cmp -s "test/save$i" "test/data$i"; then
				echo "bad"
				exit
			fi
		done
		echo "ok"
	done
done
