#ifndef GammaSort_h
#define GammaSort_h

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
#include "TTigress.h"
#include "TSpectrum.h"
#include "TChannel.h"
#include "TParserLibrary.h"
#include "TEnv.h"
#include "Declarations.h"

using namespace std;

class GammaSort {

	public :

		GammaSort(){;} 
		void Calibrate_gamma_spectra(const char*, const char*, const char*);
		void Initialise();
};
#endif

void GammaSort::Initialise() {

  printf("Start initializations\n");
  printf("Creating list\n");

  // Histogram Lists
  // Singles:
  tigList = new TList;
  // Coincidences
  tigBgoList = new TList;
  // Calibration plots:
  calibrationsList = new TList;

  printf("Creating histograms\n");
  
  // TIGRESS singles
  h_rawE_addback = new TH1F("h_rawE_addback","Add-Back Raw Energy",10000,0,10000); // Add-Back Energy (no doppler correction)
  tigList->Add(h_rawE_addback);

  // TIGRESS-BGO coincidences
  h_tigT_bgoT = new TH1F("h_tigT_bgoT","Tigress-BGO time (unsuppressed)",4096,-2048,2048);
  h_tigT_bgoT->GetXaxis()->SetTitle("Same-detector Tigress-BGO time (ns)");
  tigBgoList->Add(h_tigT_bgoT);
  h_tigT_bgoT_supp = new TH1F("h_tigT_bgoT_supp","Tigress-BGO time (suppressed)",4096,-2048,2048);
  h_tigT_bgoT_supp->GetXaxis()->SetTitle("Same-detector Tigress-BGO time (ns)");
  tigBgoList->Add(h_tigT_bgoT_supp);
  
  cout << "Done" << endl;

}
