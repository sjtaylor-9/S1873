void s3XYlocation() {

	TFile *file1=TFile::Open("../HitMaps/XYoffset/HitMap_XY_52829-52928.root");

	const int N=3721; // The X and Y offsets are varied from -3 -> 3 so there are 61 possible X positions and 61 possible Y positions. N = 61 x 61 = 3721
	
    // Creates arrays of size 3721 for every offset position trial
	Double_t x[N];
	Double_t y[N];
	Double_t sigma[N];
	
	Double_t minSigma = 1e9;
	Double_t minX = 0;
	Double_t minY = 0;	

	Int_t n=0;

	for(int i=0;i<61;i++){ // loop over x offsets
      	for(int j=0;j<61;j++){ // loop over y offsets
			char hname[256]; // hist name
			x[n] = i*0.1 - 3; // set x var, scan range is -3 mm -> 3 mm in steps of 0.05 mm
			y[n] = j*0.1 - 3; // set y var, scan range is -3 mm -> 3 mm in steps of 0.05 mm
			sprintf(hname,"Excite_Iterations/Excite_iter_X_%f_Y_%f",x[n],y[n]); // Assign the name of the histogram to be retreived to hname
			TH1F *h_exc = (TH1F*) file1->Get(hname); // Retrieve Ex spectrum for the given (X,Y) offset
			h_exc->Rebin(2); // Combines neighbouring bins to reduce noise
			h_exc->GetXaxis()->SetRangeUser(6.8,7.6); // Zooms in on Ex peak at 7.4 MeV by restricting the Ex range to 7.0 -> 7.6 MeV
			sigma[n] = h_exc->GetStdDev(); // Measures the standard deviation of the 7.4 MeV peak for the given (X,Y) offset
			if(sigma[n] < minSigma){
   			 minSigma = sigma[n];
    			 minX = x[n];
    			 minY = y[n];
			}
			//h_exc->Delete();
			n++;
      	}
    }

	TCanvas *c1 = new TCanvas("c1","c1",900,700);
	c1->SetLeftMargin(0.14);
	c1->SetRightMargin(0.14);
	c1->SetBottomMargin(0.14);
	c1->SetTopMargin(0.10);

	TGraph2D *g = new TGraph2D(N,x,y,sigma); // Create a contour plot where x = X offset (mm), y = Y offset (mm) and z = standard deviation of the 7.4 MeV Ex peak
	g->SetTitle("S3 XY Location");
	g->GetXaxis()->SetTitle("X Offset (mm)");
	g->GetYaxis()->SetTitle("Y Offset (mm)");
	g->SetNpx(400);
	g->SetNpy(400);
	g->Draw("colz");

	gPad->Update();

	TH2 *h = g->GetHistogram();
	h->GetXaxis()->SetTitle("X Offset (mm)");
	h->GetYaxis()->SetTitle("Y Offset (mm)");
	h->GetXaxis()->SetTitleOffset(1.2);
	h->GetYaxis()->SetTitleOffset(1.4);
	h->GetZaxis()->SetTitleOffset(1.2);

	TMarker *marker = new TMarker(minX, minY, 5);
	marker->SetMarkerColor(kRed);
	marker->SetMarkerSize(3);
	marker->Draw("same");
	
	cout << "Minimum sigma = " << minSigma
     		<< " at X = " << minX
     		<< " mm, Y = " << minY << " mm" << endl;
	c1->SaveAs("S3_XYPosition.pdf");
}
