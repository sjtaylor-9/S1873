void FitGammaGatedEx() {

    gStyle->SetOptStat(0);      // Disable statistics box
    gStyle->SetOptFit(1111);    // Enable fit parameter box

    // Open the sorted ROOT file
    TFile *inputFile = TFile::Open("/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/HistFiles/Runs_52829-52928.root");
    TDirectory *dir = inputFile->GetDirectory("SamPlots");
    // TFile *inputFile = TFile::Open("/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/pterodon/cam/HistFiles/Runs_52828-52929.root");
    // TDirectory *dir = inputFile->GetDirectory("CamPlots");

    // Excitation energy with 4556 keV gamma gate
    TCanvas *c1 = new TCanvas("c1", "Excitation energy, 4556 keV gamma gated", 800, 600);
    TH1F *h_excite_PIdgated_4556keVgated = (TH1F*)dir->Get("h_excite_PIdgated_4556keVgated");
    h_excite_PIdgated_4556keVgated->Draw();
    h_excite_PIdgated_4556keVgated->Rebin(12);
    h_excite_PIdgated_4556keVgated->GetXaxis()->SetTitle("Excitation energy [MeV]");
	h_excite_PIdgated_4556keVgated->GetYaxis()->SetTitle("Counts / 120 keV");
    // Fitting the excitation energy
    h_excite_PIdgated_4556keVgated->GetXaxis()->SetRangeUser(4,12);
    TF1 *gaus_fit_4556 = new TF1("gaus_fit_4556", "gaus(0)+pol1(3)", 6.9, 7.7);
    int maxBin_4556 = h_excite_PIdgated_4556keVgated->GetMaximumBin();
    double amp_guess_4556 = h_excite_PIdgated_4556keVgated->GetBinContent(maxBin_4556);
    double mean_guess_4556 = h_excite_PIdgated_4556keVgated->GetBinCenter(maxBin_4556);
    gaus_fit_4556->SetParameter(0, amp_guess_4556); // height of the tallest bin
    gaus_fit_4556->SetParameter(1, mean_guess_4556); // energy of the tallest bin
    gaus_fit_4556->SetParameter(2, 0.025);  // initial guess of 25 keV for sigma     
	gaus_fit_4556->SetParLimits(1,7,7.6); // Set limits for the mean parameter to be between 7 and 7.6 MeV
	gaus_fit_4556->SetParLimits(2, 0.001, 0.400); // Set limits for the sigma parameter to be between 1 and 100 keV
    gaus_fit_4556->SetParName(0, "Gaus Amp");
    gaus_fit_4556->SetParName(1, "Mean");
    gaus_fit_4556->SetParName(2, "Sigma");
    gaus_fit_4556->SetParName(3, "Bgk Intercept");
    gaus_fit_4556->SetParName(4, "Bgk Slope");
    h_excite_PIdgated_4556keVgated->Fit(gaus_fit_4556, "R");
    gaus_fit_4556->SetLineColor(kRed);
    gaus_fit_4556->Draw("same");
    c1->SaveAs("FittedGammaGatedEx/Ex_4556keV_gamma_gated.pdf");
    c1->SaveAs("FittedGammaGatedEx/Ex_4556keV_gamma_gated.root");



    // Excitation energy with 6408 keV gamma gate
    TCanvas *c2 = new TCanvas("c2", "Excitation energy, 6408 keV gamma gated", 800, 600);
    TH1F *h_excite_PIdgated_6408keVgated = (TH1F*)dir->Get("h_excite_PIdgated_6408keVgated");
    h_excite_PIdgated_6408keVgated->Draw();
    h_excite_PIdgated_6408keVgated->Rebin(12);
    h_excite_PIdgated_6408keVgated->GetXaxis()->SetTitle("Excitation energy [MeV]");
	h_excite_PIdgated_6408keVgated->GetYaxis()->SetTitle("Counts / 120 keV");
    h_excite_PIdgated_6408keVgated->SetTitle("S3 Excitation Energy PID and 6408 keV gamma gated");
    // Fitting the excitation energy
    h_excite_PIdgated_6408keVgated->GetXaxis()->SetRangeUser(4,12);
    TF1 *gaus_fit_6408 = new TF1("gaus_fit_6408", "gaus(0)+pol1(3)", 7.5, 8.7);
    int maxBin_6408 = h_excite_PIdgated_6408keVgated->GetMaximumBin();
    double amp_guess_6408 = h_excite_PIdgated_6408keVgated->GetBinContent(maxBin_6408);
    double mean_guess_6408 = h_excite_PIdgated_6408keVgated->GetBinCenter(maxBin_6408);
    gaus_fit_6408->SetParameter(0, amp_guess_6408); // height of the tallest bin
    gaus_fit_6408->SetParameter(1, mean_guess_6408); // energy of the tallest bin
    gaus_fit_6408->SetParameter(2, 0.025);  // initial guess of 25 keV for sigma     
	gaus_fit_6408->SetParLimits(1,7.8,8.3); // Set limits for the mean parameter to be between 8 and 8.3 MeV
	gaus_fit_6408->SetParLimits(2, 0.001, 0.400); // Set limits for the sigma parameter to be between 1 and 100 keV
    gaus_fit_6408->SetParName(0, "Gaus Amp");
    gaus_fit_6408->SetParName(1, "Mean");
    gaus_fit_6408->SetParName(2, "Sigma");
    gaus_fit_6408->SetParName(3, "Bgk Intercept");
    gaus_fit_6408->SetParName(4, "Bgk Slope");
    h_excite_PIdgated_6408keVgated->Fit(gaus_fit_6408, "R");
    gaus_fit_6408->SetLineColor(kRed);
    gaus_fit_6408->Draw("same");
    c2->SaveAs("FittedGammaGatedEx/Ex_6408keV_gamma_gated.pdf");
    c2->SaveAs("FittedGammaGatedEx/Ex_6408keV_gamma_gated.root");



    // Excitation energy with 5902 keV gamma gate
    TCanvas *c3 = new TCanvas("c3", "Excitation energy, 5902 keV gamma gated", 800, 600);
    TH1F *h_excite_PIdgated_5902keVgated = (TH1F*)dir->Get("h_excite_PIdgated_5902keVgated");
    h_excite_PIdgated_5902keVgated->Draw();
    h_excite_PIdgated_5902keVgated->Rebin(12);
    h_excite_PIdgated_5902keVgated->GetXaxis()->SetTitle("Excitation energy [MeV]");
	h_excite_PIdgated_5902keVgated->GetYaxis()->SetTitle("Counts / 120 keV");
    h_excite_PIdgated_5902keVgated->SetTitle("S3 Excitation Energy PID and 5902 keV gamma gated");
    // Fitting the excitation energy
    h_excite_PIdgated_5902keVgated->GetXaxis()->SetRangeUser(4,12);
    // TF1 *gaus_fit_5902 = new TF1("gaus_fit_5902", "gaus(0)+pol1(3)", 7.5, 8.7);
    // int maxBin_5902 = h_excite_PIdgated_5902keVgated->GetMaximumBin();
    // double amp_guess_5902 = h_excite_PIdgated_5902keVgated->GetBinContent(maxBin_5902);
    // double mean_guess_5902 = h_excite_PIdgated_5902keVgated->GetBinCenter(maxBin_5902);
    // gaus_fit_5902->SetParameter(0, amp_guess_5902); // height of the tallest bin
    // gaus_fit_5902->SetParameter(1, mean_guess_5902); // energy of the tallest bin
    // gaus_fit_5902->SetParameter(2, 0.025);  // initial guess of 25 keV for sigma     
	// gaus_fit_5902->SetParLimits(1,7.8,8.3); // Set limits for the mean parameter to be between 8 and 8.3 MeV
	// gaus_fit_5902->SetParLimits(2, 0.001, 0.400); // Set limits for the sigma parameter to be between 1 and 100 keV
    // gaus_fit_5902->SetParName(0, "Gaus Amp");
    // gaus_fit_5902->SetParName(1, "Mean");
    // gaus_fit_5902->SetParName(2, "Sigma");
    // gaus_fit_5902->SetParName(3, "Bgk Intercept");
    // gaus_fit_5902->SetParName(4, "Bgk Slope");
    // h_excite_PIdgated_5902keVgated->Fit(gaus_fit_5902, "R");
    // gaus_fit_5902->SetLineColor(kRed);
    // gaus_fit_5902->Draw("same");
    c3->SaveAs("FittedGammaGatedEx/Ex_5902keV_gamma_gated.pdf");
    c3->SaveAs("FittedGammaGatedEx/Ex_5902keV_gamma_gated.root");

    
    // Excitation energy with 1120 keV gamma gate
    TCanvas *c4 = new TCanvas("c4", "Excitation energy, 1120 keV gamma gated", 800, 600);
    TH1F *h_excite_PIdgated_1120keVgated = (TH1F*)dir->Get("h_excite_PIdgated_1120keVgated");
    h_excite_PIdgated_1120keVgated->Draw();
    h_excite_PIdgated_1120keVgated->Rebin(12);
    h_excite_PIdgated_1120keVgated->GetXaxis()->SetTitle("Excitation energy [MeV]");
	h_excite_PIdgated_1120keVgated->GetYaxis()->SetTitle("Counts / 120 keV");
    // Fitting the excitation energy
    h_excite_PIdgated_1120keVgated->GetXaxis()->SetRangeUser(1,5);
    TF1 *gaus_fit_1120 = new TF1("gaus_fit_1120", "gaus(0)+pol1(3)", 2.0, 3.5);
    int maxBin_1120 = h_excite_PIdgated_1120keVgated->GetMaximumBin();
    double amp_guess_1120 = h_excite_PIdgated_1120keVgated->GetBinContent(maxBin_1120);
    double mean_guess_1120 = h_excite_PIdgated_1120keVgated->GetBinCenter(maxBin_1120);
    gaus_fit_1120->SetParameter(0, amp_guess_1120); // height of the tallest bin
    gaus_fit_1120->SetParameter(1, mean_guess_1120); // energy of the tallest bin
    gaus_fit_1120->SetParameter(2, 0.025);  // initial guess of 25 keV for sigma     
	gaus_fit_1120->SetParLimits(1,2,3); // Set limits for the mean parameter
	gaus_fit_1120->SetParLimits(2, 0.001, 0.400); // Set limits for the sigma parameter to be between 1 and 100 keV
    gaus_fit_1120->SetParName(0, "Gaus Amp");
    gaus_fit_1120->SetParName(1, "Mean");
    gaus_fit_1120->SetParName(2, "Sigma");
    gaus_fit_1120->SetParName(3, "Bgk Intercept");
    gaus_fit_1120->SetParName(4, "Bgk Slope");
    h_excite_PIdgated_1120keVgated->Fit(gaus_fit_1120, "R");
    gaus_fit_1120->SetLineColor(kRed);
    gaus_fit_1120->Draw("same");
    c4->SaveAs("FittedGammaGatedEx/Ex_1120keV_gamma_gated.pdf");
    c4->SaveAs("FittedGammaGatedEx/Ex_1120keV_gamma_gated.root");
}