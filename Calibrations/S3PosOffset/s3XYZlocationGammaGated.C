void s3XYZlocationGammaGated() {

    TFile *inputFile = TFile::Open("../HitMaps/GammaGated/2020/HitMap_XYZ_52829-52923.root");

    const int nx = 18;
    const int ny = 16;
    const int nz = 31;

    // Initialise and declare variables
    Double_t minDeltaMean = 1e9;
    Double_t OptimalMean = 0;
    Double_t nominal_mean = 2.866; // NNDC value for the energy of this state
    Double_t Sigma_atOptimalMean = 0;
    Double_t OptimalX = 0;
    Double_t OptimalY = 0;
    Double_t OptimalZ = 0;

    ofstream out("sigma_grid_GammaGated.csv");
    out << "x,y,z,mean,sigma\n";

    for (int i=0; i<nx; i++) {

        double x = i*0.1 -3.0;

        for (int j=0; j<ny; j++) {

            double y = j*0.1 - 1.5;

            for (int k=0; k<nz; k++) {

                double z = k*0.1 + 3.0;

                // Sets the name of the histogram to be retrived using the x,y,z indices of the given iteration
                char hname[256];
                sprintf(hname,
                        "ExGammaGated_Iterations/ExGammaGated_iter_X_%f_Y_%f_Z_%f",
                        x,y,z);
                
                // Retrive the histogram
                TH1F *h_excGammaGated = (TH1F*)inputFile->Get(hname);
                if (!h_excGammaGated) {
                    cout << "Missing histogram " << hname << endl;
                    continue;
                }
                if (h_excGammaGated->GetEntries() == 0) {
                    cout << "No entries in: " << hname << endl;
                    continue;
                }

                // Focussing on the 2.86 MeV peak
                h_excGammaGated->GetXaxis()->SetRangeUser(0,4);

                int maxBin = h_excGammaGated->GetMaximumBin();
                double amp   = h_excGammaGated->GetBinContent(maxBin); // Initial guess of amplitude is the height of the bin which the peak is in
                double mean0 = h_excGammaGated->GetBinCenter(maxBin); // Initial guess of mean is the bin number which the peak is in


                TF1 fit("fit","gaus(0)+pol1(3)", 2.0,3.5);
                fit.SetParameter(0, amp); // height of the tallest bin
                fit.SetParameter(1, mean0); // energy of the tallest bin
                fit.SetParameter(2, 0.05);  // initial guess of 50 keV for sigma     
                fit.SetParLimits(1,2,3.5); // Limits of the mean are 2-3.5 MeV
                fit.SetParLimits(2,0.01,0.30); // Limits of the sigma are 10-300 keV

                h_excGammaGated->Fit(&fit,"RQ0");
                double mean = fit.GetParameter(1);
                double sigma = fabs(fit.GetParameter(2));

                // Writes the XYZ offset and the mean and standard deviation of the 2.8 MeV peak fit to the output file
                out << x << ","
                << y << ","
                << z << ","
                << mean << ","
                << sigma << "\n";

                // Determines the minimum and maximum sigma values and their corresponding XYZ offsets
                double Delta_mean = fabs(mean - nominal_mean);
                if (Delta_mean < minDeltaMean) {
                    minDeltaMean = Delta_mean; // Needed to update the minimum deviation from the nominal mean
                    OptimalMean = mean;
                    OptimalX = x;
                    OptimalY = y;
                    OptimalZ = z;
                    Sigma_atOptimalMean = sigma;
                }

            } // end of loop over z indices
        } // end of loop over y indices
    } // end of loop over x indices

    cout << "Mean closest to 2.866 MeV NNDC value = " << OptimalMean
         << " Standard deviation of this peak = " << Sigma_atOptimalMean
         << ". Offset values, X = " << OptimalX
         << " mm, Y = " << OptimalY
         << " mm, Z = " << OptimalZ << " mm" << endl;

}