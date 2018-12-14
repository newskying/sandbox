#!/bin/bash
 
fun(){
    local timeNum=$1
    echo "fun is begin.timeNum:$timeNum"
    {
    echo "sleep $timeNum" ; sleep $timeNum ; echo "sleep $timeNum end"
    } &
echo "$! start1"
    {
    echo "sleep $((timeNum-5))" ; sleep $((timeNum-5)) ; echo "sleep $((timeNum-5)) end"
    } &
echo "$! start2"
    wait
    
    echo "fun is end.timeNum:$timeNum"
}
 
fun 10 &
echo "fun1 $! start"
fun 13 &
echo "fun2 $! start"
 
wait
sleep 60
echo "all is ending"

