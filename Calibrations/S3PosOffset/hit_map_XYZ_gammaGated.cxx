//g++ hit_map_XYZ_gammaGated.cxx -std=c++0x -I$GRSISYS/include -L$GRSISYS/lib `grsi-config --cflags --all-libs --GRSIData-libs` -I$GRSISYS/GRSIData/include -L$GRSISYS/GRSIData/lib `root-config --cflags --libs` -o HitMap_XYZ_gammaGated

// This plots the gamma gated excitation energy spectrum for the XYZ offsets that produce the best energy resolution
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
  Neon_Gate = (TCutG *)cuts->Get("neon_cut_sam");		// cut on EMMA IC vs IC+Silicon
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
void HitMap(char const* infile, double phioffset, char const* calfile, char const* outfile, char const * cutfile = "NULL", char const * s3dl = "NULL"){

  TList *excite_iter = new TList; // create Hist List
  TList *excite_gammaGated_iter = new TList; // create Hist List

  TH1F *h_excite[18][16][31]; // initialise Ex hist point
  TH1F *h_excite_gammaGated[18][16][31]; // intialise Ex gamma gated hist pointer

	
  for(int i=0;i<18;i++){ // loop over x offsets
      for(int j=0;j<16;j++){ // loop over y offsets
          for(int k=0;k<31;k++){ // loop over z offsets

              char hname[256]; // hist name
              char hname2[256]; // hist name
              
              // Iterative ranges for X Y and Z
              float x = i*0.1 - 3.0; // set x var 
              float y = j*0.1 - 1.5; // set y var
              float z = k*0.1 + 3.0; // set z var 

              sprintf(hname,"Ex_iter_X_%f_Y_%f_Z_%f",x,y,z); // set hist name
              sprintf(hname2,"ExGammaGated_iter_X_%f_Y_%f_Z_%f",x,y,z); // set hist name
              // Create histograms for given X(i)-Y(j)-Z(k) offset value
              // 1. Ex spectrum
              // 2. Ex spectrum gamma gated

              h_excite[i][j][k] = new TH1F(hname,hname,500,-2,18); // create hists 
              excite_iter->Add(h_excite[i][j][k]); // add hist to list
              h_excite_gammaGated[i][j][k] = new TH1F(hname2,hname2,500,-2,18); // create hists 
              excite_gammaGated_iter->Add(h_excite_gammaGated[i][j][k]); // add hist to list

          } // loop over z offsets
    } // end loop over y offsets
  } // end loop over x offsets

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
  double energy, thetalab, excite, thetacm, rekin, Beta, recoiltheta, theta_s3_ring, DL_corr_energy, DL_and_t_in_LiF_corr_energy;

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
  double gamma_rawE, gamma_dopE;

  // Get SRIM files
  TSRIM * srim_17oC = new TSRIM;
  srim_17oC->ReadEnergyLossFile("O17_in_C.txt"); // Eloss in Carbon
  TSRIM * srim_17oLiF = new TSRIM;
  srim_17oLiF->ReadEnergyLossFile("O17_in_LiF.txt"); // Eloss in LiF
  //double EBeam = 4.0 * 16.9991315; // 4 MeV/u O-17 beam => 68 MeV
  double EBeam = 68.0;
  printf("Beam energy: %f MeV\n", EBeam);
  printf("Target characteristics: 0.39 \u03BCm LiF + 0.14 \u03BCm Carbon\n");
  // Adjust beam energy for middle of the target based on 17O through LiF.
  double targetThicknessugcm2 = 100.;//approximately 100 ug/cm2 LiF targets
  double targetThicknessum = 0.39; //100 ug/cm2 LiF are approx 0.39 um thick.
  EBeam = srim_17oLiF->GetAdjustedEnergy(EBeam*1000,targetThicknessum/2.0,0.001)/1000.;
  // *1000 and /1000 convert from MeV to keV and back. we use /2.0 to get the beam energy in the middle of the target. 0.001 is the integration step size
  printf("Beam energy at the centre of the target: %f MeV\n", EBeam);
  TReaction * o17 = new TReaction("o17", "li7", "t", "ne21", EBeam, 0, true);

  // Triton energy loss in the silicon dead layer on the S3  
  TSRIM * srim_3H_in_Si = new TSRIM;
  srim_3H_in_Si->ReadEnergyLossFile("Triton_in_Si.txt"); // Eloss of triton in silicon
  // S3 dead layer thicknesses for each triple alpha calibration data set
  double s3DL = 0.0; // must declare s3DL outside of the scope  
  if (strcmp(s3dl, "Dec2019") == 0) { 	// strcmp is used to compare contents of a string
	s3DL = 0.694; // um +- 0.113 um
  } else if (strcmp(s3dl, "Nov2020_20V") == 0) {
	s3DL = 0.692; // um +- 131 um
  } else if (strcmp(s3dl, "Nov2020_40V") == 0) {
	s3DL = 0.748; // um +- 0.111 um
  } else if (strcmp(s3dl, "Default") == 0) {
  s3DL = 0.72; // um (arithmetic mean of the above)
  } else {
	printf("The argument supplying the S3 dead layer thickness was not provided or was incorrect\n");
	return;
  }
  printf("Using S3 dead layer = %f um\n", s3DL);

  // Triton energy loss in front-half of target
  TSRIM * srim_3H_in_LiF = new TSRIM;
  srim_3H_in_LiF->ReadEnergyLossFile("Triton_in_LiF.txt");

  
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
              TVector3 iter, orig; // iterate s3 postion
              orig = pos;
                
                for(int j=0;j<18;j++){ // loop over x-pos
                    for(int k=0;k<16;k++){ // loop over y-pos
                      for(int l=0;l<31;l++){ // loop over z-pos
                        iter = pos; // set iter vector
                        iter.SetX(orig.X()+j*0.1 - 3.0); // set iter vector X	
                        iter.SetY(orig.Y()+k*0.1 - 1.5); // set iter vector Y											
                        iter.SetZ(orig.Z()+l*0.1 + 3.0); // set iter vector Z
                        
                        // Ejectile kinematics
                        energy = s3hit->GetEnergy(); // lab energy
                        thetalab = iter.Theta(); // theta
                        // CoM angle and recoil kinematics
						            thetacm = o17->ConvertThetaLabToCm(thetalab, 2); // theta CoM
        	              rekin = o17->GetTLabFromThetaCm(TMath::Pi() - thetacm, 3) * 1e3; // recoil lab energy
        	              Beta = o17->AnalysisBeta(rekin * 1e-3, 3); //get laboratory beta of the recoil
                        recoiltheta = o17->ConvertThetaCmToLab(thetacm, 3); // recoil lab angle
                        TVector3 recoil_vec; // (TVector recoil_vec must be declared in this scope)
                    	  recoil_vec.SetMagThetaPhi(1., recoiltheta, pos.Phi() - TMath::Pi());
                        // Gamma-ray energy
                        gamma_rawE = add_hit->GetEnergy();
                        gamma_dopE = add_hit->GetDoppler(Beta, & recoil_vec); // get doppler corrected gamma energy
                        // Energy loss corrections
                        theta_s3_ring = TMath::Pi() - thetalab; // Angle of S3 ring to beam axis -> for effective thicknesses (energy loss)
                        DL_corr_energy = energy + srim_3H_in_Si->GetEnergyLost(energy, s3DL/TMath::Cos(theta_s3_ring), 0.001);
						            DL_and_t_in_LiF_corr_energy = DL_corr_energy + srim_3H_in_LiF->GetEnergyLost(DL_corr_energy, (targetThicknessum/2)/TMath::Cos(theta_s3_ring), 0.001);
						
                        excite = o17->GetExcEnergy(DL_and_t_in_LiF_corr_energy * 1e-3, thetalab, 2); // excitation energy with DL correction and triton energy loss in target correction
                        
                        h_excite[j][k][l]->Fill(excite);
                        if (gamma_dopE > 1105.9 && gamma_dopE < 1138.1) { // 3 sigma
                          h_excite_gammaGated[j][k][l]->Fill(excite);
										    } // end of 1120 keV gamma gate condition

                      } // end of z loop
              		  } // end of y loop
							} // end X loop
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
	
  TFile * myfile = new TFile(outfile, "RECREATE"); // create output file
  myfile->SetCompressionLevel(0); // ROOT file is not compressed - increase performance speed when writing histograms to the TDirectories
  myfile->cd();
  TDirectory *iter_dir = myfile->mkdir("Excite_Iterations");
  iter_dir->cd();
  excite_iter->Write();
  myfile->cd();
  TDirectory *iter_dir2 = myfile->mkdir("ExGammaGated_Iterations");
  iter_dir2->cd();
  excite_gammaGated_iter->Write();
  myfile->cd();
  myfile->Close();
}
int main(int argc, char **argv){
	
  char const *afile;
  char const *outfile;
  char const *calfile;
  char const *cutfile;
  double phi;
  char const *s3dl;
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
    s3dl = "Default";
  }
  else if(argc == 3) {
    afile   = argv[1];
    phi     = atof(argv[2]);
    calfile = "/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Calibrations/CalibFileSetup/MyCalibrations/CalibrationFile_2020_40V.cal";
    outfile = "HitMap_XYoffset.root";
    cutfile = "/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Sam_Sort/cuts.root";
    s3dl = "Default";
  }
  else if(argc == 4) {
    afile   = argv[1];
    phi     = atof(argv[2]);
    calfile = argv[3];
    outfile = "HitMap_XYoffset.root";
    cutfile = "/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Sam_Sort/cuts.root";
    s3dl = "Default";
  }
  else if(argc == 5) {
    afile   = argv[1];
    phi     = atof(argv[2]);
    calfile = argv[3];
    outfile = argv[4];
    cutfile = "/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Sam_Sort/cuts.root";
    s3dl = "Default";
  }
  else if (argc == 6) {
    afile = argv[1];
    phi     = atof(argv[2]);
    calfile = argv[3];
    outfile = argv[4];
    cutfile = argv[5];
    s3dl = "Default";
  } 
  else if(argc == 7) {
    afile = argv[1];
    phi     = atof(argv[2]);
    calfile = argv[3];
    outfile = argv[4];
    cutfile = argv[5];
    s3dl = argv[6];
  }
  else if(argc > 7) {
    printf("Too many arguments\n");
    printf("Provide argument tree files, phi offset in (optional), calibration file (optional), output file (optional), cutfile (optional), S3 dead layer (optional)\n");
    return 0;
  }

  TParserLibrary::Get()->Load();

  printf("Input file: %s\n Phi Offset (deg): %.2f\n Calibration file: %s\n Output file: %s\n Cut file: %s\n S3 dead layer: %s\n",afile, phi, calfile, outfile, cutfile, s3dl);
  HitMap(afile, phi, calfile, outfile, cutfile, s3dl);
  return 0;
}
