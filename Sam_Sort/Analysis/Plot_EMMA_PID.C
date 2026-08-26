void Plot_EMMA_PID() {
    TFile *inputFile = TFile::Open("/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/HistFiles/Runs_52829-52928.root");

    TCanvas *c1 = new TCanvas("c1", "EMMA singles PID", 800, 600);
    c1->SetRightMargin(0.18);
    TDirectory *dir = inputFile->GetDirectory("SamPlots");
    TH2F *h_icSumVSi = (TH2F*)dir->Get("h_icSumVSi");

    h_icSumVSi->GetYaxis()->SetTitle("Energy loss of recoils [keV]");
    h_icSumVSi->GetXaxis()->SetTitle("Total energy of recoils [keV]");
    h_icSumVSi->GetXaxis()->SetRangeUser(1000,5000);
    h_icSumVSi->GetYaxis()->SetRangeUser(800,3000);
    h_icSumVSi->GetZaxis()->SetTitle("Counts");
    h_icSumVSi->GetZaxis()->SetTitleOffset(1.2);
    h_icSumVSi->Draw("COLZ");
    c1->SetLogz();
    gStyle->SetOptStat(0);

    TFile *f = TFile::Open("/shared/physics/physdata/nuclear/Nuclear_Astro/EMMA/S1873/york/Sam_Sort/cuts.root");
    TCutG *cut = (TCutG*)f->Get("neon_cut_sam");
    cut->SetLineColor(kRed);
    cut->SetLineWidth(4);
    cut->Draw("L SAME");

    TLatex *label = new TLatex();
    label->SetNDC();   // use normalised canvas coordinates (0-1)
    label->SetTextSize(0.04);
    label->SetTextColor(kRed);
    label->DrawLatex(0.62, 0.40, "^{21}Ne");
    label->DrawLatex(0.61, 0.30, "^{19}F");
    label->DrawLatex(0.56, 0.22, "^{17}O");

    c1->SaveAs("EMMA_singles_PID.pdf");
    c1->SaveAs("EMMA_singles_PID.root");

}