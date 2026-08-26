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
#include "TSpectrum.h"
//#include <bits/stdc++.h>

using namespace std;


void s3Zoffset(){
	TFile *file1=TFile::Open("../HitMaps/Zoffset/HitMap_EZ_52829-52928.root");
	//TFile *file1=TFile::Open("../../../pterodon/matt/HitMaps/HitMap_EZ_Target1.root");

	
	TDirectory *dir1 = (TDirectory*)file1->Get("ExcitePhi_Iterations");

	ofstream outfile1;
	outfile1.open ("ZoffsetOutputs/Ex1745keV.dat");
	ofstream outfile2;
	outfile2.open ("ZoffsetOutputs/Ex2866keV.dat");
	ofstream outfile3;
	outfile3.open ("ZoffsetOutputs/SumDiff.dat");
	ofstream outfile4;
	outfile4.open ("ZoffsetOutputs/Ex1745keV_E0.dat");
	ofstream outfile5;
	outfile5.open ("ZoffsetOutputs/Ex2866keV_E0.dat");

	// t is the number of Z-offset iterations. Z is allowed to float between -1.0 mm and 1.0 mm in steps of 0.1 mm so 21 possible Z offsets. 
	const int t = 21;
	double z[t];
	double Ex1745keV_Diff[t];
	double Ex1745keV_Diff_Error[t];
	double Ex2866keV_Diff[t];
	double Ex2866keV_Diff_Error[t];
	double dummy[t] = {0};

	// k is the number of beam energy iterations. EBeam_Iter is allowed to float between EBeam - 0.5 MeV -> EBeam + 0.5 MeV; therefore, there are 11 possible beam energies.
	const int k = 11;
	double Eb[k];
	double E0_1745keV[k];
	double E0_2866keV[k];
	double E0_1745keV_error[k];
	double E0_2866keV_error[k];
	double dummy2[k] = {0};

	double SumDiff[t];
	double SumDiffError[t];

	for (int m=0; m<k; m++) { // Iterates through each beam energy (m is the beam energy index)
		for (int n=0; n<t; n++) { // Iterates through each Z offset at a given beam energy (n is the Z offset index)

			z[n] = n*0.1 - 1.0; // set z var
			Eb[m] = m*0.1 - 0.5; // set beam energy offset
			
			cout << "Z-offset (mm) " << z[n] << ", " << "Deviation from nominal beam energy (MeV) " << Eb[m] << endl;

			char hname[256]; // hist name
			sprintf(hname,"ExcitePhi_iter_Z_%f_Eb_%f",z[n],Eb[m]); // Builds histogram name using the values of z and Eb for the specific iteration
	
			TCanvas *c1 = new TCanvas("c1");
			c1->SetTicky();
			c1->SetTickx();
			TH2F *h_exc_phi = (TH2F*) dir1->Get(hname); // Extracts the Ex vs phi spectrum for the given z and Eb
			// This sums the entries in phi (x) bins 1-5 for a given excitation energy (y) and projects this total number of counts onto the y axis of the new TH1 (h_Exc) histogam. The x axis in h_Exc is Ex, taken from the y axis of the TH2 h_exc_phi 
			TH1F *hExc = (TH1F*)h_exc_phi->ProjectionY("h_phi_v_excite_gated",1,5);
			hExc->SetLineColor(1);
			// Draws the Ex energy spectrum for in sectors 1-5
			hExc->Draw();
			hExc->SetTitle(hname);
			hExc->GetYaxis()->SetTitle("Counts [100 keV/bin]");
			hExc->GetXaxis()->SetTitle("Excitation Energy [MeV]");
			hExc->GetXaxis()->SetRangeUser(0.8,8.6); // Only plots the spectrum in the range 1 < Ex < 8.6 MeV

			// Uses TSpectrum to search for the peaks in the Ex spectrum.
			TSpectrum *s = new TSpectrum();
			// 0.15 is the smoothing sigma. This tells TSpectrum Search what the expected peak width is (i.e. 150 keV). This helps to prevent TSpectrum Search from becoming sensitive to statisitical noise, i.e. if a smaller value was used it may pick up very narrow peaks that are just random fluctuations.
			// 0.03 is the threshold. This is the minimum height a peak must have relative to the tallest peak (given as a percentage, i.e. 3%). This also helps prevent TSpectrum Search from being sensitive to background/noise.
   			Int_t nfound = s->Search(hExc,0.15,"",0.03);
			printf("Found %d candidate peaks to fit\n",nfound);

			// This requires TSpectrum Search to find exactly 8 peaks, otherwise the iteration is skipped.
			if (nfound != 8) { continue; }

			Int_t npeaks = 0;
			double par[25] = {0};

			Double_t *xpeaks;
   			xpeaks = s->GetPositionX(); // Retrieves the X position (the energy in MeV) of the peaks found by TSpectrum Search

   			for (int p=0;p<nfound;p++) { // Sets the length of the loop to be the number of peaks found in the energy range.
      				Double_t xp = xpeaks[p]; // Uses the loop index to select the peak of interest (through its energy) in the loop iteration  
      				Int_t bin = hExc->GetXaxis()->FindBin(xp); // Finds the x bin that contains the examined peak
      				Double_t yp = hExc->GetBinContent(bin); // Retrieves the number of counts in the x bin
      				// Sets the initial guesses for a Gaussian fit to the peak
					// There are 8 peaks in the energy range; therefore, 8 Gaussian fits.
					// Each Gaussian fit has 3 parameters: amplitude, mean. and sigma.
					// Therefore par array has 24 elements.
					// par[0], par[1], par[2] -> peak 1 height, peak 1 mean, peak 1 sigma.
					// par[3], par[4], par[5] -> peak 2 height, peak 2 mean, peak 2 sigma, etc. 
					par[3*npeaks] = yp; // "height"
      				par[3*npeaks+1] = xp; // "mean"
      				par[3*npeaks+2] = 0.15; // "sigma"
      				npeaks++;
   			}

			// Creates a fit function of 8 Gaussians and a polynomial (pol0 = flat) background in the set energy range (0.9 -> 8.5 MeV)
			TF1 *total = new TF1("total","gaus(0)+gaus(3)+gaus(6)+gaus(9)+gaus(12)+gaus(15)+gaus(18)+gaus(21)+pol0(24)",0.9,8.5);
   			total->SetParameters(par); // Use the parameters on the sum.
			// This is used to make the fit function look smoother/more continuous.
			//Sets the sample rate of the fit function. Fit range is 8.5 - 1.2 = 7.3 MeV. Setting sample rate to be 1000 means, root plots the fit function every 7.3/1000 = 0.0073 MeV.
			total->SetNpx(1000);
   			hExc->Fit(total,"R"); // Fits the fit function to the data
			
			char c1outputfilename[256];
			sprintf(c1outputfilename, "ZoffsetOutputs/Ex/%.2f/Ex_Z_%.2f_Eb_%.2f.pdf",Eb[m],z[n],Eb[m]);
			c1->SaveAs(c1outputfilename);

			const int a = 8;
			const int b = 2;
			double lvl[a][b]; // 2D array with 8 rows and 2 columns -> stores the fitted energy and its associated uncertainty for all 8 peaks

			for (int i=0; i<nfound; i++) { // loops through all 8 peaks with index i
				lvl[i][0] = total->GetParameter(3*i + 1)*1000.; // assigns row i column 0 the mean of Gaussian i in keV
				lvl[i][1] = total->GetParError(3*i + 1)*1000.; // assigns row i column 1 the error associated with the mean of Gaussian i in keV
			}

			std::qsort(lvl, 8, sizeof(*lvl), // sorts the rows of the lvl array by energy
        		[](const void *arg1, const void *arg2)->int
        		{
            			double const *lhs = static_cast<double const*>(arg1);
            			double const *rhs = static_cast<double const*>(arg2);
            			return (lhs[0] < rhs[0]) ? -1
                		:  ((rhs[0] < lhs[0]) ? 1
                		:  (lhs[1] < rhs[1] ? -1
                		:  ((rhs[1] < lhs[1] ? 1 : 0))));
        		});

			double true_lvl[8] = {1745.91, 2866.6, 0., 0., 0., 0., 0., 0.}; // defines the reference excitation energies (NNDC)

			delete total;
			delete hExc;
			delete h_exc_phi;
			delete c1;
			delete s;
			
			// Calculates the difference between fitted energy and NNDC energy for the energy levels.
			// The energy uncertainty associated is the fitted energy uncertainty.
			Ex1745keV_Diff[n] = lvl[0][0] - true_lvl[0]; 
			Ex1745keV_Diff_Error[n] = lvl[0][1];
			Ex2866keV_Diff[n] = lvl[1][0] - true_lvl[1];
			Ex2866keV_Diff_Error[n] = lvl[1][1];
			
			// Computes the total energy offset by summing the absolute values of the individual energy differences for all levels.
			// The associated error in the offset is calculated by summing the individual uncertainties in quadrature.
			SumDiff[n] = abs(Ex2866keV_Diff[n]) + abs(Ex1745keV_Diff[n]);
			SumDiffError[n] = sqrt(pow(Ex1745keV_Diff_Error[n],2) + pow(Ex2866keV_Diff_Error[n],2));

			outfile1 << "Z-offset (mm) " << z[n] << " " << "Beam energy offset (MeV) " << Eb[m] << " " << "1745 keV energy offset (keV) " << Ex1745keV_Diff[n] << " " << "1745 keV energy sigma (keV) " << Ex1745keV_Diff_Error[n] << endl;
			outfile2 << "Z-offset (mm) " << z[n] << " " << "Beam energy offset (MeV) " << Eb[m] << " " << "2866 keV energy offset (keV) " << Ex2866keV_Diff[n] << " " << "2866 keV energy sigma (keV) " << Ex2866keV_Diff_Error[n] << endl;
			outfile3 << "Z-offset (mm) " << z[n] << " " << "Beam energy offset (MeV) " << Eb[m] << " " << "Total energy offset (keV) " << SumDiff[n] << " " << "Total energy offset uncertainty (keV) " << SumDiffError[n] << endl;

			cout << "Z-offset (mm) " << z[n] << " " << "Beam energy offset (MeV) " << Eb[m] << " " << "1745 keV fitted energy (keV) " << lvl[0][0] << " " << "1745 keV energy sigma (keV) " << Ex1745keV_Diff_Error[n] << endl;
			cout << "Z-offset (mm) " << z[n] << " " << "Beam energy offset (MeV) " << Eb[m] << " " << "2866 keV fitted energy (keV) " << lvl[1][0] << " " << "2866 keV energy sigma (keV) " << Ex2866keV_Diff_Error[n] << endl;
			
		} // end of Z offset loop


		// Plots Delta E against Z for the 1745 keV level: on the y axis is the energy offset between fitted and NNDC values and on the x axis is the Z offset
		TCanvas *c2 = new TCanvas("c2");
		TGraphErrors *g1 = new TGraphErrors(t,z,Ex1745keV_Diff,dummy,Ex1745keV_Diff_Error);
		char c2title[256];
		sprintf(c2title, "E_{x} = 1745.91 keV, E_{b} = %.2f MeV", Eb[m]);
		g1->SetTitle(c2title);
		g1->GetXaxis()->SetTitle("#DeltaZ [mm]");
		g1->GetYaxis()->SetTitle("E_{x} Difference [keV]");
		g1->Draw("AP*");
		TF1 *f1 = new TF1("f1","pol1",-0.5,0.5); // Fits the data with a linear fit.
		g1->Fit("f1");

		Double_t intercept, slope, intercept_error, slope_error;

		// Extracts the linear fit fit parameters.
		intercept = f1->GetParameter(0);
		slope = f1->GetParameter(1);
		intercept_error = f1->GetParError(0);
		slope_error = f1->GetParError(1);

		E0_1745keV[m] = -1*(intercept/slope); // This extreacts the Z offset at which Delta E = 0.
		E0_1745keV_error[m] = abs(E0_1745keV[m]*sqrt(pow((intercept_error/intercept),2) + pow((slope_error/slope),2))); // Propagates the uncertainty on Z.

		outfile4 << "Beam energy offset (MeV) " << Eb[m] << " " << "Z(Delta E=0) (mm) " << E0_1745keV[m] << " " << "Sigma Z(Delta E=0) (mm) " << E0_1745keV_error[m] << endl;
		cout << "Beam energy offset (MeV) " << Eb[m] << " " << "Z(Delta E=0) (mm) " << E0_1745keV[m] << " " << "Sigma Z(Delta E=0) (mm) " << E0_1745keV_error[m] << endl;

		// Plots Delta E against Z for the 2866 keV level.
		TCanvas *c3 = new TCanvas("c3");
		TGraphErrors *g2 = new TGraphErrors(t,z,Ex2866keV_Diff,dummy,Ex2866keV_Diff_Error);
		char c3title[256];
		sprintf(c3title, "E_{x} = 2866.6 keV, E_{b} = %.2f MeV", Eb[m]);
		g2->SetTitle(c3title);
		g2->GetXaxis()->SetTitle("#DeltaZ [mm]");
		g2->GetYaxis()->SetTitle("E_{x} Difference [keV]");
		g2->Draw("AP*");

		TF1 *f2 = new TF1("f2","pol1",-0.5,0.5);
		g2->Fit("f2");

		Double_t intercept_2, slope_2, intercept_error_2, slope_error_2;

		intercept_2 = f2->GetParameter(0);
		slope_2 = f2->GetParameter(1);
		intercept_error_2 = f2->GetParError(0);
		slope_error_2 = f2->GetParError(1);

		E0_2866keV[m] = -1*(intercept_2/slope_2);
		E0_2866keV_error[m] = abs(E0_2866keV[m]*sqrt(pow((intercept_error_2/intercept_2),2) + pow((slope_error_2/slope_2),2)));

		outfile5 << "Beam energy offset (MeV) " << Eb[m] << " " << "Z(Delta E=0) (mm) " << E0_2866keV[m] << " " << "Sigma Z(Delta E=0) (mm) " << E0_2866keV_error[m] << endl;
		cout << "Beam energy offset (MeV) " << Eb[m] << " " << "Z(Delta E=0) (mm) " << E0_2866keV[m] << " " << "Sigma Z(Delta E=0) (mm) " << E0_2866keV_error[m] << endl;
		
		char c2outputfilename[256];
		sprintf(c2outputfilename, "ZoffsetOutputs/ZVsEdiff/Ex1745_ZVSEdiff_Eb_%.2f.pdf", Eb[m]);
		c2->SaveAs(c2outputfilename);
		char c3outputfilename[256];
		sprintf(c3outputfilename, "ZoffsetOutputs/ZVsEdiff/Ex2866_ZVSEdiff_Eb_%.2f.pdf", Eb[m]);
		c3->SaveAs(c3outputfilename);
		
		delete c2;
		delete g1;
		delete f1;
		delete c3;
		delete g2;
		delete f2;

	} // end of beam energy loop


	// This section attempts to find the overall optimal Delta Z and Delta Eb.
	
	// Finds the maximum and minimum y axis values acrosss both data sets
	double ymin = 1e9;
	double ymax = -1e9;
	for (int i=0; i<k; i++) {
		if (E0_1745keV[i] - E0_1745keV_error[i] < ymin)
			ymin = E0_1745keV[i] - E0_1745keV_error[i];
		if (E0_1745keV[i] + E0_1745keV_error[i] > ymax)
			ymax = E0_1745keV[i] + E0_1745keV_error[i];
		if (E0_2866keV[i] - E0_2866keV_error[i] < ymin)
			ymin = E0_2866keV[i] - E0_2866keV_error[i];
		if (E0_2866keV[i] + E0_2866keV_error[i] > ymax)
			ymax = E0_2866keV[i] + E0_2866keV_error[i];
	}

	// Plots the optimal Z offset on the y axis as a function of the beam energy offset at which it was calculated at (for the 1745 keV peak).
	TCanvas *c4 = new TCanvas("c4");
	TGraphErrors *g4 = new TGraphErrors(k,Eb,E0_1745keV,dummy2,E0_1745keV_error);
	g4->SetMarkerStyle(20);
	g4->SetTitle("Optimal #DeltaZ as a function of #DeltaE_{b} for E_{x} = 1745.9, 2866.6 keV");
	g4->GetYaxis()->SetTitle("Optimal #DeltaZ [mm]");
	g4->GetXaxis()->SetTitle("#DeltaE [MeV]");
	double padding = 0.1*(ymax - ymin);
	g4->SetMinimum(ymin - padding);
	g4->SetMaximum(ymax + padding);
	g4->Draw("AP");
	TF1 *f4 = new TF1("f4","pol2",-0.5,0.5); // Fits a quadratic to the data.
	f4->SetLineColor(kBlack);
	g4->Fit(f4,"R");
	
	// Same plot as above but for the 2866 keV peak.
	TGraphErrors *g5 = new TGraphErrors(k,Eb,E0_2866keV,dummy2,E0_2866keV_error);
	g5->SetMarkerStyle(21);
	g5->SetMarkerColor(2);
	g5->Draw("Psame");
	TF1 *f5 = new TF1("f5","pol2",-0.5,0.5);
	g5->Fit(f5,"R");

	TLegend *leg = new TLegend(0.60,0.70,0.88,0.88);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->AddEntry(f4, "1745.9 keV level", "l");
	leg->AddEntry(f5, "2866.6 keV level", "l");
	leg->Draw();

	// Finds the optimal beam energy offset by finding the point of intersection of the two fit lines (neglects second order terms)
	// Quadratic fit expression: DeltaZ = par[0] + par[1] * DeltaE + par[2] * DeltaE
	Double_t Optimum_Eb = (f5->GetParameter(0) - f4->GetParameter(0)) / (f4->GetParameter(1) - f5->GetParameter(1));
	Double_t Optimum_Z = f5->GetParameter(1) * Optimum_Eb + f5->GetParameter(0);

	cout << "Optimum delta Z = " << Optimum_Z << endl;
	cout << "Optimum delta E = " << Optimum_Eb << endl; // Optimum_E_1 returns the same value as Optimum_E_0, as expected

	c4->SaveAs("ZoffsetOutputs/DeltaEVsDeltaZ.pdf");
}
