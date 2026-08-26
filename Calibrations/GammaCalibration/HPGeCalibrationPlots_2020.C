#include <TF1.h>
#include <TMath.h>

void HPGeCalibrationPlots_2020() {
	gStyle->SetOptStat(0);

	// ---------------------------- Co60 ------------------------------ //
	//TFile *co60_before_cal_file = TFile::Open("../../HistFiles/WithBeforeCal/Hist_52777_000.root");
	TFile *co60_before_cal_file = TFile::Open("../../HistFiles/WithAfterCal/Hist_52777_000.root");
	if(!co60_before_cal_file) {
        std::cout << "Error opening before calibration file \n";
        return;
    }
	//TFile *co60_after_cal_file = TFile::Open("../../HistFiles/WithBeforeCal/Hist_52932_000.root");
    TFile *co60_after_cal_file = TFile::Open("../../HistFiles/WithAfterCal/Hist_52932_000.root");
	if(!co60_after_cal_file) {
        std::cout << "Error opening after calibration file \n";
        return;
    }
	
	// Retrieves tig TTrees inside input files
    TTree *co60_tig_before_cal = (TTree*)co60_before_cal_file->Get("tig");
	if (!co60_tig_before_cal || co60_tig_before_cal->GetEntries() == 0) {
    	std::cout << "TIGRESS tree not found or is empty in the before calibration file\n";
    	return;
	}
	TTree *co60_tig_after_cal = (TTree*)co60_after_cal_file->Get("tig");
	if (!co60_tig_after_cal || co60_tig_after_cal->GetEntries() == 0) {
    	std::cout << "TIGRESS tree not found or is empty in the after calibration file\n";
    	return;
	} 		
	
	TCanvas *c1 = new TCanvas("Co60");
	c1->SetLogy();

	TH1D *h_co60_before_rawE = new TH1D("h_co60_before_rawE","^{60}Co Spectrum for Nov 2020 before and after calibration runs",10000,0,10000);
	h_co60_before_rawE->SetLineColor(kBlue);
	co60_tig_before_cal->Draw("gamma.rawE>>h_co60_before_rawE","", "goff");
	h_co60_before_rawE->GetYaxis()->SetRangeUser(10,1100000);
	h_co60_before_rawE->GetYaxis()->SetTitle("Counts [1 keV/bin]");
	h_co60_before_rawE->GetXaxis()->SetRangeUser(0,2900);
	h_co60_before_rawE->GetXaxis()->SetTitle("Energy [keV]");

	TH1D *h_co60_after_rawE = new TH1D("h_co60_after_rawE","^{60}Co Spectrum for Nov 2020 before and after calibration runs",10000,0,10000);
	h_co60_after_rawE->SetLineColor(kRed);
	co60_tig_after_cal->Draw("gamma.rawE>>h_co60_after_rawE","","goff");

	// Draw manually
	h_co60_before_rawE->Draw("hist");
	h_co60_after_rawE->Draw("hist same");

	TLegend *legend = new TLegend(0.72,0.78,0.90,0.88);
   	legend->AddEntry(h_co60_before_rawE,"Before (Run 52777)","l");
   	legend->AddEntry(h_co60_after_rawE,"After (Run 52932)","l");
   	legend->Draw();
	
	c1->Update();
	//c1->SaveAs("SamCalibrationFile/Nov2020/WithBeforeCal/Co60.root");
	c1->SaveAs("SamCalibrationFile/Nov2020/WithAfterCal/Co60.root");

	TCanvas *c4 = new TCanvas("c4","Co60 1173 keV Peak Fits");
	c4->SetLogy();

	// Clone histograms so the original display is untouched
	TH1D *h_co60_before_zoom = (TH1D*)h_co60_before_rawE->Clone("h_co60_before_zoom");
	TH1D *h_co60_after_zoom  = (TH1D*)h_co60_after_rawE->Clone("h_co60_after_zoom");
	h_co60_before_zoom->GetXaxis()->SetRangeUser(1140,1210);
	h_co60_before_zoom->GetYaxis()->SetRangeUser(10,10000000);
	h_co60_before_zoom->SetTitle("^{60}Co 1173 keV Peak");
	h_co60_before_zoom->GetXaxis()->SetTitle("Energy [keV]");
	h_co60_before_zoom->GetYaxis()->SetTitle("Counts [1 keV/bin]");
	h_co60_before_zoom->Draw("hist");
	h_co60_after_zoom->Draw("hist same");

	TF1 *co60_peak_fit_before = new TF1("co60_peak_fit_before","gaus(0)+pol1(3)",1140,1210);
	co60_peak_fit_before->SetLineColor(kBlue);
	co60_peak_fit_before->SetParameters(
		900000, // amplitude
		1173,  // centroid
		1.5,     // sigma
		120000,   // background constant
		-100      // background slope
	);
	co60_peak_fit_before->SetParLimits(0, 1e4, 1e8);     // amplitude
	co60_peak_fit_before->SetParLimits(1, 1170, 1190);   // centroid
	co60_peak_fit_before->SetParLimits(2, 0.5, 10.0);     // sigma

	h_co60_before_zoom->Fit(co60_peak_fit_before, "R");

	TF1 *co60_peak_fit_after = new TF1("co60_peak_fit_after","gaus(0)+pol1(3)",1140,1210);
	co60_peak_fit_after->SetLineColor(kRed);
	co60_peak_fit_after->SetParameters(
		900000,
		1173.0,
		1.5,
		120000,
		-100
	);
	co60_peak_fit_after->SetParLimits(0, 1e4, 1e8);
	co60_peak_fit_after->SetParLimits(1, 1170, 1190);
	co60_peak_fit_after->SetParLimits(2, 0.5, 10.0);

	h_co60_after_zoom->Fit(co60_peak_fit_after, "R+");

	double mean_co60_before  = co60_peak_fit_before->GetParameter(1);
	double sigma_co60_before = co60_peak_fit_before->GetParameter(2);
	double mean_co60_after  = co60_peak_fit_after->GetParameter(1);
	double sigma_co60_after = co60_peak_fit_after->GetParameter(2);

	TLegend *leg4 = new TLegend(0.12,0.70,0.40,0.88);
	leg4->AddEntry(h_co60_before_zoom,"Before (Run 52777)","l");
	leg4->AddEntry((TObject*)0,Form("#mu = %.3f keV", mean_co60_before),"");
	leg4->AddEntry((TObject*)0,Form("#sigma = %.3f keV", sigma_co60_before),"");
	leg4->AddEntry(h_co60_after_zoom,"After (Run 52932)","l");
	leg4->AddEntry((TObject*)0,Form("#mu = %.3f keV", mean_co60_after),"");
	leg4->AddEntry((TObject*)0,Form("#sigma = %.3f keV", sigma_co60_after),"");
	leg4->Draw();

	h_co60_before_zoom->Draw("hist");
	h_co60_after_zoom->Draw("hist same");
	co60_peak_fit_before->Draw("same");
	co60_peak_fit_after->Draw("same");
	leg4->Draw();

	c4->Modified();
	c4->Update();
	//c4->SaveAs("SamCalibrationFile/Nov2020/WithBeforeCal/Co60_1173Fit.pdf");
	c4->SaveAs("SamCalibrationFile/Nov2020/WithAfterCal/Co60_1173Fit.pdf");
	// ---------------------------------------------------------------- //


	// ---------------------------- Co56 ------------------------------ //
	//TFile *co56_before_cal_file = TFile::Open("../../HistFiles/WithBeforeCal/Hist_52781_000.root");
	TFile *co56_before_cal_file = TFile::Open("../../HistFiles/WithAfterCal/Hist_52781_000.root");
	if(!co56_before_cal_file) {
        std::cout << "Error opening before calibration file \n";
        return;
    }
	//TFile *co56_after_cal_file = TFile::Open("../../HistFiles/WithBeforeCal/Sum_52937.root");
    TFile *co56_after_cal_file = TFile::Open("../../HistFiles/WithAfterCal/Sum_52937.root");
	if(!co56_after_cal_file) {
        std::cout << "Error opening after calibration file \n";
        return;
    }
	
	// Retrieves tig TTrees inside input files
    TTree *co56_tig_before_cal = (TTree*)co56_before_cal_file->Get("tig");
	if (!co56_tig_before_cal || co56_tig_before_cal->GetEntries() == 0) {
    	std::cout << "TIGRESS tree not found or is empty in the before calibration file\n";
    	return;
	}
	TTree *co56_tig_after_cal = (TTree*)co56_after_cal_file->Get("tig");
	if (!co56_tig_after_cal || co56_tig_after_cal->GetEntries() == 0) {
    	std::cout << "TIGRESS tree not found or is empty in the after calibration file\n";
    	return;
	} 		
	
	TCanvas *c2 = new TCanvas("co56");
	c2->SetLogy();

	TH1D *h_co56_before_rawE = new TH1D("h_co56_before_rawE","^{56}Co Spectrum for Nov 2020 before and after calibration runs",10000,0,10000);
	h_co56_before_rawE->SetLineColor(kBlue);
	co56_tig_before_cal->Draw("gamma.rawE>>h_co56_before_rawE","", "goff");
	h_co56_before_rawE->GetYaxis()->SetRangeUser(10,1100000);
	h_co56_before_rawE->GetYaxis()->SetTitle("Counts [1 keV/bin]");
	h_co56_before_rawE->GetXaxis()->SetRangeUser(0,2900);
	h_co56_before_rawE->GetXaxis()->SetTitle("Energy [keV]");

	TH1D *h_co56_after_rawE = new TH1D("h_co56_after_rawE","^{56}Co Spectrum for Nov 2020 before and after calibration runs",10000,0,10000);
	h_co56_after_rawE->SetLineColor(kRed);
	co56_tig_after_cal->Draw("gamma.rawE>>h_co56_after_rawE","","goff");

	// Draw manually
	h_co56_before_rawE->Draw("hist");
	h_co56_after_rawE->Draw("hist same");

	TLegend *legend2 = new TLegend(0.72,0.78,0.90,0.88);
   	legend2->AddEntry(h_co56_before_rawE,"Before (Run 52781)","l");
   	legend2->AddEntry(h_co56_after_rawE,"After (Run 52937)","l");
   	legend2->Draw();
	
	c2->Update();
	//c2->SaveAs("SamCalibrationFile/Nov2020/WithBeforeCal/Co56.root");
	c2->SaveAs("SamCalibrationFile/Nov2020/WithAfterCal/Co56.root");


	TCanvas *c5 = new TCanvas("c5","Co56 1238 keV Peak Fits");
	c5->SetLogy();

	// Clone histograms so the original display is untouched
	TH1D *h_co56_before_zoom = (TH1D*)h_co56_before_rawE->Clone("h_co56_before_zoom");
	TH1D *h_co56_after_zoom  = (TH1D*)h_co56_after_rawE->Clone("h_co56_after_zoom");
	h_co56_before_zoom->GetXaxis()->SetRangeUser(1220,1270);
	h_co56_before_zoom->GetYaxis()->SetRangeUser(10,10000000);
	h_co56_before_zoom->SetTitle("^{56}Co 1238 keV Peak");
	h_co56_before_zoom->GetXaxis()->SetTitle("Energy [keV]");
	h_co56_before_zoom->GetYaxis()->SetTitle("Counts [1 keV/bin]");
	h_co56_before_zoom->Draw("hist");
	h_co56_after_zoom->Draw("hist same");

	TF1 *co56_peak_fit_before = new TF1("co56_peak_fit_before","gaus(0)+pol1(3)",1220,1270);
	co56_peak_fit_before->SetLineColor(kBlue);
	co56_peak_fit_before->SetParameters(
		900000, // amplitude
		1238,  // centroid
		1.5,     // sigma
		120000,   // background constant
		-100      // background slope
	);
	co56_peak_fit_before->SetParLimits(0, 1e4, 1e8);     // amplitude
	co56_peak_fit_before->SetParLimits(1, 1235, 1252);   // centroid
	co56_peak_fit_before->SetParLimits(2, 0.5, 10.0);     // sigma

	h_co56_before_zoom->Fit(co56_peak_fit_before, "R");

	TF1 *co56_peak_fit_after = new TF1("co56_peak_fit_after","gaus(0)+pol1(3)",1220,1270 );
	co56_peak_fit_after->SetLineColor(kRed);
	co56_peak_fit_after->SetParameters(
		900000,
		1238.0,
		1.5,
		120000,
		-100
	);
	co56_peak_fit_after->SetParLimits(0, 1e4, 1e8);
	co56_peak_fit_after->SetParLimits(1, 1235, 1252);
	co56_peak_fit_after->SetParLimits(2, 0.5, 10.0);

	h_co56_after_zoom->Fit(co56_peak_fit_after, "R+");

	double mean_co56_before  = co56_peak_fit_before->GetParameter(1);
	double sigma_co56_before = co56_peak_fit_before->GetParameter(2);
	double mean_co56_after  = co56_peak_fit_after->GetParameter(1);
	double sigma_co56_after = co56_peak_fit_after->GetParameter(2);

	TLegend *leg5 = new TLegend(0.60,0.15,0.88,0.40);
	leg5->AddEntry(h_co56_before_zoom,"Before (Run 52781)","l");
	leg5->AddEntry((TObject*)0,Form("#mu = %.3f keV", mean_co56_before),"");
	leg5->AddEntry((TObject*)0,Form("#sigma = %.3f keV", sigma_co56_before),"");
	leg5->AddEntry(h_co56_after_zoom,"After (Run 52937)","l");
	leg5->AddEntry((TObject*)0,Form("#mu = %.3f keV", mean_co56_after),"");
	leg5->AddEntry((TObject*)0,Form("#sigma = %.3f keV", sigma_co56_after),"");
	leg5->Draw();

	h_co56_before_zoom->Draw("hist");
	h_co56_after_zoom->Draw("hist same");
	co56_peak_fit_before->Draw("same");
	co56_peak_fit_after->Draw("same");
	leg5->Draw();

	c5->Modified();
	c5->Update();
	//c5->SaveAs("SamCalibrationFile/Nov2020/WithBeforeCal/Co56_1238Fit.pdf");
	c5->SaveAs("SamCalibrationFile/Nov2020/WithAfterCal/Co56_1238Fit.pdf");
	// ---------------------------------------------------------------- //
	
	
	// --------------------------- Eu152 ------------------------------ //
	//TFile *eu152_before_cal_file = TFile::Open("../../HistFiles/WithBeforeCal/Sum_52779.root");
	TFile *eu152_before_cal_file = TFile::Open("../../HistFiles/WithAfterCal/Sum_52779.root");
	if(!eu152_before_cal_file) {
        std::cout << "Error opening before calibration file \n";
        return;
    }
	//TFile *eu152_after_cal_file = TFile::Open("../../HistFiles/WithBeforeCal/Hist_52933_000.root");
    TFile *eu152_after_cal_file = TFile::Open("../../HistFiles/WithAfterCal/Hist_52933_000.root");
	if(!eu152_after_cal_file) {
        std::cout << "Error opening after calibration file \n";
        return;
    }
	
	// Retrieves tig TTrees inside input files
    TTree *eu152_tig_before_cal = (TTree*)eu152_before_cal_file->Get("tig");
	if (!eu152_tig_before_cal || eu152_tig_before_cal->GetEntries() == 0) {
    	std::cout << "TIGRESS tree not found or is empty in the before calibration file\n";
    	return;
	}
	TTree *eu152_tig_after_cal = (TTree*)eu152_after_cal_file->Get("tig");
	if (!eu152_tig_after_cal || eu152_tig_after_cal->GetEntries() == 0) {
    	std::cout << "TIGRESS tree not found or is empty in the after calibration file\n";
    	return;
	} 		
	
	TCanvas *c3 = new TCanvas("eu152");
	c3->SetLogy();

	TH1D *h_eu152_before_rawE = new TH1D("h_eu152_before_rawE","^{152}Eu Spectrum for Nov 2020 before and after calibration runs",10000,0,10000);
	h_eu152_before_rawE->SetLineColor(kBlue);
	eu152_tig_before_cal->Draw("gamma.rawE>>h_eu152_before_rawE","", "goff");
	h_eu152_before_rawE->GetYaxis()->SetRangeUser(10,1100000);
	h_eu152_before_rawE->GetYaxis()->SetTitle("Counts [1 keV/bin]");
	h_eu152_before_rawE->GetXaxis()->SetRangeUser(0,2900);
	h_eu152_before_rawE->GetXaxis()->SetTitle("Energy [keV]");

	TH1D *h_eu152_after_rawE = new TH1D("h_eu152_after_rawE","^{152}Eu Spectrum for Nov 2020 before and after calibration runs",10000,0,10000);
	h_eu152_after_rawE->SetLineColor(kRed);
	eu152_tig_after_cal->Draw("gamma.rawE>>h_eu152_after_rawE","","goff");

	// Draw manually
	h_eu152_before_rawE->Draw("hist");
	h_eu152_after_rawE->Draw("hist same");

	TLegend *legend3 = new TLegend(0.72,0.78,0.90,0.88);
   	legend3->AddEntry(h_eu152_before_rawE,"Before (Run 52779)","l");
   	legend3->AddEntry(h_eu152_after_rawE,"After (Run 52933)","l");
   	legend3->Draw();
	
	c3->Update();
	//c3->SaveAs("SamCalibrationFile/Nov2020/WithBeforeCal/Eu152.root");
	c3->SaveAs("SamCalibrationFile/Nov2020/WithAfterCal/Eu152.root");


	TCanvas *c6 = new TCanvas("c5","Eu152 345 keV Peak Fits");
	c6->SetLogy();

	// Clone histograms so the original display is untouched
	TH1D *h_eu152_before_zoom = (TH1D*)h_eu152_before_rawE->Clone("h_eu152_before_zoom");
	TH1D *h_eu152_after_zoom  = (TH1D*)h_eu152_after_rawE->Clone("h_eu152_after_zoom");
	h_eu152_before_zoom->GetXaxis()->SetRangeUser(330,360);
	h_eu152_before_zoom->GetYaxis()->SetRangeUser(10,10000000);
	h_eu152_before_zoom->SetTitle("^{152}Eu 344 keV Peak");
	h_eu152_before_zoom->GetXaxis()->SetTitle("Energy [keV]");
	h_eu152_before_zoom->GetYaxis()->SetTitle("Counts [1 keV/bin]");
	h_eu152_before_zoom->Draw("hist");
	h_eu152_after_zoom->Draw("hist same");

	TF1 *eu152_peak_fit_before = new TF1("eu152_peak_fit_before","gaus(0)+pol1(3)",330,360);
	eu152_peak_fit_before->SetLineColor(kBlue);
	eu152_peak_fit_before->SetParameters(
		900000, // amplitude
		344,  // centroid
		1.5,     // sigma
		120000,   // background constant
		-100      // background slope
	);
	eu152_peak_fit_before->SetParLimits(0, 1e4, 1e8);     // amplitude
	eu152_peak_fit_before->SetParLimits(1, 342, 358);   // centroid
	eu152_peak_fit_before->SetParLimits(2, 0.5, 10.0);     // sigma

	h_eu152_before_zoom->Fit(eu152_peak_fit_before, "R");

	TF1 *eu152_peak_fit_after = new TF1("eu152_peak_fit_after","gaus(0)+pol1(3)",330,360);
	eu152_peak_fit_after->SetLineColor(kRed);
	eu152_peak_fit_after->SetParameters(
		900000,
		344.0,
		1.5,
		120000,
		-100
	);
	eu152_peak_fit_after->SetParLimits(0, 1e4, 1e8);
	eu152_peak_fit_after->SetParLimits(1, 342, 358);
	eu152_peak_fit_after->SetParLimits(2, 0.5, 10.0);

	h_eu152_after_zoom->Fit(eu152_peak_fit_after, "R+");

	double mean_eu152_before  = eu152_peak_fit_before->GetParameter(1);
	double sigma_eu152_before = eu152_peak_fit_before->GetParameter(2);
	double mean_eu152_after  = eu152_peak_fit_after->GetParameter(1);
	double sigma_eu152_after = eu152_peak_fit_after->GetParameter(2);

	TLegend *leg6 = new TLegend(0.60,0.15,0.88,0.40);
	leg6->AddEntry(h_eu152_before_zoom,"Before (Run 52779)","l");
	leg6->AddEntry((TObject*)0,Form("#mu = %.3f keV", mean_eu152_before),"");
	leg6->AddEntry((TObject*)0,Form("#sigma = %.3f keV", sigma_eu152_before),"");
	leg6->AddEntry(h_eu152_after_zoom,"After (Run 52933)","l");
	leg6->AddEntry((TObject*)0,Form("#mu = %.3f keV", mean_eu152_after),"");
	leg6->AddEntry((TObject*)0,Form("#sigma = %.3f keV", sigma_eu152_after),"");
	leg6->Draw();

	h_eu152_before_zoom->Draw("hist");
	h_eu152_after_zoom->Draw("hist same");
	eu152_peak_fit_before->Draw("same");
	eu152_peak_fit_after->Draw("same");
	leg6->Draw();

	c6->Modified();
	c6->Update();
	//c6->SaveAs("SamCalibrationFile/Nov2020/WithBeforeCal/Eu152_344Fit.pdf");
	c6->SaveAs("SamCalibrationFile/Nov2020/WithAfterCal/Eu152_344Fit.pdf");
	// ---------------------------------------------------------------- //
}
