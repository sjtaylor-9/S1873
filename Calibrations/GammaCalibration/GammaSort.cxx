// g++ GammaSort.cxx -std=c++0x -I$GRSISYS/include -L$GRSISYS/lib `grsi-config --cflags --all-libs --GRSIData-libs` -I$GRSISYS/GRSIData/include -L$GRSISYS/GRSIData/lib `root-config --cflags --libs` -lTTigress -o GammaSort
// S1873 - original sort codes borrowed from M. Williams and C. Angus
// GammaSort.cxx
// S. Taylor
// 29/04/2026

#define GammaSort_cxx
#include "GammaSort.h"

using namespace std;

Double_t r2d = TMath::RadToDeg();

bool suppAdd = false;

void GammaSort::Calibrate_gamma_spectra(char const * afile, char const * calfile, char const * outfile) {

	Initialise();
  
	// rootfile branch structure
	typedef struct {double rawE, dopE, time, det, core, beta;} GAMMA;
	GAMMA gamma;
	typedef struct {double rawE, dopE, time, det, core, beta;} GAMMA2;
	GAMMA2 gamma2;

	// define output rootfile and branches
	TFile * myfile = new TFile(outfile, "RECREATE");
	// TIGRESS Singles
	TTree * tig = new TTree("tig","TIGRESS Singles");
  	tig->Branch("gamma",&gamma,"rawE/D:dopE/D:time/D:det/D:core/D:beta/D");
	tig->Branch("gamma2",&gamma2,"rawE/D:dopE/D:time/D:det/D:core/D:beta/D");

  // Open Analysis Tree
  TFile * analysisfile = new TFile(afile, "READ");   
  if (!analysisfile->IsOpen()) {
    printf("Opening file %s failed, aborting\n", afile);
    return;
  }
 
  printf("File %s opened\n", afile);
  TChain * AnalysisTree = (TChain * ) analysisfile->Get("AnalysisTree");
  long analentries = AnalysisTree->GetEntries();
  const char * testval = "NULL";

  // Checks for branches and sets pointers
  // Set TIGRESS hits
  TTigress * tig_data = 0;
  if (AnalysisTree->FindBranch("TTigress")) {
    AnalysisTree->SetBranchAddress("TTigress", & tig_data);
  } else {
    cout << "Branch 'TTigress' not found! TTigress variable is NULL pointer" << endl;
  }

  // Define Hit Pointers
  TTigressHit * tig_hit, * add_hit, * add_hit2;

  printf("Reading calibration file: %s\n", calfile);
  TChannel::ReadCalFile(calfile); // Read calibration file
  printf("\nSorting analysis events...\n");

  for (int jentry = 0; jentry < analentries; jentry++) { // loop over events in analysis tree
  
	AnalysisTree->GetEntry(jentry);
	
	if (tig_data)	{
		for (int t = 0; t < tig_data->GetAddbackMultiplicity(); t++) { // Loop over tigress events
            tig_hit = tig_data->GetTigressHit(t);
            suppAdd = tig_hit->BGOFired();
            if (!suppAdd && tig_hit->GetEnergy() > 15) { //h_pid_s3emma suppressor condition?
				gamma.rawE = tig_hit->GetEnergy(); // get doppler corrected gamma energy
				//gamma.dopE = tig_hit->GetDoppler();
				gamma.time = tig_hit->GetTime();
				gamma.det = tig_hit->GetDetector();

				tig->Fill(); // Fill TIGRESS singles tree
				h_rawE_addback->Fill(tig_hit->GetEnergy()); // Fill raw energy

				for(int bgoInd=0; bgoInd < tig_data->GetBGOMultiplicity(); bgoInd++){
					if((tig_data->GetBGO(bgoInd).GetDetector() == tig_hit->GetDetector()) && (tig_data->GetBGO(bgoInd).GetEnergy() > 0.)){
              			h_tigT_bgoT_supp->Fill(tig_hit->GetCfd() - tig_data->GetBGO(bgoInd).GetCfd());
            		}
				}

			} // End Suppressor Conditon
			if(tig_hit->GetEnergy() > 15){
          		for(int bgoInd=0; bgoInd < tig_data->GetBGOMultiplicity(); bgoInd++){
            		if((tig_data->GetBGO(bgoInd).GetDetector() == tig_hit->GetDetector()) && (tig_data->GetBGO(bgoInd).GetEnergy() > 0.)){
              			h_tigT_bgoT->Fill(tig_hit->GetCfd() - tig_data->GetBGO(bgoInd).GetCfd());
            		}
          		}
       		}
		} // End Loop over TIGRESS singles Events
	}

	tig_data->ResetAddback();	//================== Stupid GRSIsort bug

	if (jentry % 10000 == 0)
      	cout << setiosflags(ios::fixed) << "Entry " << jentry << " of " << analentries << ", " << 100 * jentry / analentries << "% complete" << "\r" << flush; // Event counter
} // End Analysis Tree Loop

  cout << "Entry " << analentries << " of " << analentries << ", 100% complete" << endl;
  cout << "Event sorting complete" << endl;

  cout << "Writing histograms to " << outfile << endl;

  myfile->cd();
  
  TDirectory *CalibrationDir = myfile->mkdir("GammaCalibrationPlots");
  CalibrationDir->cd();
  calibrationsList->Write();
  myfile->cd();

  TDirectory *tigDir = myfile->mkdir("TIG");
  tigDir->cd();
  tigList->Write();
  myfile->cd();

  TDirectory *tigbgoDir = myfile->mkdir("TIG_BGO");
  tigbgoDir->cd();
  tigBgoList->Write();
  myfile->cd();

  myfile->Write();
  myfile->Close();

}

int main(int argc, char ** argv) {


  GammaSort * mysort = new GammaSort();

  char const * afile;
  char const * outfile;
  char const * calfile;
  printf("Starting sortcode\n");

  std::string grsi_path = getenv("GRSISYS"); // Finds the GRSISYS path to be used by other parts of the grsisort code
  if (grsi_path.length() > 0) {
    grsi_path += "/";
  }
  // Read in grsirc in the GRSISYS directory to set user defined options on grsisort startup
  grsi_path += ".grsirc";
  gEnv->ReadFile(grsi_path.c_str(), kEnvChange);
  TParserLibrary::Get()->Load();

  // Input-chain-file, output-histogram-file
  if (argc == 1) {
    cout << "Insufficient arguments, provide analysis tree" << endl;
    return 0;
  } else if (argc == 2) {
    afile = argv[1];
    //calfile = "CalibrationFile.cal";
    //calfile = "CalibrationFile2020Nov20.cal";
    calfile = "CalibrationFile_Dec19.cal";
    outfile = "HistFiles/Histograms.root";
    printf("Analysis file: %s\nCalibration file: %s\nOutput file: %s\n", afile, calfile, outfile);
    mysort->Calibrate_gamma_spectra(afile, calfile, outfile);
  } else if (argc == 3) {
    afile = argv[1];
    calfile = argv[2];
    outfile = "HistFiles/Histograms.root";
    printf("Analysis file: %s\nCalibration file: %s\nOutput file: %s\n", afile, calfile, outfile);
    mysort->Calibrate_gamma_spectra(afile, calfile, outfile);
  } else if (argc == 4) {
    afile = argv[1];
    calfile = argv[2];
    outfile = argv[3];
    printf("Analysis file: %s#define Sortcode_cxx\nCalibration file: %s\nOutput file: %s\n", afile, calfile, outfile);
    mysort->Calibrate_gamma_spectra(afile, calfile, outfile);
  } else if (argc >= 4) {
    afile = argv[1];
    calfile = argv[2];
    outfile = argv[3];
    printf("Analysis file: %s\nCalibration file: %s\nOutput file: %s\n", afile, calfile, outfile);
    if (argc > 4) {
        printf("Extra arguments detected and ignored\n");
    }
    mysort->Calibrate_gamma_spectra(afile, calfile, outfile);
}

  return 0;
}