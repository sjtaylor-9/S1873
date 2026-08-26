//g++ Conf_Setup.cxx -std=c++0x -o SetupConfFile

#include "CalibrationParameters_2020_40V.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

int num_known_exp = 7; //number of known experiments
string exp_names[] = {
  "tigress",
  "emma",
  "emmas3",
  "griffin",
  "bambino",
  "descant",
  "tip"
};
string exp_description[] = {
  "TIGRESS 16 Clover Configuration",
  "TIGRESS 12 Clovers + SSB + EMMA Focal Plane",
  "TIGRESS 12 Clovers + Upstream S3 + SSB + EMMA Focal Plane",
  "GRIFFIN 16 Clover + PACES + SCEPTAR + ZDS + LaBr3",
  "TIGRESS 16 Clover + 2 S3 Detectors Standard BAMBINO Arrangment",
  "GRIFFIN 16 Clover + PACES + SCEPTAR + ZDS + LaBr3 + DESCANT",
  "TIGRESS 16 Clover + TIP"
};

int search_array(string array[], string search, int len) {
	for (int i = 0; i < len; i++) {
		if (search == array[i]) {
			return i;
		}//if
	}//for
	
	return -1;
}//search_array

bool CreateConfFile(const char * experiment, const char * MSC = "NULL", const char * seginp = "NULL") {

  char line[128];
  char var [64];
  int DetType = 0;
  int DetNum = 0;
  int ab = 0;
  int channel = 0;
  int collector = 0;
  int port = 0;
  int cryNum = 0;
  int aNum = 0;
  int chancounter = 0;
  int ring[24] = {23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0}; // Ring Order of S3s
  
  std::vector<std::string> MNEMONIC;
  std::string tempmnemonic;
  std::vector<int> customcollector, customport, customchannel;

  ifstream custom;
  custom.open("custom.dat");
  if(custom.is_open()){
    while(!custom.eof()){
      custom >> std::hex >> tempmnemonic >> collector >> port >> channel;
      MNEMONIC.push_back(tempmnemonic);
      customcollector.push_back(collector);
      customport.push_back(port);
      customchannel.push_back(channel);
    }
  }
  else printf("File custom.dat not found\nCustom channel settings will not be used\n");

  printf("Creating conf file: Conf_File.txt\n");
  ofstream outfile;
  outfile.open("Conf_File.txt");
  if (!outfile.is_open()) {
    printf("Output not opened\n");
    return false;
  }

  ofstream mscnames;
  bool msc_flag = false;
  const char * testval = "NULL";
  if (strcmp(MSC, testval) != 0) {
    printf("Creating %s\n",MSC);
    mscnames.open(MSC);
    if (!mscnames.is_open()) {
      printf("Failed to create MSCTable, will not be filled\n");
      msc_flag = false;
    } else msc_flag = true;
  }
  int expID = search_array(exp_names, experiment, num_known_exp);

  switch (expID) {
  case 0:
    if (strcmp(seginp, testval) != 0) {
      ifstream segpar;
      segpar.open(seginp);
      if (segpar.is_open()) {
        printf("Segment parameter file: %s opened!\n", seginp);
        int j = 0;
        while (!segpar.eof() && j < 960) {
          segpar >> segoffsets[j] >> seggains[j];
          j++;
        }
      } else {
        printf("%s not opened\n", seginp);
        return false;
      }
      printf("Segment parameters read in successfully\n");
    } else {
      printf("No segment parameter file declared, setting gains, offsets and nonlinear components to zero\n");
      for (int i = 0; i < 960; i++) {
        seggains[i] = 1;
        segoffsets[i] = 0;
      }
    }

    printf("Creating TIGRESS 16 Clovers\n");

    for (int i = 0; i < 1024; i++) {
      DetNum = (i / 64) + 1;
      port = (i % 256) / 16;
      channel = i % 16;
      cryNum = (port % 4);
      collector = (i / 256);
      char electronicaddress[32];
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      if (channel < 15) {

        char colour[1];

        if (cryNum == 0) sprintf(colour, "B");
        else if (cryNum == 1) sprintf(colour, "G");
        else if (cryNum == 2) sprintf(colour, "R");
        else if (cryNum == 3) sprintf(colour, "W");

        if (channel < 8) {
          DetType = 2;
          sprintf(var, "TIG%2.2i%sP%2.2ix", DetNum, colour, channel + 1);
        } else if (channel == 8) {
          DetType = 1;
          sprintf(var, "TIG%2.2i%sN00B", DetNum, colour);
        } else if (channel == 9) {
          DetType = 0;
          sprintf(var, "TIG%2.2i%sN00A", DetNum, colour);
        } else if (channel < 15 && channel > 9) {
          DetType = 3;
          sprintf(var, "TIS%2.2i%sN%2.2ix", DetNum, colour, channel - 9);
        }
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
        aNum = ((DetNum) - 1) * 4 + cryNum;
        if (channel == 9) outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << gain[aNum] << "\t" << offset[aNum] << "\t" << non_lin[aNum] << "\t" << "\tGRF16\n";
        else outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\t" << "\tGRF16\n";
        if (msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
          mscnames << line << "\n";
        }
        chancounter++;
      }
    }

    DetType=15;
    channel=15;
    port=0;
    sprintf(var,"RFL00XS00x");
    char electronicaddress[32];
    sprintf(electronicaddress, "0x%01x%02x", port, channel);
    for(int m = 0; m < MNEMONIC.size(); m++) { 
      if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
        sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
      }
    }
    outfile << chancounter << "\t" << electronicaddress << "\t" <<  var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
    if(msc_flag) {
      sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
      mscnames << line << "\n";
      chancounter++;
    }
    break;

  case 1:
    if (strcmp(seginp, testval) != 0) {
      ifstream segpar;
      segpar.open(seginp);
      if (segpar.is_open()) {
        printf("Segment parameter file: %s opened!\n", seginp);
        int j = 0;
        while (!segpar.eof() && j < 960) {
          segpar >> segoffsets[j] >> seggains[j];
          j++;
        }
      } else {
        printf("%s not opened\n", seginp);
        return false;
      }
      printf("Segment parameters read in successfully\n");
    } else {
      printf("No segment parameter file declared, setting gains, offsets and nonlinear components to zero\n");
      for (int i = 0; i < 960; i++) {
        seggains[i] = 1;
        segoffsets[i] = 0;
      }
    }
    printf("Creating TIGRESS-EMMA 12 Clovers\n");

    for (int i = 0; i < 768; i++) {
      DetNum = (i / 64) + 5;
      port = (i % 256) / 16;
      channel = i % 16;
      cryNum = (port % 4);
      collector = (i / 256);
      char electronicaddress[32];
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      if (channel < 15) {

        char colour[1];

        if (cryNum == 0) sprintf(colour, "B");
        else if (cryNum == 1) sprintf(colour, "G");
        else if (cryNum == 2) sprintf(colour, "R");
        else if (cryNum == 3) sprintf(colour, "W");

        if (channel < 8) {
          DetType = 2;
          sprintf(var, "TIG%2.2i%sP%2.2ix", DetNum, colour, channel + 1);
        } else if (channel == 8) {
          DetType = 1;
          sprintf(var, "TIG%2.2i%sN00B", DetNum, colour);
        } else if (channel == 9) {
          DetType = 0;
          sprintf(var, "TIG%2.2i%sN00A", DetNum, colour);
        } else if (channel < 15 && channel > 9) {
          DetType = 3;
          sprintf(var, "TIS%2.2i%sN%2.2ix", DetNum, colour, channel - 9);
        }
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
        aNum = ((DetNum) - 1) * 4 + cryNum;
        if (channel == 9) outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << gain[aNum] << "\t" << offset[aNum] << "\t" << non_lin[aNum] << "\tGRF16\n";
        else outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 <<  "\tGRF16\n";
        if (msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
          mscnames << line << "\n";
        }
        chancounter++;
      }
    }

    DetType=15;
    channel=15;
    port=0;
    sprintf(var,"RFL00XS00x");
    sprintf(electronicaddress, "0x%01x%02x", port, channel);
    for(int m = 0; m < MNEMONIC.size(); m++) { 
      if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
        sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
      }
    }
    outfile << chancounter << "\t" << electronicaddress << "\t" <<  var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
    if(msc_flag) {
      sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
      mscnames << line << "\n";
    }
    chancounter++;

    DetType=5;
    channel=15;
    port=1;
    sprintf(var,"EMT00XP00x");
    sprintf(electronicaddress, "0x%01x%02x", port, channel);
    for(int m = 0; m < MNEMONIC.size(); m++) { 
      if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
        sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
      }
    }
    outfile << chancounter << "\t" << electronicaddress << "\t" <<  var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
    if(msc_flag) {
      sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
      mscnames << line << "\n";
    }
    chancounter++;

    DetType=4;
    channel=15;
    for (int j = 0; j<2; j++) { 
      port=2+j;
      sprintf(var,"ETO%02xXP00x",j+1);
      sprintf(electronicaddress, "0x%01x%02x", port, channel);
      for(int m = 0; m < MNEMONIC.size(); m++) { 
	if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
          sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
        }
      }
      outfile << chancounter << "\t" << electronicaddress << "\t" <<  var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
      if(msc_flag) {
        sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
        mscnames << line << "\n";
      }
    chancounter++;
    }
    break;


  case 2:
    if (strcmp(seginp, testval) != 0) {
      ifstream segpar;
      segpar.open(seginp);
      if (segpar.is_open()) {
        printf("Segment parameter file: %s opened!\n", seginp);
        int j = 0;
        while (!segpar.eof() && j < 512) {
          segpar >> segoffsets[j] >> seggains[j];
          j++;
        }
      } else {
        printf("%s not opened\n", seginp);
        return false;
      }
      printf("Segment parameters read in successfully\n");
    } else {
      printf("No segment parameter file declared, setting gains, offsets and nonlinear components to zero\n");
      for (int i = 0; i < 512; i++) {
        seggains[i] = 1;
        segoffsets[i] = 0;
      }
    }
/*
    for (int i = 0; i < 60; i++) {
      s3gains[i] = 1;
      s3offsets[i] = 0;
    }
*/
    printf("Creating TIGRESS-EMMA 12 Clovers + Upstream S3\n");

    for (int i = 0; i < 768; i++) {
      DetNum = (i / 64) + 5;
      port = (i % 256) / 16;
      channel = i % 16;
      cryNum = (port % 4);
      collector = (i / 256) + 1;
      char electronicaddress[32];
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      if (channel < 15) {

        char colour[2];

        if (cryNum == 0) sprintf(colour, "B");
        else if (cryNum == 1) sprintf(colour, "G");
        else if (cryNum == 2) sprintf(colour, "R");
        else if (cryNum == 3) sprintf(colour, "W");

        if (channel < 8) {
          DetType = 2;
          sprintf(var, "TIG%2.2i%sP%2.2ix", DetNum, colour, channel + 1);
        } else if (channel == 8) {
          DetType = 1;
          sprintf(var, "TIG%2.2i%sN00B", DetNum, colour);
        } else if (channel == 9) {
          DetType = 0;
          sprintf(var, "TIG%2.2i%sN00A", DetNum, colour);
        } else if (channel < 15 && channel > 9) {
          DetType = 3;
          sprintf(var, "TIS%2.2i%sN%2.2ix", DetNum, colour, channel - 9);
        }
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
        aNum = ((DetNum) - 1) * 4 + cryNum;
        if (channel == 9) outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << gain[aNum] << "\t" << offset[aNum] << "\t" << non_lin[aNum] << "\tGRF16\n";
        else outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 <<  "\tGRF16\n";
        if (msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
          mscnames << line << "\n";
        }
        chancounter++;
      }
    }

    for (int i = 0; i < 60; i++) {
      port = (i % 256)/16 ;
      channel = i % 16;
      collector = (i/256);
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      if(i<32) {
        DetType = 4;
        sprintf(var,"ETE01EP%02iX",i);
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
        outfile << chancounter << "\t" << electronicaddress << "\t" <<  var << "\t" << s3gains[i] << "\t" << s3offsets[i] << "\t" << 0 << "\tGRF16\n";
        if(msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
          mscnames << line << "\n";
        }
        chancounter++;
      }
      else if (i>31 && i < 56) {
        DetType = 5;
        sprintf(var,"ETE01EN%02iX",ring[i-32]);
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
        outfile << chancounter << "\t" << electronicaddress << "\t" <<  var << "\t" << s3gains[i] << "\t" << s3offsets[i] << "\t" << 0 << "\tGRF16\n";
        if(msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
          mscnames << line << "\n";
        }
        chancounter++;
      }
    }

    DetType=15;
    channel=15;
    port=0;
    collector=1;
    sprintf(var,"RFL00XS00x");
    sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel); 
    for(int m = 0; m < MNEMONIC.size(); m++) { 
      if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
        sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
      }
    }
    outfile << chancounter << "\t" << electronicaddress << "\t" <<  var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
    if(msc_flag) {
      sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
      mscnames << line << "\n";
    }
    chancounter++;

    DetType=7;
    channel=15;
    port=1;
    collector=1;
    sprintf(var,"EMT00XP00x");
    sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
    for(int m = 0; m < MNEMONIC.size(); m++) { 
      if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
        sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
      }
    }
    outfile << chancounter << "\t" << electronicaddress << "\t" <<  var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
    if(msc_flag) {
      sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
      mscnames << line << "\n";
    }
    chancounter++;

    DetType=6;
    channel=15;
    collector=1;
    for (int j = 0; j<2; j++) { 
      port=2+j;
      sprintf(var,"ETO%02xXP00x",j+1);
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      for(int m = 0; m < MNEMONIC.size(); m++) { 
        if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
          sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
        }
      }
      outfile << chancounter << "\t" << electronicaddress << "\t" <<  var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
      if(msc_flag) {
        sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
        mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
      mscnames << line << "\n";

      }
      chancounter++;
    }
    break;

  case 3:

    printf("Creating GRIFFIN 16 Clovers + PACES + ZDS + SCEPTAR + Labr3\n");

    for (int i = 0; i < 512; i++) {
      DetNum = (i / 32) + 1;
      port = (i % 256) / 16;
      channel = i % 16;
      ab = port%2;
      if(channel == 0) cryNum = 0;
      if(channel == 1) cryNum = 1;
      if(channel == 2) cryNum = 2;
      if(channel == 3) cryNum = 3;
      if(channel < 10 && channel > 4){
        if(ab==0) cryNum = 0;
        else cryNum = 2;
      }
      else if(channel < 15 && channel > 9){
        if(ab==0) cryNum = 1;
        else cryNum = 3;
      }
      collector = (i / 256);
      char electronicaddress[32];
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      if (channel < 15 && channel != 4 ) {
        char colour[1];
        if (cryNum == 0) sprintf(colour, "B");
        else if (cryNum == 1) sprintf(colour, "G");
        else if (cryNum == 2) sprintf(colour, "R");
        else if (cryNum == 3) sprintf(colour, "W");

        if (channel < 4 && ab == 0) {
          DetType = 0;
          sprintf(var, "GRG%2.2i%sN00A", DetNum, colour);
        } else if (channel < 4 && ab != 0) {
          DetType = 1;
          sprintf(var, "GRG%2.2i%sN00B", DetNum, colour);
        } else if (channel > 4 && channel < 10) {
          DetType = 7;
          sprintf(var, "GRS%2.2i%sN%2.2iX", DetNum, colour,channel - 4);
        } else if (channel < 15 && channel > 9) {
          DetType = 7;
          sprintf(var, "GRS%2.2i%sN%2.2iX", DetNum, colour, channel - 9);
        }
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
        aNum = (DetNum -1) * 4 + cryNum;
        if (channel < 4) outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << gain[aNum] << "\t" << offset[aNum] << "\t" << non_lin[aNum] << "\tGRF16\n";
        else outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
        if (msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
	  mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/digitizer[%i]\" 'GRF16'", chancounter);
   	  mscnames << line << "\n";
        }
        chancounter++;
      }
    }

    for(int i = 0; i < 32; i++) {
      DetType = 2;
      collector = 2;
      port = (i % 256) / 16;
      channel = i % 16;
      char electronicaddress[32];
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      if(channel < 10) {
	DetNum = channel + 1 + port * 10;
        sprintf(var, "SEP%2.2iXN00X", DetNum);
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
	outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
        if (msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/digitizer[%i]\" 'GRF16'", chancounter);
          mscnames << line << "\n";
        }
        chancounter++;
      }
    }

    for(int i = 0; i < 16; i++) {
      collector = 2;
      port = (i % 256) / 16 + 2;
      channel = i % 16;
      char electronicaddress[32];
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      if(channel== 0) {
        DetType = 9;
        sprintf(var, "ZDS01XN00A");
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
	outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
        if (msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/digitizer[%i]\" 'GRF16'", chancounter);
	  mscnames << line << "\n";
        }
      chancounter++;
      }
      if(channel>10) {
        DetType = 5;
	DetNum = channel - 10;
        sprintf(var, "PAC%2.2iXN00A", DetNum);
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
	outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << paces_gain[DetNum - 1] << "\t" << paces_offset[DetNum - 1] << "\t" << 0 << "\tGRF16\n";
        if (msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/digitizer[%i]\" 'GRF16'", chancounter);
	  mscnames << line << "\n";
        }
      chancounter++;
      }
    }
    for(int i = 0; i < 48; i++) {
      collector = 2;
      port = (i % 256) / 16 + 4;
      channel = i % 16;
      char electronicaddress[32];
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      if(i < 32) { 
        if(channel < 4) {
          DetType = 3;
	  DetNum = channel + 1 + (port - 4) * 4;
          sprintf(var, "LBL%2.2iXN00X", DetNum);
	  for(int m = 0; m < MNEMONIC.size(); m++) { 
	    if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
              sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	    }
	  }
	  outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
          if (msc_flag) {
            sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
	    mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/digitizer[%i]\" 'GRF16'", chancounter);
	    mscnames << line << "\n";
          }
          chancounter++;
        }
        else {
          DetType = 8;
	  char colour[1];
          DetNum = (channel - 4) / 3 + 1 + 4*(port-4); 
          //if (channel > 4) DetNum = (channel - 4) / 3 + 1 + port-3); 
	  cryNum = (channel - 4) % 3;
	  if(cryNum == 0) sprintf(colour, "A");
	  else if(cryNum == 1) sprintf(colour, "B");
	  else if(cryNum == 2) sprintf(colour, "C");
          sprintf(var, "LBS%2.2i%sN00X", DetNum, colour);
	  for(int m = 0; m < MNEMONIC.size(); m++) { 
	    if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
              sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	    }
	  }
	  outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
          if (msc_flag) {
            sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
	    mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/digitizer[%i]\" 'GRF16'", chancounter);
	    mscnames << line << "\n";
          }
          chancounter++;
	}
      }
      else {
	if(channel % 2 == 0) {
          DetType = 4;
	  DetNum = channel / 2 + 1;
          sprintf(var, "LBT%2.2iXT00X", DetNum);
	  for(int m = 0; m < MNEMONIC.size(); m++) { 
	    if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
              sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	    }
	  }
	  outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
          if (msc_flag) {
            sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
	    mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/digitizer[%i]\" 'GRF16'", chancounter);
	    mscnames << line << "\n";
          }
          chancounter++;
	}
      }
    }
    break;

  case 4:
    if (strcmp(seginp, testval) != 0) {
      ifstream segpar;
      segpar.open(seginp);
      if (segpar.is_open()) {
        printf("Segment parameter file: %s opened!\n", seginp);
        int j = 0;
        while (!segpar.eof() && j < 960) {
          segpar >> segoffsets[j] >> seggains[j];
          j++;
        }
      } else {
        printf("%s not opened\n", seginp);
        return false;
      }
      printf("Segment parameters read in successfully\n");
    } else {
      printf("No segment parameter file declared, setting gains, offsets and nonlinear components to zero\n");
      for (int i = 0; i < 960; i++) {
        seggains[i] = 1;
        segoffsets[i] = 0;
      }
    }
/*
    for (int i = 0; i < 60; i++) {
      s3gains[i] = 1;
      s3offsets[i] = 0;
    }
*/
    printf("Creating TIGRESS + Standard Bambino \n");

    for (int i = 0; i < 1024; i++) {
      DetNum = (i / 64) + 1;
      port = (i % 256) / 16;
      channel = i % 16;
      cryNum = (port % 4);
      collector = (i / 256);
      char electronicaddress[32];
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      if (channel < 15) {

        char colour[1];

        if (cryNum == 0) sprintf(colour, "B");
        else if (cryNum == 1) sprintf(colour, "G");
        else if (cryNum == 2) sprintf(colour, "R");
        else if (cryNum == 3) sprintf(colour, "W");

        if (channel < 8) {
          DetType = 2;
          sprintf(var, "TIG%2.2i%sP%2.2ix", DetNum, colour, channel + 1);
        } else if (channel == 8) {
          DetType = 1;
          sprintf(var, "TIG%2.2i%sN00B", DetNum, colour);
        } else if (channel == 9) {
          DetType = 0;
          sprintf(var, "TIG%2.2i%sN00A", DetNum, colour);
        } else if (channel < 15 && channel > 9) {
          DetType = 3;
          sprintf(var, "TIS%2.2i%sN%2.2ix", DetNum, colour, channel - 9);
        }
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
        aNum = ((DetNum) - 1) * 4 + cryNum;
        if (channel == 9) outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << gain[aNum] << "\t" << offset[aNum] << "\t" << non_lin[aNum] << "\tGRF16\n";
        else outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 <<  "\tGRF16\n";
        if (msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
          mscnames << line << "\n";
        }
        chancounter++;
      }
    }

    for (int i = 0; i < 128; i++) {
      port = (i % 256)/16 ;
      channel = i % 16;
      collector = (i/256) + 4;
      DetNum = i / 64 + 1;
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      if((i % 64)<32) {
        DetType = 4;
        sprintf(var,"BAE%02iEP%02iX",DetNum, i % 64);
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
        outfile << chancounter << "\t" << electronicaddress << "\t" <<  var << "\t" << s3gains[i] << "\t" << s3offsets[i] << "\t" << 0 << "\tGRF16\n";
        if(msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
          mscnames << line << "\n";
        }
        chancounter++;
      }
      else if ((i % 64) > 31 && (i % 64) < 56) {
        DetType = 5;
        sprintf(var,"BAE%02iEN%02iX",DetNum,ring[(i % 64)-32]);
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
        outfile << chancounter << "\t" << electronicaddress << "\t" <<  var << "\t" << s3gains[i] << "\t" << s3offsets[i] << "\t" << 0 << "\tGRF16\n";
        if(msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
          mscnames << line << "\n";
        }
        chancounter++;
      }
    }

    DetType=15;
    channel=15;
    port=0;
    collector=1;
    sprintf(var,"RFL00XS00x");
    sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel); 
    for(int m = 0; m < MNEMONIC.size(); m++) { 
      if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
        sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
      }
    }
    outfile << chancounter << "\t" << electronicaddress << "\t" <<  var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
    if(msc_flag) {
      sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
      mscnames << line << "\n";
    }
    chancounter++;

    break;

  case 5:

    printf("Creating GRIFFIN 16 Clovers + PACES + ZDS + SCEPTAR + Labr3 + DESCANT\n");

    for (int i = 0; i < 512; i++) {
      DetNum = (i / 32) + 1;
      port = (i % 256) / 16;
      channel = i % 16;
      ab = port%2;
      if(channel == 0) cryNum = 0;
      if(channel == 1) cryNum = 1;
      if(channel == 2) cryNum = 2;
      if(channel == 3) cryNum = 3;
      if(channel < 10 && channel > 4){
        if(ab==0) cryNum = 0;
        else cryNum = 2;
      }
      else if(channel < 15 && channel > 9){
        if(ab==0) cryNum = 1;
        else cryNum = 3;
      }
      collector = (i / 256);
      char electronicaddress[32];
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      if (channel < 15 && channel != 4 ) {
        char colour[1];
        if (cryNum == 0) sprintf(colour, "B");
        else if (cryNum == 1) sprintf(colour, "G");
        else if (cryNum == 2) sprintf(colour, "R");
        else if (cryNum == 3) sprintf(colour, "W");

        if (channel < 4 && ab == 0) {
          DetType = 0;
          sprintf(var, "GRG%2.2i%sN00A", DetNum, colour);
        } else if (channel < 4 && ab != 0) {
          DetType = 1;
          sprintf(var, "GRG%2.2i%sN00B", DetNum, colour);
        } else if (channel > 4 && channel < 10) {
          DetType = 7;
          sprintf(var, "GRS%2.2i%sN%2.2iX", DetNum, colour,channel - 4);
        } else if (channel < 15 && channel > 9) {
          DetType = 7;
          sprintf(var, "GRS%2.2i%sN%2.2iX", DetNum, colour, channel - 9);
        }
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
        aNum = (DetNum -1) * 4 + cryNum;
        if (channel < 4) outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << gain[aNum] << "\t" << offset[aNum] << "\t" << non_lin[aNum] << "\tGRF16\n";
        else outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
        if (msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
	  mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/digitizer[%i]\" 'GRF16'", chancounter);
   	  mscnames << line << "\n";
        }
        chancounter++;
      }
    }

    for(int i = 0; i < 32; i++) {
      DetType = 2;
      collector = 2;
      port = (i % 256) / 16;
      channel = i % 16;
      char electronicaddress[32];
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      if(channel < 10) {
	DetNum = channel + 1 + port * 10;
        sprintf(var, "SEP%2.2iXN00X", DetNum);
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
	outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
        if (msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/digitizer[%i]\" 'GRF16'", chancounter);
          mscnames << line << "\n";
        }
        chancounter++;
      }
    }

    for(int i = 0; i < 16; i++) {
      collector = 2;
      port = (i % 256) / 16 + 2;
      channel = i % 16;
      char electronicaddress[32];
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      if(channel== 0) {
        DetType = 9;
        sprintf(var, "ZDS01XN00A");
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
	outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
        if (msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/digitizer[%i]\" 'GRF16'", chancounter);
	  mscnames << line << "\n";
        }
      }
      if(channel>10) {
        DetType = 5;
	DetNum = channel - 10;
        sprintf(var, "PAC%2.2iXN00A", DetNum);
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
	outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << paces_gain[DetNum - 1] << "\t" << paces_offset[DetNum - 1] << "\t" << 0 << "\tGRF16\n";
        if (msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/digitizer[%i]\" 'GRF16'", chancounter);
	  mscnames << line << "\n";
        }
      }
      chancounter++;
    }
    for(int i = 0; i < 48; i++) {
      collector = 2;
      port = (i % 256) / 16 + 4;
      channel = i % 16;
      char electronicaddress[32];
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      if(i < 32) { 
        if(channel < 4) {
          DetType = 3;
	  DetNum = channel + 1 + (port - 4) * 4;
          sprintf(var, "LBL%2.2iXN00X", DetNum);
	  for(int m = 0; m < MNEMONIC.size(); m++) { 
	    if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
              sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	    }
	  }
	  outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
          if (msc_flag) {
            sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
	    mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/digitizer[%i]\" 'GRF16'", chancounter);
	    mscnames << line << "\n";
          }
          chancounter++;
        }
        else {
          DetType = 8;
	  char colour[1];
          DetNum = (channel - 4) / 3 + 1 + 4*(port-4); 
          //if (channel > 4) DetNum = (channel - 4) / 3 + 1 + port-3); 
	  cryNum = (channel - 4) % 3;
	  if(cryNum == 0) sprintf(colour, "A");
	  else if(cryNum == 1) sprintf(colour, "B");
	  else if(cryNum == 2) sprintf(colour, "C");
          sprintf(var, "LBS%2.2i%sN00X", DetNum, colour);
	  for(int m = 0; m < MNEMONIC.size(); m++) { 
	    if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
              sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	    }
	  }
	  outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
          if (msc_flag) {
            sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
	    mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/digitizer[%i]\" 'GRF16'", chancounter);
	    mscnames << line << "\n";
          }
          chancounter++;
	}
      }
      else {
	if(channel % 2 == 0) {
          DetType = 4;
	  DetNum = channel / 2 + 1;
          sprintf(var, "LBT%2.2iXT00X", DetNum);
	  for(int m = 0; m < MNEMONIC.size(); m++) { 
	    if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
              sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	    }
	  }
	  outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
          if (msc_flag) {
            sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
            mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
	    mscnames << line << "\n";
            sprintf(line, "set \"/DAQ/MSC/digitizer[%i]\" 'GRF16'", chancounter);
	    mscnames << line << "\n";
          }
          chancounter++;
	}
      }
    }

    for (int i = 0; i < 70; i++) {
      collector = 8 + i / 48;
      DetNum = i + 1;
      port = ((i / 16 + 1) * 4) % 16;
      channel = i % 16;

      char electronicaddress[32];
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      DetType = 6;
      sprintf(var, "DSC%2.2iXN00X", DetNum);
      for(int m = 0; m < MNEMONIC.size(); m++) { 
	if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
          sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	}
      }
      outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tCAEN\n";
      if (msc_flag) {
        sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
	mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/digitizer[%i]\" 'CAEN'", chancounter);
        mscnames << line << "\n";
      }
      chancounter++;
    }

    break;

  case 6:
    if (strcmp(seginp, testval) != 0) {
      ifstream segpar;
      segpar.open(seginp);
      if (segpar.is_open()) {
        printf("Segment parameter file: %s opened!\n", seginp);
        int j = 0;
        while (!segpar.eof() && j < 960) {
          segpar >> segoffsets[j] >> seggains[j];
          j++;
        }
      } else {
        printf("%s not opened\n", seginp);
        return false;
      }
      printf("Segment parameters read in successfully\n");
    } else {
      printf("No segment parameter file declared, setting gains, offsets and nonlinear components to zero\n");
      for (int i = 0; i < 960; i++) {
        seggains[i] = 1;
        segoffsets[i] = 0;
      }
    }
/*
    for (int i = 0; i < 60; i++) {
      s3gains[i] = 1;
      s3offsets[i] = 0;
    }
*/
    printf("Creating TIGRESS + TIP \n");

    for (int i = 0; i < 1024; i++) {
      DetNum = (i / 64) + 1;
      port = (i % 256) / 16;
      channel = i % 16;
      cryNum = (port % 4);
      collector = (i / 256);
      char electronicaddress[32];
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      if (channel < 15) {

        char colour[1];

        if (cryNum == 0) sprintf(colour, "B");
        else if (cryNum == 1) sprintf(colour, "G");
        else if (cryNum == 2) sprintf(colour, "R");
        else if (cryNum == 3) sprintf(colour, "W");

        if (channel < 8) {
          DetType = 2;
          sprintf(var, "TIG%2.2i%sP%2.2ix", DetNum, colour, channel + 1);
        } else if (channel == 8) {
          DetType = 1;
          sprintf(var, "TIG%2.2i%sN00B", DetNum, colour);
        } else if (channel == 9) {
          DetType = 0;
          sprintf(var, "TIG%2.2i%sN00A", DetNum, colour);
        } else if (channel < 15 && channel > 9) {
          DetType = 3;
          sprintf(var, "TIS%2.2i%sN%2.2ix", DetNum, colour, channel - 9);
        }
	for(int m = 0; m < MNEMONIC.size(); m++) { 
	  if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
            sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	  }
	}
        aNum = ((DetNum) - 1) * 4 + cryNum;
        if (channel == 9) outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << gain[aNum] << "\t" << offset[aNum] << "\t" << non_lin[aNum] << "\tGRF16\n";
        else outfile << chancounter << "\t" << electronicaddress << "\t" << var << "\t" << 1 << "\t" << 0 << "\t" << 0 <<  "\tGRF16\n";
        if (msc_flag) {
          sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
          mscnames << line << "\n";
          sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
          mscnames << line << "\n";
        }
        chancounter++;
      }
    }

    for (int i = 0; i < 128; i++) {
      port = (i % 256)/16 ;
      channel = i % 16;
      collector = (i/256) + 4;
      DetNum = i + 1;
      sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel);
      DetType = 8;
      sprintf(var,"TPC%03iN00X",DetNum);
      for(int m = 0; m < MNEMONIC.size(); m++) { 
	if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
          sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
	}
      }
      outfile << chancounter << "\t" << electronicaddress << "\t" <<  var << "\t" << s3gains[i] << "\t" << s3offsets[i] << "\t" << 0 << "\tGRF16\n";
      if(msc_flag) {
        sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
        mscnames << line << "\n";
        sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
        mscnames << line << "\n";
      }
      chancounter++;
    }

    DetType=15;
    channel=15;
    port=0;
    collector=1;
    sprintf(var,"RFL00XS00x");
    sprintf(electronicaddress, "0x%01x%01x%02x", collector, port, channel); 
    for(int m = 0; m < MNEMONIC.size(); m++) { 
      if (strcmp(var,MNEMONIC.at(m).c_str()) == 0) {
        sprintf(electronicaddress, "0x%01x%01x%02x", customcollector.at(m), customport.at(m), customchannel.at(m));
      }
    }
    outfile << chancounter << "\t" << electronicaddress << "\t" <<  var << "\t" << 1 << "\t" << 0 << "\t" << 0 << "\tGRF16\n";
    if(msc_flag) {
      sprintf(line, "set \"/DAQ/MSC/MSC[%i]\" '%s'", chancounter, electronicaddress);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/chan[%i]\" '%s'", chancounter, var);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/datatype[%i]\" '%i'", chancounter, DetType);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/gain[%i]\" '1'", chancounter);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/offset[%i]\" '0'", chancounter);
      mscnames << line << "\n";
      sprintf(line, "set \"/DAQ/MSC/quadratic[%i]\" '0'", chancounter);
      mscnames << line << "\n";
    }
    chancounter++;

    break;

  default: 
    printf("Experiment not recognised\nKnown Experiments are:\n");
    for(int i = 0; i < num_known_exp; i++) printf("%s:\t%s\n",exp_names[i].c_str(),exp_description[i].c_str());
    return false; 
  }

  if (msc_flag) {
    sprintf(line, "trunc \"/DAQ/MSC/MSC\" '%i'", chancounter);
    mscnames << line << "\n";
    sprintf(line, "trunc \"/DAQ/MSC/chan\" '%i'", chancounter);
    mscnames << line << "\n";
    sprintf(line, "trunc \"/DAQ/MSC/datatype\" '%i'", chancounter);
    mscnames << line << "\n";
    sprintf(line, "trunc \"/DAQ/MSC/gain\" '%i'", chancounter);
    mscnames << line << "\n";
    sprintf(line, "trunc \"/DAQ/MSC/offset\" '%i'", chancounter);
    mscnames << line << "\n";
    sprintf(line, "trunc \"/DAQ/MSC/quadratic\" '%i'", chancounter);
    mscnames << line << "\n";
    mscnames.close();
  }
  outfile.close();
  return true;

}

int main(int argc, char * * argv) {

  bool success = false;
  if (argc == 1) {
    printf("Too few inputs, give experiment name\nKnown Experiments are:\n");
    for(int i = 0; i < num_known_exp; i++) printf("%s:\t%s\n",exp_names[i].c_str(),exp_description[i].c_str());
    return 0;
  }
  if (argc > 4) {
    printf("Too many inputs, max three\n");
    return 0;
  }

  if (argc == 2)
    success = CreateConfFile(argv[1]);

  if (argc == 3)
    success = CreateConfFile(argv[1], argv[2]);

  if (argc == 4)
    success = CreateConfFile(argv[1], argv[2], argv[3]);

  if (success) {
    printf("Config files created successfully!\n");
    return 1;
  } else {
    printf("Config creation failed!\n");
    return 0;
  }

  return 0;

}
