#ifndef SortCode_h
#define SortCode_h

#include <iostream>
#include <iomanip>
#include "TStopwatch.h"
#include "TCutG.h"
#include "TH1.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TTree.h"
#include "TChain.h"
#include "TH2.h"
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
//using namespace std;

TList *tigList, *s3List, *tigtigList, *tigs3List, *emmaList, *icList, *tigemmaList, *tgList, *bgList, *PIDgatedList, *PIDgatedListS3,*emmas3List; 

TH1F *ssb_anode_dt1,*ssb_anode_dt2;

//Raw TIGRESS
TH1F *tigE, *addE, *addDoppRaw, *tigRate;
TH2F *tigE_ANum,*addE_ANum, *addDoppRaw_ANum, *num_addr, *tigE_theta, *tigRate_ANum, *addE_time; 

//Raw S3
TH1F *s3_E, *s3_rings_sectors_singlesT, *s3_ringNum, *s3_secNum, *s3Rate;
TH2F *rings, *sectors, *ringsTCut, *sectorsTCut, *s3_E_theta, *hitmap, *s3_rings_sectors, *s3_rings_sectors_singles, *s3_rings_sectors_singlesTvT, *s3_rings_sectors_singlesTvSec, *s3_rings_sectors_singlesTvRing, *s3_rings_sectors_singlesTvE, *s3_ringNum_secNum_singles;
TH2F *s3_E_ringNum;

//TIGRESS-TIGRESS
TH1F *addT_addT; 
TH2F *addE_addE, *addE_addE_tg;

//TIGRESS-S3 
TH1F *addT_s3T, *addT_s3T_s3ECut, *excE, *addDopp, *s3_E_gated, *s3_E_gated_PIDG_gamG;
TH2F *excE_theta, *addDopp_ANum_s3, *addDopp_exc, *s3_E_theta_gated, *addE_s3_E, *addDopp_s3_E, *s3_E_theta_coinc; 
TH2F *s3_E_ringNumb_4556_gated;

TH1F* addDopp_addECut;
TH2F* addDopp_ANum_s3_addECut;
TH2F* addDopp_exc_addECut;
TH2F* addE_s3_E_addECut;
TH2F* addDopp_s3_E_addECut;


//Raw EMMA
TH1F *xPos, *yPos, *siE, *icSum, *icE[5], *ssbE[2];
TH2F *emma_dE_E,*emma_tig_dt_E;
TH2F *pgac, *icN, *iC0V1, *iC0V2, *iC0V3, *iC1V2, *iC1V3, *iC2V3, *tDE, *tDE_nocut, *siET, *ssbET[2],*icSumVSi,*icSumVicPlusSi, *ic0VSi, *ic1VSi, *ic2VSi, *ic3VSi, *anodessbT[2];
TH2F *pgac_o17;
TH1F *yPos_21Ne_gated, *xPos_21Ne_gated;;

TH2F* iC0VSum; 
// EMMA-S3
TH1F *emma_s3_exc, *emma_s3_E, *s3_gatedon_Em_and_Tig;
TH2F *emma_s3_exc_ring,*emma_s3_exc_addback_dopp, *emma_s3_E_ring, *emma_s3_E_ring_tig;
TH2F *icSumVicPlusSi_s3gate;


TH2F *s3_E_theta_Ne_PID_Gated, *s3_E_theta_Ne_PID_4556_gamma_gated, *excE_thetaCm_Ne_PID_Gated, *excE_thetaCm_Ne_PID_4556_gamma_gated;

//ToF Spectra
TH1F *excE_tg, *addDopp_tg, *excE_bg, *addDopp_bg, *addemmasitof, *s3emmatof, *ssbICtof, *ssbSItof, *tigEMtof, *tigEMtof_nobgo, *tigICtof, *tigAnodetof, *ssbEMTtof;
TH2F *addE_tof, *excE_theta_tg, *addDopp_ANum_tg, *addE_addE_tofg, *addDopp_exc_tg, *addDopp_addDopp_tg, *excE_theta_bg, *addDopp_ANum_bg, *addDopp_exc_bg, *hitmap_time_gated, *icSumVSi_gated_350, *addemmasitofdet; 

// TIGRESS-EMMA
TH1F *addDopp_emma, *addDopp_emma_corona, *addDopp_emma_lampshade;
TH1F *emTrigE;
//pid gated
TH1F *doppE_19F_pidg, *doppE_21Ne_pidg, *doppE_21Ne_pidg_xPosG, *doppE_21Ne_pidg_delayT;
TH1F *doppE_Ne_pid_s3_gated;
TH2F *excE_doppE_Ne_PID_Gated;

//Mass Spectra
TH1F *excE_PIDG, *addDopp_PIDG, *aE_icSumVSi_gated;
TH2F *excE_theta_PIDG, *addDopp_ANum_PIDG, *addDopp_exc_PIDG, *rings_PIDG, *s3_E_theta_gated_PIDG; 

//TCuts
TCutG *massGate, *siicGate;

TCutG *Fluorine_Gate;
TCutG *Neon_Gate;

class SortCode {

	public :

		SortCode(){;} 
		void SortData(const char*, const char*, const char*, const char*, const char*);
		void Initialise();
};
#endif

void SortCode::Initialise() {

  printf("Start initializations\n");
  printf("Creating list\n");

  tigList = new TList;
  s3List = new TList;
  tigtigList = new TList;
  tigs3List = new TList;
  emmaList = new TList;
  icList = new TList;
  tigemmaList = new TList;
  tgList = new TList;
  bgList = new TList;
  PIDgatedList = new TList;
  PIDgatedListS3 = new TList;
	emmas3List = new TList;

  printf("Creating histograms\n");


  //////////////////////////////
  //
  //Raw TIGRESS Spectra
  //
  //////////////////////////////
  tigE = new TH1F("tigE", "Tigress_Energy; Energy (keV); Counts/2keV", 8192, 0, 16384);
  tigList->Add(tigE);
  tigE_ANum = new TH2F("tigE_ANum", "Tigress_Energy_V_ArrayNumber; Array Number; Energy (keV)", 50, 15, 65, 8192, 0, 16384);
  tigList->Add(tigE_ANum);
  addE = new TH1F("addE", "Addback_Energy; Energy (keV); Counts/2keV", 8192, 0, 16384);
  tigList->Add(addE);
  addDoppRaw = new TH1F("addDoppRaw", "Addback_Doppler_Raw; Energy (keV); Counts/2keV", 8192, 0, 16384);
  tigList->Add(addDoppRaw);
  addE_ANum = new TH2F("addE_ANum", "Addback_Energy_V_ArrayNumber; Array Number; Energy (keV)", 50, 15, 65, 8192, 0, 16384);
  tigList->Add(addE_ANum);
  addDoppRaw_ANum = new TH2F("addDoppRaw_ANum", "Addback_Doppler_Energy_V_ArrayNumber; Array Number; Energy (keV)", 50, 15, 65, 8192, 0, 16384);
  tigList->Add(addDoppRaw_ANum);
  num_addr = new TH2F("num_addr", "TIGRESS_position_vs_address; Array Number; Electronic Address", 50, 15, 65, 8192, 0, 16384);
  tigList->Add(num_addr);
  tigE_theta = new TH2F("tigE_theta","TIGRESS energy vs lab angle; Lab Energy (keV); #theta_{lab} (#circ)",8192,0,16384,180,0,180);
  tigList->Add(tigE_theta);
  tigRate = new TH1F("tigRate", "TIGRESS Rate", 10000, 0, 10000);
  tigRate->GetXaxis()->SetTitle("Run Time (s)");
  tigRate->GetYaxis()->SetTitle("Count/s");
  tigList->Add(tigRate);
  tigRate_ANum = new TH2F("tigRate_ANum", "TIGRESS Rate", 10000, 0, 10000, 50, 15, 65);
  tigRate_ANum->GetXaxis()->SetTitle("Run Time (s)");
  tigRate_ANum->GetYaxis()->SetTitle("Array Number");
  tigList->Add(tigRate_ANum);
  addE_time = new TH2F("addE_time","TIGRESS addback energy vs time; Run Time (s); Energy (keV)", 1000, 0, 10000, 8192, 0, 16384);
  tigList->Add(addE_time);

  //////////////////////////////
  //
  //Raw S3 Spectra
  //
  /////////////////////////////
  s3_E_ringNum = new TH2F("s3_E_ringNum","S3 Energy vs Ring Number; Ring Number; Energy (keV)",25,0,25,8192,0,16384);
  s3List->Add(s3_E_ringNum);
  s3_ringNum = new TH1F("S3_ringNum","S3 Ring Number; Ring Number; Counts",25,0,25);
  s3List->Add(s3_ringNum);
  s3_secNum = new TH1F("S3_secNum","S3 Sector Number; Sector Number; Counts",33,0,33);
  s3List->Add(s3_secNum);
  s3_ringNum_secNum_singles = new TH2F("s3_ringNum_secNum_singles","S3 Ring and Sector Number",25,0,25,33,0,33);
  s3_ringNum_secNum_singles->GetXaxis()->SetTitle("Ring Number");
  s3_ringNum_secNum_singles->GetYaxis()->SetTitle("Sector Number");
  s3List->Add(s3_ringNum_secNum_singles);
  rings = new TH2F("rings","rings",25,0,25,8192,0,16384);
  rings->GetXaxis()->SetTitle("Ring Number");
  rings->GetYaxis()->SetTitle("Energy (uncalibrated)");
  s3List->Add(rings);
  ringsTCut = new TH2F("ringsTCut","rings_TimeCut_Mult1",25,0,25,8192,0,16384);
  ringsTCut->GetXaxis()->SetTitle("Ring Number");
  ringsTCut->GetYaxis()->SetTitle("Ring Energy (uncalibrated)");
  s3List->Add(ringsTCut);
  sectors = new TH2F("sectors","sectors",33,0,33,8192,0,16384);
  sectors->GetXaxis()->SetTitle("Sector Number");
  sectors->GetYaxis()->SetTitle("Energy (uncalibrated)");
  s3List->Add(sectors);
  sectorsTCut = new TH2F("sectorsTCut","sectors_TimeCut_Mult1",33,0,33,8192,0,16384);
  sectorsTCut->GetXaxis()->SetTitle("Sector Number");
  sectorsTCut->GetYaxis()->SetTitle("Sector Energy (uncalibrated)");
  s3List->Add(sectorsTCut);
  s3_E = new TH1F("S3_E","S3_E;Energy (keV);Counts/8keV",4096,0,32768);
  s3List->Add(s3_E); 
  s3_E_theta = new TH2F("s3_E_theta","s3_E_theta; #theta_{lab} (#circ); Energy (keV)", 60, 120, 180, 4096, 0, 32768); 
  s3List->Add(s3_E_theta);
  hitmap = new TH2F("hitmap","Hitmap",200,-50,50,200,-50,50); 
  hitmap->GetXaxis()->SetTitle("S3 X [mm]");
  hitmap->GetYaxis()->SetTitle("S3 Y [mm]");
  s3List->Add(hitmap);
  s3_rings_sectors = new TH2F("s3_rings_sectors","S3_sectorE_vs_ringE; Ring Energy; Sector Energy",4096,0,16384,4096,0,16384);
  s3List->Add(s3_rings_sectors);
  s3_rings_sectors_singles = new TH2F("s3_rings_sectors_singles","S3_sectorE_vs_ringE_Multiplicity_1; Ring Energy; Sector Energy",4096,0,16384,4096,0,16384);
  s3List->Add(s3_rings_sectors_singles);
  s3_rings_sectors_singlesT = new TH1F("s3_rings_sectors_singles_timing","S3_sectorT_vs_ringT_Multiplicity_1; Sector Time - Ring Time",2048,-1024,1024);
  s3List->Add(s3_rings_sectors_singlesT);
  s3_rings_sectors_singlesTvT = new TH2F("s3_rings_sectors_singles_timing_v_time","S3_sectorT_vs_ringT_Multiplicity_1_vs_Time; Run Time (min); Sector Time - Ring Time",120,0,120,1024,-256,256);
  s3List->Add(s3_rings_sectors_singlesTvT);
  s3_rings_sectors_singlesTvSec = new TH2F("s3_rings_sectors_singles_timing_v_sector","S3_sectorT_vs_ringT_Multiplicity_1_vs_Sector; Sector; Sector Time - Ring Time",33,0,33,1024,-256,256);
  s3List->Add(s3_rings_sectors_singlesTvSec);
  s3_rings_sectors_singlesTvRing = new TH2F("s3_rings_sectors_singles_timing_v_ring","S3_sectorT_vs_ringT_Multiplicity_1_vs_Ring; Ring;Sector Time - Ring Time",25,0,25,1024,-256,256);
  s3List->Add(s3_rings_sectors_singlesTvRing);
  s3_rings_sectors_singlesTvE = new TH2F("s3_rings_sectors_singles_timing_v_energy","S3_sectorT_vs_ringT_Multiplicity_1_vs_Energy; Energy (keV); Sector Time - Ring Time",4096,0,32768,1024,-256,256);
  s3List->Add(s3_rings_sectors_singlesTvE);
  s3Rate = new TH1F("s3Rate", "S3 Rate", 10000, 0, 10000);
  s3Rate->GetXaxis()->SetTitle("Run Time (s)");
  s3Rate->GetYaxis()->SetTitle("Count/s");
  s3List->Add(s3Rate);
  hitmap_time_gated = new TH2F("hitmap_time_gated","Hitmap_gated_on_s3-emma_tof",200,-50,50,200,-50,50);
  hitmap_time_gated->GetXaxis()->SetTitle("S3 X [mm]");
  hitmap_time_gated->GetYaxis()->SetTitle("S3 Y [mm]");
  s3List->Add(hitmap_time_gated);

  excE = new TH1F("excE","Excitation_Energy_MeV; Exc Energy (MeV); Counts/50keV",500,-5.,20.); 
  s3List->Add(excE);
  excE_theta = new TH2F("excE_theta","Excitation_Energy_Theta;#theta_{lab} (#circ); Exc Energy (MeV)",60,120,180,500,-5.,20.); 
  s3List->Add(excE_theta);

  //////////////////////////////
  //
  //TIGRESS-TIGRESS
  //
  //////////////////////////////
  addT_addT = new TH1F("addT_addT","Tigress-Tigress_time;Time Difference (ns);Counts/ns",4096,-2048,2048); 
  tigtigList->Add(addT_addT);
  addE_addE = new TH2F("addE_addE", "Addback_Gamma_Gamma; Energy (keV); Energy (keV)", 8192, 0, 16384, 8192, 0, 16384);
  tigtigList->Add(addE_addE);
  addE_addE_tg = new TH2F("addE_addE_tg", "Addback_Gamma_Gamma_Time_Gated; Energy (keV); Energy (keV)", 8192, 0, 16384, 8192, 0, 16384);
  tigtigList->Add(addE_addE_tg);

  //////////////////////////////
  //
  //TIGRESS-S3
  //
  //////////////////////////////
  s3_E_ringNumb_4556_gated = new TH2F("s3_E_ringNumb_4556_gated","S3 E vs ring number, gated on 4556 gamma; Ring Number; Energy (keV)",25,0,25,8192,0,16384);
  tigs3List->Add(s3_E_ringNumb_4556_gated);
  addT_s3T = new TH1F("addT_s3T","Tigress-S3_time; Time Difference (ns); Counts/ns",4096,-2048,2048); 
  tigs3List->Add(addT_s3T);

  // investigate this
  addT_s3T_s3ECut = new TH1F("addT_s3T_s3ECut","Tigress-S3_time cut on S3 E; Time Difference (ns); Counts/ns",4096,-2048,2048); 
  tigs3List->Add(addT_s3T_s3ECut);

  addDopp = new TH1F("addDopp", "Addback_Doppler_Energy; Doppler Energy (keV); Counts/2keV", 8192, 0, 16384);
  tigs3List->Add(addDopp);
  addDopp_ANum_s3 = new TH2F("addDopp_ANum_s3", "Addback_Doppler_Energy_V_ArrayNumber; Array Number; Doppler Energy (keV)", 50, 15, 65, 8192, 0, 16384);
  tigs3List->Add(addDopp_ANum_s3);
  addDopp_exc = new TH2F("addDopp_Exc", "Addback_Doppler_Energy_V_Excitation_Energy; Doppler Energy (keV); Exc Energy (MeV)", 8192, 0, 16384, 500, -5., 20.);
  tigs3List->Add(addDopp_exc);
  addE_s3_E  = new TH2F("addE_s3E", "Addback_Energy_V_S3_Energy; Addback Energy (keV); S3 Energy (keV)", 8192, 0, 16384, 4096, 0, 32768);
  tigs3List->Add(addE_s3_E);
  addDopp_s3_E  = new TH2F("addDopp_s3E", "Addback_Doppler_Energy_V_S3_Energy; Doppler Energy (keV); S3 Energy (keV)", 8192, 0, 16384, 4096, 0, 32768);
  tigs3List->Add(addDopp_s3_E);

  // investigate these
  addDopp_addECut = new TH1F("addDopp_addECut", "Addback_Doppler_Energy; Doppler Energy (keV); Counts/2keV", 8192, 0, 16384);
  tigs3List->Add(addDopp_addECut);
  addDopp_ANum_s3_addECut = new TH2F("addDopp_ANum_s3_addECut", "Addback_Doppler_Energy_V_ArrayNumber; Array Number; Doppler Energy (keV)", 50, 15, 65, 8192, 0, 16384);
  tigs3List->Add(addDopp_ANum_s3_addECut);
  addDopp_exc_addECut = new TH2F("addDopp_Exc_addECut", "Addback_Doppler_Energy_V_Excitation_Energy; Doppler Energy (keV); Exc Energy (MeV)", 8192, 0, 16384, 500, -5., 20.);
  tigs3List->Add(addDopp_exc_addECut);
  addE_s3_E_addECut  = new TH2F("addE_s3E_addECut", "Addback_Energy_V_S3_Energy; Addback Energy (keV); S3 Energy (keV)", 8192, 0, 16384, 4096, 0, 32768);
  tigs3List->Add(addE_s3_E_addECut);
  addDopp_s3_E_addECut  = new TH2F("addDopp_s3E_addECut", "Addback_Doppler_Energy_V_S3_Energy; Doppler Energy (keV); S3 Energy (keV)", 8192, 0, 16384, 4096, 0, 32768);
  tigs3List->Add(addDopp_s3_E_addECut);

  s3_E_theta_coinc = new TH2F("s3_E_theta_coinc","s3_E_theta_coin; #theta_{lab} (#circ); S3 Energy (keV)",60,120,180, 4096,0,32768); 
  tigs3List->Add(s3_E_theta_coinc);

  //////////////////////////////
  //
  // Raw EMMA
  //
  //////////////////////////////
  xPos = new TH1F("xPos", "PGAC_X_Position; X Position (mm?); Counts/mm", 160, -80, 80);
  emmaList->Add(xPos);
  yPos = new TH1F("yPos", "PGAC_Y_Position; Y Position (mm?); Counts/mm", 160, -80, 80);
  emmaList->Add(yPos);
  pgac = new TH2F("pgac", "PGAC_Hit_Pattern", 160, -80, 80, 160, -80, 80);
  pgac->GetXaxis()->SetTitle("PGAC X Position [mm]");
  pgac->GetYaxis()->SetTitle("PGAC Y Position [mm]");
  emmaList->Add(pgac);
  
  xPos_21Ne_gated = new TH1F("xPos_21Ne_gated","PGAC X Pos gated on 21Ne PID; X Position (mm); Counts/mm",160,-80,80);
  emmaList->Add(xPos_21Ne_gated);

  yPos_21Ne_gated = new TH1F("yPos_21Ne_gated","PGAC Y Pos gated on 21Ne PID; Y Position (mm); Counts/mm",160,-80,80);
  emmaList->Add(yPos_21Ne_gated);

  siE = new TH1F("siE", "EMMA_Focal_Plane_Silicon_Energy; Energy (keV); Counts/keV", 2048, 0, 2048);
  emmaList->Add(siE);
  siET = new TH2F("siET", "EMMA_Focal_Plane_Silicon_Energy_V_Time; EMMA Si Time (s); Energy (keV)", 5500,0,5500, 2048, 0, 2048);
  emmaList->Add(siET);

  emma_dE_E = new TH2F("emma_dE_E","Si_Energy_VS_IC_SUM, EMMA; EMMA Silicon Energy (arb.); IC Sum (arb.)",2048,0,2048,8096,0,8096);
  emmaList->Add(emma_dE_E);

 ///////////////////////
 //
 // EMMA / IC
 //
 ///////////////////////

  icSumVSi = new TH2F("icSumVSi","Si_Energy_VS_IC_SUM; Silicon Energy (arb.); IC Sum (arb.)",2048,0,2048,8096,0,8096);
  icList->Add(icSumVSi);
  icSumVicPlusSi = new TH2F("icSumVicPlusSi","Si_+_IC_SUM_Energy_VS_IC_SUM",8096,0,8096,8096,0,8096);
  icSumVicPlusSi->GetXaxis()->SetTitle("Silicon + IC Sum");
  icSumVicPlusSi->GetYaxis()->SetTitle("IC Sum");
  icList->Add(icSumVicPlusSi);
  ic0VSi = new TH2F("ic0VSi","Si_Energy_VS_IC0",2048,0,2048,2048,0,2048);
  icList->Add(ic0VSi);
  ic1VSi = new TH2F("ic1VSi","Si_Energy_VS_IC1",2048,0,2048,2048,0,2048);
  icList->Add(ic1VSi);
  ic2VSi = new TH2F("ic2VSi","Si_Energy_VS_IC2",2048,0,2048,2048,0,2048);
  icList->Add(ic2VSi);
  ic3VSi = new TH2F("ic3VSi","Si_Energy_VS_IC3",2048,0,2048,2048,0,2048);
  icList->Add(ic3VSi);

  iC0VSum = new TH2F("iC0VSum","IC0 vs IC sum",1024,0,4096,1250,0,5000);
  iC0VSum->GetXaxis()->SetTitle("IC sum");
  iC0VSum->GetYaxis()->SetTitle("IC0");
  icList->Add(iC0VSum);

  for (int i = 0; i < 2; i++) {
    char hname[64];
    char hname2[64];
    sprintf(hname, "SSB_%i", i + 1);
    sprintf(hname2, "SSB_%i_Versus_Time", i + 1);
    ssbE[i] = new TH1F(hname, hname, 16384, 0, 98304);
    ssbE[i]->GetXaxis()->SetTitle("Energy [keV]");
    ssbE[i]->GetYaxis()->SetTitle("Counts/6keV");
    ssbET[i] = new TH2F(hname2, hname2, 5500, 0, 5500, 16384, 0, 98304);
    ssbET[i]->GetXaxis()->SetTitle("SSB Time [s]");
    ssbET[i]->GetYaxis()->SetTitle("Energy [keV]");
    emmaList->Add(ssbE[i]);
    emmaList->Add(ssbET[i]);
    sprintf(hname2, "SSB_%i_Vs_Anode_Time", i + 1);
    anodessbT[i] = new TH2F(hname2, hname2, 4096, -4096, 4096, 4, 0, 4);
    anodessbT[i]->GetXaxis()->SetTitle("SSB Time -- EMMA anode Time [ns]");
    anodessbT[i]->GetYaxis()->SetTitle("EMMA anode segment");
    emmaList->Add(anodessbT[i]);
  }

  ssbICtof = new TH1F("ssbICtof", "SSB_Time - EMMA IC Time; Time Difference (ns); Counts/ns", 18000, -8000, 10000);
  emmaList->Add(ssbICtof);
  ssbSItof = new TH1F("ssbSItof", "SSB_Time - EMMA Si Time; Time Difference (ns); Counts/ns", 20000, -10000, 10000);
  emmaList->Add(ssbSItof);
  ssbEMTtof = new TH1F("ssbEMTtof", "SSB_Time - EMMA Trigger Time; Time Difference (ns); Counts/ns", 20000, -10000, 10000);
  emmaList->Add(ssbEMTtof);

  for (int i = 0; i < 5; i++) {
    char hname[64];
    sprintf(hname, "EMMA_IC-Segment_%i", i);
    icE[i] = new TH1F(hname, hname, 4096, 0, 4096);
    icE[i]->GetXaxis()->SetTitle("IC Energy [keV]");
    icE[i]->GetYaxis()->SetTitle("Counts/keV");
    icList->Add(icE[i]);
  }
  icN = new TH2F("icN", "IC_Energy_V_Segment_Number; IC Segment; Counts", 5, 0, 5, 4096, 0, 4096);
  icList->Add(icN);
  icSum = new TH1F("icSum", "IC_Summed_Energy; IC Energy Sum [keV]; Counts/keV", 16384, 0, 16384);
  icList->Add(icSum);
  iC0V1= new TH2F("iC0V1", "IC0_Energy_V_IC1_Energy; IC0 Energy [keV]; IC1 Energy [keV]", 4096, 0, 4096, 4096, 0, 4096);
  icList->Add(iC0V1);
  iC0V2= new TH2F("iC0V2", "IC0_Energy_V_IC2_Energy; IC0 Energy [keV]; IC2 Energy [keV]", 4096, 0, 4096, 4096, 0, 4096);
  icList->Add(iC0V2);
  iC0V3= new TH2F("iC0V3", "IC0_Energy_V_IC3_Energy; IC0 Energy [keV]; IC3 Energy [keV]", 4096, 0, 4096, 4096, 0, 4096);
  icList->Add(iC0V3);
  iC1V2= new TH2F("iC1V2", "IC1_Energy_V_IC2_Energy; IC1 Energy [keV]; IC2 Energy [keV]", 4096, 0, 4096, 4096, 0, 4096);
  icList->Add(iC1V2);
  iC1V3= new TH2F("iC1V3", "IC1_Energy_V_IC3_Energy; IC1 Energy [keV]; IC3 Energy [keV]", 4096, 0, 4096, 4096, 0, 4096);
  icList->Add(iC1V3);
  iC2V3= new TH2F("iC2V3", "IC2_Energy_V_IC3_Energy; IC2 Energy [keV]; IC3 Energy [keV]", 4096, 0, 4096, 4096, 0, 4096);
  icList->Add(iC2V3);

  /*
  iC0VSum_ne22 = new TH2F("iC0VSum_ne22","IC0 vs IC sum, 22Ne gated",1024,0, 4096,1024,0,4096);
  emmaList->Add(iC0VSum_ne22);
  iC0VSum_na22 = new TH2F("iC0VSum_na22","IC0 vs IC sum, 22Na gated",1024,0, 4096,1024,0,4096);
  emmaList->Add(iC0VSum_na22);
  iC0VSum = new TH2F("iC0VSum","IC0 vs IC sum",1024,0, 4096,1024,0,4096);
  emmaList->Add(iC0VSum);

  */

  //////////////////////////////
  //
	// EMMA - S3
  //
  //////////////////////////////
  
	emma_s3_exc = new TH1F("emma_s3_exc","Emma gated S3 excitation energy; Exc Energy (MeV); Counts/50keV",500,-5,20);  	 
	emmas3List->Add(emma_s3_exc);
	emma_s3_exc_ring = new TH2F("emma_s3_exc_ring","Emma gated S3 excitation energy vs ring; S3 Ring Number; Exc Energy (MeV)",25,0,25,500,-5,20);  	 
	emmas3List->Add(emma_s3_exc_ring);
	emma_s3_exc_addback_dopp = new TH2F("emma_s3_exc_addback_dopp","Emma gated S3 excitation energy vs addback Doppler corrected; Doppler Energy (keV); Exc Energy (MeV)",4096,0,8192,500,-5,20);  	 
	emmas3List->Add(emma_s3_exc_addback_dopp);
	emma_s3_E = new TH1F("emma_s3_E","Emma gated S3 energy; S3 Energy (keV); Counts/2keV",8192,0,16384);
  emmas3List->Add(emma_s3_E);
	emma_s3_E_ring = new TH2F("emma_s3_E_ring","Emma gated S3 energy vs ring; Ring Number; S3 Energy (keV)",25,0,25,8192,0,16384);
  emmas3List->Add(emma_s3_E_ring);
  emma_s3_E_ring_tig = new TH2F("emma_s3_E_ring_tig","Emma & tigress gated S3 energy vs ring; Ring Number; S3 Energy (keV)",25,0,25,8192,0,16384);
  emmas3List->Add(emma_s3_E_ring_tig);

	//pid gated

	s3_sector0_mg_pid_gated = new TH1F("s3_sector0_mg_pid_gated","S3 sector 0 energy gated on Ne PID; Energy (keV); Counts/2keV",8192,0,16384);
	emmas3List->Add(s3_sector0_mg_pid_gated);
	s3_sector1_mg_pid_gated = new TH1F("s3_sector1_mg_pid_gated","S3 sector 1 energy gated on Ne PID; Energy (keV); Counts/2keV",8192,0,16384);
	emmas3List->Add(s3_sector1_mg_pid_gated);
	s3_sector2_mg_pid_gated = new TH1F("s3_sector2_mg_pid_gated","S3 sector 2 energy gated on Ne PID; Energy (keV); Counts/2keV",8192,0,16384);
	emmas3List->Add(s3_sector2_mg_pid_gated);
	s3_sector3_mg_pid_gated = new TH1F("s3_sector3_mg_pid_gated","S3 sector 3 energy gated on Ne PID; Energy (keV); Counts/2keV",8192,0,16384);
	emmas3List->Add(s3_sector3_mg_pid_gated);
	
	s3_ring0_mg_pid_gated = new TH1F("s3_ring0_mg_pid_gated","S3 ring 0 energy gated on Ne PID; Energy (keV); Counts/2keV",8192,0,16384);
	emmas3List->Add(s3_ring0_mg_pid_gated);
	s3_ring1_mg_pid_gated = new TH1F("s3_ring1_mg_pid_gated","S3 ring 1 energy gated on Ne PID; Energy (keV); Counts/2keV",8192,0,16384);
	emmas3List->Add(s3_ring1_mg_pid_gated);
	s3_ring20_mg_pid_gated = new TH1F("s3_ring20_mg_pid_gated","S3 ring 20 energy gated on Ne PID; Energy (keV); Counts/2keV",8192,0,16384);
	emmas3List->Add(s3_ring20_mg_pid_gated);
	s3_ring21_mg_pid_gated = new TH1F("s3_ring21_mg_pid_gated","S3 ring 21 energy gated on Ne PID; Energy (keV); Counts/2keV",8192,0,16384);
	emmas3List->Add(s3_ring21_mg_pid_gated);
	s3_E_ringNum_mg_pid_gated = new TH2F("s3_E_ringNum_mg_pid_gated","S3 energy vs ring number gated on Ne PID; Ring Number; Energy (keV)",25,0,25,8192,0,16384);
	emmas3List->Add(s3_E_ringNum_mg_pid_gated);
	s3_E_ringNum_na_pid_gated = new TH2F("s3_E_ringNum_na_pid_gated","S3 energy vs ring number gated on Na PID; Ring Number; Energy (keV)",25,0,25,8192,0,16384);
	emmas3List->Add(s3_E_ringNum_na_pid_gated);
	
  icSumVicPlusSi_s3gate = new TH2F("icSumVicPlusSi_s3gate ","Si_+_IC_SUM_Energy_VS_IC_SUM_S3_Gated",220,2000,4200,250,1500,4000);
  icSumVicPlusSi_s3gate ->GetXaxis()->SetTitle("Silicon + IC Sum");
  icSumVicPlusSi_s3gate ->GetYaxis()->SetTitle("IC Sum");
  emmas3List->Add(icSumVicPlusSi_s3gate);

	ssb_anode_dt1 = new TH1F("SSB_EMMA_dT1","SSB_EMMA_dT1; SSB-EMMA Time Difference (ns); Counts/10ns",1000,-5000,5000);
	emmas3List->Add(ssb_anode_dt1);  
	ssb_anode_dt2 = new TH1F("SSB_EMMA_dT2","SSB_EMMA_dT2; SSB-EMMA Time Difference (ns); Counts/10ns",1000,-5000,5000);
	emmas3List->Add(ssb_anode_dt2);  
  
  s3emmatof = new TH1F("s3emmatof", "S3_Time - EMMA Time; S3-EMMA Time Difference (ns); Counts/ns", 18000, -8000, 10000);
  emmas3List->Add(s3emmatof);

  //////////////////////////////
  //
	// TIGRESS - EMMA - S3 triples
  //
  /////////////////////////////

  //ToF Spectra
  addemmasitofdet = new TH2F("addemmasitof_ANum","Addback_Time - EMMA Si Time vs TIGRESS det num; Array Number; TIG - EMMA Si Time Difference (ns)",50,15,65,18000,-8000,10000);
  tigemmaList->Add(addemmasitofdet);
  addemmasitof = new TH1F("addemmasitof", "Addback_Time - EMMA Si Time; TIG-EMMA Time Difference (ns); Counts/ns", 18000, -8000, 10000);
  tigemmaList->Add(addemmasitof);

  emTrigE = new TH1F("emTrigE", "Emma Trigger Energy; Energy [keV]; Counts/5keV", 4000, 0, 20000);
  tigemmaList->Add(emTrigE);

  addE_tof = new TH2F("addE_tof", "Addback_Energy_vs_(Addback_Time - EMMA Time); TIG-EMMA Time Difference (ns); Energy (keV)", 18000, -8000, 10000, 8192, 0, 16384);
  tigemmaList->Add(addE_tof);
  tigEMtof = new TH1F("tigEMtof", "Addback_Time - EMMA Trigger Time; Time Difference (ns); Counts/ns", 18000, -8000, 10000);
  tigemmaList->Add(tigEMtof);
  tigEMtof_nobgo = new TH1F("tigEMtof_nobgo", "Addback_Time - EMMA Trigger Time, no BGO; Time Difference (ns); Counts/ns", 18000, -8000, 10000);
  tigemmaList->Add(tigEMtof_nobgo);

  tigICtof = new TH1F("tigICtof", "Addback_Time - EMMA IC Time; Time Difference (ns); Counts/ns", 18000, -8000, 10000);
  tigemmaList->Add(tigICtof);
  tigAnodetof = new TH1F("tigAnodetof", "Addback_Time - EMMA Anode Time; Time Difference (ns); Counts/ns", 18000, -8000, 10000);
  tigemmaList->Add(tigAnodetof);
  tDE = new TH2F("tDE","tDE EMMA Trigger <= 200 keV; EMMA Trigger Time [s]; EMMA Hit Time - EMMA Trigger Time [ns]",5500,0,5500,600,-2000,1000);
  tigemmaList->Add(tDE);
  tDE_nocut = new TH2F("tDE_nocut","tDE; EMMA Trigger Time [s]; EMMA Hit Time - EMMA Trigger Time [ns]",5500,0,5500,600,-2000,1000);
  tigemmaList->Add(tDE_nocut);
	addDopp_emma = new TH1F("addDopp_emma","TIGRESS Addback Doppler, EMMA coinc.; Doppler Energy (keV); Counts/2keV",8192,0,16384);
	tigemmaList->Add(addDopp_emma);
  addDopp_emma_corona = new TH1F("addDopp_emma_corona","TIGRESS Addback Doppler (90#circ detectors), EMMA coinc.; Doppler Energy (keV); Counts/2keV",8192,0,16384);
	tigemmaList->Add(addDopp_emma_corona);
  addDopp_emma_lampshade = new TH1F("addDopp_emma_lampshade","TIGRESS Addback Doppler (135#circ detectors), EMMA coinc.; Doppler Energy (keV); Counts/2keV",8192,0,16384);
	tigemmaList->Add(addDopp_emma_lampshade);

  emma_tig_dt_E = new TH2F("emma_tig_dt_E","emma_tig_dt_E; EMMA Si - TIG Time Difference (ns); Doppler Energy (keV)",18000,-10000,8000,8196,0,16384);
  tigemmaList->Add(emma_tig_dt_E);

  //////////////////////////////
  //
  // Time Gated TIGRESS Spectra
  //
  //////////////////////////////
  excE_tg = new TH1F("excE_tg","Excitation_Energy_MeV_Time_Gated; Exc Energy (MeV); Counts/50keV",500,-5.,20.); 
  tgList->Add(excE_tg);
  excE_theta_tg = new TH2F("excE_theta_tg","Excitation_Energy_Theta_Time_Gated; S3 #theta_{lab} (#circ); Exc Energy (MeV)",60,120,180,500,-5.,20.); 
  tgList->Add(excE_theta_tg);
  addDopp_tg = new TH1F("addDopp_tg", "Addback_Doppler_Energy_Time_Gated; Doppler Energy (keV); Counts/2keV", 8192, 0, 16384);
  tgList->Add(addDopp_tg);
  addDopp_ANum_tg = new TH2F("addDopp_ANum_tg", "Addback_Doppler_Energy_V_ArrayNumber_Time_Gated; Array Number; Doppler Energy (keV)", 50, 15, 65, 8192, 0, 16384);
  tgList->Add(addDopp_ANum_tg);
  addDopp_exc_tg = new TH2F("addDopp_Exc_tg", "Addback_Doppler_Energy_V_Excitation_Energy_Time_Gated; Doppler Energy (keV); Exc Energy (MeV)", 8192, 0, 16384, 500, -5., 20.);
  tgList->Add(addDopp_exc_tg);
  addE_addE_tofg = new TH2F("addE_addE_tofg", "Addback_Gamma_Gamma_Time_Gated; Energy (keV); Energy (keV)", 8192, 0, 16384, 8192, 0, 16384);
  tgList->Add(addE_addE_tofg);
  addDopp_addDopp_tg = new TH2F("addDopp_addDopp_tg", "Addback_Gamma_Gamma_Time_Gated_Doppler_Corrected; Energy (keV); Energy (keV)", 8192, 0, 16384, 8192, 0, 16384);
  tgList->Add(addDopp_addDopp_tg);
  s3_E_theta_gated = new TH2F("s3_E_theta_gated","s3_E_theta_gated; #theta_{lab} (#circ); Energy (keV)",60,120,180, 8192, 0, 16384); 
  tgList->Add(s3_E_theta_gated);
  s3_E_gated = new TH1F("S3_E_gated","S3_E_gated; Energy (keV); Counts/8keV",2048,0,16384);
  tgList->Add(s3_E_gated); 
  icSumVSi_gated_350 = new TH2F("icSumVSi_gated_350","Si_Energy_VS_IC_SUM_gated_on_350_gamma; EMMA Si Energy [keV]; IC Energy [keV]",8192,0,16384,8192,0,16384);
  tgList->Add(icSumVSi_gated_350);


  //////////////////////////////
  //
  // Time Random Gated Spectra
  //
  //////////////////////////////
  excE_bg = new TH1F("excE_bg","Excitation_Energy_MeV_Random_Time_Gated; Exc Energy (MeV); Counts/50keV",500,-5.,20.); 
  bgList->Add(excE_bg);
  excE_theta_bg = new TH2F("excE_theta_bg","Excitation_Energy_Theta_Random_Time_Gated; #theta_{lab} (#circ); Exc Energy (MeV)",60,120,180,500,-5.,20.); 
  bgList->Add(excE_theta_bg);
  addDopp_bg = new TH1F("addDopp_bg", "Addback_Doppler_Energy_Random_Time_Gated; Doppler Energy (keV); Counts/2keV", 8192, 0, 16384);
  bgList->Add(addDopp_bg);
  addDopp_ANum_bg = new TH2F("addDopp_ANum_bg", "Addback_Doppler_Energy_V_ArrayNumber_Random_Time_Gated; Array Number; Doppler Energy (keV)", 50, 15, 65, 8192, 0, 16384);
  bgList->Add(addDopp_ANum_bg);
  addDopp_exc_bg = new TH2F("addDopp_Exc_bg", "Addback_Doppler_Energy_V_Excitation_Energy_Random_Time_Gated; Doppler Energy (keV); Exc Energy (MeV)", 8192, 0, 16384, 500, -5., 20.);
  bgList->Add(addDopp_exc_bg);

  //////////////////////////////
  //
  // PID Gated TIGRESS Spectra
  //
  //////////////////////////////
  doppE_19F_pidg = new TH1F("doppE_19F_pidg","Doppler energy gated on 19F PID and TIG-EMMA Timing; Energy (keV); Counts/2keV",8192,0,16384);
  PIDgatedList->Add(doppE_19F_pidg);
  doppE_21Ne_pidg = new TH1F("doppE_21Ne_pidg","Doppler energy gated on 21Ne PID and TIG-EMMA Timing; Energy (keV); Counts/2keV",8192,0,16384);
  PIDgatedList->Add(doppE_21Ne_pidg);
  //doppE_21Ne_pidg_delayT = new TH1F("doppE_21Ne_pidg_delayT","Doppler energy gated on 21Ne PID and delayed TIG-EMMA Timing;Energy (keV);Counts",8192,0,16384);
  //PIDgatedList->Add(doppE_21Ne_pidg_delayT);
  doppE_21Ne_pidg_xPosG = new TH1F("doppE_21Ne_pidg_xPosG","Doppler energy gated on 21Ne PID & PGAC xPos<0 and TIG-EMMA Timing; Energy (keV); Counts/2keV",8192,0,16384);
  PIDgatedList->Add(doppE_21Ne_pidg_xPosG);

  doppE_Ne_pid_s3_gated = new TH1F("doppE_Ne_pid_s3_gated","Doppler energy gated on Ne PID and TIG-EMMA Timing and hit in S3; Energy (keV); Counts/2keV",8192,0,16384);
  PIDgatedList->Add(doppE_Ne_pid_s3_gated);

  excE_PIDG = new TH1F("excE_PIDG","Excitation_Energy_MeV_PID_Gated;Exc Energy (MeV); Counts/50keV",500,-5.,20.); 
  PIDgatedList->Add(excE_PIDG);
  excE_theta_PIDG = new TH2F("excE_theta_PIDG","Excitation_Energy_Theta_PID_Gated; #theta_{lab} (#circ); Exc Energy (MeV)",60,120,180,500,-5.,20.); 
  PIDgatedList->Add(excE_theta_PIDG);

  addDopp_PIDG = new TH1F("addDopp_PIDG", "Addback_Doppler_Energy_PID_Gated; Doppler Energy (keV); Counts/2keV", 8192, 0, 16384);
  PIDgatedList->Add(addDopp_PIDG);
  addDopp_ANum_PIDG = new TH2F("addDopp_ANum_PIDG", "Addback_Doppler_Energy_V_ArrayNumber_PID_Gated; Array Number; Doppler Energy (keV)", 50, 15, 65, 8192, 0, 16384);
  PIDgatedList->Add(addDopp_ANum_PIDG);
  addDopp_exc_PIDG = new TH2F("addDopp_exc_PIDG", "Addback_Doppler_Energy_V_Excitation_Energy_PID_GatedDoppler Energy (keV); Exc Energy (MeV)", 8192, 0, 16384, 500, -5., 20.);
  PIDgatedList->Add(addDopp_exc_PIDG);

  aE_icSumVSi_gated = new TH1F("aE_icSumVSi_gated","Addback_gamma_energy_gated_on_IC_Si_cut; Energy (keV); Counts/2keV",8192,0,16384);
  PIDgatedList->Add(aE_icSumVSi_gated); 

  //////////////////////////////
  //
  // PID Gated S3 Spectra
  //
  //////////////////////////////
	s3_gatedon_Em_and_Tig = new TH1F("s3_gatedon_Em_and_Tig","S3_E; Energy (keV) gated on EMMA 21Ne PID and TIGRESS 4556 keV; Counts/8keV",4096,0,32768); //here i am!
	PIDgatedListS3->Add(s3_gatedon_Em_and_Tig); 

  s3_E_theta_Ne_PID_Gated = new TH2F("s3_E_theta_Ne_PID_Gated","S3 E vs #theta gated on Ne PID; #theta_{lab} (#circ); Energy (keV)",60,120,180,8192,0,16384);
  PIDgatedListS3->Add(s3_E_theta_Ne_PID_Gated);
  s3_E_theta_Ne_PID_4556_gamma_gated = new TH2F("s3_E_theta_Ne_PID_4556_gamma_gated","S3 E vs #theta gated on Ne PID, TIG-EMMA TOF, 4556 keV #gamma; #theta_{cm} (#circ); Energy (keV)",60,120,180,8192,0,16384);
  PIDgatedListS3->Add(s3_E_theta_Ne_PID_4556_gamma_gated);

  excE_thetaCm_Ne_PID_Gated = new TH2F("excE_thetaCm_Ne_PID_Gated","Exc Energy vs #theta_{cm} gated on Ne PID; #theta_{cm} (#circ); Exc Energy (keV)",90,0,90,500,-5000,20000); // 50keV binning
  PIDgatedListS3->Add(excE_thetaCm_Ne_PID_Gated);
  excE_thetaCm_Ne_PID_4556_gamma_gated = new TH2F("excE_thetaCm_Ne_PID_4556_gamma_gated","Exc Energy vs #theta gated on Ne PID, TIG-EMMA TOF, 4556 keV #gamma; #theta_{cm} (#circ); Exc Energy (keV)",90,0,90,500,-5000,20000); // 50keV binning
  PIDgatedListS3->Add(excE_thetaCm_Ne_PID_4556_gamma_gated);

  excE_doppE_Ne_PID_Gated = new TH2F("excE_doppE_Ne_PID_Gated","Exc Energy vs Doppler Energy, Ne PID and EMMA-TIG TOF Gated; Doppler Energy (keV); Exc Energy (keV)",8192,0,16384,500,-5000,20000); // 50keV vs 2keV binning
  PIDgatedListS3->Add(excE_doppE_Ne_PID_Gated);

  /*
  rings_PIDG = new TH2F("rings_PID_Gated","rings_PID_Gated; Ring Num; ",25,0,25,2048,0,8192); 
  PIDgatedListS3->Add(rings_PIDG);
  s3_E_theta_gated_PIDG = new TH2F("s3_E_theta_gated_PID_Gated","s3_E_theta_gated_PID_Gated; Energy (keV); #theta_{lab} (#circ)",180,0,180, 820, 0, 16384); 
  PIDgatedListS3->Add(s3_E_theta_gated_PIDG);
  */
  s3_E_gated_PIDG_gamG = new TH1F("s3_E_gated_PIDG_gamG","s3_E_theta_gated_PID_Gated_1806keV_Gated; Excitation Energy (MeV); Counts/50keV",500, -5, 20); 
  PIDgatedListS3->Add(s3_E_gated_PIDG_gamG);

}