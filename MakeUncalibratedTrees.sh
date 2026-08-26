#!/bin/bash

DATADIR_2020=./MidasFiles
DATADIR_2019=./MidasFiles_2019
UNCALDIR=./UncalibratedTrees
DLEN_2020=${#DATADIR_2020}
DLEN_2019=${#DATADIR_2019}
CFILE_RAW=./Calibrations/CalibFileSetup/MyCalibrations/RawCalibrationFile.cal

Cal_runs_2020=(52754 52777 52781 52779 52931 52930 52932 52937 52933 52935)
Cal_runs_2019=(52016 52018 52022 52020 52167 52169 52170 52171 52172)


if [ $# -eq 0 ] # If no positional arguments are passed to the script then the script runs through all MIDAS files
	then
for f in $DATADIR_2020/*.mid; do
 g=${f:DLEN_2020+4} # Strips everything in f apart from XXXXX_XXX.mid, which is then stored in g
 h=${g:0:${#g}-4} # h stores the result of stripping .mid from g so h is in the form of XXXXX_XXX
 i=${g:0:${#g}-8} # i stores the result of stripping _XXX.mid from g so i is in the form of XXXXX
 
 if [[ " ${Cal_runs_2020[*]} " =~ " $i " ]]; then
 	FFILE=$UNCALDIR/fragment$h.root
 	AFILE=$UNCALDIR/analysis$h.root
 fi

if [ ! -f $AFILE ];  then # This GRSISort command is executed if the analysis tree does not already exist
			echo "File $AFILE does not exist."
			grsisort --recommended $f $CFILE_RAW --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
			mv -f analysis$h.root $UNCALDIR
			mv -f fragment$h.root $UNCALDIR

	fi
     
if [ -f $AFILE ]; then

if [ "$AFILE" -ot "$f" ];	then # This GRSISort command is executed if the analaysis tree is older than the MIDAS file
			echo "File $AFILE exists but is older than $f"
			grsisort --recommended $f $CFILE_RAW --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
			mv -f analysis$h.root $UNCALDIR
			mv -f fragment$h.root $UNCALDIR
fi
fi
done 

for f in $DATADIR_2019/*.mid; do
 g=${f:DLEN_2019+4} # Strips everything in f apart from XXXXX_XXX.mid, which is then stored in g
 h=${g:0:${#g}-4} # h stores the result of stripping .mid from g so h is in the form of XXXXX_XXX
 i=${g:0:${#g}-8} # i stores the result of stripping _XXX.mid from g so i is in the form of XXXXX
 
 if [[ " ${Cal_runs_2019[*]} " =~ " $i " ]]; then
 	FFILE=$UNCALDIR/fragment$h.root
 	AFILE=$UNCALDIR/analysis$h.root
 fi

if [ ! -f $AFILE ];  then # This GRSISort command is executed if the analysis tree does not already exist
			echo "File $AFILE does not exist."
			grsisort --recommended $f $CFILE_RAW --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
			mv -f analysis$h.root $UNCALDIR
			mv -f fragment$h.root $UNCALDIR

	fi
     
if [ -f $AFILE ]; then

if [ "$AFILE" -ot "$f" ];	then # This GRSISort command is executed if the analaysis tree is older than the MIDAS file
			echo "File $AFILE exists but is older than $f"
			grsisort --recommended $f $CFILE_RAW --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
			mv -f analysis$h.root $UNCALDIR
			mv -f fragment$h.root $UNCALDIR
fi
fi
done 



else

shopt -s nullglob # This enables nullglob, which makes unmatched globs expand to nothing
for f in $DATADIR_2020/run"$@"_*.mid; do # If the run number is given as a positional argument to the script then the script cycles through each of the MIDAS files with that run number
 g=${f:DLEN_2019+4}
 h=${g:0:${#g}-4} 
 i=${g:0:${#g}-8} 
 FFILE=$UNCALDIR/fragment$h.root
 AFILE=$UNCALDIR/analysis$h.root
 echo "Processing run$g "

if [ ! -f $AFILE ]; then
			echo "File $AFILE does not exist."
			grsisort --recommended $f $CFILE_RAW --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
			mv -f analysis$h.root $UNCALDIR
			mv -f fragment$h.root $UNCALDIR

	fi

if [ -f $AFILE ]; then 
    
if [ "$AFILE" -ot "$f" ];
		then
			echo "File $AFILE exists but is older than $f"
			grsisort --recommended $f $CFILE_RAW --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
			mv -f analysis$h.root $UNCALDIR
			mv -f fragment$h.root $UNCALDIR
fi
fi
done

shopt -s nullglob # This enables nullglob, which makes unmatched globs expand to nothing
for f in $DATADIR_2019/run"$@"_*.mid; do # If the run number is given as a positional argument to the script then the script cycles through each of the MIDAS files with that run number
 g=${f:_2019+4}
 h=${g:0:${#g}-4} 
 i=${g:0:${#g}-8} 
 FFILE=$UNCALDIR/fragment$h.root
 AFILE=$UNCALDIR/analysis$h.root
 echo "Processing run$g "

if [ ! -f $AFILE ]; then
			echo "File $AFILE does not exist."
			grsisort --recommended $f $CFILE_RAW --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
			mv -f analysis$h.root $UNCALDIR
			mv -f fragment$h.root $UNCALDIR

	fi

if [ -f $AFILE ]; then 
    
if [ "$AFILE" -ot "$f" ];
		then
			echo "File $AFILE exists but is older than $f"
			grsisort --recommended $f $CFILE_RAW --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
			mv -f analysis$h.root $UNCALDIR
			mv -f fragment$h.root $UNCALDIR
fi
fi
done


fi
