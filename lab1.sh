#!/bin/bash

if [ -z "$1" ]
then 
	list=$(ls *.* )
else
	list=$(cd $1 && ls *.*)
fi
#echo $list
for file in $list; do echo "${file##*.}"; done
