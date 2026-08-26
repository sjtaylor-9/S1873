//g++ hit_map_XYZ.cxx -std=c++0x -I$GRSISYS/include -L$GRSISYS/lib `grsi-config --cflags --all-libs --GRSIData-libs` -I$GRSISYS/GRSIData/include -L$GRSISYS/GRSIData/lib `root-config --cflags --libs` -o HitMap_XYZ

// This Draws a hit pattern of the S3 Detector and plots the excitation energy for all possible X-Y-Z offset positions in the range -2.5 mm -> 2.5 mm
#include <iostream> 
#include <iomanip> 
#include "TCutG.h"
#include "TH1.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TChain.h"
#include "TH2.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TList.h"
#include "TReaction.h"
#include "TSRIM.h"
#include "TSpectrum.h"
#include "TTigress.h"
#include "TS3.h"
#include "TEmma.h"
#include "TSpectrum.h"
#include "TChannel.h"
#include "TRandom.h"
#include "TParserLibrary.h"
#include "TEnv.h"
#include "TReaction.h"
#include "TSRIM.h"

double pi = TMath::Pi();
double r2d = TMath::RadToDeg();
double d2r = TMath::DegToRad();
bool suppAdd = false;

double s3_emma_T[2] = {450, 750};       // S3 - EMMA Timing.
double tig_emma_T[2] = {450, 700};     // TIGRESS - EMMA Timing.
double tig_s3_T[2] = {-45, 100};        // TIGRESS - S3 Timing.

TCutG *Neon_Gate;

bool loadCutG(char const *cutfile)
{ // 2D Gate Loader. Code only uses mass gate if cut file given
  TFile *cuts = new TFile(cutfile, "READ");
  Neon_Gate = (TCutG *)cuts->Get("neon_cut");		// cut on EMMA IC vs IC+Silicon
  return true;
}

bool gate1D(Double_t value, Double_t min, Double_t max)
{
  if (min < value && value < max)
    return true;
  else
    return false;
}

bool CutG_loaded = false;

using namespace std;
void HitMap(char const* infile, double phioffset, char const* calfile, char const* outfile, char const * cutfile = "NULL"){

  bool theta_phi_iter = true; // Are you doing the theta-phi iter?
  TList * list = new TList; // create hist list for hitmaps
  TH2F *hitmap = new TH2F("hitmap","S3 Hitmap",200,-50,50,200,-50,50);list->Add(hitmap); // S3 hitmap 
  TH2F *theta_phi_hitmap = new TH2F("theta_phi_hitmap","theta_phi_hitmap",500,120,170,400,-200,200); list->Add(theta_phi_hitmap); // default theta phi hitmap
  TH2F *h_pid = new TH2F("h_pid","IC Sum vs IC+Silicon",1000,0,10000,1000,0,10000); list->Add(h_pid);

  TList *thetaphi_iter = new TList; // create Hist List
  TList *excitephi_iter = new TList; // create Hist List
  TList *excite_iter = new TList; // create Hist List
  TList *excitetheta_iter = new TList; // create Hist List

  TH2F *thetaphi_offset_iter[46][31][21]; // initialise theta-phi offset iter hist pointer
  TH2F *h_phi_v_excite[46][31][21]; // initialise Ex-phi offset iter hist pointer
  TH2F *h_theta_v_excite[46][31][21]; // intialise Ex-theta offset iter hist pointer
  TH1F *h_excite[46][31][21]; // intialise Ex offset hist pointer

  if(theta_phi_iter) {
	
    for(int i=0;i<46;i++){ // loop over x offsets
        for(int j=0;j<31;j++){ // loop over y offsets
            for(int k=0;k<21;k++){ // loop over z offsets

                char hname[256]; // hist name
                char hname2[256]; // hist name
                char hname3[256]; // hist name
                char hname4[256]; // hist name
                
                // Iterative ranges for X Y and Z
                float x = i*0.1 - 4.5; // set x var 
                float y = j*0.1 - 1.5; // set y var
                float z = k*0.1 + 2; // set z var 
            
                sprintf(hname,"Hitmap_iter_X_%f_Y_%f_Z_%f",x,y,z); // set hist name
                sprintf(hname2,"ExcitePhi_iter_X_%f_Y_%f_Z_%f",x,y,z); // set hist name
                sprintf(hname3,"Excite_iter_X_%f_Y_%f_Z_%f",x,y,z); // set hist name
                sprintf(hname4,"ExciteTheta_iter_X_%f_Y_%f_Z_%f",x,y,z); // set hist name
                // Create histograms for given X(i)-Y(j)-Z(k) offset value
                // 1. Theta vs phi hit map (theta on x, phi on y)
                // 2. Ex vs phi
                // 3. Ex spectrum
                // 4. Ex vs theta
                thetaphi_offset_iter[i][j][k] = new TH2F(hname,hname,500,120,170,100,-200,200); // create hists
                thetaphi_iter->Add(thetaphi_offset_iter[i][j][k]); // add hist to list

                h_phi_v_excite[i][j][k] = new TH2F(hname2,hname2,36,-180,180,200,-2,18); // create hists
                excitephi_iter->Add(h_phi_v_excite[i][j][k]); // add hist to list

                h_excite[i][j][k] = new TH1F(hname3,hname3,500,-2,18); // create hists 
                excite_iter->Add(h_excite[i][j][k]); // add hist to list

                h_theta_v_excite[i][j][k] = new TH2F(hname4,hname4,90,90,180,200,-2,18); // create hists 
                excitetheta_iter->Add(h_theta_v_excite[i][j][k]); // add hist to list
            } // loop over z offsets
    	} // end loop over y offsets
  	} // end loop over x offsets
  } // end if theta_phi_iter condition

  TFile * inputfile = new TFile(infile, "READ"); // create new TFile and read input analysis tree
  if (!inputfile->IsOpen()) { // is open?
    printf("Opening file failed, aborting\n"); // error if not open
    return;
  }

  TChain * AnalysisTree = (TChain * ) inputfile->Get("AnalysisTree"); // get Analysis TChain
  printf("%i tree files, details:\n", AnalysisTree->GetNtrees()); // get TTree details
  AnalysisTree->ls(); // print TTree
  TTree * tree = (TTree * ) AnalysisTree->GetTree(); // initialize TTree 
  printf("Reading calibration file: %s\n", calfile); // print which cal file is used
  TChannel::ReadCalFile(calfile); // read in cal file
  Int_t nentries = AnalysisTree->GetEntries(); // get N enetries
  
  TS3 *s3 = 0; // initialize s3 null pointer
  AnalysisTree->SetBranchAddress("TS3",&s3); // assign TS3 branch to s3 pointer
  TS3Hit *s3hit, *ringhit, *sectorhit; // initialize S3 hit pointers
  double energy, thetalab, excite;

  // Load cut file:
  const char * testval = "NULL";
  if (strcmp(cutfile, testval) != 0)
  {
    printf("Cuts Loaded from %s \n", cutfile);
    loadCutG(cutfile);
    CutG_loaded = true;
  }

  TEmma *emma = 0; // init emma pointer
  AnalysisTree->SetBranchAddress("TEmma", &emma);
  TEmmaHit *em_hit, *si_hit, *ic_hit, *ssb_hit, *trigger_hit;
  double silicon_energy, icsum, eseg[5], total_energy;

  TTigress *tigress = 0; // init tigress pointer
  AnalysisTree->SetBranchAddress("TTigress", &tigress);
  TTigressHit * tig_hit, * add_hit, * add_hit2;


  // Get SRIM files
  TSRIM * srim_17oC = new TSRIM;
  srim_17oC->ReadEnergyLossFile("O17_in_C.txt"); // Eloss in Carbon
  TSRIM * srim_17oLiF = new TSRIM;
  srim_17oLiF->ReadEnergyLossFile("O17_in_LiF.txt"); // Eloss in LiF
  //double EBeam = 4.0 * 16.9991315; // 4 MeV/u O-17 beam => 68 MeV
  double EBeam = 68.; // 4 MeV/u O-17 beam => 68 MeV
  // Adjust beam energy with SRIM data
  EBeam = srim_17oLiF->GetAdjustedEnergy(EBeam * 1000, 0.19, 0.001) / 1000; 
  printf("Beam energy: %f MeV\n", EBeam);  
  // Since EBeam is constant in this script, TReaction definition does not need to be redefined for every iteration. This massively improves computation time.
  TReaction * o17 = new TReaction("o17", "li7", "t", "ne21", EBeam, 0, true);
  
  for(int jentry=0;jentry<nentries;jentry++){ // loop over analysis events
	AnalysisTree->GetEntry(jentry);  // get event

	if(emma && s3 && tigress){ // EMMA && TIGRESS && S3 coincidence

		s3->SetFrontBackEnergy(0.85); // Needed if spectrum isn't calibrated, front/back must be within 95%
    s3->SetFrontBackTime(500); // Needed to build S3 pixels properly


		for (int e = 0; e < emma->GetMultiplicity(); e++) { // loop over emma events
			em_hit = emma->GetEmmaHit(e);	

			// IC data
			eseg[5] = {0};
			icsum = 0;
			for (int j = 0; j < emma->GetICMultiplicity(); j++) { // IC multi = how many segments have a hit
            			ic_hit = emma->GetICHit(j);
            			eseg[ic_hit->GetSegment()]=ic_hit->GetEnergy(); // get ic segement energy
            			icsum += ic_hit->GetEnergy(); // get ic sum energy
          	}

		// Silicon data
		silicon_energy = 0;
			
    total_energy=0; // Reinitialise total_energy as 0
		for (int k = 0; k < emma->GetSiMultiplicity(); k++) { // Can there be multihit in the silicon?
				si_hit = emma->GetSiHit(k);
				silicon_energy = si_hit->GetEnergy(); // get FP silicon energy
				total_energy = icsum+silicon_energy;	
			}

		//h_pid->Fill(total_energy,icsum);

		if (CutG_loaded) {
      if (Neon_Gate->IsInside(total_energy,icsum)) {
        for(int i=0;i<s3->GetPixelMultiplicity();i++){  // loop over pixel events
      		
					s3hit = s3->GetPixelHit(i); //get pixel hit
					for (int t = 0; t < tigress->GetAddbackMultiplicity(); t++) { // Loop over tigress hits
            add_hit = tigress->GetAddbackHit(t);
        	  suppAdd = add_hit->BGOFired();
						// suppressor condition
						// If a BGO scintillator records a hit then the photon has Compton scattered out of the main detector.
						// Therefore, want suppAdd to be false and deposited energy to be at least 15 keV
        	  if (!suppAdd && add_hit->GetEnergy() > 15) { 
              if (gate1D(s3hit->GetTime() - em_hit->GetTime(), s3_emma_T[0], s3_emma_T[1])) { // S3 - EMMA timing condition
              if (gate1D(add_hit->GetTime() - em_hit->GetTime(), tig_emma_T[0], tig_emma_T[1])) { // TIGRESS - EMMA timing condition
              if (gate1D(add_hit->GetTime() - s3hit->GetTime(), tig_s3_T[0], tig_s3_T[1])) { // TIGRESS - S3 timing condition

                TVector3 pos = s3hit->GetPosition(phioffset*d2r,true); // gets position of S3 with phi-offset		
                hitmap->Fill(pos.X(),pos.Y()); // fill hitmap with default X-Y
              //theta_phi_hitmap->Fill(pos.Theta()*r2d, pos.Phi()*r2d); // fill theta-phi hitmap with default X-Y

              if(theta_phi_iter) {
                  TVector3 iter, orig; // iterate s3 postion
                            orig = pos;
                
                for(int j=0;j<46;j++){ // loop over x-pos
                    for(int k=0;k<31;k++){ // loop over y-pos
                      for(int l=0;l<21;l++){ // loop over z-pos
                            iter = pos; // set iter vector
                            iter.SetX(orig.X()+j*0.1 - 4.5); // set iter vector X	
                            iter.SetY(orig.Y()+k*0.1 - 1.5); // set iter vector Y											
                            iter.SetZ(orig.Z()+l*0.1 + 2); // set iter vector Z


                            energy = s3hit->GetEnergy(); // lab energy
                            thetalab = iter.Theta(); // theta
                            excite = o17->GetExcEnergy(energy * 1e-3, thetalab, 2); // excitation
                          
                      //thetaphi_offset_iter[j][k][l]->Fill(iter.Theta()*r2d,iter.Phi()*r2d); // fill thetaphi_offset hist
                      //h_theta_v_excite[j][k][l]->Fill(iter.Theta()*r2d,excite); // fill 2D hist.
                      //h_phi_v_excite[j][k][l]->Fill(iter.Phi()*r2d,excite); // fill 2D hist.
                      h_excite[j][k][l]->Fill(excite);
                                      } // end of z loop
              		            } // end of y loop
							} // end X loop
		    			} // close iter if statement
					} // close TIGRESS - S3 timing condition
            } // close TIGRESS - EMMA timing condition
          } // close S3 - EMMA timing condition
        } // close BGO suppressor condition
      } // end of tigress loop
	            } // end pixel hit loop
			} // close PID condition
		} // close cut file condition
	    } // close EMMA event loop
	} // close emma s3 tigress coinc condition
  if(jentry%10000 == 0) cout << setiosflags(ios::fixed) << 100 * jentry/nentries << " % complete" << "\r" << flush;  // event counter
  } // end event loop
  
  cout << "100% COMPLETE!!!!" << endl; // print when done event loop
	
  TCanvas *c1 = new TCanvas();c1->cd();
  hitmap->Draw("colz");

  TFile * myfile = new TFile(outfile, "RECREATE"); // create output file
  myfile->SetCompressionLevel(0); // ROOT file is not compressed - increase performance speed when writing histograms to the TDirectories
  myfile->cd();
  list->Write();
  if(theta_phi_iter) {
    // TDirectory *iter_dir = myfile->mkdir("ThetaPhi_Iterations");
    // iter_dir->cd();
    // thetaphi_iter->Write();
    // myfile->cd();

    // TDirectory *iter_dir2 = myfile->mkdir("ExcitePhi_Iterations");
    // iter_dir2->cd();
    // excitephi_iter->Write();
    // myfile->cd();

    TDirectory *iter_dir3 = myfile->mkdir("Excite_Iterations");
    iter_dir3->cd();
    excite_iter->Write();
    myfile->cd();

    // TDirectory *iter_dir4 = myfile->mkdir("ExciteTheta_Iterations");
    // iter_dir4->cd();
    // excitetheta_iter->Write();
    // myfile->cd();
  }
  myfile->Close();
}
int main(int argc, char **argv){
	
  char const *afile;
  char const *outfile;
  char const *calfile;
  char const *cutfile;
  double phi;
  printf("Starting sortcode\n");

  std::string grsi_path = getenv("GRSISYS"); // Finds the GRSISYS path to be used by other parts of the grsisort code
  if(grsi_path.length() > 0) {
          grsi_path += "/";
  }
  // Read in grsirc in the GRSISYS directory to set user defined options on grsisort startup
  grsi_path += ".grsirc";
  gEnv->ReadFile(grsi_path.c_str(), kEnvChange);
 
  // Input-chain-file, output-histogram-file
  if(argc == 1) {
    printf("Insufficient arguments, provide argument tree files, phi offset in deg (optional), calibration file (optional), output file (optional)\n");
    return 0;
  }
  else if(argc == 2) {
    afile   = argv[1];
    phi     = 90.;
    calfile = "/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal";
    outfile = "HitMap_XYoffset.root";
    cutfile = "/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Sam_Sort/cuts.root";
  }
  else if(argc == 3) {
    afile   = argv[1];
    phi     = atof(argv[2]);
    calfile = "/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal";
    outfile = "HitMap_XYoffset.root";
    cutfile = "/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Sam_Sort/cuts.root";
  }
  else if(argc == 4) {
    afile   = argv[1];
    phi     = atof(argv[2]);
    calfile = argv[3];
    outfile = "HitMap_XYoffset.root";
    cutfile = "/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Sam_Sort/cuts.root";
  }
  else if(argc == 5) {
    afile   = argv[1];
    phi     = atof(argv[2]);
    calfile = argv[3];
    outfile = argv[4];
    cutfile = "/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Sam_Sort/cuts.root";
  }
  else if (argc == 6) {
    afile = argv[1];
    phi     = atof(argv[2]);
    calfile = argv[3];
    outfile = argv[4];
    cutfile = argv[5];
  } 
  else if(argc > 6) {
    printf("Too many arguments\n");
    printf("Provide argument tree files, phi offset in (optional), calibration file (optional), output file (optional)\n");
    return 0;
  }

  TParserLibrary::Get()->Load();

  printf("Input file: %s\n Phi Offset (deg): %.2f\n Calibration file: %s\n Output file: %s\n Cut file: %s\n",afile, phi, calfile, outfile, cutfile);
  HitMap(afile, phi, calfile, outfile, cutfile);
  return 0;
}
