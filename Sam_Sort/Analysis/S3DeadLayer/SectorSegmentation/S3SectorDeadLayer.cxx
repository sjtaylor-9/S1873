// g++ S3SectorDeadLayer.cxx -std=c++0x -I$GRSISYS/include -L$GRSISYS/lib `grsi-config --cflags --all-libs --GRSIData-libs` -I$GRSISYS/GRSIData/include -L$GRSISYS/GRSIData/lib `root-config --cflags --libs` -o S3SectorDeadLayer

#include <iostream>
#include <iomanip>
#include <cmath>

#include "TSRIM.h"
#include "TF1.h"
#include "TCutG.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TEnv.h" // Required for declaring gEnv

double ring_to_lab_angle(int ring_num) {
    static const double ring_angles[24] = {
        159.1, 157.4, 155.8, 154.2, 152.7, 151.2, 149.8, 148.4,
        147.0, 145.7, 144.4, 143.1, 141.9, 140.8, 139.7, 138.6,
        137.5, 136.5, 135.5, 134.6, 133.7, 132.8, 131.9, 131.0
    };
    return 180 - ring_angles[ring_num];
}

void get_cos_bins(double xbins[25]) {
        static const double ring_edges[25] = {
        159.9, 158.2, 156.6, 155.0, 153.4, 151.9, 150.5, 149.0,
        147.7, 146.3, 145.0, 143.7, 142.5, 141.3, 140.2, 139.1,
        138.0, 137.0, 136.0, 135.0, 134.1, 133.2, 132.3, 131.4,
        130.6
    };

    for (int i = 0; i < 25; i++) {
        double theta_lab = 180.0 - ring_edges[i];
        xbins[i] = 1.0/std::cos(theta_lab*TMath::DegToRad());
    }
}

void S3DeadLayerCalc(double DLSlopeCoeff, int sector) {
    
    double DLThickness = 0.0;

    if (DLSlopeCoeff == 0) {
        DLThickness = 0;
        std::cout << "Faulty sector, dead layer thickness is 0 um." << std::endl;
    } else {
        // Read in the SRIM energy loss data for an alpha in silicon.
        TSRIM * srim_4He_in_Si = new TSRIM;
        srim_4He_in_Si->ReadEnergyLossFile("4He_in_Si.txt"); // Eloss of alpha particle in silicon
        
        double alphaEnergy = 5156.59; // keV, 239Pu dominant alpha 
        //double alphaEnergy = 5485.56; // keV, 241Am dominant alpha 
        //double alphaEnergy = 5804.77; // keV, 244Cm dominant alpha 
        Double_t dEdx = srim_4He_in_Si->GetEnergyLost(alphaEnergy, 1., 0.001); // Get dE/dX at reference alpha energy in keV/um (energy, distance, step size)
        std::cout << "Energy loss of " << alphaEnergy << " MeV alpha in Si " << dEdx << " keV/um" << std::endl;

        DLThickness = - DLSlopeCoeff / dEdx; // Dead layer thickness in um
        std::cout << "Dead layer thickness of sector " << sector <<": " << DLThickness << " um" << std::endl;

        // Read in the SRIM energy loss data for a triton in silicon
        TSRIM * srim_3H_in_Si = new TSRIM;
        srim_3H_in_Si->ReadEnergyLossFile("Triton_in_Si.txt"); // Eloss of triton in silicon
        double min_t_E = 1000; // keV
        Double_t min_t_E_loss = srim_3H_in_Si->GetEnergyLost(min_t_E, 1., 0.001);
        std::cout << "Energy loss of 1 MeV triton in Si " << min_t_E_loss << " keV/um" << std::endl;
        double max_t_E = 5000; // keV
        Double_t max_t_E_loss = srim_3H_in_Si->GetEnergyLost(max_t_E, 1., 0.001);
        std::cout << "Energy loss of 5 MeV triton in Si " << max_t_E_loss << " keV/um" << std::endl;
        double gamow_t_E = 2600; // keV
        Double_t gamow_t_E_loss = srim_3H_in_Si->GetEnergyLost(gamow_t_E, 1., 0.001);
        std::cout << "Energy loss of 2.6 MeV triton in Si " << gamow_t_E_loss << " keV/um" << std::endl;
    }

    // Append sector number and dead layer thickness to file
    std::ofstream outfile("DeadLayerThickness.dat", std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Error: could not open DeadLayerThickness.dat" << std::endl;
        return;
    }
    outfile << "Sector:" << sector << " " << "Dead layer thickness (um): " << DLThickness << std::endl;
    outfile.close();
}

void S3DeadLayer(std::string calibrationRun, int sector) {
    // 2020 20V
    //TFile *inputFile = TFile::Open("/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Calibrations/AlphaCalibration/Triple_alpha_plots/SamCalibrationFile/Nov2020/WithAfterCal/20V/S3_triple_alpha_calibration_plots.root");
    // 2020 40V
    //TFile *inputFile = TFile::Open("/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Calibrations/AlphaCalibration/Triple_alpha_plots/SamCalibrationFile/Nov2020/WithAfterCal/40V/S3_triple_alpha_calibration_plots.root");
    // 2019
    TFile *inputFile = TFile::Open("/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Calibrations/AlphaCalibration/Triple_alpha_plots/SamCalibrationFile/Dec2019/WithAfterCal/S3_triple_alpha_calibration_plots.root");
    if (!inputFile || inputFile->IsZombie()) {
        std::cerr << "Error: Could not open input file!" << std::endl;
        return;
    }
    TDirectory *dir = inputFile->GetDirectory("SectorEnergiesVRings");
    if (!dir) {
        std::cerr << "Error: Directory 'SectorEnergiesVRings' not found in file!" << std::endl;
        inputFile->Close();
        return;
    }


    // Plot the S3 sector energy against the ring number
    TCanvas *c1 = new TCanvas("c1", "Sector energy vs ring number for triple alpha run", 800, 600);
    TH2F *h_sectorE_v_ring_num = (TH2F*)dir->Get(Form("h_sector%dE_v_ring_num_after_cal", sector)); // Use the sector number to extract the data for the given sector
    if (!h_sectorE_v_ring_num) {
        Error("S3DeadLayer", "Histogram for sector %d not found!", sector);
        return;
    }
    h_sectorE_v_ring_num->Draw("COLZ");
    h_sectorE_v_ring_num->GetYaxis()->SetRangeUser(4500,6500);
    c1->SetLogz();
    c1->SaveAs(Form("Sectors/%s/%d/SectorEVRingNum.pdf", calibrationRun.c_str(), sector));
    
    // Checks to see whether the sectors are faulty and sets dead layer to 0 thickness if they are.
    double Pu_slope = 0.0;
    double Am_slope = 0.0;
    double Cm_slope = 0.0;
    if (h_sectorE_v_ring_num->GetEntries() < 1000) {
        std::cout << "There are less than 1000 entries in sector " << sector << ". The sector is faulty and will be skipped (dead layer thickness set to 0 um)." << std::endl;
    }
     else { 
        // Create a new histogram for sector energy vs 1/cos(theta)
        double xbins[25];
        get_cos_bins(xbins);    
        TH2F *h_sectorE_v_inv_cos = new TH2F("h_sectorE_v_inv_cos", 
                                            Form("Sector %d Energy vs 1/cos(#theta); 1/cos(#theta); Sector Energy (50 keV bins)", sector), 
                                            24, // 1/cos(theta) bins
                                            xbins, // 1/cos(theta) bin edges
                                            200, // Sector energy bins
                                            0, // y axis lower limit
                                            10000 // y axis upper limit
                                        );

        // Converts the ring number into lab angle
        for (int i = 1; i <= h_sectorE_v_ring_num->GetNbinsX(); i++) {
            int ring_num = (int)h_sectorE_v_ring_num->GetXaxis()->GetBinCenter(i);
            double lab_angle = ring_to_lab_angle(ring_num); // Converts ring number to lab angle in degrees
            // Convert to radians and compute 1/cos(theta)
            double inv_cos = 1.0 / std::cos(lab_angle * TMath::DegToRad());

            for (int j = 1; j <= h_sectorE_v_ring_num->GetNbinsY(); j++) {
                double energy = h_sectorE_v_ring_num->GetYaxis()->GetBinCenter(j);
                double counts = h_sectorE_v_ring_num->GetBinContent(i, j);
                // Pass inv_cos (X), energy (Y), and counts (z axis) to the new histogram
                h_sectorE_v_inv_cos->Fill(inv_cos, energy, counts); // x = inv_cos, y = energy, z = counts

            } // end of loop over sector energies
        } // end of loop over ring numbers
        std::cout << "Original entries: "
                << h_sectorE_v_ring_num->GetEntries() << '\n';

        std::cout << "Original sum weights: "
                << h_sectorE_v_ring_num->GetSumOfWeights() << '\n';

        std::cout << "New entries: "
                << h_sectorE_v_inv_cos->GetEntries() << '\n';

        std::cout << "New sum weights: "
                << h_sectorE_v_inv_cos->GetSumOfWeights() << '\n';

        TCanvas *c2 = new TCanvas("c2", "Sector Energy vs 1/cos(theta)", 800, 600);
        h_sectorE_v_inv_cos->Draw("COLZ");
        h_sectorE_v_inv_cos->GetYaxis()->SetRangeUser(4500,6500);
        c2->SetLogz();
        c2->SaveAs(Form("Sectors/%s/%d/SectorEVInvCos.pdf", calibrationRun.c_str(), sector));

        // Fit a linear function in the form of E = Mean alpha energy - m * (1/cos(theta))) to each of the alpha energies
        // Pu239 alphas
        TF1 *PuFit = new TF1("PuFit", "[0] + [1]*x", 1, 1.535);
        h_sectorE_v_inv_cos->GetYaxis()->SetRangeUser(5156 - 150, 5156 + 150);
        PuFit->SetParameters(5156, -100); // initial guesses for the parameters
        //PuFit->SetParLimits(0, 5145, 5165); // Set limits for the mean energy parameter
        h_sectorE_v_inv_cos->Fit(PuFit, "R");
        PuFit->SetLineColor(kRed);
        h_sectorE_v_inv_cos->GetYaxis()->SetRangeUser(4500,6500);
        c2->Update();
        // Am241 alphas
        TF1 *AmFit = new TF1("AmFit", "[0] + [1]*x", 1, 1.535);
        h_sectorE_v_inv_cos->GetYaxis()->SetRangeUser(5486 - 150, 5486 + 150);
        AmFit->SetParameters(5486, -100); // initial guesses for the parameters
        //AmFit->SetParLimits(0, 5475, 5495); // Set limits for the mean energy parameter
        h_sectorE_v_inv_cos->Fit(AmFit, "R+"); // R+ keeps the previous fits
        AmFit->SetLineColor(kRed);
        h_sectorE_v_inv_cos->GetYaxis()->SetRangeUser(4500,6500);
        c2->Update();
        // Cm244 alphas
        TF1 *CmFit = new TF1("CmFit", "[0] + [1]*x", 1, 1.535);
        h_sectorE_v_inv_cos->GetYaxis()->SetRangeUser(5804 - 150, 5804 + 150);
        CmFit->SetParameters(5804, -100); // initial guesses for the parameters
        //CmFit->SetParLimits(0, 5795, 5815); // Set limits for the mean energy parameter
        h_sectorE_v_inv_cos->Fit(CmFit, "R+"); // R+ keeps the previous fits
        CmFit->SetLineColor(kRed);
        h_sectorE_v_inv_cos->GetYaxis()->SetRangeUser(4500,6500);
        c2->Update();

        c2->SaveAs(Form("Sectors/%s/%d/FittedAlphaCurves.pdf", calibrationRun.c_str(), sector));

    
        // ------------------------------------------------------------
        // Remove the dead-layer slope using the selected alpha line.
        //
        // Fit has the form:
        //     E_measured = Fit(0) + Fit(1)*x
        //
        // where x = 1/cos(theta).
        //
        // We remove only the angle-dependent component of the energy loss,
        // preserving the alpha-energy differences between Pu, Am and Cm.
        //
        // At x = 1 (Theta lab = 180 degrees), no correction is applied -> tritons are not detected as this is along the beam line.
        // ------------------------------------------------------------
        
        double Pu_intercept = PuFit->GetParameter(0);
        Pu_slope     = PuFit->GetParameter(1);
        double Am_intercept = AmFit->GetParameter(0);
        Am_slope     = AmFit->GetParameter(1);
        double Cm_intercept = CmFit->GetParameter(0);
        Cm_slope     = CmFit->GetParameter(1);
                
        std::cout << "\nPu fit parameters:" << std::endl;
        std::cout << "  Intercept = " << Pu_intercept << " keV" << std::endl;
        std::cout << "  Slope     = " << Pu_slope << " keV per (1/cos(theta))" << std::endl;
        std::cout << "\nAm fit parameters:" << std::endl;
        std::cout << "  Intercept = " << Am_intercept << " keV" << std::endl;
        std::cout << "  Slope     = " << Am_slope << " keV per (1/cos(theta))" << std::endl;
        std::cout << "\nCm fit parameters:" << std::endl;
        std::cout << "  Intercept = " << Cm_intercept << " keV" << std::endl;
        std::cout << "  Slope     = " << Cm_slope << " keV per (1/cos(theta))" << std::endl;

        // Create a fresh histogram rather than cloning/filling the original.
        // This avoids retaining the old contents and then filling them again.
        TH2F *h_sectorE_v_inv_cos_deadlayer_removed = new TH2F("h_sectorE_v_inv_cos_deadlayer_removed",
                    Form("Sector %d Energy vs 1/cos(#theta), dead layer corrected;"
                    "1/cos(#theta); Sector Energy (50 keV bins)", sector),
                    24,
                    xbins,
                    200,
                    0,
                    10000);

        // Loop over the original histogram.
        for (int i = 1; i <= h_sectorE_v_inv_cos->GetNbinsX(); i++) {

            double inv_cos = h_sectorE_v_inv_cos->GetXaxis()->GetBinCenter(i);

            // Dead-layer energy loss relative to x = 1.
            //
            // For the fitted alpha line:
            //     E = E0 + m*x
            //
            // the change in energy between x and x=1 is:
            //     DeltaE = m*(x - 1)
            //
            // Since the fitted slope should be negative, this is the
            // measured energy loss and subtracting it corrects the energy.
            
            double deadlayer_correction = Pu_slope * (inv_cos - 1.0);
            //double deadlayer_correction = Am_slope * (inv_cos - 1.0);
            //double deadlayer_correction = Cm_slope * (inv_cos - 1.0);

            for (int j = 1; j <= h_sectorE_v_inv_cos->GetNbinsY(); j++) {

                double energy = h_sectorE_v_inv_cos->GetYaxis()->GetBinCenter(j);
                double counts = h_sectorE_v_inv_cos->GetBinContent(i, j);

                if (counts <= 0)
                    continue;

                // Remove the angle-dependent dead-layer contribution.
                double corrected_energy = energy - deadlayer_correction;

                // Keep the same x position and event counts, but move the
                // energy to the corrected value.
                h_sectorE_v_inv_cos_deadlayer_removed->Fill(
                    inv_cos,
                    corrected_energy,
                    counts
                );
            }
        }


        // ------------------------------------------------------------
        // Plot the corrected spectrum
        // ------------------------------------------------------------

        TCanvas *c3 = new TCanvas(
            "c3",
            "Sector energy vs 1/cos(theta), dead layer effect removed",
            800,
            600
        );
        h_sectorE_v_inv_cos_deadlayer_removed->GetYaxis()->SetRangeUser(4500, 6500);
        h_sectorE_v_inv_cos_deadlayer_removed->Draw("COLZ");
        c3->SetLogz();
        c3->Update();
        c3->SaveAs(Form("Sectors/%s/%d/SectorEVInvCosDeadLayerRemoved.pdf", calibrationRun.c_str(), sector));
    }
    
    S3DeadLayerCalc(Pu_slope, sector);
    //S3DeadLayerCalc(Am_slope, sector);
    //S3DeadLayerCalc(Cm_slope, sector);
}

int main(int argc, char ** argv) {

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <Calibration_Run> <sector_number>" << std::endl;
        return 1;
    }

    std::string calibrationRun = argv[1];
    int sector = std::atoi(argv[2]); // atoi is ASCII to integer (converts the character argument to an integer)
    std::cout << "Beginning deadlayer calculation for sector " << sector << std::endl;

    std::string grsi_path = getenv("GRSISYS"); // Finds the GRSISYS path to be used by other parts of the grsisort code
  	if(grsi_path.length() > 0){
	  grsi_path += "/";
 	}
  	// Read in grsirc in the GRSISYS directory to set user defined options on grsisort startup
	grsi_path += ".grsirc";
  	gEnv->ReadFile(grsi_path.c_str(), kEnvChange);

    S3DeadLayer(calibrationRun, sector);

    return 0;
}