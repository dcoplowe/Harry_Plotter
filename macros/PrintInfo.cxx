const string testing_mc("/pnfs/minerva/persistent/users/dcoplowe/CC1P1Pi_Full.root");

const string flag("sel_");

void PrintInfo()
{

    TFile * infile = new TFile(testing_mc.c_str(), "READ");
    
    //Get the number of
    // 1) new tracks created
    // 2) additional tracks after last cut
    // 3) michel tag purity of signal.
    
    //New tracks:
    TTree * intree = (TTree*)infile->Get("sel");
    
    TH1D * new_tracks_0 = new TH1D("new_tracks_0","",1,0, 1);
    TH1D * new_tracks_gt0 = new TH1D("new_tracks_gt0","",1,0, 1);
    
    intree->Project("new_tracks_0","0.5","new_tracks == 0");
    intree->Project("new_tracks_gt0","0.5","new_tracks > 0");
    
    
    new_tracks_gt0->Divide(new_tracks_0);
    
    std::cout << "Increase = " << new_tracks_gt0->GetBinContent(1) << std::endl;
    
}