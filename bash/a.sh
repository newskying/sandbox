#!/bin/bash
declare -A INNERVIPS=(
   [amf]=1
   [smf]=1
   [nrf]=1
   [h2proxy]=1
)

echo "1 ${INNERVIPS}"
echo "2 ${INNERVIPS[@]}"
echo "3 ${#INNERVIPS[@]}"
echo "4 ${!INNERVIPS[@]}"
echo ".............."
declare -gA gencmd
echo "before: $gencmd"
declare idx=0
for item in ${!INNERVIPS[@]}
do
   echo "$idx: $item"
   gencmd+=( [aaa_${idx}]="iptables -A $item" )
   idx=$(( idx+1 ))
done
echo "after: ${#gencmd[@]}"
echo "after: ${gencmd[@]}"
echo ".............."

declare -a my_array=(amf smf nrf h2proxy ttt aaa)
echo "${my_array[@]}"

for id_array_iter in "${my_array[@]}"
#for id_array_iter in "amf smf nrf h2proxy ttt"
do
  echo "$id_array_iter":"${INNERVIPS[${id_array_iter}]}"
  echo "----------"
  if [[ "${INNERVIPS[${id_array_iter}]}" -eq "1" ]]
  then
    echo "$id_array_iter":"${INNERVIPS[${id_array_iter}]}"
  fi
  echo ""
done

