// g++ SortCode_Sam.cxx -std=c++0x -I$GRSISYS/include -L$GRSISYS/lib `grsi-config --cflags --all-libs --GRSIData-libs` -I$GRSISYS/GRSIData/include -L$GRSISYS/GRSIData/lib `root-config --cflags --libs` -lTTigress -lTEmma -lTS3 -o SortData_Sam
// S1873 - original sort codes borrowed from M. Williams and C. Angus
// SortCode_Sam.cxx
// S. Taylor
// 27/02/2026

#define Sortcode_Sam_cxx
#include "SortCode_Sam.h"

using namespace std;

Double_t r2d = TMath::RadToDeg();
Double_t d2r = TMath::DegToRad();

bool gate1D(Double_t value, Double_t min, Double_t max)
{
  if (min < value && value < max)
    return true;
  else
    return false;
}

bool loadCutG(char const *cutfile)
{ // 2D Gate Loader. Code only uses mass gate if cut file given
  TFile *cuts = new TFile(cutfile, "READ");
  Neon_Gate = (TCutG *)cuts->Get("neon_cut_sam");
  Fluorine_Gate = (TCutG *)cuts->Get("fluorine_cut");		// cut on EMMA IC vs IC+Silicon
  return true;
}

double tigtigT[2] = {-100, 100};

double pi = TMath::Pi();
double s3_phi_offset = 90;
double s3_x_offset = -3.0;  // S3 x offset (mm)
double s3_y_offset = -0.2;  // S3 y offset (mm)
double s3_z_offset = 3.4;    // S3 z offset (mm)
double Beta;
double thetalab; 
double excite; 
double ekin;
double recoiltheta;
double recoilphi;
double thetacm;
double rekin;
double Ex;

double sector_fired;

double SiE;
double totalE;
double Esum_Norm;
double Eic_Norm;
double pid_param;

bool suppAdd = false;
bool CutG_loaded = false;

void SortCode_Sam::SortData(char const * afile, char const * calfile, char const * outfile, char const * s3dl = "NULL", char const * cutfile = "NULL") {

	Initialise();
  
	// rootfile branch structure
	typedef struct {double xpos, ypos, multi;} PGAC;
	PGAC pgac;
	typedef struct {double energy, time, multi;} SILICON;
	SILICON silicon;
	typedef struct {double eseg[5], esum, multi;} IONCHAMBER;
	IONCHAMBER ic;
	typedef struct {double energy[2], time[2], multi;} SSB;
	SSB sb;
	typedef struct {double time, multi;} TOF;
	TOF tof;
	typedef struct {double time;} TRIG;
	TRIG trig;
	typedef struct {double energy, time, multi, hit;} RINGS;
	RINGS ring;
	typedef struct {double energy, time, multi, hit;} SECTORS;
	SECTORS sector;
	typedef struct {double x, y, z, theta, phi, multi;} S3POSITION;
	S3POSITION s3pos;
	typedef struct {double exc, kin, excE;} S3ENERGY;
	S3ENERGY s3energy;
	typedef struct {double rawE, dopE, time, det, core, beta;} GAMMA;
	GAMMA gamma;
	typedef struct {double rawE, dopE, time, det, core, beta;} GAMMA2;
	GAMMA2 gamma2;
	typedef struct {double theta, phi, kin, vang, hang;} RECOIL;
	RECOIL recoil;
	typedef struct {double multi;} TIGRESS;
	TIGRESS tigress;
	typedef struct {double etot;} EMMAFP;
	EMMAFP emmafp;

	// define output rootfile and branches
	TFile * myfile = new TFile(outfile, "RECREATE");
	// SSB
	TTree * ssb = new TTree("ssb","SSB Singles");
	ssb->Branch("sb",&sb,"energy[2]/D:time[2]/D:multi/D");
	// EMMA singles
  	TTree * emma = new TTree("emma","EMMA Singles");
  	emma->Branch("pgac",&pgac,"xpos/D:ypos/D:multi/D");
	emma->Branch("silicon",&silicon,"energy/D:time/D:multi/D");
	emma->Branch("ic",&ic,"eseg[5]/D:esum/D:multi/D");
	emma->Branch("tof",&tof,"time/D:multi/D");
	emma->Branch("trig",&trig,"time/D");
	// TIGRESS Singles
	TTree * tig = new TTree("tig","TIGRESS Singles");
  	tig->Branch("gamma",&gamma,"rawE/D:dopE/D:time/D:det/D:core/D:beta/D");
	tig->Branch("gamma2",&gamma2,"rawE/D:dopE/D:time/D:det/D:core/D:beta/D");
	// S3 Singles
	TTree * s3 = new TTree("s3","S3 Singles");
	s3->Branch("ring",&ring,"energy/D:time/D:multi/D:hit/D");
	s3->Branch("sector",&sector,"energy/D:time/D:multi/D:hit/D");
	s3->Branch("s3pos",&s3pos,"x/D:y/D:z/D:theta/D:phi/D:multi/D");
	s3->Branch("s3energy",&s3energy,"exc/D:kin/D");
	// S3-EMMA coincidences
	TTree * s3emma = new TTree("s3emma","S3-EMMA Coincidences");
	s3emma->Branch("pgac",&pgac,"xpos/D:ypos/D:multi/D");
	s3emma->Branch("silicon",&silicon,"energy/D:time/D:multi/D");
	s3emma->Branch("ic",&ic,"eseg[5]/D:esum/D:multi/D");
	s3emma->Branch("tof",&tof,"time/D:multi/D");
	s3emma->Branch("ring",&ring,"energy/D:multi/D:hit/D");
	s3emma->Branch("sector",&sector,"energy/D:multi/D:hit/D");
	s3emma->Branch("s3pos",&s3pos,"x/D:y/D:z/D:theta/D:phi/D:multi/D");
	s3emma->Branch("s3energy",&s3energy,"exc/D:kin/D");
	// S3-TIGRESS coincidences
	TTree * s3tig = new TTree("s3tig","S3-TIGRESS Coincidences");
	s3tig->Branch("ring",&ring,"energy/D:multi/D:hit/D");
	s3tig->Branch("sector",&sector,"energy/D:multi/D:hit/D");
	s3tig->Branch("s3pos",&s3pos,"x/D:y/D:z/D:theta/D:phi/D:multi/D");
	s3tig->Branch("s3energy",&s3energy,"exc/D:kin/D");
	s3tig->Branch("gamma",&gamma,"rawE/D:dopE/D:time/D:det/D:core/D:beta/D");
	s3tig->Branch("gamma2",&gamma2,"rawE/D:dopE/D:time/D:det/D:core/D:beta/D");
	// S3-TIGRESS-EMMA coincidences
	TTree * s3tigemma = new TTree("s3tigemma","S3-TIGRESS-EMMA Coincidences");
	s3tigemma->Branch("pgac",&pgac,"xpos/D:ypos/D:multi/D");
	s3tigemma->Branch("silicon",&silicon,"energy/D:time/D:multi/D");
	s3tigemma->Branch("ic",&ic,"eseg[5]/D:esum/D:multi/D");
	s3tigemma->Branch("tof",&tof,"time/D:multi/D");
	s3tigemma->Branch("ring",&ring,"energy/D:multi/D:hit/D");
	s3tigemma->Branch("sector",&sector,"energy/D:multi/D:hit/D");
	s3tigemma->Branch("s3pos",&s3pos,"x/D:y/D:z/D:theta/D:phi/D:multi/D");
	s3tigemma->Branch("s3energy",&s3energy,"exc/D:kin/D:excE/D");
	s3tigemma->Branch("recoil",&recoil,"theta/D:phi/D:kin/D:vang/D:hang/D");
	s3tigemma->Branch("gamma",&gamma,"rawE/D:dopE/D:time/D:det/D:core/D:beta/D");
	s3tigemma->Branch("gamma2",&gamma2,"rawE/D:dopE/D:time/D:det/D:core/D:beta/D");
	s3tigemma->Branch("tigress",&tigress,"multi/D");
	s3tigemma->Branch("emmafp",&emmafp,"etot/D");

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

  // Load cut file:
  if (strcmp(cutfile, testval) != 0)
  {
    printf("Cuts Loaded from %s \n", cutfile);
    loadCutG(cutfile);
    CutG_loaded = true;
  }
  else
    printf("Couldn't find 2D cuts... 1D gates will be used instead.\n");


  // Checks for branches and sets pointers
  // Set EMMA hits
  TEmma * emma_data = 0;
  if (AnalysisTree->FindBranch("TEmma")) {
    AnalysisTree->SetBranchAddress("TEmma", & emma_data);
  } else {
    cout << "Branch 'TEmma' not found! TEmma variable is NULL pointer" << endl;
  }
  // Set S3 hits
  TS3 * s3_data = 0;
  if (AnalysisTree->FindBranch("TS3")) {
    AnalysisTree->SetBranchAddress("TS3", & s3_data);
  } else {
    cout << "Branch 'TS3' not found! TS3 variable is NULL pointer" << endl;
  }
  // Set TIGRESS hits
  TTigress * tig_data = 0;
  if (AnalysisTree->FindBranch("TTigress")) {
    AnalysisTree->SetBranchAddress("TTigress", & tig_data);
  } else {
    cout << "Branch 'TTigress' not found! TTigress variable is NULL pointer" << endl;
  }
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

  TReaction * o17 = new TReaction("o17", "li7", "h3", "ne21", EBeam, 0, true);  // Reaction definition
  
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
  } else {
	printf("The argument supplying the S3 dead layer thickness was not provided or was incorrect\n");
	return;
  }
  printf("Using S3 dead layer = %f um\n", s3DL);

  // Triton energy loss in front-half of target
  TSRIM * srim_3H_in_LiF = new TSRIM;
  srim_3H_in_LiF->ReadEnergyLossFile("Triton_in_LiF.txt");

  // Define Hit Pointers
  TEmmaHit * em_hit, * si_hit, * ic_hit, * ssb_hit, * trigger_hit;
  TTigressHit * tig_hit, * add_hit, * add_hit2;
  TS3Hit * ring_hit, * sector_hit, * s3hit;
  if(s3_data)
    s3_data->SetFrontBackTime(500); // Needed to build S3 pixels properly Nov2020??
    //s3_data->SetFrontBackTime(140); // Needed to build S3 pixels properly
    s3_data->SetFrontBackEnergy(0.85);
  TVector3 pos, recoil_vec;

  printf("Reading calibration file: %s\n", calfile);
  TChannel::ReadCalFile(calfile); // Read calibration file
  printf("\nSorting analysis events...\n");

  for (int jentry = 0; jentry < analentries; jentry++) { // loop over events in analysis tree
	AnalysisTree->GetEntry(jentry);

	if (s3_data)	{
		s3_data->SetFrontBackTime(500); // Needed to build S3 pixels properly // Nov 2020??
		//s3_data->SetFrontBackTime(140); // Needed to build S3 pixels properly
    	s3_data->SetFrontBackEnergy(0.85);
		s3_data->SetMultiHit();
		// ring data
		ring.energy = 0;
		for (int i = 0; i < s3_data->GetRingMultiplicity(); i++) {
			ring_hit = s3_data->GetRingHit(i);
			ring.energy += ring_hit->GetEnergy(); // ring energy
			ring.hit = ring_hit->GetRing();
			ring.time = ring_hit->GetTime(); 			
		}
		ring.multi = s3_data->GetRingMultiplicity();
		// sector data
		sector.energy = 0;
		for (int j = 0; j < s3_data->GetSectorMultiplicity(); j++){
			sector_hit = s3_data->GetSectorHit(j);
			sector.energy += sector_hit->GetEnergy(); // sector energy
			sector.hit = sector_hit->GetSector();
			sector.time = sector_hit->GetTime();
		}
		sector.multi = s3_data->GetSectorMultiplicity();

		s3pos.multi = s3_data->GetPixelMultiplicity();

		for (int i = 0; i < s3_data->GetPixelMultiplicity(); i++) {
        	s3hit = s3_data->GetPixelHit(i);
        	pos = s3hit->GetPosition(s3_phi_offset*d2r,true);
			pos.SetX(pos.X() + s3_x_offset); s3pos.x = pos.X(); // s3 xpos
			pos.SetY(pos.Y() + s3_y_offset); s3pos.y = pos.Y(); // s3 ypos
			pos.SetZ(pos.Z() + s3_z_offset); s3pos.z = pos.Z(); // s3 zpos
        	thetalab = pos.Theta(); s3pos.theta = thetalab * r2d; // s3 theta
			s3pos.phi = pos.Phi() * r2d; // s3 phi
        	ekin = s3hit->GetEnergy(); s3energy.kin = ekin; // lab energy
        	excite = o17->GetExcEnergy(ekin * 1e-3, thetalab, 2); s3energy.exc = excite; // excitation energy
        	o17->SetExcEnergy(excite);
			
			s3->Fill();
		}
		
	}

	if (emma_data)	{

		// PGAC hits
		for (int e = 0; e < emma_data->GetMultiplicity(); e++) { // For a given jentry, this loops over all of the PGAC segments that registered a hit. So if GetMultiplicity returns 4, then loop over e = 0-3 for the 4 segments in the PGAC that recorded a hit for that event
			em_hit = emma_data->GetEmmaHit(e);
			pgac.xpos = em_hit->GetPosition().X(); // Get PGAC xpos
          		pgac.ypos = em_hit->GetPosition().Y(); // Get PGAC ypos

			// IC data
			ic.eseg[5] = {0};
			ic.esum = 0;
			for (int j = 0; j < emma_data->GetICMultiplicity(); j++) { // IC multi = how many segments have a hit
            			ic_hit = emma_data->GetICHit(j);
            			ic.eseg[ic_hit->GetSegment()]=ic_hit->GetEnergy(); // get ic segement energy
            			ic.esum += ic_hit->GetEnergy(); // get ic sum energy
          		}

			// Silicon data
			silicon.energy = 0;
			silicon.time = 0;
			
			totalE=0; // Reinitialise totalE as 0
			for (int k = 0; k < emma_data->GetSiMultiplicity(); k++) {
				si_hit = emma_data->GetSiHit(k);
				silicon.energy = si_hit->GetEnergy(); // get FP silicon energy
            			silicon.time = si_hit->GetTime() / 1e9; // get FP silicon time

				totalE = ic.esum+silicon.energy;
				
			}
			emmafp.etot=totalE;

			// Trigger
			for (int m = 0; m < emma_data->GetTriggerMultiplicity(); m++) {
            			trigger_hit = emma_data->GetTriggerHit(m);
            			//if (trigger_hit->GetEnergy() > 200) continue;
            				//tDE->Fill(trigger_hit->GetTime() / pow(10, 9), em_hit->GetTime() - trigger_hit->GetTime()); // get time between EMMA and TIGRESS events
					tof.time = em_hit->GetTime() - trigger_hit->GetTime();
          		}
		
		h_icSumVSi->Fill(totalE,ic.esum); // Fill EMMA singles IC vs IC+silicon
		}
	}	

	if (tig_data && s3_data && emma_data) { // TIGRESS & S3 & EMMA hit condition

		// EMMA event multiplicities
		pgac.multi = emma_data->GetMultiplicity(); // Get EMMA Multiplicity
		silicon.multi = emma_data->GetSiMultiplicity(); // Get EMMA Si Multi
		ic.multi = emma_data->GetICMultiplicity(); // Get IC Multi
		tof.multi = emma_data->GetTriggerMultiplicity(); // Get Trigger Multi

		// PGAC hits
		for (int e = 0; e < emma_data->GetMultiplicity(); e++) { // For a given jentry, this loops over all of the PGAC segments that registered a hit. So if GetMultiplicity returns 4, then loop over e = 0-3 for the 4 segments in the PGAC that recorded a hit for that event
			em_hit = emma_data->GetEmmaHit(e);
			pgac.xpos = em_hit->GetPosition().X(); // Get PGAC xpos
          		pgac.ypos = em_hit->GetPosition().Y(); // Get PGAC ypos

			// IC data
			ic.eseg[5] = {0};
			ic.esum = 0;
			for (int j = 0; j < emma_data->GetICMultiplicity(); j++) { // IC multi = how many segments have a hit
            			ic_hit = emma_data->GetICHit(j);
            			ic.eseg[ic_hit->GetSegment()]=ic_hit->GetEnergy(); // get ic segement energy
            			ic.esum += ic_hit->GetEnergy(); // get ic sum energy
          		}

			// Silicon data
			silicon.energy = 0;
			silicon.time = 0;
			
			totalE=0; // Reinitialise totalE as 0
			for (int k = 0; k < emma_data->GetSiMultiplicity(); k++) {
				si_hit = emma_data->GetSiHit(k);
				silicon.energy = si_hit->GetEnergy(); // get FP silicon energy
            			silicon.time = si_hit->GetTime() / 1e9; // get FP silicon time

				totalE = ic.esum+silicon.energy;
				
			}
			emmafp.etot=totalE;

			// Trigger
			for (int m = 0; m < emma_data->GetTriggerMultiplicity(); m++) {
            			trigger_hit = emma_data->GetTriggerHit(m);
            			//if (trigger_hit->GetEnergy() > 200) continue;
            				//tDE->Fill(trigger_hit->GetTime() / pow(10, 9), em_hit->GetTime() - trigger_hit->GetTime()); // get time between EMMA and TIGRESS events
					tof.time = em_hit->GetTime() - trigger_hit->GetTime();
          		}

			// S3 Data:
			s3_data->SetFrontBackTime(500); // Needed to build S3 pixels properly
			//s3_data->SetFrontBackTime(140); // Needed to build S3 pixels properly
    		s3_data->SetFrontBackEnergy(0.85);
			s3_data->SetMultiHit();

			// ring data
			//ring.multi = 0;
			ring.energy = 0;
			for (int i = 0; i < s3_data->GetRingMultiplicity(); i++) {
				ring_hit = s3_data->GetRingHit(i);
				ring.energy += ring_hit->GetEnergy(); // ring energy
				ring.hit = ring_hit->GetRing();				
			}
			ring.multi = s3_data->GetRingMultiplicity();

			// sector data
			//sector.multi = 0;
			sector.energy = 0;
			for (int j = 0; j < s3_data->GetSectorMultiplicity(); j++){
				sector_hit = s3_data->GetSectorHit(j);
				sector.energy += sector_hit->GetEnergy(); // sector energy
				sector.hit = sector_hit->GetSector();
			}
			sector.multi = s3_data->GetSectorMultiplicity();

			tigress.multi = tig_data->GetAddbackMultiplicity();
			// Pixel data:
			s3pos.multi = s3_data->GetPixelMultiplicity();
			for (int i = 0; i < s3_data->GetPixelMultiplicity(); i++) {
        			
				s3hit = s3_data->GetPixelHit(i);

				for (int t = 0; t < tig_data->GetAddbackMultiplicity(); t++) { // Loop over tigress hits

					add_hit = tig_data->GetAddbackHit(t);
        	        		suppAdd = add_hit->BGOFired();
							// suppressor condition
							// If a BGO scintillator records a hit then the photon has Compton scattered out of the main detector.
							// Therefore, want suppAdd to be false and deposited energy to be at least 15 keV
        	        		if (!suppAdd && add_hit->GetEnergy() > 15) { 

						// S3 positions
        				pos = s3hit->GetPosition(s3_phi_offset*d2r,true); // If true then the position is randomly smeared. When false the ring and sector widths are an error bar in the final analysis
						pos.SetX(pos.X() + s3_x_offset); s3pos.x = pos.X(); // s3 xpos
						pos.SetY(pos.Y() + s3_y_offset); s3pos.y = pos.Y(); // s3 ypos
						pos.SetZ(pos.Z() + s3_z_offset); s3pos.z = pos.Z(); // s3 zpos


						// S3 Angles
        				thetalab = pos.Theta(); 
						s3pos.theta = thetalab * r2d; // s3 theta
						s3pos.phi = pos.Phi() * r2d; // s3 phi
						// S3 Energies
						ekin = s3hit->GetEnergy(); // Uncorrected lab energy
						excite = o17->GetExcEnergy(ekin * 1e-3, thetalab, 2); // Uncorrected excitation energy
						double theta_s3_ring = TMath::Pi() - thetalab; // Angle of S3 ring to beam axis -> for effective thicknesses (energy loss)
                        double DL_corr_energy = ekin + srim_3H_in_Si->GetEnergyLost(ekin, s3DL/TMath::Cos(theta_s3_ring), 0.001);
						double DL_corr_excite = o17->GetExcEnergy(DL_corr_energy * 1e-3, thetalab, 2); // excitation energy with DL correction
						double DL_and_t_in_LiF_corr_energy = DL_corr_energy + srim_3H_in_LiF->GetEnergyLost(DL_corr_energy, (targetThicknessum/2)/TMath::Cos(theta_s3_ring), 0.001);
						double DL_and_t_in_LiF_corr_excite = o17->GetExcEnergy(DL_and_t_in_LiF_corr_energy * 1e-3, thetalab, 2); // excitation energy with DL correction and triton energy loss in target correction
						
						s3energy.kin = DL_and_t_in_LiF_corr_energy;
						o17->SetExcEnergy(DL_and_t_in_LiF_corr_excite); s3energy.exc = DL_and_t_in_LiF_corr_excite;


						// CoM angle and recoil beta
						thetacm = o17->ConvertThetaLabToCm(thetalab, 2); // theta CoM
        	            rekin = o17->GetTLabFromThetaCm(TMath::Pi() - thetacm, 3) * 1e3; // recoil lab energy
        	            Beta = o17->AnalysisBeta(rekin * 1e-3, 3); //get laboratory beta of the recoil
                    	recoiltheta = o17->ConvertThetaCmToLab(thetacm, 3); // recoil lab angle
                    	recoil_vec.SetMagThetaPhi(1., recoiltheta, pos.Phi() - TMath::Pi());
						// Gamma-ray energy
						gamma.dopE = add_hit->GetDoppler(Beta, & recoil_vec); // get doppler corrected gamma energy
						gamma.time = add_hit->GetTime();
						gamma.rawE = add_hit->GetEnergy();
						gamma.det = add_hit->GetDetector();
						gamma.core = add_hit->GetArrayNumber();
						gamma.beta = Beta;
						// recoil kinematics
						recoil.theta = recoiltheta * r2d;
						recoil.phi = (pos.Phi() - TMath::Pi())*r2d;
						recoil.kin = rekin;
						recoil.vang = (recoiltheta*TMath::Cos((pos.Phi() - TMath::Pi())))*r2d;
						recoil.hang = (recoiltheta*TMath::Sin((pos.Phi() - TMath::Pi())))*r2d;

						if (gate1D(s3hit->GetTime() - em_hit->GetTime(), 450.0, 750.0)) {
						if (gate1D(add_hit->GetTime() - em_hit->GetTime(), 450.0, 700.0)) {

							if (gamma.dopE > 340.0 && gamma.dopE < 630.0) {
								h_S3spec_noMassGate->Fill(DL_and_t_in_LiF_corr_excite);
							}
							
							if (em_hit->GetPosition().X()>-4.5 && em_hit->GetPosition().X()<0.5){
									h_excite_pgac_gated->Fill(DL_and_t_in_LiF_corr_excite);
							}

							if (CutG_loaded) {
									h_s3energy->Fill(sector.energy,ring.energy);
									h_s3sector_energy->Fill(s3hit->GetSector(),s3energy.kin);
									h_s3ring_energy->Fill(s3hit->GetRing(),s3energy.kin);

                					if (Neon_Gate->IsInside(totalE,ic.esum)) {
										h_sectorE_v_ring_num->Fill(s3hit->GetRing(),sector.energy);
										h_recoil_theta->Fill(recoiltheta * r2d);
										h_s3energy_pid_gated->Fill(sector.energy,ring.energy);
										s3_E_theta->Fill(thetacm * r2d, DL_and_t_in_LiF_corr_energy);
										h_excite_pid_gated->Fill(DL_and_t_in_LiF_corr_excite);
										h_s3_E->Fill(s3energy.kin);

										h_S3EMMA_Exc_RngN->Fill(s3hit->GetRing(),DL_and_t_in_LiF_corr_excite);
										h_excite_s3_E->Fill(DL_and_t_in_LiF_corr_energy, DL_and_t_in_LiF_corr_excite);
										if (s3hit->GetRing()== 0) {
											h_excite_ring0->Fill(DL_and_t_in_LiF_corr_excite);
										}
										h_S3EMMA_Exc_SecN->Fill(s3hit->GetSector(),DL_and_t_in_LiF_corr_excite);
										h_excite_v_dopE_addback->Fill(gamma.dopE,DL_and_t_in_LiF_corr_excite);
										h_dopE_addback_gated->Fill(gamma.dopE);
										h_ejectile_energy_v_thetalab->Fill(thetalab * r2d,DL_and_t_in_LiF_corr_energy);										
										
										// Excitation energy gamma gated
										// Low energy gammas for normalisations
										if (gamma.dopE > 344.5 && gamma.dopE < 358.3) { // 3 sigma
											h_excite_PIdgated_351keVgated->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (gamma.dopE > 1376.3 && gamma.dopE < 1417.7) { // 3 sigma
											h_excite_PIdgated_1395keVgated->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (gamma.dopE > 1719.0 && gamma.dopE < 1779.0) { // 3 sigma
											h_excite_PIdgated_1745keVgated->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (gamma.dopE > 1105.9 && gamma.dopE < 1138.1) { // 3 sigma
											h_excite_PIdgated_1120keVgated->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (gamma.dopE > 2485.4 && gamma.dopE < 2554.7) { // 3 sigma
											h_excite_PIdgated_2516keVgated->Fill(DL_and_t_in_LiF_corr_excite);
										}
										// High energy gammas for identifying Gamow window states
										if (gamma.dopE > 6366.0 && gamma.dopE < 6474.0) {
											h_excite_PIdgated_6408keVgated->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (gamma.dopE > 4528.0 && gamma.dopE < 4592.0) {
											h_excite_PIdgated_4556keVgated->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (gamma.dopE > 5891.0 && gamma.dopE < 5913.0) {
											h_excite_PIdgated_5902keVgated->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (gamma.dopE > 3509.0 && gamma.dopE < 3579.0) {
											h_excite_PIdgated_3538keVgated->Fill(DL_and_t_in_LiF_corr_excite);
										}
										// if (gamma.dopE > 5442.0 && gamma.dopE < 5406.0) {
										// 	h_excite_PIdgated_5374keVgated->Fill(DL_and_t_in_LiF_corr_excite);
										// }
										// States below alpha threshold
										if (gamma.dopE > 4963.26 && gamma.dopE < 5018.74) { // 2 sigma
											h_excite_PIdgated_4984keVgated->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (gamma.dopE > 4241.59 && gamma.dopE < 4346.41) { // 3 sigma
											h_excite_PIdgated_4286keVgated->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (gamma.dopE > 3139.01 && gamma.dopE < 3206.99) { // 3 sigma
											h_excite_PIdgated_3166keVgated->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (gamma.dopE > 5060.8 && gamma.dopE < 5133.2) { // 2 sigma
											h_excite_PIdgated_5080_5093keVgated->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (gamma.dopE > 3698.28 && gamma.dopE < 3783.72) { // 3 sigma
											h_excite_PIdgated_3745keVgated->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (gamma.dopE > 3351.89 && gamma.dopE < 3456.11) { // 3 sigma
											h_excite_PIdgated_3400keVgated->Fill(DL_and_t_in_LiF_corr_excite);
										}

										
										// Reverse gates
										if (DL_and_t_in_LiF_corr_excite > 7.180 && DL_and_t_in_LiF_corr_excite < 7.460) {
											h_dopE_addback_reverseGate_lower->Fill(gamma.dopE);	//Reverse gate 1
										}
										if (DL_and_t_in_LiF_corr_excite > 7.911 && DL_and_t_in_LiF_corr_excite < 8.233) {
											h_dopE_addback_reverseGate_higher->Fill(gamma.dopE);	//Reverse gate 2
										}
										if (DL_and_t_in_LiF_corr_excite > 7.460 && DL_and_t_in_LiF_corr_excite < 7.911) {
											h_dopE_addback_reverseGate_middle->Fill(gamma.dopE);	//Reverse gate 2
										}

									//if (gamma.dopE > 4528.0 && gamma.dopE < 4584.0) { //Two sigma
									if (gamma.dopE > 4547.0 && gamma.dopE < 4575.0) { //One sigma
							
										if (s3hit->GetRing()== 0 || s3hit->GetRing()== 1 || s3hit->GetRing()== 2|| s3hit->GetRing()== 3) {
											h_excite_ringA_4556->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (s3hit->GetRing()== 4|| s3hit->GetRing()== 5 || s3hit->GetRing()== 6 || s3hit->GetRing()== 7 ) {
											h_excite_ringB_4556->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (s3hit->GetRing()== 8|| s3hit->GetRing()== 9 || s3hit->GetRing()== 10 || s3hit->GetRing()== 11) {
											h_excite_ringC_4556->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (s3hit->GetRing()== 12|| s3hit->GetRing()== 13 || s3hit->GetRing()== 14 || s3hit->GetRing()== 15) {
											h_excite_ringD_4556->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (s3hit->GetRing()== 16|| s3hit->GetRing()== 17 || s3hit->GetRing()== 18 || s3hit->GetRing()== 19) {
											h_excite_ringE_4556->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (s3hit->GetRing()== 20 || s3hit->GetRing()== 21 || s3hit->GetRing()== 22 || s3hit->GetRing()== 23) {
											h_excite_ringF_4556->Fill(DL_and_t_in_LiF_corr_excite);
										}
									}

									//if (gamma.dopE > 6380.0 && gamma.dopE < 6436.0) {	// Two sigma
									if (gamma.dopE > 6394.0 && gamma.dopE < 6422.0) {	// One sigma
							
										if (s3hit->GetRing()== 0 || s3hit->GetRing()== 1 || s3hit->GetRing()== 2|| s3hit->GetRing()== 3|| s3hit->GetRing()== 4|| s3hit->GetRing()== 5 || s3hit->GetRing()== 6|| s3hit->GetRing()== 7) {
											h_excite_ringA_6408->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (s3hit->GetRing()== 8|| s3hit->GetRing()== 9 || s3hit->GetRing()== 10 || s3hit->GetRing()== 11 || s3hit->GetRing()== 12 || s3hit->GetRing()== 13|| s3hit->GetRing()== 14|| s3hit->GetRing()== 15) {
											h_excite_ringB_6408->Fill(DL_and_t_in_LiF_corr_excite);
										}
										if (s3hit->GetRing()== 16|| s3hit->GetRing()== 17 || s3hit->GetRing()== 18 || s3hit->GetRing()== 19|| s3hit->GetRing()== 20  || s3hit->GetRing()== 21 || s3hit->GetRing()== 22 || s3hit->GetRing()== 23) {
											h_excite_ringC_6408->Fill(DL_and_t_in_LiF_corr_excite);
										}
									}

										for (int t2 = t+1; t2 < tig_data->GetAddbackMultiplicity(); t2++) {
                        				add_hit2 = tig_data->GetAddbackHit(t2);
                        				suppAdd = add_hit2->BGOFired();
                        				if (gate1D((add_hit->GetTime() - add_hit2->GetTime()), tigtigT[0], tigtigT[1])) {
                          					gamma2.dopE = add_hit2->GetDoppler(Beta, & recoil_vec);
                          					h_gammagamma->Fill(add_hit->GetEnergy(),add_hit2->GetEnergy()); // gamma-gamma

								gamma2.time = add_hit2->GetTime();
								gamma2.rawE = add_hit2->GetEnergy();
                          				} // end gamma-gamma time gate
                      				} // end 2nd gamma loop
						
								}   // End PID condition
							}  // End Ne Cut open condition
							
							 if (Fluorine_Gate->IsInside(totalE,ic.esum)) {
								F_excite_pid_gated->Fill(DL_and_t_in_LiF_corr_excite);
								F_excite_v_dopE_addback->Fill(gamma.dopE,DL_and_t_in_LiF_corr_excite);
								F_dopE_addback_gated->Fill(gamma.dopE);
							} // end of the fluorine cut condition
							
							
							s3tigemma->Fill(); // Fill S3-TIGRESS-EMMA coincidence tree
						} // end EMMA-TIG Time Cut
						} // End S3-TIG Time Cut 
					} // End Suppressor Conditon

				} // End Loop over TIGRESS Events

			} // End Loop over S3 pixel hits

		} // Loop over emma events

	} // End Tig & S3 & EMMA hit condition

	tig_data->ResetAddback();	//================== Stupid GRSIsort bug

	if (jentry % 10000 == 0)
      		cout << setiosflags(ios::fixed) << "Entry " << jentry << " of " << analentries << ", " << 100 * jentry / analentries << "% complete" << "\r" << flush; // Event counter
} // End Analysis Tree Loop

  cout << "Entry " << analentries << " of " << analentries << ", 100% complete" << endl;
  cout << "Event sorting complete" << endl;

  cout << "Writing histograms to " << outfile << endl;

  myfile->cd();
  
  TDirectory *samDir = myfile->mkdir("SamPlots");
  samDir->cd();
  samList->Write();
  myfile->cd();

  myfile->cd();
  myfile->Write();
  myfile->Close();

}

int main(int argc, char ** argv) {


  SortCode_Sam * mysort = new SortCode_Sam();

  char const * afile;
  char const * outfile;
  char const * calfile;
  char const * s3dl;
  char const * cutfile;
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
    mysort->SortData(afile, calfile, outfile);
  } else if (argc == 3) {
    afile = argv[1];
    calfile = argv[2];
    outfile = "HistFiles/Histograms.root";
    printf("Analysis file: %s\nCalibration file: %s\nOutput file: %s\n", afile, calfile, outfile);
    mysort->SortData(afile, calfile, outfile);
  } else if (argc == 4) {
    afile = argv[1];
    calfile = argv[2];
    outfile = argv[3];
    printf("Analysis file: %s#define Sortcode_cxx\nCalibration file: %s\nOutput file: %s\n", afile, calfile, outfile);
    mysort->SortData(afile, calfile, outfile);
  } else if (argc == 5) {
    afile = argv[1];
    calfile = argv[2];
    outfile = argv[3];
    s3dl = argv[4];
    printf("Analysis file: %s\nCalibration file: %s\nOutput file: %s\nS3 dead layer: %s\n", afile, calfile, outfile, s3dl);
    mysort->SortData(afile, calfile, outfile, s3dl);
  } else if (argc == 6) {
    afile = argv[1];
    calfile = argv[2];
    outfile = argv[3];
    s3dl = argv[4];
    cutfile = argv[5];
    printf("Analysis file: %s\nCalibration file: %s\nOutput file: %s\nS3 dead layer: %s\nCuts File: %s\n", afile, calfile, outfile, s3dl, cutfile);
    mysort->SortData(afile, calfile, outfile, s3dl, cutfile);
  } else if (argc > 6) {
    printf("Doh! Too many arguments\n");
    return 0;
  }

  return 0;
}
