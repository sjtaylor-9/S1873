void s3XYZlocation_2019() {

    TFile *file1 = TFile::Open("../HitMaps/XYZoffset/2019/HitMap_XYZ_52041-52164.root");

    const int nx = 46;
    const int ny = 31;
    const int nz = 21;

    Double_t minSigma = 1e9;
    Double_t maxSigma = -1e9;

    Double_t minX = 0;
    Double_t minY = 0;
    Double_t minZ = 0;

    // Histogram storing sigma(x,y,z)
    TH3D *hSigma = new TH3D(
        "hSigma",
        ";X Offset (mm);Y Offset (mm);Z Offset (mm)",
        nx, -4.55, 0.05,
        ny, -1.55, 1.55,
        nz, -0.05, 2.05);
    
    ofstream out("sigma_grid_2019.csv");
    out << "x,y,z,sigma,mean,sigma_1745,mean_1745\n";

    for (int i=0; i<nx; i++) {

        double x = i*0.1 -4.5;

        for (int j=0; j<ny; j++) {

            double y = j*0.1 - 1.5;

            for (int k=0; k<nz; k++) {

                double z = k*0.1 + 0.0;

                char hname[256];
                sprintf(hname,
                        "Excite_Iterations/Excite_iter_X_%f_Y_%f_Z_%f",
                        x,y,z);

                TH1F *h_exc = (TH1F*)file1->Get(hname);

                if (!h_exc) {
                    cout << "Missing histogram " << hname << endl;
                    continue;
                }

                if (h_exc->GetEntries() == 0) {
                    cout << "No entries in: " << hname << endl;
                    continue;
                }

                h_exc->GetXaxis()->SetRangeUser(6.8,7.6);


                int maxBin = h_exc->GetMaximumBin();
                double amp   = h_exc->GetBinContent(maxBin);
                double mean0 = h_exc->GetBinCenter(maxBin);

                TF1 fit("fit","gaus",6.8,7.6);
                fit.SetParameter(0, amp); // height of the tallest bin
                fit.SetParameter(1, mean0); // energy of the tallest bin
                fit.SetParameter(2, 0.05);  // initial guess of 50 keV for sigma     
                fit.SetParLimits(1,6.8,7.6);
                fit.SetParLimits(2,0.01,0.30);

                h_exc->Fit(&fit,"RQ0");

                double mean = fit.GetParameter(1);
                double sigma = fabs(fit.GetParameter(2));

                // 1745 keV peak 
                h_exc->GetXaxis()->SetRangeUser(1.3,2.0);

                int maxBin_1745 = h_exc->GetMaximumBin();
                double amp_1745 = h_exc->GetBinContent(maxBin_1745);
                double mean0_1745 = h_exc->GetBinCenter(maxBin_1745);

                TF1 fit_1745("fit_1745","gaus",1.3,2.0);
                fit_1745.SetParameter(0, amp_1745); // height of the tallest bin
                fit_1745.SetParameter(1, mean0_1745); // energy of the tallest bin
                fit_1745.SetParameter(2, 0.05);  // initial guess of 50 keV for sigma     
                fit_1745.SetParLimits(1,1.3,2.0);
                fit_1745.SetParLimits(2,0.01,0.30);
                h_exc->Fit(&fit_1745,"RQ0");

                double mean_1745 = fit_1745.GetParameter(1);
                double sigma_1745 = fabs(fit_1745.GetParameter(2));
                //double sigma = h_exc->GetStdDev();
                //double mean = h_exc->GetMean();
                out << x << ","
                << y << ","
                << z << ","
                << sigma << ","
                << mean << ","
                << sigma_1745 << ","
                << mean_1745 << "\n";

                hSigma->SetBinContent(i+1,j+1,k+1,sigma);

                if (sigma < minSigma) {
                    minSigma = sigma;
                    minX = x;
                    minY = y;
                    minZ = z;
                }

                if (sigma > maxSigma)
                    maxSigma = sigma;
            }
        }
    }

    cout << "Minimum sigma = " << minSigma
         << " at X = " << minX
         << " mm, Y = " << minY
         << " mm, Z = " << minZ << " mm" << endl;

    cout << "Maximum sigma = " << maxSigma << endl;

}
