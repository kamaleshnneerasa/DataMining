#!/bin/bash

if [ "$3" == "-apriori" ]
then
	./apriori $2 $1 $4
elif [ "$3" == "-fptree" ]
then
	./fptree $2 $1 $4
elif [ "$2" == "-plot" ]
then
	python3 plot.py $1
else
	echo "Wrong commands"
fi
