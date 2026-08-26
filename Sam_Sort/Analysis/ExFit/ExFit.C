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
    std::vector<double> populated_states = {5.334, 
                                            7.362, 7.4, 7.982, 8.155};
    double ex_res = 0.125; // MeV
    
    // ----------------------------------------------------------------------------
    // Fit range definition
    // ----------------------------------------------------------------------------
    // Only want to the fit the excitation energy spectrum in the region of defined populated states.
    // So +- 3 sigma of highest and lowest populated state
    double fit_min = populated_states.front() - 3.0 * ex_res;
    double fit_max = populated_states.back()  + 3.0 * ex_res;

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

    double sigma_min = ex_res - 0.050;  // ex_res - 50 keV
    double sigma_max = ex_res + 0.050;  // ex_res + 50 keV

    for (int i = 0; i < nStates; ++i) {
        // --------------------------------------------------------
        // Amplitude
        // --------------------------------------------------------
        int bin = h_excite->FindBin(populated_states[i]);
        double counts = h_excite->GetBinContent(bin);

        fitFunc->SetParameter(i, counts); // Initial amplitude = bin content at the fixed mean
        fitFunc->SetParLimits(            // Allow amplitude to vary by +/- 20%
            i,
            0.80 * counts,
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
        kCyan + 1
    };

    for (size_t i = 0; i < populated_states.size(); ++i) {

        TF1 *component = new TF1(
            Form("component_%zu", i),
            Form("[%zu]*exp(-0.5*((x-%f)/%f)^2)",
                i,
                populated_states[i],
                ex_res),
            fit_min,
            fit_max
        );

        component->SetParameter(i, fitFunc->GetParameter(i));

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

}