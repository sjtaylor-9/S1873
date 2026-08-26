void copyDir(TDirectory* inDir, TDirectory* outDir) {
    TIter next(inDir->GetListOfKeys());
    TKey* key;

    while ((key = (TKey*)next())) {
        TObject* obj = key->ReadObj();

        // DIRECTORY CASE
        if (obj->InheritsFrom(TDirectory::Class())) {

            TDirectory* inSubDir = (TDirectory*)obj;

            TDirectory* outSubDir =
                (TDirectory*)outDir->GetDirectory(inSubDir->GetName());

            if (!outSubDir) {
                outSubDir = outDir->mkdir(inSubDir->GetName());
            }

            copyDir(inSubDir, outSubDir);
        }

        // HISTOGRAM CASE
        else if (obj->InheritsFrom(TH1::Class())) {
            outDir->cd();

            TObject* clone = obj->Clone();
            clone->Write();
            delete clone;
        }
    }
}

void merge_root_dirs() {
    TFile outFile("../HitMaps/XYZoffset/2019/HitMap_XYZ_52041-52164.root", "RECREATE");

    std::vector<std::string> files = {
        "../HitMaps/XYZoffset/2019/Q1/HitMap_XYZ_52041-52164.root",
        "../HitMaps/XYZoffset/2019/Q2/HitMap_XYZ_52041-52164.root"
    };

    for (const auto& fname : files) {
        TFile inFile(fname.c_str(), "READ");

        copyDir(&inFile, &outFile);

        inFile.Close();
    }

    outFile.Close();
}
