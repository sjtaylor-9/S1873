// FitAlphaPeaks.C for the Dec 2019 triple alpha calibration data
void FitAlphaPeaks() {
    gStyle->SetOptFit(1);
    
    // Open input ROOT file containing the triple alpha source spectra
    TFile *triple_alpha_spectra = TFile::Open("S3_triple_alpha_calibration_plots.root");
    if(!triple_alpha_spectra) {
        std::cout << "Error opening input ROOT file\n";
        return;
    }

    // Enter TripleAlphaPlots TDirectory
    TDirectory *triple_alpha_peaks_dir = (TDirectory*)triple_alpha_spectra->Get("TripleAlphaPlots");
    if (!triple_alpha_peaks_dir) {
        std::cout << "TripleAlphaPlots directory not found\n";
        return;
    }

    TH1 *alpha_peaks_before = (TH1*)triple_alpha_peaks_dir->Get("h_s3energy_before");
    if (!alpha_peaks_before) {
        std::cout << "Histogram for triple alpha source energies not found\n";
        return;
    }
    TH1 *alpha_peaks_after = (TH1*)triple_alpha_peaks_dir->Get("h_s3energy_after");
    if (!alpha_peaks_after) {
        std::cout << "Histogram for triple alpha source energies not found\n";
        return;
    }
    
//     // -------------------------- 239Pu -------------------------- //
//     // Create fit function for 239Pu alpha source (before calibration)
//     TF1 *Pu_before_gaussian = new TF1("Pu_before_gaussian", "gaus", 5132, 5162);

//     Pu_before_gaussian->SetParameters(
//         alpha_peaks_before->GetMaximum(), // amplitude
//         5145,                             // mean
//         14.6                               // sigma
//     );

//     alpha_peaks_before->Fit(Pu_before_gaussian, "R0N");
//     Pu_before_gaussian->SetRange(5085, 5200);  // extend for display

//     TCanvas *c1 = new TCanvas("c1", "Gaussian fit of 239Pu alpha energy (before calibration)", 800, 600);
//     alpha_peaks_before->GetXaxis()->SetRangeUser(5050, 5230);
//     alpha_peaks_before->Draw();
//     Pu_before_gaussian->Draw("same");

//     TLegend *leg = new TLegend(0.78, 0.6, 0.98, 0.9);
//     leg->AddEntry(alpha_peaks_before, "Data", "l");
//     leg->AddEntry(Pu_before_gaussian, "Gaussian Fit", "l");
//     leg->AddEntry((TObject*)0,
//         Form("Mean = %.2f", Pu_before_gaussian->GetParameter(1)), "");
//     leg->AddEntry((TObject*)0,
//         Form("Sigma = %.2f", Pu_before_gaussian->GetParameter(2)), "");
//     leg->Draw();

//     c1->SaveAs("239Pu_before_gaussian_fit.pdf");

//     // Create fit function for 239Pu alpha source (after calibration)
//     TF1 *Pu_after_gaussian = new TF1("Pu_after_gaussian", "gaus", 5155, 5190);

//     Pu_after_gaussian->SetParameters(
//         alpha_peaks_after->GetMaximum(), // amplitude
//         5170,                             // mean
//         14                               // sigma
//     );

//     alpha_peaks_after->Fit(Pu_after_gaussian, "R0N");
//     Pu_after_gaussian->SetRange(5100, 5235);  // extend for display

//     TCanvas *c2 = new TCanvas("c2", "Gaussian fit of 239Pu alpha energy (after calibration)", 800, 600);
//     alpha_peaks_after->GetXaxis()->SetRangeUser(5050, 5260);
//     alpha_peaks_after->Draw();
//     Pu_after_gaussian->Draw("same");

//     TLegend *leg2 = new TLegend(0.78, 0.6, 0.98, 0.9);
//     leg2->AddEntry(alpha_peaks_after, "Data", "l");
//     leg2->AddEntry(Pu_after_gaussian, "Gaussian Fit", "l");
//     leg2->AddEntry((TObject*)0,
//         Form("Mean = %.2f", Pu_after_gaussian->GetParameter(1)), "");
//     leg2->AddEntry((TObject*)0,
//         Form("Sigma = %.2f", Pu_after_gaussian->GetParameter(2)), "");
//     leg2->Draw();


//     c2->SaveAs("239Pu_after_gaussian_fit.pdf");
//    // ----------------------------------------------------------- //



    // // -------------------------- 241Am -------------------------- //
    // // Create fit function for 241Am alpha source (before calibration)
    // TF1 *Am_before_gaussian = new TF1("Am_before_gaussian", "gaus", 5460, 5490);

    // Am_before_gaussian->SetParameters(
    //     alpha_peaks_before->GetMaximum(), // amplitude
    //     5476,                             // mean
    //     14                               // sigma
    // );

    // alpha_peaks_before->Fit(Am_before_gaussian, "R0N");
    // Am_before_gaussian->SetRange(5415, 5530);  // extend for display

    // TCanvas *c3 = new TCanvas("c3", "Gaussian fit of 241Am alpha energy (before calibration)", 800, 600);
    // alpha_peaks_before->GetXaxis()->SetRangeUser(5350, 5600);
    // alpha_peaks_before->Draw();
    // Am_before_gaussian->Draw("same");

    // TLegend *leg3 = new TLegend(0.78, 0.6, 0.98, 0.9);
    // leg3->AddEntry(alpha_peaks_before, "Data", "l");
    // leg3->AddEntry(Am_before_gaussian, "Gaussian Fit", "l");
    // leg3->AddEntry((TObject*)0,
    //     Form("Mean = %.2f", Am_before_gaussian->GetParameter(1)), "");
    // leg3->AddEntry((TObject*)0,
    //     Form("Sigma = %.2f", Am_before_gaussian->GetParameter(2)), "");
    // leg3->Draw();


    // c3->SaveAs("241Am_before_gaussian_fit.pdf");

    // // Create fit function for 241Am alpha source (after calibration)
    // TF1 *Am_after_gaussian = new TF1("Am_after_gaussian", "gaus", 5470, 5510);

    // Am_after_gaussian->SetParameters(
    //     alpha_peaks_after->GetMaximum(), // amplitude
    //     5486,                             // mean
    //     14                               // sigma
    // );

    // alpha_peaks_after->Fit(Am_after_gaussian, "R0N");
    // Am_after_gaussian->SetRange(5430, 5570);  // extend for display

    // TCanvas *c4 = new TCanvas("c4", "Gaussian fit of 241Am alpha energy (after calibration)", 800, 600);
    // alpha_peaks_after->GetXaxis()->SetRangeUser(5350, 5600);
    // alpha_peaks_after->Draw();
    // Am_after_gaussian->Draw("same");

    // TLegend *leg4 = new TLegend(0.78, 0.6, 0.98, 0.9);
    // leg4->AddEntry(alpha_peaks_after, "Data", "l");
    // leg4->AddEntry(Am_after_gaussian, "Gaussian Fit", "l");
    // leg4->AddEntry((TObject*)0,
    //     Form("Mean = %.2f", Am_after_gaussian->GetParameter(1)), "");
    // leg4->AddEntry((TObject*)0,
    //     Form("Sigma = %.2f", Am_after_gaussian->GetParameter(2)), "");
    // leg4->Draw();

    // c4->SaveAs("241Am_after_gaussian_fit.pdf");
    // // ----------------------------------------------------------- //



    // -------------------------- 244Cm -------------------------- //
    // Create fit function for 244Cm alpha source (before calibration)
    TF1 *Cm_before_gaussian = new TF1("Cm_before_gaussian", "gaus", 5780, 5800);

    Cm_before_gaussian->SetParameters(
        alpha_peaks_before->GetMaximum(), // amplitude
        5790,                             // mean
        14                               // sigma
    );

    alpha_peaks_before->Fit(Cm_before_gaussian, "R0N");
    Cm_before_gaussian->SetRange(5730, 5850);  // extend for display

    TCanvas *c5 = new TCanvas("c5", "Gaussian fit of 244Cm alpha energy (before calibration)", 800, 600);
    alpha_peaks_before->GetXaxis()->SetRangeUser(5650, 5900);
    alpha_peaks_before->Draw();
    Cm_before_gaussian->Draw("same");

    TLegend *leg5 = new TLegend(0.78, 0.6, 0.98, 0.9);
    leg5->AddEntry(alpha_peaks_before, "Data", "l");
    leg5->AddEntry(Cm_before_gaussian, "Gaussian Fit", "l");
    leg5->AddEntry((TObject*)0,
        Form("Mean = %.2f", Cm_before_gaussian->GetParameter(1)), "");
    leg5->AddEntry((TObject*)0,
        Form("Sigma = %.2f", Cm_before_gaussian->GetParameter(2)), "");
    leg5->Draw();

    c5->SaveAs("244Cm_before_gaussian_fit.pdf");

    // Create fit function for 244Cm alpha source (after calibration)
    TF1 *Cm_after_gaussian = new TF1("Cm_after_gaussian", "gaus", 5795, 5845);

    Cm_after_gaussian->SetParameters(
        alpha_peaks_after->GetMaximum(), // amplitude
        5802,                             // mean
        14                               // sigma
    );

    alpha_peaks_after->Fit(Cm_after_gaussian, "R0N");
    Cm_after_gaussian->SetRange(5750, 5865);  // extend for display

    TCanvas *c6 = new TCanvas("c6", "Gaussian fit of 244Cm alpha energy (after calibration)", 800, 600);
    alpha_peaks_after->GetXaxis()->SetRangeUser(5650, 5900);
    alpha_peaks_after->Draw();
    Cm_after_gaussian->Draw("same");

    TLegend *leg6 = new TLegend(0.78, 0.6, 0.98, 0.9);
    leg6->AddEntry(alpha_peaks_after, "Data", "l");
    leg6->AddEntry(Cm_after_gaussian, "Gaussian Fit", "l");
    leg6->AddEntry((TObject*)0,
        Form("Mean = %.2f", Cm_after_gaussian->GetParameter(1)), "");
    leg6->AddEntry((TObject*)0,
        Form("Sigma = %.2f", Cm_after_gaussian->GetParameter(2)), "");
    leg6->Draw();

    c6->SaveAs("244Cm_after_gaussian_fit.pdf");
    // ----------------------------------------------------------- //


}