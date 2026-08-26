#!/bin/bash

ANALDIR=../AnalysisTrees
#HISTDIR=./HitMaps/Zoffset
#HISTDIR=./HitMaps/XYoffset
HISTDIR=./HitMaps/XYZoffset/2019/Q3
DLEN=${#ANALDIR}
#SORTCODE=./S3PosOffset/HitMapBeam
#SORTCODE=./S3PosOffset/HitMap_XY
SORTCODE=./S3PosOffset/HitMap_XYZ
PHIOFFSET=90
m=52040 # Only sort after this run
p=52885 # The first Nov 2020 run that uses the 20 V calibration file
n=52165 #Only Sort until this run 

badruns_2020=(52834 52835 52837 52844 52846 52852 52853 52854 52855 52856 52857 52858 52859 52860 52863 52876 52877 52878 52879 52880 52881 52902 52903 52904 52905 52906 52907 52908 52909 52910 52911 52912 52913 52914 52915 52916)
badruns_2019=(52059 52089 52101 52102 52122 52123 52124 52125 52126 52127 52128 52129 52137 52153 52161)

if [ $# -eq 0 ]; then

for f in $ANALDIR/*.root; do
 g=${f:DLEN+9}
 h=${g:0:${#g}-5} 
 i=${g:0:${#g}-9} 

 HFILE=$HISTDIR/HitMap_$h.root

 if [ -f "$HFILE" ] && [ "$HFILE" -nt "$SORTCODE" ]; then
    echo "Skipping run $i: $HFILE is newer than $SORTCODE"
    continue
 fi

 if [[ $i == "52931" ]]; then
  CFILE_40V=./CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal # Run 52931 was the triple alpha calibration run carried out at 40 V bias in 2020
 elif [[ $i == "52167" ]]; then
	CFILE_40V=./CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal # Run 52167 was the triple alpha calibration in 2019
  CFILE_20V=./CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
 elif [[ $i -lt "52165" ]]; then # Run 52164 was the last in-beam, one hour duration run in 2019
	CFILE_40V=./CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
  CFILE_20V=./CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
 elif [[ $i -gt "52828" ]]; then # Run 52829 was the first in-beam, one hour duration run in 2020
	CFILE_40V=./CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal
	CFILE_20V=./CalibFileSetup/MyCalibrations/CalibrationFile_2020_20V.cal
 fi

 if [[ $i -gt $m && $n -gt $i && ! " ${badruns_2020[*]} " =~ " $i " && ! " ${badruns_2019[*]} " =~ " $i " ]]; then
  if [ ! -f $HFILE ]; then
			if [ $i -lt $p ]; then
				echo "$SORTCODE $f $PHIOFFSET $CFILE_40V $HFILE" # The sort code takes in as arguments: <input file> <phi offset> <calibration file> <output file>
        $SORTCODE $f $PHIOFFSET $CFILE_40V $HFILE 
			elif [ $i -ge $p ]; then
				echo "$SORTCODE $f $PHIOFFSET $CFILE_20V $HFILE"
        $SORTCODE $f $PHIOFFSET $CFILE_20V $HFILE 
			fi
   fi
   if [ -f $HFILE ]; then
     if [ $HFILE -ot $SORTCODE ] || [ $HFILE -ot $f ]; then
         if [ $i -lt $p ]; then
           echo "$SORTCODE $f $PHIOFFSET $CFILE_40V $HFILE"
           $SORTCODE $f $PHIOFFSET $CFILE_40V $HFILE 
         elif [ $i -ge $p ]; then
           echo "$SORTCODE $f $PHIOFFSET $CFILE_20V $HFILE"
           $SORTCODE $f $PHIOFFSET $CFILE_20V $HFILE 
         fi            
     fi
    fi
  fi
done

else 

for f in $ANALDIR/analysis"$@"_*.root; do
 g=${f:DLEN+9}
 h=${g:0:${#g}-5} 
 i=${g:0:${#g}-9} 

 HFILE=$HISTDIR/HitMap_$h.root

 if [[ $i == "52931" ]]; then
  CFILE_40V=./CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal # Run 52931 was the triple alpha calibration run carried out at 40 V bias in 2020
 elif [[ $i == "52167" ]]; then
	CFILE_40V=./CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal # Run 52167 was the triple alpha calibration in 2019
  CFILE_20V=./CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
 elif [[ $i -lt "52165" ]]; then # Run 52164 was the last in-beam, one hour duration run in 2019
	CFILE_40V=./CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
  CFILE_20V=./CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
 elif [[ $i -gt "52828" ]]; then # Run 52829 was the first in-beam, one hour duration run in 2020
	CFILE_40V=./CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal
	CFILE_20V=./CalibFileSetup/MyCalibrations/CalibrationFile_2020_20V.cal
 fi

 if [[ $i -gt $m && $n -gt $i && ! " ${badruns_2020[*]} " =~ " $i " && ! " ${badruns_2019[*]} " =~ " $i " ]]; then
  if [ ! -f $HFILE ]; then
    if [ $i -lt $p ]; then
      echo "$SORTCODE $f $PHIOFFSET $CFILE_40V $HFILE"
      $SORTCODE $f $PHIOFFSET $CFILE_40V $HFILE
    elif [ $i -ge $p ]; then
      echo "$SORTCODE $f $PHIOFFSET $CFILE_20V $HFILE"
      $SORTCODE $f $PHIOFFSET $CFILE_20V $HFILE 
    fi 
   fi
   if [ -f $HFILE ]; then
     if [ $HFILE -ot $SORTCODE ] || [ $HFILE -ot $f ]; then
         if [ $i -lt $p ]; then
           echo "$SORTCODE $f $PHIOFFSET $CFILE_40V $HFILE"
           $SORTCODE $f $PHIOFFSET $CFILE_40V $HFILE
         elif [ $i -ge $p ]; then
           echo "$SORTCODE $f $PHIOFFSET $CFILE_20V $HFILE"
           $SORTCODE $f $PHIOFFSET $CFILE_20V $HFILE
         fi 
     fi
    fi
  fi
done

fi
