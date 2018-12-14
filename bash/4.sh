#!/bin/bash
trap 'echo "interupt!"; exit' EXIT
function tt() {
    echo "$a func tt begin..."
    sleep 20
    echo "...func tt end"
}

echo "pid: $$"
a="123"
tt &
child_id=$!
echo "child_id: $child_id"

a="456"
tt &
echo "child_id: $!"
sleep 10

