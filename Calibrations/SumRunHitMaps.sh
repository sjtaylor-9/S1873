#!/bin/bash

# To remove all the summed files from XYOfsset and ZOffset directories but not the individual sub-run files, rum the following command:
# find . -maxdepth 1 -type f -name 'HitMap*.root' ! -regex '.*/.*_[0-9][0-9][0-9]\.root' -delete

declare -a FILE_LIST # array to hold file names
HIST_DIR=./HitMaps/XYZoffset/2019/Q2

firstrun=52829
lastrun=52928

if [ $# -eq 0 ]; then 
  echo "usage $0 <first run> <last run>"
  exit
fi

if [ $# -eq 1 ]; then 
  firstrun=$1
  lastrun=$1
fi

if [ $# -eq 2 ]; then
  firstrun=$1
  lastrun=$2
fi

# summing on a run by run basis
for run in `seq $firstrun $lastrun` ; do
  if [[ -e $HIST_DIR"/HitMap_$run.root" ]] ; then 
   echo "HitMap_$run.root already exists!"
   continue 
  fi 

  echo "::: Summing submaps for $run ... "
  echo ""
  for file in "$HIST_DIR"/HitMap_"$run"_*; do 
    FILE_LIST=(${FILE_LIST[@]} "$file")
    done

  # sum all subruns into single .root file
if [ ${#FILE_LIST[@]} -ne 0 ]; then
#echo "${#FILE_LIST[@]}   "
  echo "hadd -f $HIST_DIR/HitMap_$run.root ${FILE_LIST[@]}"
  hadd -f $HIST_DIR/HitMap_$run.root ${FILE_LIST[@]}

else
  if [ -e ${FILE_LIST[0]} ] ; then
  echo "cp ${FILE_LIST[0]} $HIST_DIR/HitMap_$run.root"
  cp ${FILE_LIST[0]} $HIST_DIR/HitMap_$run.root
  fi
fi
  # need to clear array after each loop 

  unset FILE_LIST
  echo ""
  echo "::: Summing submaps for $run ... [Done]"

done

# sum all individual runs together
echo "::: Summing All Hit Maps :::"
  ./SumAllHitMaps.sh $firstrun $lastrun
echo "::: [DONE] :::"
