//g++ hit_map_beam.cxx -std=c++0x -I$GRSISYS/include -L$GRSISYS/lib `grsi-config --cflags --all-libs --GRSIData-libs` -I$GRSISYS/GRSIData/include -L$GRSISYS/GRSIData/lib `root-config --cflags --libs` -o HitMapBeam

// This Draws a hit pattern of the S3 Detector and  plots the excitation energy for all possible Z offset position in the range -1 mm -> 1 mm and beam energies in the range EBeam - 0.5 MeV -> EBeam + 0.5 MeV
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
  TList * list = new TList; // creat hist list for hitmaps
  TH2F *hitmap = new TH2F("hitmap","S3 Hitmap",200,-50,50,200,-50,50);list->Add(hitmap); // S3 hitmap 
  TH2F *theta_phi_hitmap = new TH2F("theta_phi_hitmap","theta_phi_hitmap",500,120,170,400,-200,200); list->Add(theta_phi_hitmap); // default theta phi hitmap
  TH2F *h_pid = new TH2F("h_pid","IC Sum vs IC+Silicon",1000,0,10000,1000,0,10000); list->Add(h_pid);

  TList *thetaphi_iter = new TList; // create Hist List
  TList *excitephi_iter = new TList; // create Hist List
  TList *excite_iter = new TList; // create Hist List
  TList *excitetheta_iter = new TList; // create Hist List

  TH2F *thetaphi_offset_iter[11][21]; // init theta-phi offset iter hist pointer
  TH2F *h_phi_v_excite[11][21]; // init theta-phi offset iter hist pointer
  TH2F *h_theta_v_excite[11][21];
  TH1F *h_excite[11][21];

  if(theta_phi_iter) {
	
    	for(int i=0;i<11;i++){ // loop over EBeam_Iter
      	for(int j=0;j<21;j++){ // loop over Z

          char hname[256]; // hist name
          char hname2[256]; // hist name
          char hname3[256]; // hist name
          char hname4[256]; // hist name

          // Iterative ranges are -1.0 -> 1.0 mm for Z and EBeam - 0.5 MeV -> EBeam + 0.5 MeV. Therefore, there are 21 x 11 possible combinations.
          float z = j*0.1 - 1.0; // set Z var 
          float Eb = i*0.1 - 0.5; // Ebeam
        
          sprintf(hname,"Hitmap_iter_Z_%f_Eb_%f",z,Eb); // set hist name
          sprintf(hname2,"ExcitePhi_iter_Z_%f_Eb_%f",z,Eb); // set hist name
          sprintf(hname3,"Excite_iter_Z_%f_Eb_%f",z,Eb); // set hist name
          sprintf(hname4,"ExciteTheta_iter_Z_%f_Eb_%f",z,Eb); // set hist name
      
          // Create histograms for given Eb(i)-Z(j) offset value
          // 1. Theta vs phi hit map (theta on x, phi on y)
          // 2. Ex vs phi
          // 3. Ex spectrum
          // 4. Ex vs theta
          thetaphi_offset_iter[i][j] = new TH2F(hname,hname,500,120,170,100,-200,200); // create hists 
          thetaphi_iter->Add(thetaphi_offset_iter[i][j]); // add hist to list

          h_phi_v_excite[i][j] = new TH2F(hname2,hname2,36,-180,180,200,-2,18); // create hists 
          excitephi_iter->Add(h_phi_v_excite[i][j]); // add hist to list

          h_excite[i][j] = new TH1F(hname3,hname3,500,-2,18); // create hists 
          excite_iter->Add(h_excite[i][j]); // add hist to list

          h_theta_v_excite[i][j] = new TH2F(hname4,hname4,90,90,180,200,-2,18); // create hists 
          excitetheta_iter->Add(h_theta_v_excite[i][j]); // add hist to list
    		} // end loop over Z offsets
  	  } // end loop over EBeam_Iter
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

  // Get SRIM files
  TSRIM * srim_17oC = new TSRIM;
  srim_17oC->ReadEnergyLossFile("O17_in_C.txt"); // Eloss in Carbon
  TSRIM * srim_17oLiF = new TSRIM;
  srim_17oLiF->ReadEnergyLossFile("O17_in_LiF.txt"); // Eloss in LiF
  //double EBeam = 4.0 * 16.9991315; // 4 MeV/u O-17 beam => 68 MeV
  double EBeam = 68.; // 4 MeV/u O-17 beam => 68 MeV
  double EBeam_Iter;
  // Adjust beam energy with SRIM data
  EBeam = srim_17oLiF->GetAdjustedEnergy(EBeam * 1000, 0.19, 0.001) / 1000; 
  printf("Beam energy: %f MeV\n", EBeam);  
  
  for(int jentry=0;jentry<nentries;jentry++){ // loop over analysis events
	AnalysisTree->GetEntry(jentry);  // get event

	if(emma && s3){

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
      } // end of emma IC multiplicity loop

			// Silicon data
			silicon_energy = 0;

      total_energy=0; // Reinitialise total_energy as 0
			for (int k = 0; k < emma->GetSiMultiplicity(); k++) { // Can there be multihit in the silicon?
				si_hit = emma->GetSiHit(k);
				silicon_energy = si_hit->GetEnergy(); // get FP silicon energy
				total_energy = icsum+silicon_energy;	
			} // end of emma Si FP multiplicity loop

			h_pid->Fill(total_energy,icsum);

			if (CutG_loaded) {
        if (Neon_Gate->IsInside(total_energy,icsum)) {
          for(int i=0;i<s3->GetPixelMultiplicity();i++){  // loop over pixel events
      		
						s3hit = s3->GetPixelHit(i); //get pixel hit
      				
						if (gate1D(s3hit->GetTime() - em_hit->GetTime(), 450.0, 750.0)) {

							TVector3 pos = s3hit->GetPosition(phioffset*d2r,true); // gets position of S3 with phi-offset		

							hitmap->Fill(pos.X(),pos.Y()); // fill hitmap with default X-Y
							theta_phi_hitmap->Fill(pos.Theta()*r2d, pos.Phi()*r2d); // fill theta-phi hitmap with default X-Y

							if(theta_phi_iter) {

								TVector3 iter, orig; // iterate s3 postion
          			orig = pos;
          		
								for(int j=0;j<11;j++){ // loop over EBeam_Iter
	            		for(int k=0;k<21;k++){ // loop over Z offset

	              		  iter = pos; // set iter vector
											iter.SetX(orig.X()-1.8); // set iter vector X	
	              			iter.SetY(orig.Y()-0.8); // set iter vector Y
											iter.SetZ(orig.Z()+k*0.1-1.0); // set iter vector Z
											

											EBeam_Iter = EBeam + j*0.1 - 0.5;
											TReaction * o17 = new TReaction("o17", "li7", "t", "ne21", EBeam_Iter, 0, true); // Redefine TReaction every iteration due to changing beam energy

											energy = s3hit->GetEnergy(); // lab energy
											thetalab = iter.Theta(); // theta
        							excite = o17->GetExcEnergy(energy * 1e-3, thetalab, 2); // excitation
										
											thetaphi_offset_iter[j][k]->Fill(iter.Theta()*r2d,iter.Phi()*r2d); // fill thetaphi_offset hist
											h_theta_v_excite[j][k]->Fill(iter.Theta()*r2d,excite); // fill 2D hist.
											h_phi_v_excite[j][k]->Fill(iter.Phi()*r2d,excite); // fill 2D hist.
											h_excite[j][k]->Fill(excite);

											delete o17;
              		} // end of Z offset loop
								} // end of EBeam_Iter loop
		    			} // close iter if statement
						} // timing condition
	        } // close pixel hit loop
				} // close PID condition
			} // close cut file condition
		} // close EMMA event loop
	} // close emma s3 coinc condition
  if(jentry%10000 == 0) cout << setiosflags(ios::fixed) << 100 * jentry/nentries << " % complete" << "\r" << flush;  // event counter
  } // end event loop
  
  cout << "100% COMPLETE!!!!" << endl; // print when done event loop
	
  TCanvas *c1 = new TCanvas();c1->cd();
  hitmap->Draw("colz");

  TFile * myfile = new TFile(outfile, "RECREATE"); // create output file
  myfile->cd();
  list->Write();
  if(theta_phi_iter) {
    TDirectory *iter_dir = myfile->mkdir("ThetaPhi_Iterations");
    iter_dir->cd();
    thetaphi_iter->Write();
    myfile->cd();

    TDirectory *iter_dir2 = myfile->mkdir("ExcitePhi_Iterations");
    iter_dir2->cd();
    excitephi_iter->Write();
    myfile->cd();

    TDirectory *iter_dir3 = myfile->mkdir("Excite_Iterations");
    iter_dir3->cd();
    excite_iter->Write();
    myfile->cd();

    TDirectory *iter_dir4 = myfile->mkdir("ExciteTheta_Iterations");
    iter_dir4->cd();
    excitetheta_iter->Write();
    myfile->cd();
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
    outfile = "HitMap_Zoffset.root";
    cutfile = "/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Sam_Sort/cuts.root";
  }
  else if(argc == 3) {
    afile   = argv[1];
    phi     = atof(argv[2]);
    calfile = "/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal";
    outfile = "HitMap_Zoffset.root";
    cutfile = "/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Sam_Sort/cuts.root";
  }
  else if(argc == 4) {
    afile   = argv[1];
    phi     = atof(argv[2]);
    calfile = argv[3];
    outfile = "HitMap_Zoffset.root";
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
