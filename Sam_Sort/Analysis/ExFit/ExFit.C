#include <vector>
#include <iostream>
#include <cmath>

#include "TFile.h"
#include "TDirectory.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TMath.h"
#include "TString.h"

void ExFit() {
    gStyle->SetOptStat(0);

    // 2020
    TFile *inputFile = TFile::Open("/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/HistFiles/Runs_52829-52928.root");
    // 2019
    //TFile *inputFile = TFile::Open("/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/HistFiles/Runs_52040-52164.root");
    if (!inputFile->IsOpen()) {
        std::cout << "Failed to open input ROOT file" << std::endl;
        return;
    }
    
    TDirectory *dir = inputFile->GetDirectory("SamPlots");
    if (!dir) {
        std::cout << "Failed to open the TDirectory in the input ROOT file" << std::endl;
        return;
    }

    TH1F *h_excite = (TH1F*)dir->Get("h_excite_pid_gated");
    if (!h_excite) {
        printf("Error: Could not find histogram of excitation energy spectrum\n");
        return;
    }

    TCanvas *c1 = new TCanvas("Ex", "S3 excitation energy", 800, 600);
    h_excite->Draw();
    h_excite->GetXaxis()->SetTitle("Energy [MeV]");
	h_excite->Rebin(12);  // Rebin the excitation energy to bin widths comparable to the excitation energy resolution (~125 keV)
    h_excite->GetYaxis()->SetTitle("Counts / 120 keV");

    // ----------------------------------------------------------------------------
    // Populated states and experimental resolution in the excitation energy
    // ----------------------------------------------------------------------------
    std::vector<double> populated_states = {2.866, 3.735, 4.433, 5.334, 5.431, 6.033, 
                                            7.362, 7.42, 7.982, 8.155,
                                            //2.794, 4.526, 4.725, 5.549, 5.822, // States that I have not yet confirmed are populated but are seen Angus 2025
                                            //7.47, 7.559, 7.602, 7.619, 7.655, 7.74, 7.82, 7.96, 7.98, 8.009, 8.069, 8.146, 8.16 // States that I have not yet confirmed are populated but are seen in Hammache 2024
                                        };
    double ex_res = 0.125; // MeV
    
    // ----------------------------------------------------------------------------
    // Fit range definition
    // ----------------------------------------------------------------------------
    // Only want to the fit the excitation energy spectrum in the region of defined populated states.
    // So +- 3 sigma of highest and lowest populated state
    double fit_min = *std::min_element(populated_states.begin(), populated_states.end())
                    - 3.0 * ex_res;
    double fit_max = *std::max_element(populated_states.begin(), populated_states.end())
                    + 3.0 * ex_res;


    std::cout << "Fit range: "
              << fit_min << " - " << fit_max << " MeV"
              << std::endl;
    // ------------------------------------------------------------
    // Build sum of Gaussians
    // ------------------------------------------------------------
    TString formula = "";
    int nStates = populated_states.size();  // Number of states populated
    for (int i = 0; i < nStates; ++i) {
        if (i > 0)
            formula += " + ";
        formula += Form(
            "[%d]*exp(-0.5*((x-%f)/[%d])^2)",   // Total fit function summed across all individual Gaussians
            i,                                  // amplitude
            populated_states[i],                // mean is fixed to energy of the state (defined in populated_states)
            nStates + i                         // sigma
        );
    }

    std::cout << "Fit function:" << std::endl;
    std::cout << formula << std::endl;
    // ------------------------------------------------------------
    // Create fit function
    // ------------------------------------------------------------
    TF1 *fitFunc = new TF1(
        "fitFunc",
        formula.Data(),
        fit_min,            // Only fit to the lowest populated state -3 ex_res
        fit_max             // Only fit to highest populated state + 3 ex_res
    );
    // ------------------------------------------------------------
    // Initial parameters and limits
    // Allowing the sigma of each Gaussian to float between ex resolution +- 50 keV
    // The amplitude is the bin content at the mean energy of the state +- 20%
    // ------------------------------------------------------------

    double sigma_min = ex_res - 0.05;  // ex_res - 50 keV
    double sigma_max = ex_res + 0.05;  // ex_res + 50 keV

    for (int i = 0; i < nStates; ++i) {
        // Set fit parameter names
        fitFunc->SetParName(i, Form("Amp_%.3fMeV", populated_states[i]));
        fitFunc->SetParName(nStates + i, Form("Sigma_%.3fMeV", populated_states[i]));

        // --------------------------------------------------------
        // Amplitude
        // --------------------------------------------------------
        int bin = h_excite->FindBin(populated_states[i]);
        double counts = h_excite->GetBinContent(bin);

        fitFunc->SetParameter(i, counts); // Initial amplitude guess = bin content at the fixed mean
        fitFunc->SetParLimits(            
            i,
            0.5 * counts,
            1.20 * counts
        );
        // --------------------------------------------------------
        // Sigma
        // --------------------------------------------------------
        fitFunc->SetParameter(nStates + i, ex_res);   // Initial sigma = excitation-energy resolution
        fitFunc->SetParLimits(                        // Allow sigma to vary by +/- 50 keV
            nStates + i,
            sigma_min,
            sigma_max
        );
    }

    // ------------------------------------------------------------
    // Perform fit
    // ------------------------------------------------------------
    h_excite->Fit(fitFunc, "R");
    // ------------------------------------------------------------
    // Draw total fit
    // ------------------------------------------------------------
    fitFunc->SetLineColor(kRed);
    fitFunc->SetLineWidth(3);
    fitFunc->Draw("same");
    // ------------------------------------------------------------
    // Draw individual Gaussian components
    // ------------------------------------------------------------

    int colors[] = {
        kBlue,
        kGreen + 2,
        kMagenta,
        kOrange + 1,
        kCyan + 1,
        kSpring-7,
        kViolet,
        kYellow,
        // kBlack,
        // kBlack,
        // kBlack,
        // kBlack,
        // kBlack,
        // kGray+2,
        // kGray+2,
        // kGray+2,
        // kGray+2,
        // kGray+2,
        // kGray+2,
        // kGray+2,
        // kGray+2,
        // kGray+2,
        // kGray+2,
        // kGray+2,
        // kGray+2,
        // kGray+2,
    };

    for (size_t i = 0; i < populated_states.size(); ++i) {

        // Define TF1 using parameter indices [0] for amplitude and [1] for sigma
        TF1 *component = new TF1(
            Form("component_%zu", i),
            Form("[0]*exp(-0.5*((x-%f)/[1])^2)", populated_states[i]),
            fit_min,
            fit_max
        );

        // Set parameters directly from the master fit result
        component->SetParameter(0, fitFunc->GetParameter(i));           // Fitted Amplitude
        component->SetParameter(1, fitFunc->GetParameter(nStates + i)); // Fitted Sigma

        component->SetLineColor(colors[i]);
        component->SetLineWidth(2);
        component->SetLineStyle(2);

        component->Draw("same");
    }



    TLegend *leg = new TLegend(0.60, 0.60, 0.88, 0.88);

    leg->AddEntry(h_excite, "Excitation spectrum", "l");
    leg->AddEntry(fitFunc, "Total fit", "l");

    for (size_t i = 0; i < populated_states.size(); ++i) {
        leg->AddEntry(
            Form("component_%zu", i),
            Form("E_{x} = %.3f MeV", populated_states[i]),
            "l"
        );
    }

    leg->Draw();

        // Print fitted amplitudes
        std::cout << "\nFitted populations:\n";

        for (size_t i = 0; i < populated_states.size(); ++i) {

            std::cout << "State "
                    << populated_states[i]
                    << " MeV : "
                    << fitFunc->GetParameter(i)
                    << " +/- "
                    << fitFunc->GetParError(i)
                    << std::endl;
        }

        c1->SaveAs("FittedEx.pdf");
}