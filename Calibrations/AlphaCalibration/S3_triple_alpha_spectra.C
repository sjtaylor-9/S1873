// g++ S3_triple_alpha_spectra.C -std=c++0x `root-config --cflags --libs` -o S3_triple_alpha_calibration
#include <TCanvas.h>
#include <TLegend.h>
#include "S3_triple_alpha_spectra.h"

void S3_triple_alpha_spectra::Sort_triple_alpha_data() {

	Initialise();

	double ratio;

	// Output ROOT file branch structure
    struct Ring_before {
        double energy;
		double time;
        double multi;
        double hit;
    } ring_before;

    struct Sector_before {
        double energy;
		double time;
        double multi;
        double hit;
    } sector_before;

    struct S3Energy_before {
        double exc;
        double kin;
    } s3energy_before;

	    struct Ring_after {
        double energy;
		double time;
        double multi;
        double hit;
    } ring_after;

    struct Sector_after {
        double energy;
		double time;
        double multi;
        double hit;
    } sector_after;

    struct S3Energy_after {
        double exc;
        double kin;
    } s3energy_after;

	// Open input ROOT files
	//TFile *Before_calibration_file = TFile::Open("../../HistFiles/WithAfterCal/Hist_52754_000.root"); // 2020 
    TFile *Before_calibration_file = TFile::Open("../../HistFiles/WithAfterCal/Hist_52016_000.root"); // 2019
    if(!Before_calibration_file) {
        std::cout << "Error opening before calibration file \n";
        return;
    }
	//TFile *After_calibration_file = TFile::Open("../../HistFiles/WithAfterCal/Hist_52931_000.root"); // 2020 40 V
	//TFile *After_calibration_file = TFile::Open("../../HistFiles/WithAfterCal/Hist_52930_000.root"); // 2020 20 V
    TFile *After_calibration_file = TFile::Open("../../HistFiles/WithAfterCal/Sum_52167.root"); // 2019
    if(!After_calibration_file) {
        std::cout << "Error opening after calibration file \n";
        return;
    }

	// Retrieves s3 TTrees inside input files
    TTree *s3_before_cal = (TTree*)Before_calibration_file->Get("s3");
	if (!s3_before_cal || s3_before_cal->GetEntries() == 0) {
    	std::cout << "S3 tree not found or is empty in the before calibration file\n";
    	return;
	}
	TTree *s3_after_cal = (TTree*)After_calibration_file->Get("s3");
	if (!s3_after_cal || s3_after_cal->GetEntries() == 0) {
    	std::cout << "S3 tree not found or is empty in the after calibration file\n";
    	return;
	}
	long before_cal_entries = s3_before_cal->GetEntries();
	long after_cal_entries = s3_after_cal->GetEntries();
	// The relevant TBranches in the input s3 TTree are ring, sector and energy.
	// The ring and sector TBranches have energy and hit TLeaves
	// SetBranchAddress connects the TBranches in the s3 TTree in HistFile to the TBranches in the output ROOT file
	s3_before_cal->SetBranchAddress("ring", &ring_before);
	s3_before_cal->SetBranchAddress("sector", &sector_before);
	s3_before_cal->SetBranchAddress("s3energy", &s3energy_before);
	
	s3_after_cal->SetBranchAddress("ring", &ring_after);
	s3_after_cal->SetBranchAddress("sector", &sector_after);
	s3_after_cal->SetBranchAddress("s3energy", &s3energy_after);

	// Define output ROOT file and associated TBranches
	// "RECREATE" creates a ROOT file if it does not exist and overwrites it if it does
	TFile * outputfile = new TFile("S3_triple_alpha_calibration_plots.root", "RECREATE");

    // Event loop for the before calibration run
	for (int jentry = 0; jentry < before_cal_entries; jentry++) { // loop over events in histogran tree
        s3_before_cal->GetEntry(jentry);
		// Fill histograms
		h_s3energy_before_cal->Fill(s3energy_before.kin);
		h_energyVring_before_cal->Fill(ring_before.hit,s3energy_before.kin);
		if (ring_before.multi == 1) {
			h_energyVring_multi_1_before_cal->Fill(ring_before.hit,s3energy_before.kin);
		}
		
		if (ring_before.hit == 0) {
			h_ring0_before_cal->Fill(s3energy_before.kin);
		}
		if (ring_before.hit == 2) {
			h_ring2_before_cal->Fill(s3energy_before.kin);
		}
		if (ring_before.hit == 4) {
			h_ring4_before_cal->Fill(s3energy_before.kin);
		}
		if (ring_before.hit == 6) {
			h_ring6_before_cal->Fill(s3energy_before.kin);
		}
		if (ring_before.hit == 8) {
			h_ring8_before_cal->Fill(s3energy_before.kin);
		}
		if (ring_before.hit == 10) {
			h_ring10_before_cal->Fill(s3energy_before.kin);
		}
		if (ring_before.hit == 12) {
			h_ring12_before_cal->Fill(s3energy_before.kin);
		}
		if (ring_before.hit == 14) {
			h_ring14_before_cal->Fill(s3energy_before.kin);
		}
		if (ring_before.hit == 16) {
			h_ring16_before_cal->Fill(s3energy_before.kin);
		}		
		if (ring_before.hit == 18) {
			h_ring18_before_cal->Fill(s3energy_before.kin);
		}
		if (ring_before.hit == 20) {
			h_ring20_before_cal->Fill(s3energy_before.kin);
		}
		if (ring_before.hit == 22) {
			h_ring22_before_cal->Fill(s3energy_before.kin);
		}

		h_sectorE_v_ring_num_before_cal->Fill(ring_before.hit,sector_before.energy);
    } // end of before calibration event loop

	// Event loop for the after calibration run
	for (int jentry = 0; jentry < after_cal_entries; jentry++) { // loop over events in histogran tree
        s3_after_cal->GetEntry(jentry);
		// Fill histograms
		h_s3energy_after_cal->Fill(s3energy_after.kin);
		h_energyVring_after_cal->Fill(ring_after.hit,s3energy_after.kin);
		if (ring_after.multi == 1) {
			h_energyVring_multi_1_after_cal->Fill(ring_after.hit,s3energy_after.kin);
		}

		if (ring_after.hit == 0) {
			h_ring0_after_cal->Fill(s3energy_after.kin);
		}
		if (ring_after.hit == 2) {
			h_ring2_after_cal->Fill(s3energy_after.kin);
		}
		if (ring_after.hit == 4) {
			h_ring4_after_cal->Fill(s3energy_after.kin);
		}
		if (ring_after.hit == 6) {
			h_ring6_after_cal->Fill(s3energy_after.kin);
		}
		if (ring_after.hit == 8) {
			h_ring8_after_cal->Fill(s3energy_after.kin);
		}
		if (ring_after.hit == 10) {
			h_ring10_after_cal->Fill(s3energy_after.kin);
		}
		if (ring_after.hit == 12) {
			h_ring12_after_cal->Fill(s3energy_after.kin);
		}
		if (ring_after.hit == 14) {
			h_ring14_after_cal->Fill(s3energy_after.kin);
		}
		if (ring_after.hit == 16) {
			h_ring16_after_cal->Fill(s3energy_after.kin);
		}		
		if (ring_after.hit == 18) {
			h_ring18_after_cal->Fill(s3energy_after.kin);
		}
		if (ring_after.hit == 20) {
			h_ring20_after_cal->Fill(s3energy_after.kin);
		}
		if (ring_after.hit == 22) {
			h_ring22_after_cal->Fill(s3energy_after.kin);
		}

		if (sector_after.hit == 0) {
			h_sector0E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[0]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 1) {
			h_sector1E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[1]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 2) {
			h_sector2E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[2]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 3) {
			h_sector3E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[3]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 4) {
			h_sector4E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[4]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 5) {
			h_sector5E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[5]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 6) {
			h_sector6E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[6]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 7) {
			h_sector7E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[7]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 8) {
			h_sector8E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[8]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 9) {
			h_sector9E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[9]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 10) {
			h_sector10E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[10]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 11) {
			h_sector11E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[11]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 12) {
			h_sector12E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[12]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 13) {
			h_sector13E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[13]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 14) {
			h_sector14E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[14]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 15) {
			h_sector15E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[15]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 16) {
			h_sector16E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[16]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 17) {
			h_sector17E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[17]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 18) {
			h_sector18E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[18]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 19) {
			h_sector19E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[19]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 20) {
			h_sector20E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[20]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 21) {
			h_sector21E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[21]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 22) {
			h_sector22E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[22]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 23) {
			h_sector23E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[23]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 24) {
			h_sector24E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[24]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 25) {
			h_sector25E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[25]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 26) {
			h_sector26E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[26]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 27) {
			h_sector27E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[27]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 28) {
			h_sector28E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[28]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 29) {
			h_sector29E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[29]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 30) {
			h_sector30E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[30]->Fill(sector_after.energy);
		}
		if (sector_after.hit == 31) {
			h_sector31E_v_ring_num_after_cal->Fill(ring_after.hit, sector_after.energy);
			h_s3Sector_alphaE[31]->Fill(sector_after.energy);
		}

		h_sectorE_v_ring_num_after_cal->Fill(ring_after.hit,sector_after.energy);
    } // end of after calibration event loop

	outputfile->cd();
	
	TDirectory *triple_alpha_dir = outputfile->mkdir("TripleAlphaPlots");
	triple_alpha_dir->cd();
	triple_alpha_List->Write();
	outputfile->cd();
	TDirectory *s3_energies_dir = outputfile->mkdir("S3EnergyPlots");
	s3_energies_dir->cd();
	s3_energies_List->Write();
	outputfile->cd();
	TDirectory *s3_rings_dir = outputfile->mkdir("RingEnergies");
	s3_rings_dir->cd();
	s3_rings_List->Write();
	outputfile->cd();
	TDirectory *s3_sectors_dir = outputfile->mkdir("SectorEnergiesVRings");
	s3_sectors_dir->cd();
	s3_sectors_List->Write();
	outputfile->cd();
	TDirectory *s3_sectors_alphaE_dir = outputfile->mkdir("SectorEnergies");
	s3_sectors_alphaE_dir->cd();
	s3_sectors_alphaE_List->Write();
	outputfile->cd();

	h_s3energy_before_cal->SetLineColor(kBlue);
	h_s3energy_after_cal->SetLineColor(kRed);
	TCanvas *c1 = new TCanvas("c1", "S3 energy for triple alpha calibration runs");
	h_s3energy_before_cal->Draw();
	h_s3energy_after_cal->Draw("SAME");
	TLegend *leg = new TLegend(0.1, 0.7, 0.4, 0.9);
	leg->AddEntry(h_s3energy_before_cal, "Before calibration", "l");
	leg->AddEntry(h_s3energy_after_cal, "After calibration", "l");
	leg->Draw();
	c1->SaveAs("S3_energy_triple_alpha_calib.pdf");
    // Save everything
    outputfile->Write();
    outputfile->Close();
	
    
}

int main() {
  S3_triple_alpha_spectra * triple_alpha_sort_code = new S3_triple_alpha_spectra();
  printf("Sorting triple alpha data\n");
  triple_alpha_sort_code->Sort_triple_alpha_data();
  printf("Sorted triple alpha data\n");
  return 0;
}