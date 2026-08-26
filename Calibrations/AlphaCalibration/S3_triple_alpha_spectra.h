#ifndef S3_triple_alpha_spectra_h
#define S3_triple_alpha_spectra_h

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
#include "TEnv.h"
#include "Declarations.h"

using namespace std;

class S3_triple_alpha_spectra {

	public :

		S3_triple_alpha_spectra(){;} 
		void Sort_triple_alpha_data();
		void Initialise();
};
#endif

void S3_triple_alpha_spectra::Initialise() {

  printf("Start initializations\n");
  printf("Creating list\n");

  // Histogram Lists
  // Triple alpha energy peaks
  triple_alpha_List = new TList;
  // S3 energies
  s3_energies_List = new TList;
  // S3 energies in each ring
  s3_rings_List = new TList;
  // S3 energies in each sector as a function of ring number
  s3_sectors_List = new TList;
  // The alpha energies in a given sector
  s3_sectors_alphaE_List = new TList;


  printf("Creating histograms\n");
  
  // -------------- tiple_alpha_List histograms -------------- //
  // Energies of the triple alpha sources (238Pu, 241Am, 24Cm)
	h_s3energy_before_cal = new TH1D("h_s3energy_before","S3 energy for triple alpha source before calibration run",3000,4000,7000);
	h_s3energy_before_cal->GetYaxis()->SetTitle("Counts [1 keV/bin]");
	h_s3energy_before_cal->GetXaxis()->SetTitle("Energy [keV]");
  triple_alpha_List->Add(h_s3energy_before_cal);
	h_s3energy_after_cal = new TH1D("h_s3energy_after","S3 energy for triple alpha source after calibration run",3000,4000,7000);
	h_s3energy_after_cal->GetYaxis()->SetTitle("Counts [1 keV/bin]");
	h_s3energy_after_cal->GetXaxis()->SetTitle("Energy [keV]");
  triple_alpha_List->Add(h_s3energy_after_cal);
  // -------------------------------------------------------- //


  // --------------- s3_energies_List histograms ------------- //
	h_energyVring_before_cal = new TH2D("h_energyVring_before","S3 Energy vs Ring number (before triple alpha calibration run)",24,0,24,1000,0,10000);
	h_energyVring_before_cal->GetYaxis()->SetTitle("S3 pixel energy (keV)");
	h_energyVring_before_cal->GetXaxis()->SetTitle("Ring number");
  s3_energies_List->Add(h_energyVring_before_cal);
	h_energyVring_after_cal = new TH2D("h_energyVring_after","S3 Energy vs Ring number (after triple alpha calibration run)",24,0,24,1000,0,10000);
	h_energyVring_after_cal->GetYaxis()->SetTitle("S3 pixel energy (keV)");
	h_energyVring_after_cal->GetXaxis()->SetTitle("Ring number");
  s3_energies_List->Add(h_energyVring_after_cal);
  h_energyVring_multi_1_before_cal = new TH2D("h_energyVring_multi_1_before_cal", "S3 Energy vs Ring number ring multi == 1 (before triple alpha calibration run)",24,0,24,1000,0,10000);
  h_energyVring_multi_1_before_cal->GetYaxis()->SetTitle("S3 pixel energy (keV)");
	h_energyVring_multi_1_before_cal->GetXaxis()->SetTitle("Ring number");
  s3_energies_List->Add(h_energyVring_multi_1_before_cal);
  h_energyVring_multi_1_after_cal = new TH2D("h_energyVring_multi_1_after_cal", "S3 Energy vs Ring number ring multi == 1 (after triple alpha calibration run)",24,0,24,1000,0,10000);
  h_energyVring_multi_1_after_cal->GetYaxis()->SetTitle("S3 pixel energy (keV)");
	h_energyVring_multi_1_after_cal->GetXaxis()->SetTitle("Ring number");
  s3_energies_List->Add(h_energyVring_multi_1_after_cal);
  h_sectorE_v_ring_num_before_cal = new TH2F("h_sectorE_v_ring_num_before_cal", "S3 sector energy vs ring number (before calibration)", 24, 0, 24,10000, 0, 10000);
  s3_energies_List->Add(h_sectorE_v_ring_num_before_cal);
  h_sectorE_v_ring_num_before_cal->GetYaxis()->SetTitle("S3 sector energy (keV)");
  h_sectorE_v_ring_num_before_cal->GetXaxis()->SetTitle("S3 ring number");
  h_sectorE_v_ring_num_after_cal = new TH2F("h_sectorE_v_ring_num_after_cal", "S3 sector energy vs ring number (after calibration)", 24, 0, 24,10000, 0, 10000);
  s3_energies_List->Add(h_sectorE_v_ring_num_after_cal);
  h_sectorE_v_ring_num_after_cal->GetYaxis()->SetTitle("S3 sector energy (keV)");
  h_sectorE_v_ring_num_after_cal->GetXaxis()->SetTitle("S3 ring number");
  // -------------------------------------------------------- //


  // --------------- s3_rings_List histograms --------------- //
  h_ring0_before_cal = new TH1D("h_ring0_before_cal","S3 pixel energy for ring 0 (before calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring0_before_cal);
  h_ring2_before_cal = new TH1D("h_ring2_before_cal","S3 pixel energy for ring 2 (before calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring2_before_cal);
  h_ring4_before_cal = new TH1D("h_ring4_before_cal","S3 pixel energy for ring 4 (before calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring4_before_cal);
  h_ring6_before_cal = new TH1D("h_ring6_before_cal","S3 pixel energy for ring 6 (before calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring6_before_cal);
  h_ring8_before_cal = new TH1D("h_ring8_before_cal","S3 pixel energy for ring 8 (before calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring8_before_cal);
  h_ring10_before_cal = new TH1D("h_ring10_before_cal","S3 pixel energy for ring 10 (before calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring10_before_cal);
  h_ring12_before_cal = new TH1D("h_ring12_before_cal","S3 pixel energy for ring 12 (before calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring12_before_cal);
  h_ring14_before_cal = new TH1D("h_ring14_before_cal","S3 pixel energy for ring 14 (before calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring14_before_cal);
  h_ring16_before_cal = new TH1D("h_ring16_before_cal","S3 pixel energy for ring 16 (before calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring16_before_cal);
  h_ring18_before_cal = new TH1D("h_ring18_before_cal","S3 pixel energy for ring 18 (before calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring18_before_cal);
  h_ring20_before_cal = new TH1D("h_ring20_before_cal","S3 pixel energy for ring 20 (before calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring20_before_cal);
  h_ring22_before_cal = new TH1D("h_ring22_before_cal","S3 pixel energy for ring 22 (before calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring22_before_cal);

  h_ring0_after_cal = new TH1D("h_ring0_after_cal","S3 pixel energy for ring 0 (after calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring0_after_cal);
  h_ring2_after_cal = new TH1D("h_ring2_after_cal","S3 pixel energy for ring 2 (after calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring2_after_cal);
  h_ring4_after_cal = new TH1D("h_ring4_after_cal","S3 pixel energy for ring 4 (after calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring4_after_cal);
  h_ring6_after_cal = new TH1D("h_ring6_after_cal","S3 pixel energy for ring 6 (after calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring6_after_cal);
  h_ring8_after_cal = new TH1D("h_ring8_after_cal","S3 pixel energy for ring 8 (after calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring8_after_cal);
  h_ring10_after_cal = new TH1D("h_ring10_after_cal","S3 pixel energy for ring 10 (after calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring10_after_cal);
  h_ring12_after_cal = new TH1D("h_ring12_after_cal","S3 pixel energy for ring 12 (after calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring12_after_cal);
  h_ring14_after_cal = new TH1D("h_ring14_after_cal","S3 pixel energy for ring 14 (after calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring14_after_cal);
  h_ring16_after_cal = new TH1D("h_ring16_after_cal","S3 pixel energy for ring 16 (after calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring16_after_cal);
  h_ring18_after_cal = new TH1D("h_ring18_after_cal","S3 pixel energy for ring 18 (after calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring18_after_cal);
  h_ring20_after_cal = new TH1D("h_ring20_after_cal","S3 pixel energy for ring 20 (after calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring20_after_cal);
  h_ring22_after_cal = new TH1D("h_ring22_after_cal","S3 pixel energy for ring 22 (after calibration)",3000,4000,7000);
  s3_rings_List->Add(h_ring22_after_cal);
  // -------------------------------------------------------- //

  // -------------- s3_sectors_List histograms -------------- //
  h_sector0E_v_ring_num_after_cal = new TH2F("h_sector0E_v_ring_num_after_cal","S3 sector 0 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector0E_v_ring_num_after_cal);
  h_sector1E_v_ring_num_after_cal = new TH2F("h_sector1E_v_ring_num_after_cal","S3 sector 1 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector1E_v_ring_num_after_cal);
  h_sector2E_v_ring_num_after_cal = new TH2F("h_sector2E_v_ring_num_after_cal","S3 sector 2 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector2E_v_ring_num_after_cal);
  h_sector3E_v_ring_num_after_cal = new TH2F("h_sector3E_v_ring_num_after_cal","S3 sector 3 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector3E_v_ring_num_after_cal);
  h_sector4E_v_ring_num_after_cal = new TH2F("h_sector4E_v_ring_num_after_cal","S3 sector 4 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector4E_v_ring_num_after_cal);
  h_sector5E_v_ring_num_after_cal = new TH2F("h_sector5E_v_ring_num_after_cal","S3 sector 5 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector5E_v_ring_num_after_cal);
  h_sector6E_v_ring_num_after_cal = new TH2F("h_sector6E_v_ring_num_after_cal","S3 sector 6 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector6E_v_ring_num_after_cal);
  h_sector7E_v_ring_num_after_cal = new TH2F("h_sector7E_v_ring_num_after_cal","S3 sector 7 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector7E_v_ring_num_after_cal);
  h_sector8E_v_ring_num_after_cal = new TH2F("h_sector8E_v_ring_num_after_cal","S3 sector 8 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector8E_v_ring_num_after_cal);
  h_sector9E_v_ring_num_after_cal = new TH2F("h_sector9E_v_ring_num_after_cal","S3 sector 9 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector9E_v_ring_num_after_cal);
  h_sector10E_v_ring_num_after_cal = new TH2F("h_sector10E_v_ring_num_after_cal","S3 sector 10 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector10E_v_ring_num_after_cal);
  h_sector11E_v_ring_num_after_cal = new TH2F("h_sector11E_v_ring_num_after_cal","S3 sector 11 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector11E_v_ring_num_after_cal);
  h_sector12E_v_ring_num_after_cal = new TH2F("h_sector12E_v_ring_num_after_cal","S3 sector 12 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector12E_v_ring_num_after_cal);
  h_sector13E_v_ring_num_after_cal = new TH2F("h_sector13E_v_ring_num_after_cal","S3 sector 13 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector13E_v_ring_num_after_cal);
  h_sector14E_v_ring_num_after_cal = new TH2F("h_sector14E_v_ring_num_after_cal","S3 sector 14 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector14E_v_ring_num_after_cal);
  h_sector15E_v_ring_num_after_cal = new TH2F("h_sector15E_v_ring_num_after_cal","S3 sector 15 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector15E_v_ring_num_after_cal);
  h_sector16E_v_ring_num_after_cal = new TH2F("h_sector16E_v_ring_num_after_cal","S3 sector 16 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector16E_v_ring_num_after_cal);
  h_sector17E_v_ring_num_after_cal = new TH2F("h_sector17E_v_ring_num_after_cal","S3 sector 17 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector17E_v_ring_num_after_cal);
  h_sector18E_v_ring_num_after_cal = new TH2F("h_sector18E_v_ring_num_after_cal","S3 sector 18 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector18E_v_ring_num_after_cal);
  h_sector19E_v_ring_num_after_cal = new TH2F("h_sector19E_v_ring_num_after_cal","S3 sector 19 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector19E_v_ring_num_after_cal);
  h_sector20E_v_ring_num_after_cal = new TH2F("h_sector20E_v_ring_num_after_cal","S3 sector 20 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector20E_v_ring_num_after_cal);
  h_sector21E_v_ring_num_after_cal = new TH2F("h_sector21E_v_ring_num_after_cal","S3 sector 21 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector21E_v_ring_num_after_cal);
  h_sector22E_v_ring_num_after_cal = new TH2F("h_sector22E_v_ring_num_after_cal","S3 sector 22 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector22E_v_ring_num_after_cal);
  h_sector23E_v_ring_num_after_cal = new TH2F("h_sector23E_v_ring_num_after_cal","S3 sector 23 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector23E_v_ring_num_after_cal);
  h_sector24E_v_ring_num_after_cal = new TH2F("h_sector24E_v_ring_num_after_cal","S3 sector 24 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector24E_v_ring_num_after_cal);
  h_sector25E_v_ring_num_after_cal = new TH2F("h_sector25E_v_ring_num_after_cal","S3 sector 25 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector25E_v_ring_num_after_cal);
  h_sector26E_v_ring_num_after_cal = new TH2F("h_sector26E_v_ring_num_after_cal","S3 sector 26 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector26E_v_ring_num_after_cal);
  h_sector27E_v_ring_num_after_cal = new TH2F("h_sector27E_v_ring_num_after_cal","S3 sector 27 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector27E_v_ring_num_after_cal);
  h_sector28E_v_ring_num_after_cal = new TH2F("h_sector28E_v_ring_num_after_cal","S3 sector 28 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector28E_v_ring_num_after_cal);
  h_sector29E_v_ring_num_after_cal = new TH2F("h_sector29E_v_ring_num_after_cal","S3 sector 29 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector29E_v_ring_num_after_cal);
  h_sector30E_v_ring_num_after_cal = new TH2F("h_sector30E_v_ring_num_after_cal","S3 sector 30 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector30E_v_ring_num_after_cal);
  h_sector31E_v_ring_num_after_cal = new TH2F("h_sector31E_v_ring_num_after_cal","S3 sector 31 energy (50 keV bins) vs ring number (after calibration)", 24, 0, 24,200, 0, 10000);
  s3_sectors_List->Add(h_sector31E_v_ring_num_after_cal);
  // -------------------------------------------------------- //

  // ---------- s3_sectors_alphaE_List histograms ----------- // 
  for (int i = 0; i < 32; i++) {
      h_s3Sector_alphaE[i] = new TH1F(Form("h_s3Sector%d_alphaE", i), Form("S3 Sector %d energy for triple alpha source after calibration run", i), 3000, 4000, 7000);
      s3_sectors_alphaE_List->Add(h_s3Sector_alphaE[i]);
  }
  // -------------------------------------------------------- //

  cout << "Done" << endl;
}
