void PlotTritonElossEffects () {
    gStyle->SetOptStat(0);

    // 2020
    TFile *inputFile = TFile::Open("/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/HistFiles/Runs_52829-52928.root");
    // 2019
    //TFile *inputFile = TFile::Open("/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/HistFiles/Runs_52040-52164.root");

    if (!inputFile->IsOpen()) {
        std::cout << "Failed to open input ROOT file" << std::endl;
        return;
    }

    // --------------------- Effect of S3 dead layer correction --------------------- //
    TDirectory *dir = inputFile->GetDirectory("SamPlots");
    // Excitation energy effect
    TH1F *h_excite = (TH1F*)dir->Get("h_excite_pid_gated");
    TH1F *h_excite_DLcorr = (TH1F*)dir->Get("h_excite_pid_gated_DL_corr");

    if (!h_excite || !h_excite_DLcorr) {
        printf("Error: Could not find excitation energy histograms in SamPlots directory\n");
        return;
    }

    TCanvas *c1 = new TCanvas("c1", "Excitation energy with and without DL correction", 800, 600);
    h_excite_DLcorr->SetLineColor(kRed);
    h_excite->Draw("HIST");
    h_excite_DLcorr->Draw("HIST SAME");

    h_excite->GetXaxis()->SetTitle("Excitation energy [MeV]");
    h_excite->GetYaxis()->SetTitle("Counts / 10 keV");

    TLegend *leg = new TLegend(0.60, 0.72, 0.90, 0.88);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.035);
    leg->AddEntry(h_excite, "Uncorrected for DL", "l");
    leg->AddEntry(h_excite_DLcorr, "S3 DL Corrected", "l");
    leg->Draw();

    c1->SaveAs("Ex_DL_shift.pdf");

    // Triton energy effect
    TH1F *h_s3E = (TH1F*)dir->Get("h_s3_E");
    TH1F *h_s3E_DLcorr = (TH1F*)dir->Get("h_s3_E_DL_corr");

    if (!h_s3E || !h_s3E_DLcorr) {
        printf("Error: Could not find S3 energy histograms in SamPlots directory\n");
        return;
    }

    TCanvas *c2 = new TCanvas("c2", "S3 energy with and without DL correction", 800, 600);
    h_s3E_DLcorr->SetLineColor(kRed);
    h_s3E->Rebin(30);
    h_s3E_DLcorr->Rebin(30);
    h_s3E->Draw("HIST");
    h_s3E_DLcorr->Draw("HIST SAME");

    h_s3E->GetXaxis()->SetTitle("S3 triton energy [keV]");
    h_s3E->GetXaxis()->SetRangeUser(0,7000);
    h_s3E->GetYaxis()->SetTitle("Counts / 30 keV");

    TLegend *leg2 = new TLegend(0.60, 0.72, 0.90, 0.88);
    leg2->SetBorderSize(0);
    leg2->SetFillStyle(0);
    leg2->SetTextSize(0.035);
    leg2->AddEntry(h_s3E, "Uncorrected for DL", "l");
    leg2->AddEntry(h_s3E_DLcorr, "S3 DL Corrected", "l");
    leg2->Draw();

    c2->SaveAs("S3E_DL_shift.pdf");

    // ------------------------------------------------------------------------------ //
    
    // ------- Effect of S3 dead layer and triton Eloss in target corrections ------- //
    // Excitation energy effect
    TH1F *h_excite_DL_tLiF_corr = (TH1F*)dir->Get("h_excite_pid_gated_DL_tLiF_corr");
    if (!h_excite_DL_tLiF_corr) {
        printf("Error: Could not find excitation energy histogram corrected for DL thickness and triton energy loss in LiF in SamPlots directory\n");
        return;
    }

    TCanvas *c3 = new TCanvas("c1", "Excitation energy with DL correction and t Eloss in LiF", 800, 600);
    h_excite_DLcorr->SetLineColor(kRed);
    h_excite_DL_tLiF_corr->SetLineColor(kGreen);
    h_excite->Draw("HIST");
    h_excite_DLcorr->Draw("HIST SAME");
    h_excite_DL_tLiF_corr->Draw("HIST SAME");

    h_excite->GetXaxis()->SetTitle("Excitation energy [MeV]");
    h_excite->GetYaxis()->SetTitle("Counts / 10 keV");

    TLegend *leg3 = new TLegend(0.60, 0.72, 0.90, 0.88);
    leg3->SetBorderSize(0);
    leg3->SetFillStyle(0);
    leg3->SetTextSize(0.035);
    leg3->AddEntry(h_excite, "Uncorrected for DL", "l");
    leg3->AddEntry(h_excite_DLcorr, "S3 DL Corrected", "l");
    leg3->AddEntry(h_excite_DL_tLiF_corr, "S3 DL and t Eloss in LiF corrected", "l");
    leg3->Draw();

    c3->SaveAs("Ex_DL_tLiF_shift.root");

    // Triton energy effect
    TH1F *h_s3E_DL_tLiF_corr = (TH1F*)dir->Get("h_s3_E_DL_tLiF_corr");
    if (!h_s3E_DL_tLiF_corr) {
        printf("Error: Could not find the S3 energy histogram corrected for DL thickness and triton energy loss in LiF in SamPlots directory\n");
        return;
    }

    TCanvas *c4 = new TCanvas("c2", "S3 energy with DL correction and t Eloss in LiF", 800, 600);
    h_s3E_DLcorr->SetLineColor(kRed);
    h_s3E_DL_tLiF_corr->SetLineColor(kGreen);
    // Uncorrected and DL corrected were rebinned in canvas 2 so dont need to rebin here
    h_s3E_DL_tLiF_corr->Rebin(30);
    h_s3E->Draw("HIST");
    h_s3E_DLcorr->Draw("HIST SAME");
    h_s3E_DL_tLiF_corr->Draw("HIST SAME");

    h_s3E->GetXaxis()->SetTitle("S3 triton energy [keV]");
    h_s3E->GetXaxis()->SetRangeUser(0,7000);
    h_s3E->GetYaxis()->SetTitle("Counts / 30 keV");

    TLegend *leg4 = new TLegend(0.60, 0.72, 0.90, 0.88);
    leg4->SetBorderSize(0);
    leg4->SetFillStyle(0);
    leg4->SetTextSize(0.035);
    leg4->AddEntry(h_s3E, "Uncorrected for DL", "l");
    leg4->AddEntry(h_s3E_DLcorr, "S3 DL Corrected", "l");
    leg4->AddEntry(h_s3E_DL_tLiF_corr, "S3 DL and t Eloss in LiF Corrected", "l");
    leg4->Draw();

    c4->SaveAs("S3E_DL__tLiF_shift.root");    
    // ------------------------------------------------------------------------------ //
}