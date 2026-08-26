void FitGammas() {

    gStyle->SetOptStat(0);      // Disable statistics box
    gStyle->SetOptFit(1111);    // Enable fit parameter box

    // Open the sorted ROOT file
    TFile *inputFile = TFile::Open("/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/HistFiles/Runs_52829-52928.root");
    inputFile->cd("SamPlots");


    // Create a new canvas and draw excitation energy spectrum histogram on it
    TH1F* h_tigress_spec = new TH1F("h_tigress_spec", "Doppler corrected add-back gamma energies", 10000, 0, 10000);
    h_tigress_spec = (TH1F*)inputFile->Get("SamPlots/h_dopE_addback_gated");
    TCanvas *c1 = new TCanvas("c1", "Doppler corrected add-back gamma energies", 800, 600);
    h_tigress_spec->Draw();
    h_tigress_spec->GetXaxis()->SetTitle("Energy [keV]");
	h_tigress_spec->GetYaxis()->SetTitle("Counts / 1 keV");

    // ----------------------------------------------------------------------------
    // First fitting low-lying energy levels to check that the energy is properly calibrated and peaks are where expected
    // ----------------------------------------------------------------------------
    // 351 keV gamma - decay from first excited state to ground state
    h_tigress_spec->GetXaxis()->SetRangeUser(300, 400);
    TF1 *gaus_fit_351 = new TF1("gaus_fit_351", "gaus(0)+pol1(3)", 330, 370);
    int maxBin_351 = h_tigress_spec->GetMaximumBin();
    double amp_guess_351 = h_tigress_spec->GetBinContent(maxBin_351);
    double mean_guess_351 = h_tigress_spec->GetBinCenter(maxBin_351);
    gaus_fit_351->SetParameter(0, amp_guess_351); // height of the tallest bin
    gaus_fit_351->SetParameter(1, mean_guess_351); // energy of the tallest bin
    gaus_fit_351->SetParameter(2, 25);  // initial guess of 25 keV for sigma     
	gaus_fit_351->SetParLimits(1,345,355); // Set limits for the mean parameter to be between 345 and 355 keV
	gaus_fit_351->SetParLimits(2, 1, 100); // Set limits for the sigma parameter to be between 1 and 100 keV
    gaus_fit_351->SetParName(0, "Gaus Amp");
    gaus_fit_351->SetParName(1, "Mean");
    gaus_fit_351->SetParName(2, "Sigma");
    gaus_fit_351->SetParName(3, "Bgk Intercept");
    gaus_fit_351->SetParName(4, "Bgk Slope");
    h_tigress_spec->Fit(gaus_fit_351, "R");
    gaus_fit_351->SetLineColor(kRed);
    gaus_fit_351->Draw("same");
    h_tigress_spec->GetXaxis()->SetRangeUser(300, 400);
    gPad->Modified();
    gPad->Update();
    c1->SaveAs("FittedGammas/Doppler_corrected_351keV.pdf");
    c1->SaveAs("FittedGammas/Doppler_corrected_351keV.root");


    // 1745 keV gamma - decay from 2nd excited state to ground state
    h_tigress_spec->GetXaxis()->SetRangeUser(1660, 1840);
    TF1 *gaus_fit_1745 = new TF1("gaus_fit_1745", "gaus(0)+pol1(3)", 1700, 1800);
    int maxBin_1745 = h_tigress_spec->GetMaximumBin();
    double amp_guess_1745 = h_tigress_spec->GetBinContent(maxBin_1745);
    double mean_guess_1745 = h_tigress_spec->GetBinCenter(maxBin_1745);
    gaus_fit_1745->SetParameter(0, amp_guess_1745); // height of the tallest bin
    gaus_fit_1745->SetParameter(1, mean_guess_1745); // energy of the tallest bin
    gaus_fit_1745->SetParameter(2, 25);  // initial guess of 25 keV for sigma     
	gaus_fit_1745->SetParLimits(1,1740,1750); // Set limits for the mean parameter to be between 345 and 355 keV
	gaus_fit_1745->SetParLimits(2, 1, 100); // Set limits for the sigma parameter to be between 1 and 100 keV
    gaus_fit_1745->SetParName(0, "Gaus Amp");
    gaus_fit_1745->SetParName(1, "Mean");
    gaus_fit_1745->SetParName(2, "Sigma");
    gaus_fit_1745->SetParName(3, "Bgk Intercept");
    gaus_fit_1745->SetParName(4, "Bgk Slope");
    h_tigress_spec->Fit(gaus_fit_1745, "R");
    gaus_fit_1745->SetLineColor(kRed);
    gaus_fit_1745->Draw("same");
    h_tigress_spec->GetXaxis()->SetRangeUser(1660, 1840);
    gPad->Modified();
    gPad->Update();
    c1->SaveAs("FittedGammas/Doppler_corrected_1745keV.pdf");
    c1->SaveAs("FittedGammas/Doppler_corrected_1745keV.root");


    // 1395 keV gamma - uniquely identifies the 1745 keV state (decay from 1745 keV state to 351 keV state)
    h_tigress_spec->GetXaxis()->SetRangeUser(1320, 1480);
    TF1 *gaus_fit_1395 = new TF1("gaus_fit_1395", "gaus(0)+pol1(3)", 1340, 1460);
    int maxBin_1395 = h_tigress_spec->GetMaximumBin();
    double amp_guess_1395 = h_tigress_spec->GetBinContent(maxBin_1395);
    double mean_guess_1395 = h_tigress_spec->GetBinCenter(maxBin_1395);
    gaus_fit_1395->SetParameter(0, amp_guess_1395); // height of the tallest bin
    gaus_fit_1395->SetParameter(1, mean_guess_1395); // energy of the tallest bin
    gaus_fit_1395->SetParameter(2, 25);  // initial guess of 25 keV for sigma     
	gaus_fit_1395->SetParLimits(1,1390,1405); // Set limits for the mean parameter to be between 345 and 355 keV
	gaus_fit_1395->SetParLimits(2, 1, 100); // Set limits for the sigma parameter to be between 1 and 100 keV
    gaus_fit_1395->SetParName(0, "Gaus Amp");
    gaus_fit_1395->SetParName(1, "Mean");
    gaus_fit_1395->SetParName(2, "Sigma");
    gaus_fit_1395->SetParName(3, "Bgk Intercept");
    gaus_fit_1395->SetParName(4, "Bgk Slope");
    h_tigress_spec->Fit(gaus_fit_1395, "R");
    gaus_fit_1395->SetLineColor(kRed);
    gaus_fit_1395->Draw("same");
    gPad->Modified();
    gPad->Update();
    c1->SaveAs("FittedGammas/Doppler_corrected_1395keV.pdf");
    c1->SaveAs("FittedGammas/Doppler_corrected_1395keV.root");


    // 1120 keV gamma - uniquely identifies 2866 keV state (decay from 2866 keV state to 1745 keV state)
    h_tigress_spec->GetXaxis()->SetRangeUser(1050, 1200);
    TF1 *gaus_fit_1120 = new TF1("gaus_fit_1120", "gaus(0)+pol1(3)", 1070, 1170);
    int maxBin_1120 = h_tigress_spec->GetMaximumBin();
    double amp_guess_1120 = h_tigress_spec->GetBinContent(maxBin_1120);
    double mean_guess_1120 = h_tigress_spec->GetBinCenter(maxBin_1120);
    gaus_fit_1120->SetParameter(0, amp_guess_1120); // height of the tallest bin
    gaus_fit_1120->SetParameter(1, mean_guess_1120); // energy of the tallest bin
    gaus_fit_1120->SetParameter(2, 25);  // initial guess of 25 keV for sigma     
	gaus_fit_1120->SetParLimits(1,1110,1130); // Set limits for the mean parameter to be between 345 and 355 keV
	gaus_fit_1120->SetParLimits(2, 1, 100); // Set limits for the sigma parameter to be between 1 and 100 keV
    gaus_fit_1120->SetParName(0, "Gaus Amp");
    gaus_fit_1120->SetParName(1, "Mean");
    gaus_fit_1120->SetParName(2, "Sigma");
    gaus_fit_1120->SetParName(3, "Bgk Intercept");
    gaus_fit_1120->SetParName(4, "Bgk Slope");
    h_tigress_spec->Fit(gaus_fit_1120, "R");
    gaus_fit_1120->SetLineColor(kRed);
    gaus_fit_1120->Draw("same");
    gPad->Modified();
    gPad->Update();
    c1->SaveAs("FittedGammas/Doppler_corrected_1120keV.pdf");
    c1->SaveAs("FittedGammas/Doppler_corrected_1120keV.root");


    // 2516 keV gamma - uniquely identifies the 2866 keV state (decay from 2866 keV state to 351 keV state) 
    h_tigress_spec->GetXaxis()->SetRangeUser(2350, 2720);
    TF1 *gaus_fit_2516 = new TF1("gaus_fit_2516", "gaus(0)+pol1(3)", 2400, 2650);
    int maxBin_2516 = h_tigress_spec->GetMaximumBin();
    double amp_guess_2516 = h_tigress_spec->GetBinContent(maxBin_2516);
    double mean_guess_2516 = h_tigress_spec->GetBinCenter(maxBin_2516);
    gaus_fit_2516->SetParameter(0, amp_guess_2516); // height of the tallest bin
    gaus_fit_2516->SetParameter(1, mean_guess_2516); // energy of the tallest bin
    gaus_fit_2516->SetParameter(2, 25);  // initial guess of 25 keV for sigma     
	gaus_fit_2516->SetParLimits(1,2505,2525); // Set limits for the mean parameter to be between 345 and 355 keV
	gaus_fit_2516->SetParLimits(2, 1, 100); // Set limits for the sigma parameter to be between 1 and 100 keV
    gaus_fit_2516->SetParName(0, "Gaus Amp");
    gaus_fit_2516->SetParName(1, "Mean");
    gaus_fit_2516->SetParName(2, "Sigma");
    gaus_fit_2516->SetParName(3, "Bgk Intercept");
    gaus_fit_2516->SetParName(4, "Bgk Slope");
    h_tigress_spec->Fit(gaus_fit_2516, "R");
    gaus_fit_2516->SetLineColor(kRed);
    gaus_fit_2516->Draw("same");
    gPad->Modified();
    gPad->Update();
    c1->SaveAs("FittedGammas/Doppler_corrected_2516keV.pdf");
    c1->SaveAs("FittedGammas/Doppler_corrected_2516keV.root");
    // ----------------------------------------------------------------------------
    // ----------------------------------------------------------------------------


    // ----------------------------------------------------------------------------
    // Now fitting gammas from states near/in Gamow window
    // For higher energy gammas, the TIGRESS efficiency is much lower and they are less common so rebinning the data so that the photopeaks are easier to fit
    // ----------------------------------------------------------------------------
    h_tigress_spec->Rebin(4); // Rebin the histogram to have 4 keV bins
	h_tigress_spec->GetYaxis()->SetTitle("Counts / 4 keV");

    // 4556 keV gamma uniquely identifies the 7.42 MeV state
    // Fit the 4556 keV peak with a Gaussian function
    h_tigress_spec->GetXaxis()->SetRangeUser(4490, 4610);
    TF1 *gaus_fit_4556 = new TF1("gaus_fit_4556", "gaus(0)+pol1(3)", 4470, 4650);
    int maxBin_4556 = h_tigress_spec->GetMaximumBin();
    double amp_guess_4556 = h_tigress_spec->GetBinContent(maxBin_4556);
    double mean_guess_4556 = h_tigress_spec->GetBinCenter(maxBin_4556);
    gaus_fit_4556->SetParameter(0, amp_guess_4556); // height of the tallest bin
    gaus_fit_4556->SetParameter(1, mean_guess_4556); // energy of the tallest bin
    gaus_fit_4556->SetParameter(2, 25);  // initial guess of 25 keV for sigma     
	gaus_fit_4556->SetParLimits(1,4500,4600); // Set limits for the mean parameter to be between 4.5 and 4.6 MeV
	gaus_fit_4556->SetParLimits(2, 5, 100); // Set limits for the sigma parameter to be between 5 and 100 keV
    gaus_fit_4556->SetParName(0, "Gaus Amp");
    gaus_fit_4556->SetParName(1, "Mean");
    gaus_fit_4556->SetParName(2, "Sigma");
    gaus_fit_4556->SetParName(3, "Bgk Intercept");
    gaus_fit_4556->SetParName(4, "Bgk Slope");
    h_tigress_spec->Fit(gaus_fit_4556, "R");
    gaus_fit_4556->SetLineColor(kRed);
    gaus_fit_4556->Draw("same");
    h_tigress_spec->GetXaxis()->SetRangeUser(4400, 4700);
    gPad->Modified();
    gPad->Update();
    c1->SaveAs("FittedGammas/Doppler_corrected_4556keV.pdf");
    c1->SaveAs("FittedGammas/Doppler_corrected_4556keV.root");


    // 6408 keV gamma uniquely identifies the 8.155 MeV state
    // Fit the 6408 keV peak with a Gaussian function
    h_tigress_spec->GetXaxis()->SetRangeUser(6300, 6520);
    TF1 *gaus_fit_6408 = new TF1("gaus_fit_6408", "gaus(0)+pol1(3)", 6320, 6520);
    int maxBin_6408 = h_tigress_spec->GetMaximumBin();
    double amp_guess_6408 = h_tigress_spec->GetBinContent(maxBin_6408);
    double mean_guess_6408 = h_tigress_spec->GetBinCenter(maxBin_6408);
    gaus_fit_6408->SetParameter(0, amp_guess_6408); // height of the tallest bin
    gaus_fit_6408->SetParameter(1, mean_guess_6408); // energy of the tallest bin
    gaus_fit_6408->SetParameter(2, 25);  // initial guess of 25 keV for sigma     
	gaus_fit_6408->SetParLimits(1,6390,6420); // Set limits for the mean parameter to be between 6.39 and 6.42 MeV
	gaus_fit_6408->SetParLimits(2, 5, 100); // Set limits for the sigma parameter to be between 5 and 100 keV
    gaus_fit_6408->SetParName(0, "Gaus Amp");
    gaus_fit_6408->SetParName(1, "Mean");
    gaus_fit_6408->SetParName(2, "Sigma");
    gaus_fit_6408->SetParName(3, "Bgk Intercept");
    gaus_fit_6408->SetParName(4, "Bgk Slope");
    h_tigress_spec->Fit(gaus_fit_6408, "R");
    gaus_fit_6408->SetLineColor(kRed);
    gaus_fit_6408->Draw("same");
    h_tigress_spec->GetXaxis()->SetRangeUser(6300, 6520);
    gPad->Modified();
    gPad->Update();
    c1->SaveAs("FittedGammas/Doppler_corrected_6408keV.pdf");
    c1->SaveAs("FittedGammas/Doppler_corrected_6408keV.root");


    // 5902 keV gamma uniquely identifies the 7.648 MeV state
    // Fit the 5902 keV peak with a Gaussian function
    h_tigress_spec->GetXaxis()->SetRangeUser(5860, 5930);
    TF1 *gaus_fit_5902 = new TF1("gaus_fit_5902", "gaus(0)+pol1(3)", 5880, 5920);
    int maxBin_5902 = h_tigress_spec->GetMaximumBin();
    double amp_guess_5902 = h_tigress_spec->GetBinContent(maxBin_5902);
    double mean_guess_5902 = h_tigress_spec->GetBinCenter(maxBin_5902);
    gaus_fit_5902->SetParameter(0, amp_guess_5902); // height of the tallest bin
    gaus_fit_5902->SetParameter(1, mean_guess_5902); // energy of the tallest bin
    gaus_fit_5902->SetParameter(2, 25);  // initial guess of 25 keV for sigma     
	gaus_fit_5902->SetParLimits(1,5890,5910); // Set limits for the mean parameter to be between 6.39 and 6.42 MeV
	gaus_fit_5902->SetParLimits(2, 3, 100); // Set limits for the sigma parameter to be between 5 and 100 keV
    gaus_fit_5902->SetParName(0, "Gaus Amp");
    gaus_fit_5902->SetParName(1, "Mean");
    gaus_fit_5902->SetParName(2, "Sigma");
    gaus_fit_5902->SetParName(3, "Bgk Intercept");
    gaus_fit_5902->SetParName(4, "Bgk Slope");
    h_tigress_spec->Fit(gaus_fit_5902, "R");
    gaus_fit_5902->SetLineColor(kRed);
    gaus_fit_5902->Draw("same");
    h_tigress_spec->GetXaxis()->SetRangeUser(5860, 5930);
    gPad->Modified();
    gPad->Update();
    c1->SaveAs("FittedGammas/Doppler_corrected_5902keV.pdf");
    c1->SaveAs("FittedGammas/Doppler_corrected_5902keV.root");


    // 3550 keV gamma uniquely identifies the 7.982 MeV state
    // Fit the 3550 keV peak with a Gaussian function
    h_tigress_spec->GetXaxis()->SetRangeUser(3430, 3630);
    TF1 *gaus_fit_3550 = new TF1("gaus_fit_3550", "gaus(0)+pol1(3)", 3460, 3620);
    int maxBin_3550 = h_tigress_spec->GetMaximumBin();
    double amp_guess_3550 = h_tigress_spec->GetBinContent(maxBin_3550);
    double mean_guess_3550 = h_tigress_spec->GetBinCenter(maxBin_3550);
    gaus_fit_3550->SetParameter(0, amp_guess_3550); // height of the tallest bin
    gaus_fit_3550->SetParameter(1, mean_guess_3550); // energy of the tallest bin
    gaus_fit_3550->SetParameter(2, 25);  // initial guess of 25 keV for sigma     
	gaus_fit_3550->SetParLimits(1,3540,3560); // Set limits for the mean parameter to be between 6.39 and 6.42 MeV
	gaus_fit_3550->SetParLimits(2, 3, 100); // Set limits for the sigma parameter to be between 5 and 100 keV
    gaus_fit_3550->SetParName(0, "Gaus Amp");
    gaus_fit_3550->SetParName(1, "Mean");
    gaus_fit_3550->SetParName(2, "Sigma");
    gaus_fit_3550->SetParName(3, "Bgk Intercept");
    gaus_fit_3550->SetParName(4, "Bgk Slope");
    h_tigress_spec->Fit(gaus_fit_3550, "R");
    gaus_fit_3550->SetLineColor(kRed);
    gaus_fit_3550->Draw("same");
    h_tigress_spec->GetXaxis()->SetRangeUser(3430, 3630);
    gPad->Modified();
    gPad->Update();
    c1->SaveAs("FittedGammas/Doppler_corrected_3550keV.pdf");
    c1->SaveAs("FittedGammas/Doppler_corrected_3550keV.root");
    // ----------------------------------------------------------------------------
    // ----------------------------------------------------------------------------

    // ----------------------------------------------------------------------------
    // Now fitting gammas from states below alpha threshold to determine which lower states have populated -> needed for Ex fit
    // ----------------------------------------------------------------------------
    // 4984 keV gamma uniquely identifies the 5.354 MeV state
    // Fit the 4984 keV peak with a Gaussian function
    h_tigress_spec->GetXaxis()->SetRangeUser(4850, 5100);
    TF1 *gaus_fit_4984 = new TF1("gaus_fit_4984", "gaus(0)+pol1(3)", 4930, 5045);
    int maxBin_4984 = h_tigress_spec->GetMaximumBin();
    double amp_guess_4984 = h_tigress_spec->GetBinContent(maxBin_4984);
    double mean_guess_4984 = h_tigress_spec->GetBinCenter(maxBin_4984);
    gaus_fit_4984->SetParameter(0, amp_guess_4984); // height of the tallest bin
    gaus_fit_4984->SetParameter(1, mean_guess_4984); // energy of the tallest bin
    gaus_fit_4984->SetParameter(2, 25);  // initial guess of 25 keV for sigma     
	gaus_fit_4984->SetParLimits(1,4975,4995); // Set limits for the mean parameter
	gaus_fit_4984->SetParLimits(2, 3, 100); // Set limits for the sigma parameter
    gaus_fit_4984->SetParName(0, "Gaus Amp");
    gaus_fit_4984->SetParName(1, "Mean");
    gaus_fit_4984->SetParName(2, "Sigma");
    gaus_fit_4984->SetParName(3, "Bgk Intercept");
    gaus_fit_4984->SetParName(4, "Bgk Slope");
    h_tigress_spec->Fit(gaus_fit_4984, "R");
    gaus_fit_4984->SetLineColor(kRed);
    gaus_fit_4984->Draw("same");
    h_tigress_spec->GetXaxis()->SetRangeUser(4850, 5100);
    gPad->Modified();
    gPad->Update();
    c1->SaveAs("FittedGammas/Doppler_corrected_4984keV.pdf");
    c1->SaveAs("FittedGammas/Doppler_corrected_4984keV.root");

    
}