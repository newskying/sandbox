#!/bin/bash
while read -r line
do
echo "read pipe: $line"
done < <(tail -f ./pipe)

