#ifndef SortCode_Sam_h
#define SortCode_Sam_h

#include <iostream>
#include <iomanip>
#include "TCutG.h"
#include "TH1.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TTree.h"
#include "TChain.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TList.h"
#include "TRandom.h"
#include "TS3.h"
#include "TS3Hit.h"
#include "TReaction.h"
#include "TSRIM.h"
#include "TTigress.h"
#include "TSpectrum.h"
#include "TChannel.h"
#include "TEmma.h"
#include "TParserLibrary.h"
#include "TEnv.h"
#include "Declarations.h"

using namespace std;

class SortCode_Sam {

	public :

		SortCode_Sam(){;} 
		void SortData(const char*, const char*, const char*, const char*, const char*);
		void Initialise();
};
#endif

void SortCode_Sam::Initialise() {

  printf("Start initializations\n");
  printf("Creating list\n");

  // Histogram Lists
  // Singles:
  sbList = new TList;
  emmaList = new TList;
  samList = new TList;
  s3List = new TList;
  tigList = new TList;
  tigbgoList = new TList;
  // Doubles:
  s3emmaList = new TList;
  tigemmaList = new TList;
  s3tigList = new TList;
  // Tripples:
  s3tigemmaList = new TList;

  Egam351keVgateList = new TList;
  Egam1120keVgateList = new TList;
  Egam1395keVgateList = new TList;
  Egam1567keVgateList = new TList;
  Egam4550keVgateList = new TList;

  printf("Creating histograms\n");
 
 
 
 
 
  // My plots
  h_pgac_hits = new TH1F("h_pgac_hits","PGAC hits",200,-100,100);
  samList->Add(h_pgac_hits);

  h_front_back_energy = new TH1F("h_front-back_energy","S3 Front-back energy difference",1000,-1000,1000);
  samList->Add(h_front_back_energy);
  h_front_back_energy_w_fractional_cut = new TH1F("h_front-back_energy_w_fractional_cut","S3 Front-back energy difference with online fractional cut",1000,-1000,1000);
  samList->Add(h_front_back_energy_w_fractional_cut);
  h_front_back_v_ring_energy = new TH2F("h_front_back_v_ring_energy","S3 Front-back energy difference vs ring energy",1000,-1000,1000,10000,0,10000);
  samList->Add(h_front_back_v_ring_energy);
  
  h_s3energy = new TH2F("h_sectorVring","Front-back equal energy plot",1000,0,10000, 1000,0,10000);
  samList->Add(h_s3energy);
  h_s3energy_pid_gated = new TH2F("h_sectorVring_pid_gated","Front-back equal energy PID gated",1000,0,10000, 1000,0,10000);
  samList->Add(h_s3energy_pid_gated);
  h_s3energy_pid_gated_multi_one = new TH2F("h_sectorVring_pid_gated_multi_one","Front-back equal energy PID gated and multi==1",1000,0,10000, 1000,0,10000);
  samList->Add(h_s3energy_pid_gated_multi_one);
  h_icSumVSi = new TH2F("h_icSumVSi","EMMA Singles IC Sum vs IC+Si Energy",8000,0,8000,4000,0,4000); // EMMA IC vs IC+Silicon
  samList->Add(h_icSumVSi);
  h_s3sector_energy = new TH2F("h_s3sector_energy", "S3 pixel energy vs sector number", 32, 0, 32,10000, 0, 10000);
  samList->Add(h_s3sector_energy);
  h_s3ring_energy = new TH2F("h_s3ring_energy", "S3 pixel energy vs ring number", 24, 0, 24,10000, 0, 10000);
  samList->Add(h_s3ring_energy);
  h_s3sector_zero_energy_vs_ring_number = new TH2F("h_s3sector_zero_energy_vs_ring_number", "Sector 0 energy vs ring number", 24, 0, 24,10000, 0, 10000);
  samList->Add(h_s3sector_zero_energy_vs_ring_number);
  h_sectorE_v_ring_num = new TH2F("h_sectorE_v_ring_num", "S3 sector energy vs ring number", 24, 0, 24,10000, 0, 10000);
  samList->Add(h_sectorE_v_ring_num);

  h_excite_v_dopE_addback = new TH2F("h_excite_v_dopE_addback","S3 Excitation Energy vs Doppler-Corrected Add-Back Energy",10000,0,10000,2000,0,20);
  samList->Add(h_excite_v_dopE_addback);
  h_excite_pid_gated = new TH1F("h_excite_pid_gated","S3 Excitation Energy PID gated",1500,0,15); // Excitation Energy
  samList->Add(h_excite_pid_gated);
  h_excite_pgac_gated = new TH1F("h_excite_pgac_gated","S3 Excitation Energy PGAC gated",1500,0,15); // Excitation Energy
  samList->Add(h_excite_pgac_gated);  
  h_S3EMMA_Exc_RngN = new TH2F("h_S3EMMA_Exc_RngN","S3 Excitation Energy vs Ring Number",24,0,24,1500,0,15);
  samList->Add(h_S3EMMA_Exc_RngN);
  h_excite_ring0 = new TH1F("h_excite_ring0", "Excitation energy for ring 0", 140, 0, 14);
  samList->Add(h_excite_ring0);
  h_S3EMMA_Exc_SecN = new TH2F("h_S3EMMA_Exc_SecN","S3 Excitation Energy vs Sector Number",32,0,32,1500,0,15);
  samList->Add(h_S3EMMA_Exc_SecN);
  
  // h_excite_pid_gated_DL_corr = new TH1F("h_excite_pid_gated_DL_corr", "S3 Excitation Energy PID gated, S3 DL corrected",1500,0,15);
  // samList->Add(h_excite_pid_gated_DL_corr);
  h_s3_E = new TH1F("h_s3_E", "S3 energy", 10000, 0, 10000);
  samList->Add(h_s3_E);
  // h_s3_E_DL_corr = new TH1F("h_s3_E_DL_corr", "S3 energy, DL corrected", 10000,0,10000);
  // samList->Add(h_s3_E_DL_corr);
  // h_excite_pid_gated_DL_tLiF_corr = new TH1F("h_excite_pid_gated_DL_tLiF_corr", "S3 Excitation Energy PID gated, S3 DL corrected, t in LiF corrected",1500,0,15);
  // samList->Add(h_excite_pid_gated_DL_tLiF_corr);
  // h_s3_E_DL_tLiF_corr = new TH1F("h_s3_E_DL_tLiF_corr", "S3 energy, DL corrected, t in LiF corrected", 10000, 0, 10000);
  // samList->Add(h_s3_E_DL_tLiF_corr);

  h_ejectile_energy_v_thetalab = new TH1F("h_ejectile_energy_v_thetalab", "Ejectile lab energy vs Ejectile lab angle", 120, 0, 180);
  samList->Add(h_ejectile_energy_v_thetalab);
  h_recoil_theta = new TH1F("h_recoil_theta", "Recoil lab angle", 180, 0, 180);
  samList->Add(h_recoil_theta);

  h_S3spec_noMassGate = new TH1F("h_S3spec_noMassGate","S3 Excitation Energy No Mass Gate",1500,0,15); // Excitation Energy
  samList->Add(h_S3spec_noMassGate);
  s3_E_theta = new TH2F("s3_E_theta","s3_E_theta",180,0,30, 1500, 0, 15000); 
  samList->Add(s3_E_theta);
  h_excite_s3_E = new TH2F("h_excite_s3_E", "Excitation energy vs S3 energy",1000,0,10000,1500,0,15);
  samList->Add(h_excite_s3_E);
  
  h_gammagamma = new TH2F("h_gammagamma","Addback Doppler corrected Gamma-Gamma",10000,0,10000,10000,0,10000); // Excitation Energy
  samList->Add(h_gammagamma);
  h_excite_PIdgated_351keVgated= new TH1F("h_excite_PIdgated_351keVgated","S3 Excitation Energy PID and 351 keV gamma gated",1500,0,15); // Excitation Energy
  samList->Add(h_excite_PIdgated_351keVgated);
  h_excite_PIdgated_1395keVgated= new TH1F("h_excite_PIdgated_1395keVgated","S3 Excitation Energy PID and 1395 keV gamma gated",1500,0,15); // Excitation Energy
  samList->Add(h_excite_PIdgated_1395keVgated);
  h_excite_PIdgated_1745keVgated= new TH1F("h_excite_PIdgated_1745keVgated","S3 Excitation Energy PID and 1745 keV gamma gated",1500,0,15); // Excitation Energy
  samList->Add(h_excite_PIdgated_1745keVgated);
  h_excite_PIdgated_1120keVgated= new TH1F("h_excite_PIdgated_1120keVgated","S3 Excitation Energy PID and 1120 keV gamma gated",1500,0,15); // Excitation Energy
  samList->Add(h_excite_PIdgated_1120keVgated);
  h_excite_PIdgated_2516keVgated= new TH1F("h_excite_PIdgated_2516keVgated","S3 Excitation Energy PID and 2516 keV gamma gated",1500,0,15); // Excitation Energy
  samList->Add(h_excite_PIdgated_2516keVgated);
  h_excite_PIdgated_6408keVgated = new TH1F("h_excite_PIdgated_6408keVgated","S3 Excitation Energy PID and 6408 keV gamma gated",1500,0,15); // Excitation Energy
  samList->Add(h_excite_PIdgated_6408keVgated);
  h_excite_PIdgated_5902keVgated = new TH1F("h_excite_PIdgated_5902keVgated","S3 Excitation Energy PID and 5902 keV gamma gated",1500,0,15); // Excitation Energy
  samList->Add(h_excite_PIdgated_5902keVgated);
  h_excite_PIdgated_4556keVgated= new TH1F("h_excite_PIdgated_4556keVgated","S3 Excitation Energy PID and 4556 keV gamma gated",1500,0,15); // Excitation Energy
  samList->Add(h_excite_PIdgated_4556keVgated);
  h_excite_PIdgated_3538keVgated= new TH1F("h_excite_PIdgated_3538keVgated","S3 Excitation Energy PID and 3538 keV gamma gated",1500,0,15); // Excitation Energy
  samList->Add(h_excite_PIdgated_3538keVgated);
  h_excite_PIdgated_5374keVgated= new TH1F("h_excite_PIdgated_5374keVgated","S3 Excitation Energy PID and 5374 keV gamma gated",1500,0,15); // Excitation Energy
  samList->Add(h_excite_PIdgated_5374keVgated);
  
  
  
  h_dopE_addback_gated = new TH1F("h_dopE_addback_gated","Doppler-Corrected Add-Back Energy",10000,0,10000); // Add-Back Energy
  samList->Add(h_dopE_addback_gated);
  h_dopE_addback_reverseGate_lower = new TH1F("h_dopE_addback_reverseGate_lower","Doppler-Corrected Add-Back Energy Reverse Gated on 7.4 MeV Peak",10000,0,10000); // Add-Back Energy
  samList->Add(h_dopE_addback_reverseGate_lower);
  h_dopE_addback_reverseGate_higher = new TH1F("h_dopE_addback_reverseGate_higher","Doppler-Corrected Add-Back Energy Reverse Gated on 8.0 MeV Peak",10000,0,10000); // Add-Back Energy
  samList->Add(h_dopE_addback_reverseGate_higher);
  h_dopE_addback_reverseGate_middle = new TH1F("h_dopE_addback_reverseGate_middle","Doppler-Corrected Add-Back Energy Reverse Gated between Peaks",10000,0,10000); // Add-Back Energy
  samList->Add(h_dopE_addback_reverseGate_middle);
  
  
  
   h_excite_ringA_4556 = new TH1F("h_excite_ringA_4556","S3:rings0-2 for 4556 keV gamma ray",1500,0,15); // Excitation Energy
  samList->Add(h_excite_ringA_4556);  
   h_excite_ringB_4556 = new TH1F("h_excite_ringB_4556","S3:rings3-5 for 4556 keV gamma ray",1500,0,15); // Excitation Energy
  samList->Add(h_excite_ringB_4556);  
   h_excite_ringC_4556 = new TH1F("h_excite_ringC_4556","S3:rings6-8 for 4556 keV gamma ray",1500,0,15); // Excitation Energy
  samList->Add(h_excite_ringC_4556);  
   h_excite_ringD_4556 = new TH1F("h_excite_ringD_4556","S3:rings9-11 for 4556 keV gamma ray",1500,0,15); // Excitation Energy
  samList->Add(h_excite_ringD_4556);  
   h_excite_ringE_4556 = new TH1F("h_excite_ringE_4556","S3:ring12-14 for 4556 keV gamma ray",1500,0,15); // Excitation Energy
  samList->Add(h_excite_ringE_4556);  
   h_excite_ringF_4556 = new TH1F("h_excite_ringF_4556","S3:ring15-17 for 4556 keV gamma ray",1500,0,15); // Excitation Energy
  samList->Add(h_excite_ringF_4556);  
    
   h_excite_ringA_6408 = new TH1F("h_excite_ringA_6408","S3:rings0-7 for 6408 keV gamma ray",1500,0,15); // Excitation Energy
  samList->Add(h_excite_ringA_6408);  
   h_excite_ringB_6408 = new TH1F("h_excite_ringB_6408","S3:rings8-15 for 6408 keV gamma ray",1500,0,15); // Excitation Energy
  samList->Add(h_excite_ringB_6408);  
   h_excite_ringC_6408 = new TH1F("h_excite_ringC_6408","S3:rings16-23 for 6408 keV gamma ray",1500,0,15); // Excitation Energy
  samList->Add(h_excite_ringC_6408);  

   h_excite_ringD_6408 = new TH1F("h_excite_ringD_6408","S3:rings18-23 for 6408 keV gamma ray",1500,0,15); // Excitation Energy
   //samList->Add(h_excite_ringD_6408);  
   h_excite_ringE_6408 = new TH1F("h_excite_ringE_6408","S3:ring??-?? for 6408 keV gamma ray",1500,0,15); // Excitation Energy
  //samList->Add(h_excite_ringE_6408);  
   h_excite_ringF_6408 = new TH1F("h_excite_ringF_6408","S3:ring??-?? for 6408 keV gamma ray",1500,0,15); // Excitation Energy
  //samList->Add(h_excite_ringF_6408);  
  
  
  // Fluorine plots
  F_excite_v_dopE_addback = new TH2F("F_excite_v_dopE_addback","S3 Excitation Energy vs Doppler-Corrected Add-Back Energy Fluorine Gated",10000,0,10000,2000,0,20);
  samList->Add(F_excite_v_dopE_addback);
  F_excite_pid_gated = new TH1F("F_excite_pid_gated","S3 Excitation Energy Fluorine gated",1500,0,15); // Excitation Energy
  samList->Add(F_excite_pid_gated);
  F_dopE_addback_gated = new TH1F("F_dopE_addback_gated","Doppler-Corrected Add-Back Energy Fluorine Gated",10000,0,10000); // Add-Back Energy
  samList->Add(F_dopE_addback_gated);
  
  

  cout << "Done" << endl;

}
