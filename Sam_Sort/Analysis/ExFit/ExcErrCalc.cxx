// g++ ExcErrCalc.cxx -std=c++0x -I$GRSISYS/include -L$GRSISYS/lib `grsi-config --cflags --all-libs --GRSIData-libs` -I$GRSISYS/GRSIData/include -L$GRSISYS/GRSIData/lib `root-config --cflags --libs` -o ExcErrCalc
// Estimation of the excitation energy resolution for varying triton energies
// Mostly taken from Thomas Chillery's code for S2223 analysis

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <map>

#include "TMath.h" // ROOT TMath
#include "TEnv.h" // Required for declaring gEnv
#include "TCanvas.h" // ROOT TCanvas
#include "TGraph.h" // ROOT TGraph
#include "TGraphErrors.h" // ROOT TGraphErrors
#include "TLegend.h" // ROOT TLegend

#include "TSRIM.h" // GRSI -> SRIM energy loss calc
#include "TReaction.h" // GRSI -> Reaction definition

Double_t r2d = TMath::RadToDeg();
Double_t d2r = TMath::DegToRad();

float CalcPartialDeriv_dExdTA(TReaction *reac, float TA, float Tb, float theta) {
  // Input energies in MeV, angles in radians
  // a(A,b)B - a is target, A is projectile, b is ejectile, and B is heavy recoil
  // Extract masses directly from the TReaction object in MeV
  // fM[0] = ma (Projectile), fM[1] = mA (Target), fM[2] = mb (Ejectile), fM[3] = mB (Recoil)
  float ma = reac->GetM(0); // Projectile (17O)
  float mA = reac->GetM(1); // Target (7Li)
  float mb = reac->GetM(2); // Ejectile (3H)
  float mB = reac->GetM(3); // Heavy recoil (21Ne)

  float denom = TMath::Sqrt( mA*mA + ma*ma + mb*mb + 2*mA*ma + 2*ma*TA 
                - 2*(mb+Tb)*(mA+ma+TA) 
                + 2*TMath::Sqrt(TA*(2*mA+TA))*TMath::Sqrt(Tb*(2*mb+Tb))*TMath::Cos(theta)
              );

  // Partial differential            
  float dEx_dTA = ( 2*ma - 2*(mb+Tb) + (2*(mA+TA)*TMath::Sqrt(Tb*(2*mb+Tb))*TMath::Cos(theta))/(TMath::Sqrt(TA*(2*mA+TA))) ) / (2.*denom);  

  return dEx_dTA;    
}

float CalcPartialDeriv_dExdTb(TReaction *reac, float TA, float Tb, float theta){
	// Input energies in MeV, angles in radians
	// a(A,b)B - a is target, A is projectile, b is ejectile, and B is heavy recoil
  // Extract masses directly from the TReaction object in MeV
  // fM[0] = ma (Projectile), fM[1] = mA (Target), fM[2] = mb (Ejectile), fM[3] = mB (Recoil)
  float ma = reac->GetM(0); // Projectile (17O)
  float mA = reac->GetM(1); // Target (7Li)
  float mb = reac->GetM(2); // Ejectile (3H)
  float mB = reac->GetM(3); // Heavy recoil (21Ne)

  float denom = TMath::Sqrt( mA*mA + ma*ma + mb*mb + 2*mA*ma + 2*ma*TA 
                - 2*(mb+Tb)*(mA+ma+TA) 
                + 2*TMath::Sqrt(TA*(2*mA+TA))*TMath::Sqrt(Tb*(2*mb+Tb))*TMath::Cos(theta)
              );
            
  // Partial differential            
  float dEx_dTb = ( (2*TMath::Sqrt(TA*(2*mA+TA))*(mb+Tb)*TMath::Cos(theta))/(TMath::Sqrt(Tb*(2*mb+Tb))) - 2*(mA+ma+TA) ) / (2.*denom);

	return dEx_dTb;    
}

float CalcPartialDeriv_dExdTheta(TReaction *reac, float TA, float Tb, float theta){
	// Input energies in MeV, angles in radians
	// a(A,b)B - a is target, A is projectile, b is ejectile, and B is heavy recoil
  // Extract masses directly from the TReaction object in MeV
  // fM[0] = ma (Projectile), fM[1] = mA (Target), fM[2] = mb (Ejectile), fM[3] = mB (Recoil)
  float ma = reac->GetM(0); // Projectile (17O)
  float mA = reac->GetM(1); // Target (7Li)
  float mb = reac->GetM(2); // Ejectile (3H)
  float mB = reac->GetM(3); // Heavy recoil (21Ne)

  float denom = TMath::Sqrt( mA*mA + ma*ma + mb*mb + 2*mA*ma + 2*ma*TA 
                - 2*(mb+Tb)*(mA+ma+TA) 
                + 2*TMath::Sqrt(TA*(2*mA+TA))*TMath::Sqrt(Tb*(2*mb+Tb))*TMath::Cos(theta)
              );
            
  // Partial differential            
  float dEx_dtheta = (-TMath::Sqrt(TA*(2*mA+TA))*TMath::Sqrt(Tb*(2*mb+Tb))*TMath::Sin(theta)) / denom;

	return dEx_dtheta;    
}

void PlotExcErr(
    const std::map<Int_t, std::vector<double>>& ex,
    const std::map<Int_t, std::vector<double>>& exErr,
    const std::map<Int_t, std::vector<double>>& exErrTA,
    const std::map<Int_t, std::vector<double>>& exErrTb,
    const std::map<Int_t, std::vector<double>>& exErrTheta,
    const std::vector<double>& tritonE,
    const std::vector<double>& s3Theta
) {

  TCanvas *c0 = new TCanvas ("c0","c0",900,600);
  c0->cd();
  TGraph* graphEx = new TGraph(tritonE.size(),tritonE.data(),ex.at(0).data());
  graphEx->Draw("AP");
  graphEx->SetMarkerStyle(8);
  graphEx->SetMarkerSize(2);
  graphEx->SetTitle(" ; Measured Triton Energy [keV]; Excitation Energy [MeV]");
  graphEx->GetYaxis()->CenterTitle();
  graphEx->GetXaxis()->CenterTitle();

  TCanvas *c1 = new TCanvas ("c1","c1",900,600);
  c1->cd();
  TGraph* graph = new TGraph(tritonE.size(),tritonE.data(),exErr.at(0).data());
  graph->Draw("AP");
  graph->SetMarkerStyle(22);
  graph->SetMarkerSize(2);
  graph->SetMarkerColor(kGreen+2);
  graph->SetTitle(" ; Measured Triton Energy [keV]; Excitation Energy Sigma [MeV]");
  graph->GetYaxis()->CenterTitle();
  graph->GetXaxis()->CenterTitle();
  graph->GetYaxis()->SetRangeUser(0.01, 0.03);

  TGraph* graphMid = new TGraph(tritonE.size(), tritonE.data(), exErr.at(11).data());
  TGraph* graphMax = new TGraph(tritonE.size(), &tritonE[0], exErr.at(23).data());
  graphMid->Draw("same P");
  graphMid->SetMarkerStyle(21);
  graphMid->SetMarkerSize(2);
  graphMid->SetMarkerColor(kRed+1);
  graphMax->Draw("same P");
  graphMax->SetMarkerStyle(8);
  graphMax->SetMarkerColor(kBlack);
  graphMax->SetMarkerSize(2);

  TLegend *leg = new TLegend(0.55, 0.65, 0.85, 0.85);
  leg->AddEntry(graphMax,Form("Ring %d, %.1f degrees", 23, s3Theta[23]*r2d),"p");
  leg->AddEntry(graphMid,Form("Ring %d, %.1f degrees", 11, s3Theta[11]*r2d),"p");
  leg->AddEntry(graph,Form("Ring %d, %.1f degrees", 0, s3Theta[0]*r2d),"p");
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->Draw("same");

  c1->SaveAs("temp.pdf");


  TCanvas *c2 = new TCanvas ("c2","c2",900,600);
  c2->cd();
  graphMax->Draw("AP");
  graphMax->SetTitle(Form("Ring %d, %.1f degrees", 23, s3Theta[23]*r2d));
  graphMax->GetYaxis()->SetRangeUser(0.00, 0.2);
  graphMax->SetTitle(Form("Ring %d, %.1f degrees ; Measured Triton Energy [keV]; Excitation Energy Sigma [MeV]", 23, s3Theta[23]*r2d));
  graphMax->GetYaxis()->CenterTitle();
  graphMax->GetXaxis()->CenterTitle();
  TGraph* graphTAComp = new TGraph(tritonE.size(), tritonE.data(), exErrTA.at(23).data());
  TGraph* graphTbComp = new TGraph(tritonE.size(), tritonE.data(), exErrTb.at(23).data());
  TGraph* graphThetaComp = new TGraph(tritonE.size(), tritonE.data(), exErrTheta.at(23).data());
  graphTAComp   ->Draw("same P");
  graphTbComp   ->Draw("same P");
  graphThetaComp->Draw("same P");
  graphTAComp   ->SetMarkerSize(2);
  graphTbComp   ->SetMarkerSize(2);
  graphThetaComp->SetMarkerSize(2);
  graphTAComp   ->SetMarkerStyle(21);
  graphTbComp   ->SetMarkerStyle(22);
  graphThetaComp->SetMarkerStyle(23);
  graphTAComp   ->SetMarkerColor(kRed+1);
  graphTbComp   ->SetMarkerColor(kGreen+2);
  graphThetaComp->SetMarkerColor(kBlue+1);

  TLegend *leg2 = new TLegend(0.55, 0.65, 0.85, 0.85);
  leg2->AddEntry(graphMax,"Total Ex Uncertainty","p");
  leg2->AddEntry(graphTAComp,"Beam Energy Component","p");
  leg2->AddEntry(graphTbComp,"Triton Energy Component","p");
  leg2->AddEntry(graphThetaComp,"Triton Angle Component","p");
  leg2->SetFillStyle(0);
  leg2->SetBorderSize(0);
  leg2->Draw("same");
  c2->SaveAs("temp2.pdf");
}

void ExcErrCalc() {
  
  // SRIM energy loss files
  TSRIM *srim_O17_in_LiF = new TSRIM;
  srim_O17_in_LiF->ReadEnergyLossFile("O17_in_LiF.txt");
  TSRIM *srim_Triton_in_Si = new TSRIM;
  srim_Triton_in_Si->ReadEnergyLossFile("Triton_in_Si.txt");
  TSRIM *srim_Triton_in_LiF = new TSRIM;
  srim_Triton_in_LiF->ReadEnergyLossFile("Triton_in_LiF.txt");
  
  // ----------------------- Beam energy uncertainty -------------------------- //
  // SRIM uncertainties on dE/dx for O ions and H ions in all target elements (taken from http://www.srim.org/PlotCit.htm)
  double stopPowO_Err = 6/100.; // 6% on dE/dx 
  double stopPowTriton_Err = 4.5/100.; // 4.5% on dE/dx

  // LiF target characteristics
  double targetThicknessugcm2 = 100.;// approximately 100 ug/cm2 LiF targets
  double targetThicknessum = 0.39; // 100 ug/cm2 LiF are approx 0.39 um thick.

  // Beam energy at front of target
  double EBeamFront = 4.0 * 17; // 68 MeV 17O beam. Incident beam energy uncertainty was +- 0.005 MeV/u so a 1 sigma uncertainty of 0.17%
  double BeamRadius = 0.25; // mm --> Calculated from 1 mm Gaussian beam diameter
  double EBeam_Err = EBeamFront*0.17/100.; // MeV, 1 sigma
  printf("Beam energy at front of LiF: %f +/- %f MeV\n", EBeamFront, EBeam_Err);
  
  double EBeamMid = srim_O17_in_LiF->GetAdjustedEnergy(EBeamFront*1000, targetThicknessum/2., 0.001)/1000.;
  double EBeamBack = srim_O17_in_LiF->GetAdjustedEnergy(EBeamFront*1000, targetThicknessum, 0.001)/1000.;
  // *1000 and /1000 convert from MeV to keV and back. we use /2.0 to get the beam energy in the middle of the target. 0.001 is the integration step size

  // Half target thickness in energy. Beam energy follows a flat-top trend through the thickness.
  // 1 std dev = full width / sqrt(12). 
  // References: https://www.bipm.org/documents/20126/2071204/JCGM_100_2008_E.pdf   and   https://www.itl.nist.gov/div898/handbook/mpc/section5/mpc541.htm 
  double eErrTargThick = (EBeamFront - EBeamBack)/TMath::Sqrt(12);

  // Find impact on calculated central energy from the stopping power SRIM uncertainty of oxygen on materials
  double eErrSPO = stopPowO_Err*(srim_O17_in_LiF->GetEnergyLost(EBeamFront*1000, targetThicknessum/2., 0.001)/1000);

  // Total uncertainty associated with the beam energy
  double sigma_TA = TMath::Sqrt(pow(EBeam_Err,2) + pow(eErrTargThick, 2) + pow(eErrSPO,2));
  printf("Uncertainty on beam energy at middle of LiF from incident error =  %f MeV, from 1/2 target thickness = %f MeV, and from srim uncertainty = %f MeV\n", EBeam_Err, eErrTargThick, eErrSPO);
  printf("Beam energy at middle of LiF: %f +/- %f MeV\n", EBeamMid, sigma_TA);
  printf("Beam energy at back of LiF: %f MeV\n", EBeamBack);
  // -------------------------------------------------------------------------- //

  // Reaction definiton to determine excitation energy
  // TReaction arguments: "beam", "target", "light ejectile", "heavy recoil", beam energy in MeV, excitation energy in MeV, flag true if inverse kinematics 
  TReaction *reac = new TReaction("o17", "li7", "h3", "ne21", EBeamMid, 0, true);

  // ----------------------- Lab angle uncertainty ---------------------------- //
  // S3 parameters
  int S3RingNumber = 24;
  double S3InnerRadius = 11; // mm --> Inner radius of donut active area
  double S3OuterRadius = 35; // mm --> Outer radius of donut active area
  double S3TargetDistance = 30; // mm  
  double RingWidth = (S3OuterRadius - S3InnerRadius) / 24; // = 1 mm
  // S3 dead layer
  double s3DL = 0.692; // um (2020 20V)
  double s3DL_Err = 0.131; // um (2020 20V)
  //double s3DL = 0.748; // um (2020 40V)
  //double s3DL_Err = 0.111; // um (2020 40V)
  //double s3DL = 0.694; // um +- 0.113 um (2019)
  //double s3DL_Err = 0.113; // um (2019)


  std::vector<double> s3Theta, s3ThetaMin, s3ThetaMax; // lab angles in radians
  for(Int_t r = 0; r < S3RingNumber; r++){
    // Finite size of beam spot enlarges the angular coverage of each ring
    // The minimum possible radius of a given ring due to beam spot is Rmin = Physical inner radius of ring 0 + ring index number * ring width - Gaussian beam radius
    // The maximum possible radius of a given ring due to beam spot is Rmax = Physical inner radius of ring 0 + (ring index number + 1) * ring width + Gaussian beam radius
    double Rmin = S3InnerRadius + r*RingWidth - BeamRadius;
    double Rmax = S3InnerRadius + (r+1.)*RingWidth + BeamRadius;
    double radius = Rmin + (Rmax-Rmin)/2.;
    s3ThetaMax.push_back(TMath::Pi() - TMath::ATan( Rmin/S3TargetDistance ));
    s3Theta.push_back(TMath::Pi() - TMath::ATan( radius/S3TargetDistance ));
    s3ThetaMin.push_back(TMath::Pi() - TMath::ATan( Rmax/S3TargetDistance ));
  } // end of loop over S3 rings
  // Uncertainty in lab angle given by sigma_theta later in script
  // -------------------------------------------------------------------------- //


  std::vector<double> tritonE = {800, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000}; // keV, these are the DETECTED triton energies. We will correct for S3 dead layer and, for the back of target consideration, the energy loss through target
  std::map<Int_t, std::vector<double>> ex; // integer key is the ring angle index
  std::map<Int_t, std::vector<double>> exErrTA; // integer key is the ring angle index
  std::map<Int_t, std::vector<double>> exErrTb; // integer key is the ring angle index
  std::map<Int_t, std::vector<double>> exErrTheta; // integer key is the ring angle index
  std::map<Int_t, std::vector<double>> exErr; // integer key is the ring angle index


  for(Int_t i=0; i<tritonE.size(); i++){ // Loop over sample triton energies
    std::cout<<"Triton energy = "<< tritonE[i] <<" keV"<<std::endl;
    std::cout<<"Theta [deg] , triton E [keV] , sigma_Tb [keV] , Ex [MeV] , sigma_Ex [MeV], TA Comp [MeV] , Tb Comp [MeV] , Theta Comp [MeV]\n"<<std::endl;
    for(Int_t j=0; j<s3Theta.size(); j++){ // Loop over S3 ring angles
      double theta = TMath::Pi() - s3Theta[j]; // Angle of S3 ring to beam axis -> for effective thicknesses (energy loss)

      // Triton energy corrected for S3 dead layer
      double energy = tritonE[i];
      energy += srim_Triton_in_Si->GetEnergyLost(energy, s3DL/TMath::Cos(theta));
      std::cout<< "Triton energy after S3 dead layer correction: "<< energy <<" keV\n";
      double energyFront = energy; // energy at target front after DL correction

      // Triton energy corrected for energy loss in LiF target
      energy += srim_Triton_in_LiF->GetEnergyLost(energy, targetThicknessum/2./TMath::Cos(theta), 0.001);
      std::cout<< "Triton energy after energy loss in LiF target correction: "<< energy <<" keV\n";

      // 21Ne excitation energy
      double Ex = reac->GetExcEnergy(energy * 1e-3, s3Theta[j]); // Excitation energy of 21Ne recoils
      reac->SetExcEnergy(Ex);

      // --------------------- Triton energy uncertainty -------------------------- //
      double energyBack = energyFront + srim_Triton_in_LiF->GetEnergyLost(energyFront, targetThicknessum/TMath::Cos(theta), 0.001); 
      double alphaResolution = 35.; // keV (2020) --> 1 sigma from 5.8 MeV alpha fit
      //double alphaResolution = 15.; // keV (2019) --> 1 sigma from 5.8 MeV alpha fit
      double eErrNoise = tritonE[i] * alphaResolution/5800.; // Scaled to 1 sigma resolution of 5.8 MeV alpha, estimate of electronics noise (we dont have a pulser walkthrough)
      double eErrDL = srim_Triton_in_Si->GetEnergyLost(tritonE[i], (s3DL+s3DL_Err)/TMath::Cos(theta), 0.001) - srim_Triton_in_Si->GetEnergyLost(tritonE[i], s3DL/TMath::Cos(theta), 0.001);
      double eErrSPSi = srim_Triton_in_Si->GetEnergyLost(tritonE[i], s3DL/TMath::Cos(theta), 0.001)*stopPowTriton_Err;
      double eErrSPLiF = srim_Triton_in_LiF->GetEnergyLost(energyFront, targetThicknessum/2./TMath::Cos(theta), 0.001)*stopPowTriton_Err;
      double eErrTEloss = (energyBack - energyFront)/TMath::Sqrt(12.); // Flat-top
      
      std::cout << "Triton energy Error from noise: " << eErrNoise << " keV, from DL thickness: " << eErrDL << " keV, from stopping powers: " << eErrSPSi << " keV, and " << eErrSPLiF << " keV\n" << std::endl;
      double sigma_Tb = TMath::Sqrt(pow(eErrNoise,2) + pow(eErrDL,2) + pow(eErrSPSi,2) + pow(eErrSPLiF,2) + pow(eErrTEloss,2)); // keV
      printf("Triton energy = %f +- %f keV\n", tritonE[i], sigma_Tb);
      // -------------------------------------------------------------------------- //

      double sigma_theta = (s3ThetaMax[j] - s3ThetaMin[j])/6.; // Uncertainty associated with lab angle of triton
      std::cout << "Ring number " << j << ": Uncertainty = +-" << 180/TMath::Pi() * sigma_theta << " deg\n" << std::endl;
      
      // --------------------- Compute total uncertainty -------------------------- //
      double dExdTA = TMath::Sqrt(pow(CalcPartialDeriv_dExdTA(reac, EBeamMid, energy/1000., s3Theta[j]) * sigma_TA, 2));
      std::cout << "dExdTA " << dExdTA << std::endl;
      double dExdTb = TMath::Sqrt(pow(CalcPartialDeriv_dExdTb(reac, EBeamMid, energy/1000., s3Theta[j]) * sigma_Tb/1000, 2));
      double dExdTheta = TMath::Sqrt(pow(CalcPartialDeriv_dExdTheta(reac, EBeamMid, energy/1000., s3Theta[j]) * sigma_theta, 2));
      float sigma_Ex = TMath::Sqrt(pow(dExdTA, 2) + pow(dExdTb, 2) + pow(dExdTheta, 2) );
      // -------------------------------------------------------------------------- //

      std::cout << "Lab angle " << s3Theta[j]*r2d << " deg, Triton energy " << energy << " +- " << sigma_Tb << " keV, 21Ne Excitation energy " << Ex << " +- " << sigma_Ex << " MeV\n" << std::endl;

      // Append results to relevant arrays
      ex[j].push_back(Ex);
      if(Ex>0){   
        exErrTA[j].push_back(dExdTA);
        exErrTb[j].push_back(dExdTb);
        exErrTheta[j].push_back(dExdTheta);
        exErr[j].push_back(sigma_Ex);
      }
      else{ 
        exErrTA[j].push_back(0);
        exErrTb[j].push_back(0);
        exErrTheta[j].push_back(0);
        exErr[j].push_back(0);
      } // end of check to see if excitation energy is real (positive)
    } // end of loop over S3 ring angles
  } // end of loop over triton energies

  PlotExcErr(ex, exErr, exErrTA, exErrTb, exErrTheta, tritonE, s3Theta);

} // end of ExcErrCalc function

int main() {
  std::string grsi_path = getenv("GRSISYS"); // Finds the GRSISYS path to be used by other parts of the grsisort code
  if(grsi_path.length() > 0){
	  grsi_path += "/";
 	}
  // Read in grsirc in the GRSISYS directory to set user defined options on grsisort startup
	grsi_path += ".grsirc";
  gEnv->ReadFile(grsi_path.c_str(), kEnvChange);

  ExcErrCalc();

  return 0;
}