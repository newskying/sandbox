#!/bin/bash
#reader

pipe=/cluster/storage/pipe

trap "rm -f $pipe" EXIT

if [[ ! -p $pipe ]]; then
    mkfifo $pipe
    echo "create pipe: $pipe"
fi

while true
do
    echo "wait read from pipe:"
    if read line <$pipe; then
        if [[ "$line" == 'quit' ]]; then
            break
        fi
        echo $line
    fi
done

echo "Reader exiting"

