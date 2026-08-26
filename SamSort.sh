#!/bin/bash

ANALDIR=./AnalysisTrees
HISTDIR=./HistFiles
DLEN=${#ANALDIR}
SORTCODE=Sam_Sort/SortData_Sam
#CFILE=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
CFILE_40V=./Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal
CFILE_20V=./Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_20V.cal
CUTFILE=Sam_Sort/cuts.root
# In the Nov 2020 runs the bias was reduced from 40V to 20V from run 52885 onwards
# The first usable Nov 2020 run was 52829
# The final usable Nov 2020 run was 52928
# (Nov 2020 40 V = 52829 --> 52884)
# (Nov 2020 20 V =  52885 --> 52928)
# Therefore the boundaries should be (52828, 52885) for 40 V calibration file and (52884, 52829) for 20 V calibration file
m=52828 #Only Sort after this run		// (Dec2019 = 52039) // (Nov2020 20V = 52884) // (Nov2020 40V = 52828)
n=52929 #Only Sort until this run 		// (Dec2019 = 52165) // (Nov2020 20V = 52929) // (Nov2020 40V = 52885)
p=52885

badruns_2020=(52834 52835 52837 52844 52846 52852 52853 52854 52855 52856 52857 52858 52859 52860 52863 52876 52877 52878 52879 52880 52881 52902 52903 52904 52905 52906 52907 52908 52909 52910 52911 52912 52913 52914 52915 52916)

# In the Dec 2019 runs the first first in-beam run was 52030 and the last was 52164. One hour duration runs began from run 52047.
# The first "good" in-beam run was 52040 so need runs from 52040 -> 52164.
badruns_2019=(52059 52089 52101 52102 52122 52123 52124 52125 52126 52127 52128 52129 52137 52153 52161)

if [ $# -eq 0 ]
	then

for f in $ANALDIR/*.root; do
# echo "$f"
 g=${f:DLEN+9}
 h=${g:0:${#g}-5} 
 i=${g:0:${#g}-9} 


 if [[ $i == "52931" ]]; then
  CFILE_40V=./Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal # Run 52931 was the triple alpha calibration run carried out at 40 V bias in 2020
 elif [[ $i == "52167" ]]; then
  CFILE_40V=./Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal # Run 52167 was the triple alpha calibration in 2019
  CFILE_20V=./Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
 elif [[ $i -lt "52165" ]]; then # Run 52164 was the last in-beam, one hour duration run in 2019
  CFILE_40V=./Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
  CFILE_20V=./Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
 elif [[ $i -gt "52828" ]]; then # Run 52829 was the first in-beam, one hour duration run in 2020
  CFILE_40V=./Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal
  CFILE_20V=./Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_20V.cal
 fi
 
 # Determines which S3 dead layer the sort code should use
 if [[ $i -lt "52753" ]]; then
  S3DL=Dec2019
 elif [[ $i -lt "52885" ]]; then
  S3DL=Nov2020_40V
 elif [[ $i -gt "52884" ]]; then
  S3DL=Nov2020_20V
 fi

 HFILE=$HISTDIR/Hist_$h.root

 if [[ $i -gt $m && $n -gt $i && ! " ${badruns_2020[*]} " =~ " $i " && ! " ${badruns_2019[*]} " =~ " $i " ]]; then
  if [ ! -f $HFILE ]; then
    if [ ! -f $CUTFILE ]; then
        if [ $i -lt $p ]; then
            echo "$SORTCODE $f $CFILE_40V $HFILE $S3DL"
            $SORTCODE $f $CFILE_40V $HFILE $S3DL
        else
            echo "$SORTCODE $f $CFILE_20V $HFILE $S3DL"
            $SORTCODE $f $CFILE_20V $HFILE $S3DL
        fi
     else
        if [ $i -lt $p ]; then
            echo "$SORTCODE $f $CFILE_40V $HFILE $S3DL $CUTFILE"
            $SORTCODE $f $CFILE_40V $HFILE $S3DL $CUTFILE
        else
            echo "$SORTCODE $f $CFILE_20V $HFILE $S3DL $CUTFILE"
            $SORTCODE $f $CFILE_20V $HFILE $S3DL $CUTFILE
        fi  
     fi 
   fi
   if [ -f $HFILE ]; then
     if [ $HFILE -ot $SORTCODE ] || [ $HFILE -ot $f ]; then
       if [ ! -f $CUTFILE ]; then
        if [ $i -lt $p ]; then
            echo "$SORTCODE $f $CFILE_40V $HFILE $S3DL"
            $SORTCODE $f $CFILE_40V $HFILE $S3DL
        else
            echo "$SORTCODE $f $CFILE_20V $HFILE $S3DL"
            $SORTCODE $f $CFILE_20V $HFILE $S3DL
        fi
        else
        if [ $i -lt $p ]; then
            echo "$SORTCODE $f $CFILE_40V $HFILE $S3DL $CUTFILE"
            $SORTCODE $f $CFILE_40V $HFILE $S3DL $CUTFILE
        else
            echo "$SORTCODE $f $CFILE_20V $HFILE $S3DL $CUTFILE"
            $SORTCODE $f $CFILE_20V $HFILE $S3DL $CUTFILE
        fi   
       fi      
     fi
    fi
  fi
done

else 

for f in $ANALDIR/analysis"$@"_*.root; do
 echo "$f"
 g=${f:DLEN+9}
 h=${g:0:${#g}-5} 
 i=${g:0:${#g}-9} 

 if [[ $i == "52931" ]]; then
  CFILE_40V=./Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal # Run 52931 was the triple alpha calibration run carried out at 40 V bias in 2020
 elif [[ $i == "52167" ]]; then
	CFILE_40V=./Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal # Run 52167 was the triple alpha calibration in 2019
  CFILE_20V=./Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
 elif [[ $i -lt "52165" ]]; then # Run 52164 was the last in-beam, one hour duration run in 2019
  CFILE_40V=./Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
  CFILE_20V=./Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
 elif [[ $i -gt "52828" ]]; then # Run 52829 was the first in-beam, one hour duration run in 2020
  CFILE_40V=./Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal
  CFILE_20V=./Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_20V.cal
 fi

 # Determines which S3 dead layer the sort code should use
 if [[ $i -lt "52753" ]]; then
  S3DL=Dec2019
 elif [[ $i -lt "52885" ]]; then
  S3DL=Nov2020_40V
 elif [[ $i -gt "52884" ]]; then
  S3DL=Nov2020_20V
 fi

 HFILE=$HISTDIR/Hist_$h.root

 if [[ $i -gt $m && $n -gt $i && ! " ${badruns_2020[*]} " =~ " $i " && ! " ${badruns_2019[*]} " =~ " $i " ]]; then
  if [ ! -f $HFILE ]; then
    if [ ! -f $CUTFILE ]; then
        if [ $i -lt $p ]; then
            echo "$SORTCODE $f $CFILE_40V $HFILE $S3DL"
            $SORTCODE $f $CFILE_40V $HFILE $S3DL
        else
            echo "$SORTCODE $f $CFILE_20V $HFILE $S3DL"
            $SORTCODE $f $CFILE_20V $HFILE $S3DL
        fi
     else
        if [ $i -lt $p ]; then
            echo "$SORTCODE $f $CFILE_40V $HFILE $S3DL $CUTFILE"
            $SORTCODE $f $CFILE_40V $HFILE $S3DL $CUTFILE
        else
            echo "$SORTCODE $f $CFILE_20V $HFILE $S3DL $CUTFILE"
            $SORTCODE $f $CFILE_20V $HFILE $S3DL $CUTFILE
        fi   
     fi 
   fi
   if [ -f $HFILE ]; then
     if [ $HFILE -ot $SORTCODE ] || [ $HFILE -ot $f ]; then
       if [ ! -f $CUTFILE ]; then
        if [ $i -lt $p ]; then
            echo "$SORTCODE $f $CFILE_40V $HFILE $S3DL"
            $SORTCODE $f $CFILE_40V $HFILE $S3DL
        else
            echo "$SORTCODE $f $CFILE_20V $HFILE $S3DL"
            $SORTCODE $f $CFILE_20V $HFILE $S3DL
        fi
        else
        if [ $i -lt $p ]; then
            echo "$SORTCODE $f $CFILE_40V $HFILE $S3DL $CUTFILE"
            $SORTCODE $f $CFILE_40V $HFILE $S3DL $CUTFILE
        else
            echo "$SORTCODE $f $CFILE_20V $HFILE $S3DL $CUTFILE"
            $SORTCODE $f $CFILE_20V $HFILE $S3DL $CUTFILE
        fi  
       fi      
     fi
    fi
  fi
done

fi
