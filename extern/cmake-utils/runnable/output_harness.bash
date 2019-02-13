#!/usr/bin/env bash

log_file=$1
shift
output=$(eval $@)

if [ $(echo $output | wc -c) -le 1 ]
then
	touch "${log_file}"
	exit 0
else
	echo "$output"
	echo "$output" > "${log_file}"
	exit 1
fi