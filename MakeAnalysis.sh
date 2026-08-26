#!/bin/bash

DATADIR=MidasFiles_2019
FRAGDIR=./FragmentTrees
ANALDIR=./AnalysisTrees
DLEN=${#DATADIR}
p=52885 # The first Nov 2020 run that uses the 20 V calibration file
m=0 # Use this to set the initial lower limit on the run number (i.e. only sort after this run)   
n=52929 # Only sort until this run

if [ $# -eq 0 ] # If no positional arguments are passed to the script then the script runs through all MIDAS files
	then
for f in $DATADIR/*.mid; do
 g=${f:DLEN+4} # Strips everything in f apart from XXXXX_XXX.mid, which is then stored in g
 h=${g:0:${#g}-4} # h stores the result of stripping .mid from g so h is in the form of XXXXX_XXX
 i=${g:0:${#g}-8} # i stores the result of stripping _XXX.mid from g so i is in the form of XXXXX
 FFILE=$FRAGDIR/fragment$h.root
 AFILE=$ANALDIR/analysis$h.root
 
# The first "good", in-beam run in Dec 2019 was 52040 with one hour duration runs beginning from run 52047
# The last in-beam, one hour duration run was 52164
# Therfore the boundaries should be (52039, 52165)

# In the Nov 2020 runs the bias was reduced from 40 V to 20 V from run 52885 onwards
# The first usable Nov 2020 run was 52829
# The final usable Nov 2020 run was 52928
# (Nov2020 40V = 52829 --> 52884)
# (Nov2020 20V = 52885 --> 52928)
# Therefore the boundaries should be (52828, 52885) for 40 V calibration file and (52884, 52929) for 20 V calibration file
 if [[ $i == "52931" ]]; then
    CFILE_40V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal # Run 52931 was the triple alpha calibration run carried out at 40 V bias in 2020
	CFILE_20V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal
	m=52828
	n=52932
 elif [[ $i == "52930" ]]; then
    CFILE_20V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_20V.cal # Run 52930 was the triple alpha calibration run carried out at 20 V bias in 2020
	m=52828
	n=52932
 elif [[ $i == "52167" ]]; then
	CFILE_40V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal # Run 52167 was the triple alpha calibration in 2019
	CFILE_20V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
	m=52046
	n=52168
 elif [[ $i -lt "52165" ]]; then
	CFILE_40V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
	CFILE_20V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
	m=52039 # The first "good", in-beam run was 52040 and in-beam runs ended on 52164 in Dec 2019 so only sorting after 52039 and before 52165
	n=52165
 elif [[ $i -gt "52828" && $i -lt "52929" ]]; then
	CFILE_40V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal
	CFILE_20V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_20V.cal
	m=52828 # One hour duration runs began from run 52829 and ended on 52928 in Nov 2020 so only sorting after 52828 and before 52929
	n=52929
 else
	if [[ $i -gt "52753" ]]; then # Determines if the run is in 2020
		m=52828 # Resets the value of m to be the lower limit for the Nov 2020 runs. This prevents runs between the 2019 triple alpha calibration and run 52828 being sorted.
		n=52929 # Resets the value of n to be the upper limit for the Nov 2020 1 hour runs. This prevents runs after 52928 from being sorted, other than the 2020 triple alpha calibration ones.
	else # Otherwise it is in 2019
		m=52046
		n=52165
	fi
 fi

if [ ! -f $AFILE ] && [[ $i -gt $m && $i -lt $n ]];  then # This GRSISort command is executed if the analysis tree does not already exist and the run number is between m and n
			echo "File $AFILE does not exist."
			if [ $i -lt $p ]; then
				#grsisort -laq $f $CFILE_40V --suppress-errors --write-frag-tree --word-count-offset=-1 --sort-depth 1000000000 --downscaling=1	 # Online sort GRSISort command
				grsisort --recommended $f $CFILE_40V --build-window 2500 --word-count-offset=0 --sort-depth 1000000000 	# Thomas Chillery's GRSISort command
			elif [ $i -ge $p ]; then
				#grsisort -laq $f $CFILE_20V --suppress-errors --write-frag-tree --word-count-offset=-1 --sort-depth 1000000000 --downscaling=1
				grsisort --recommended $f $CFILE_20V --build-window 2500 --word-count-offset=0 --sort-depth 1000000000
			fi
			mv -f analysis$h.root $ANALDIR
			mv -f fragment$h.root $FRAGDIR

	fi
     
if [ -f $AFILE ]; then

if [ "$AFILE" -ot "$f" ] && [[ $i -gt $m && $i -lt $n ]];	then # This GRSISort command is executed if the analaysis tree is older than the MIDAS file and the run number is between m and n
			echo "File $AFILE exists but is older than $f"
			if [ $i -lt $p ]; then
				#grsisort -laq $f $CFILE_40V --suppress-errors --write-frag-tree --word-count-offset=-1 --sort-depth 1000000000 --downscaling=1
				grsisort --recommended $f $CFILE_40V --build-window 2500 --word-count-offset=0 --sort-depth 1000000000
			elif [ $i -ge $p ]; then
				#grsisort -laq $f $CFILE_20V --suppress-errors --write-frag-tree --word-count-offset=-1 --sort-depth 1000000000 --downscaling=1
				grsisort --recommended $f $CFILE_20V --build-window 2500 --word-count-offset=0 --sort-depth 1000000000
			fi
			mv -f analysis$h.root $ANALDIR
			mv -f fragment$h.root $FRAGDIR
fi
fi
done 

else 
for f in $DATADIR/run"$@"_*.mid; do # If the run number is given as a positional argument to the script then the script cycles through each of the MIDAS files with that run number

 g=${f:DLEN+4}
 h=${g:0:${#g}-4} 
 i=${g:0:${#g}-8} 
 FFILE=$FRAGDIR/fragment$h.root
 AFILE=$ANALDIR/analysis$h.root
 echo "Processing run$g "

 if [[ $i == "52931" ]]; then
    CFILE_40V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal # Run 52931 was the triple alpha calibration run carried out at 40 V bias in 2020
	m=52828
 elif [[ $i == "52930" ]]; then
    CFILE_20V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_20V.cal # Run 52930 was the triple alpha calibration run carried out at 20 V bias in 2020
	m=52828
 elif [[ $i == "52167" ]]; then
	CFILE_40V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal # Run 52167 was the triple alpha calibration in 2019
	CFILE_20V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
	m=52046
 elif [[ $i -lt "52165" ]]; then
	CFILE_40V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
	CFILE_20V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2019.cal
	m=52046 # One hour duration runs began from run 52047 in Dec 2019 so only sorting after 52046
 elif [[ $i -gt "52828" ]]; then
	CFILE_40V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal
	CFILE_20V=Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_20V.cal
	m=52828 # One hour duration runs began from run 52829 in Nov 2020 so only sorting after 52828
 else
	m=52828 # Resets the value of m to be the lower limit for the Nov 2020 runs. This prevents runs between the 2019 triple alpha calibration and run 52828 being sorted.
 fi

if [ ! -f $AFILE ]; then
			echo "File $AFILE does not exist."
			if [ $i -lt $p ]; then
				#grsisort -laq $f $CFILE_40V --suppress-errors --write-frag-tree --word-count-offset=-1 --sort-depth 1000000000 --downscaling=1
				grsisort --recommended $f $CFILE_40V --build-window 2500 --word-count-offset=0 --sort-depth 1000000000
			elif [ $i -ge $p ]; then
				#grsisort -laq $f $CFILE_20V --suppress-errors --write-frag-tree --word-count-offset=-1 --sort-depth 1000000000 --downscaling=1
				grsisort --recommended $f $CFILE_20V --build-window 2500 --word-count-offset=0 --sort-depth 1000000000
			fi
			mv -f analysis$h.root $ANALDIR
			mv -f fragment$h.root $FRAGDIR

	fi

if [ -f $AFILE ]; then 
    
if [ "$AFILE" -ot "$f" ] && [[ $i -gt $m && $i -lt $n ]];
		then
			echo "File $AFILE exists but is older than $f"
			if [ $i -lt $p ]; then
				#grsisort -laq $f $CFILE_40V --suppress-errors --write-frag-tree --word-count-offset=-1 --sort-depth 1000000000 --downscaling=1
				grsisort --recommended $f $CFILE_40V --build-window 2500 --word-count-offset=0 --sort-depth 1000000000
			elif [ $i -ge $p ]; then
				#grsisort -laq $f $CFILE_20V --suppress-errors --write-frag-tree --word-count-offset=-1 --sort-depth 1000000000 --downscaling=1
				grsisort --recommended $f $CFILE_20V --build-window 2500 --word-count-offset=0 --sort-depth 1000000000
			fi			
			mv -f analysis$h.root $ANALDIR
			mv -f fragment$h.root $FRAGDIR
fi
fi
done
fi
