#!/bin/bash
file=$1
LTIME=`stat -c %Z ${file}`
while true
do
   ATIME=`stat -c %Z ${file}`
   if [[ "$ATIME" != "$LTIME" ]]
   then
       echo "$file changed"
       exit 0
   fi
   sleep 2
done

