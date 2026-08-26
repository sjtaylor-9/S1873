#!/bin/bash

DATADIR_2020=./MidasFiles
DATADIR_2019=./MidasFiles_2019
BEFORECALTREESDIR=./CalibratedTrees/WithBeforeCal
AFTERCALTREESDIR=./CalibratedTrees/WithAfterCal
DLEN_2020=${#DATADIR_2020}
DLEN_2019=${#DATADIR_2019}
CFILE_2019_BEFORE=./Calibrations/CalibFileSetup/MyCalibrations/Dec2019/Before/CalibrationFile_Dec19_Before.cal
CFILE_2019_AFTER=./Calibrations/CalibFileSetup/MyCalibrations/Dec2019/After/CalibrationFile_Dec19_After.cal
CFILE_2020_BEFORE=./Calibrations/CalibFileSetup/MyCalibrations/Nov2020/Before/CalibrationFile_2020_Before.cal
CFILE_2020_20V_AFTER=./Calibrations/CalibFileSetup/MyCalibrations/Nov2020/After/20V/CalibrationFile_2020_20V.cal
CFILE_2020_40V_AFTER=./Calibrations/CalibFileSetup/MyCalibrations/Nov2020/After/40V/CalibrationFile_2020_40V.cal

shopt -s nullglob # This enables nullglob, which makes unmatched globs expand to nothing
Cal_runs_2020=(52754 52777 52781 52779 52931 52930 52932 52937 52933 52935)
Cal_runs_2019=(52016 52018 52022 52020 52167 52169 52170 52171 52172)


if [ $# -eq 0 ]; then # If no positional arguments are passed to the script then the script runs through all MIDAS files
for f in $DATADIR_2020/*.mid; do
 g=${f:DLEN_2020+4} # Strips everything in f apart from XXXXX_XXX.mid, which is then stored in g
 h=${g:0:${#g}-4} # h stores the result of stripping .mid from g so h is in the form of XXXXX_XXX
 i=${g:0:${#g}-8} # i stores the result of stripping _XXX.mid from g so i is in the form of XXXXX
 
 if [[ " ${Cal_runs_2020[*]} " =~ " $i " ]]; then
 	FFILEBEFORE=$BEFORECALTREESDIR/fragment$h.root
 	AFILEBEFORE=$BEFORECALTREESDIR/analysis$h.root
    
    FFILEAFTER=$AFTERCALTREESDIR/fragment$h.root
 	AFILEAFTER=$AFTERCALTREESDIR/analysis$h.root
 fi

if [ ! -f $AFILEBEFORE ];  then # This GRSISort command is executed if the analysis tree does not already exist
    echo "File $AFILEBEFORE does not exist."
    grsisort --recommended $f $CFILE_2020_BEFORE --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
    mv -f analysis$h.root $BEFORECALTREESDIR
    mv -f fragment$h.root $BEFORECALTREESDIR

 fi
 if [ ! -f $AFILEAFTER ];  then # This GRSISort command is executed if the analysis tree does not already exist
    echo "File $AFILEAFTER does not exist."
    if [[ $i == "52930" ]]; then
        grsisort --recommended $f $CFILE_2020_20V_AFTER --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
        mv -f analysis$h.root $AFTERCALTREESDIR
        mv -f fragment$h.root $AFTERCALTREESDIR
    else
        grsisort --recommended $f $CFILE_2020_40V_AFTER --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
        mv -f analysis$h.root $AFTERCALTREESDIR
        mv -f fragment$h.root $AFTERCALTREESDIR
    fi
 fi
     
 if [ -f $AFILEBEFORE ]; then
    if [ "$AFILEBEFORE" -ot "$f" ];	then # This GRSISort command is executed if the analaysis tree is older than the MIDAS file
        echo "File $AFILEBEFORE exists but is older than $f"
        grsisort --recommended $f $CFILE_2020_BEFORE --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
        mv -f analysis$h.root $BEFORECALTREESDIR
        mv -f fragment$h.root $BEFORECALTREESDIR
    fi
 fi

 if [ -f $AFILEAFTER ];  then # This GRSISort command is executed if the analysis tree does not already exist
    if [ "$AFILEAFTER" -ot "$f" ];	then # This GRSISort command is executed if the analaysis tree is older than the MIDAS file 
        echo "File $AFILEAFTER exists but is older than $f"
        if [[ $i == "52930" ]]; then
            grsisort --recommended $f $CFILE_2020_20V_AFTER --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
            mv -f analysis$h.root $AFTERCALTREESDIR
            mv -f fragment$h.root $AFTERCALTREESDIR
        else
            grsisort --recommended $f $CFILE_2020_40V_AFTER --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
            mv -f analysis$h.root $AFTERCALTREESDIR
            mv -f fragment$h.root $AFTERCALTREESDIR
        fi
    fi
 fi
done 




for f in $DATADIR_2019/*.mid; do
 g=${f:DLEN_2019+4} # Strips everything in f apart from XXXXX_XXX.mid, which is then stored in g
 h=${g:0:${#g}-4} # h stores the result of stripping .mid from g so h is in the form of XXXXX_XXX
 i=${g:0:${#g}-8} # i stores the result of stripping _XXX.mid from g so i is in the form of XXXXX
 
 if [[ " ${Cal_runs_2019[*]} " =~ " $i " ]]; then
 	FFILEBEFORE=$BEFORECALTREESDIR/fragment$h.root
 	AFILEBEFORE=$BEFORECALTREESDIR/analysis$h.root
    
    FFILEAFTER=$AFTERCALTREESDIR/fragment$h.root
 	AFILEAFTER=$AFTERCALTREESDIR/analysis$h.root
 fi

if [ ! -f $AFILEBEFORE ];  then # This GRSISort command is executed if the analysis tree does not already exist
    echo "File $AFILEBEFORE does not exist."
    grsisort --recommended $f $CFILE_2019_BEFORE --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
    mv -f analysis$h.root $BEFORECALTREESDIR
    mv -f fragment$h.root $BEFORECALTREESDIR

 fi
 if [ ! -f $AFILEAFTER ];  then # This GRSISort command is executed if the analysis tree does not already exist
    echo "File $AFILEAFTER does not exist."
    grsisort --recommended $f $CFILE_2019_AFTER --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
    mv -f analysis$h.root $AFTERCALTREESDIR
    mv -f fragment$h.root $AFTERCALTREESDIR
 fi
     
 if [ -f $AFILEBEFORE ]; then
    if [ "$AFILEBEFORE" -ot "$f" ];	then # This GRSISort command is executed if the analaysis tree is older than the MIDAS file
        echo "File $AFILEBEFORE exists but is older than $f"
        grsisort --recommended $f $CFILE_2019_BEFORE --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
        mv -f analysis$h.root $BEFORECALTREESDIR
        mv -f fragment$h.root $BEFORECALTREESDIR
    fi
 fi

 if [ -f $AFILEAFTER ];  then # This GRSISort command is executed if the analysis tree does not already exist
    if [ "$AFILEAFTER" -ot "$f" ];	then # This GRSISort command is executed if the analaysis tree is older than the MIDAS file 
        echo "File $AFILEAFTER exists but is older than $f"
        grsisort --recommended $f $CFILE_2019_AFTER --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
        mv -f analysis$h.root $AFTERCALTREESDIR
        mv -f fragment$h.root $AFTERCALTREESDIR
    fi
 fi
done 



else

for f in $DATADIR_2020/run"$@"_*.mid; do # If the run number is given as a positional argument to the script then the script cycles through each of the MIDAS files with that run number
 g=${f:DLEN_2020+4}
 h=${g:0:${#g}-4} 
 i=${g:0:${#g}-8} 
 FFILEBEFORE=$BEFORECALTREESDIR/fragment$h.root
 AFILEBEFORE=$BEFORECALTREESDIR/analysis$h.root
 FFILEAFTER=$AFTERCALTREESDIR/fragment$h.root
 AFILEAFTER=$AFTERCALTREESDIR/analysis$h.root
 echo "Processing run$g "

if [ ! -f $AFILEBEFORE ];  then # This GRSISort command is executed if the analysis tree does not already exist
    echo "File $AFILEBEFORE does not exist."
    grsisort --recommended $f $CFILE_2020_BEFORE --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
    mv -f analysis$h.root $BEFORECALTREESDIR
    mv -f fragment$h.root $BEFORECALTREESDIR

 fi
 if [ ! -f $AFILEAFTER ];  then # This GRSISort command is executed if the analysis tree does not already exist
    echo "File $AFILEAFTER does not exist."
    if [[ $i == "52930" ]]; then
        grsisort --recommended $f $CFILE_2020_20V_AFTER --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
        mv -f analysis$h.root $AFTERCALTREESDIR
        mv -f fragment$h.root $AFTERCALTREESDIR
    else
        grsisort --recommended $f $CFILE_2020_40V_AFTER --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
        mv -f analysis$h.root $AFTERCALTREESDIR
        mv -f fragment$h.root $AFTERCALTREESDIR
    fi
 fi
     
 if [ -f $AFILEBEFORE ]; then
    if [ "$AFILEBEFORE" -ot "$f" ];	then # This GRSISort command is executed if the analaysis tree is older than the MIDAS file
        echo "File $AFILEBEFORE exists but is older than $f"
        grsisort --recommended $f $CFILE_2020_BEFORE --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
        mv -f analysis$h.root $BEFORECALTREESDIR
        mv -f fragment$h.root $BEFORECALTREESDIR
    fi
 fi

 if [ -f $AFILEAFTER ];  then # This GRSISort command is executed if the analysis tree does not already exist
    if [ "$AFILEAFTER" -ot "$f" ];	then # This GRSISort command is executed if the analaysis tree is older than the MIDAS file 
        echo "File $AFILEAFTER exists but is older than $f"
        if [[ $i == "52930" ]]; then
            grsisort --recommended $f $CFILE_2020_20V_AFTER --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
            mv -f analysis$h.root $AFTERCALTREESDIR
            mv -f fragment$h.root $AFTERCALTREESDIR
        else
            grsisort --recommended $f $CFILE_2020_40V_AFTER --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
            mv -f analysis$h.root $AFTERCALTREESDIR
            mv -f fragment$h.root $AFTERCALTREESDIR
        fi
    fi
 fi
done



for f in $DATADIR_2019/run"$@"_*.mid; do # If the run number is given as a positional argument to the script then the script cycles through each of the MIDAS files with that run number
 g=${f:DLEN_2019+4}
 h=${g:0:${#g}-4} 
 i=${g:0:${#g}-8}
 FFILEBEFORE=$BEFORECALTREESDIR/fragment$h.root
 AFILEBEFORE=$BEFORECALTREESDIR/analysis$h.root
 FFILEAFTER=$AFTERCALTREESDIR/fragment$h.root
 AFILEAFTER=$AFTERCALTREESDIR/analysis$h.root
 echo "Processing run$g "
if [ ! -f $AFILEBEFORE ];  then # This GRSISort command is executed if the analysis tree does not already exist
    echo "File $AFILEBEFORE does not exist."
    grsisort --recommended $f $CFILE_2019_BEFORE --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
    mv -f analysis$h.root $BEFORECALTREESDIR
    mv -f fragment$h.root $BEFORECALTREESDIR

 fi
 if [ ! -f $AFILEAFTER ];  then # This GRSISort command is executed if the analysis tree does not already exist
    echo "File $AFILEAFTER does not exist."
    grsisort --recommended $f $CFILE_2019_AFTER --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
    mv -f analysis$h.root $AFTERCALTREESDIR
    mv -f fragment$h.root $AFTERCALTREESDIR
 fi
     
 if [ -f $AFILEBEFORE ]; then
    if [ "$AFILEBEFORE" -ot "$f" ];	then # This GRSISort command is executed if the analaysis tree is older than the MIDAS file
        echo "File $AFILEBEFORE exists but is older than $f"
        grsisort --recommended $f $CFILE_2019_BEFORE --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
        mv -f analysis$h.root $BEFORECALTREESDIR
        mv -f fragment$h.root $BEFORECALTREESDIR
    fi
 fi

 if [ -f $AFILEAFTER ];  then # This GRSISort command is executed if the analysis tree does not already exist
    if [ "$AFILEAFTER" -ot "$f" ];	then # This GRSISort command is executed if the analaysis tree is older than the MIDAS file 
        echo "File $AFILEAFTER exists but is older than $f"
        grsisort --recommended $f $CFILE_2019_AFTER --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
        mv -f analysis$h.root $AFTERCALTREESDIR
        mv -f fragment$h.root $AFTERCALTREESDIR
    fi
 fi
done


fi
