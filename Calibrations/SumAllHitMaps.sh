#!/bin/bash

# To remove all the summed files from XYOfsset and ZOffset directories but not the individual sub-run files, rum the following command:
# find . -maxdepth 1 -type f -name 'HitMap*.root' ! -regex '.*/.*_[0-9][0-9][0-9]\.root' -delete

declare -a FILE_LIST # array to hold file names
HIST_DIR=./HitMaps/GammaGated/2020

firstrun=52829
lastrun=52928

if [ $# -eq 0 ]; then 
  echo "Sums all run histograms into single file"
  echo "usage $0 <first run> <last run>"
  exit
fi

if [ $# -eq 1 ]; then 
  echo "Sums all run histograms into single file"
  echo "usage $0 <first run> <last run>"
  exit
fi

if [ $# -eq 2 ]; then
  firstrun=$1
  lastrun=$2
fi

# summing all hit maps
for run in `seq $firstrun $lastrun` ; do
  if [[ -e $HIST_DIR/HitMap_XYZ_"$firstrun"-"$lastrun".root ]] ; then 
    echo "HitMap_XYZ_"$firstrun"_"$lastrun".root already exists!"
    exit
  fi 
  if [ -e $HIST_DIR/HitMap_"$run".root ] ; then
  FILE_LIST=(${FILE_LIST[@]} "$HIST_DIR/HitMap_"$run".root")
  fi
done

# sum all hit maps into single .root file
echo "::: Summing hit maps for $firstrun to $lastrun ..."
echo ""
echo "hadd -f $HIST_DIR/HitMap_XYZ_"$firstrun"-"$lastrun".root ${FILE_LIST[@]}"
  hadd -f $HIST_DIR/HitMap_XYZ_"$firstrun"-"$lastrun".root ${FILE_LIST[@]}
echo ""
echo "::: Summing hit maps for $firstrun to $lastrun ... [Done]"
