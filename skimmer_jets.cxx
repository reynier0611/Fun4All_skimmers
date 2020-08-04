#include "TFile.h"
#include "TString.h"
#include "TTree.h"

#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iostream>

using namespace std;

int main(int argc, char ** argv) {

        if(argc!=4){
                cout << "--------------------------------------\nRun this code as:\n./skimmer_jets /path/to/file/ filename A\n\n";
		cout << "A = 1 -> no cuts\n  = 2 -> Q^2 < 1 GeV^2" << endl;
		cout << "--------------------------------------" << endl;
                return 0;
        }

	// input file
	TString dir = argv[1];
	TString common_name = argv[2];
	TFile * Fin = new TFile(dir+common_name);
	TTree * Tin = (TTree*) Fin -> Get("T");
	int event, njets, ntruthjets, nAlltruthjets, electron_truthPID;
	float electron_truthEta,electron_truthPhi,electron_truthE,electron_truthPt,electron_truthpX,electron_truthpY, electron_truthpZ;

	const int MaxNumJets = 20;
	const int kMaxConstituents = 100;

	// Reconstructed jets
	std::array<int,MaxNumJets> id;
	std::array<int,MaxNumJets> nComponent;
	std::array<float,MaxNumJets> eta;
	std::array<float,MaxNumJets> phi;
	std::array<float,MaxNumJets> e;
	std::array<float,MaxNumJets> pt;

	// Truth jets matched to a reconstructed jet
	std::array<int,MaxNumJets> matched_truthID;
	std::array<int,MaxNumJets> matched_truthNComponent;
	std::array<float,MaxNumJets> matched_truthEta;
	std::array<float,MaxNumJets> matched_truthPhi;
	std::array<float,MaxNumJets> matched_truthE;
	std::array<float,MaxNumJets> matched_truthPt;

	// All truth jets
	std::array<int,MaxNumJets> all_truthID;
	std::array<int,MaxNumJets> all_truthNComponent;
	std::array<float,MaxNumJets> all_truthEta;
	std::array<float,MaxNumJets> all_truthPhi;
	std::array<float,MaxNumJets> all_truthE;
	std::array<float,MaxNumJets> all_truthPt;

	// Constituent for all matched jets
	std::array<std::array<float, kMaxConstituents >, MaxNumJets > matched_Constituent_truthPID;
	std::array<std::array<float, kMaxConstituents >, MaxNumJets > matched_Constituent_truthCharge;
	std::array<std::array<float, kMaxConstituents >, MaxNumJets > matched_Constituent_truthEta;
	std::array<std::array<float, kMaxConstituents >, MaxNumJets > matched_Constituent_truthPhi;
	std::array<std::array<float, kMaxConstituents >, MaxNumJets > matched_Constituent_truthPt;
	std::array<std::array<float, kMaxConstituents >, MaxNumJets > matched_Constituent_truthE;

	float _matched_Constituent_truthPID   [20][100];
        float _matched_Constituent_truthCharge[20][100];
        float _matched_Constituent_truthEta   [20][100];
        float _matched_Constituent_truthPhi   [20][100];
        float _matched_Constituent_truthPt    [20][100];
        float _matched_Constituent_truthE     [20][100];

	// Charged (neutral-subtracted) jets
	std::array<int,MaxNumJets> matched_charged_truthNComponent;
	std::array<float,MaxNumJets> matched_charged_truthEta;
	std::array<float,MaxNumJets> matched_charged_truthPhi;
	std::array<float,MaxNumJets> matched_charged_truthE;
	std::array<float,MaxNumJets> matched_charged_truthPt;

	// Constituents for all truth jets
	std::array<std::array<float, kMaxConstituents >, MaxNumJets > all_Constituent_truthPID;
	std::array<std::array<float, kMaxConstituents >, MaxNumJets > all_Constituent_truthCharge;
	std::array<std::array<float, kMaxConstituents >, MaxNumJets > all_Constituent_truthEta;
	std::array<std::array<float, kMaxConstituents >, MaxNumJets > all_Constituent_truthPhi;
	std::array<std::array<float, kMaxConstituents >, MaxNumJets > all_Constituent_truthPt;
	std::array<std::array<float, kMaxConstituents >, MaxNumJets > all_Constituent_truthE;
	
	float _all_Constituent_truthPID   [20][100];
        float _all_Constituent_truthCharge[20][100];
        float _all_Constituent_truthEta   [20][100];
        float _all_Constituent_truthPhi   [20][100];
        float _all_Constituent_truthPt    [20][100];
        float _all_Constituent_truthE     [20][100];

	Tin -> SetBranchAddress("event"                          ,&event                                  );
	Tin -> SetBranchAddress("njets"                          ,&njets                                  );
	Tin -> SetBranchAddress("ntruthjets"                     ,&ntruthjets                             );
	Tin -> SetBranchAddress("nAlltruthjets"                  ,&nAlltruthjets                          );
	Tin -> SetBranchAddress("id"                             , id.data()                              );
	Tin -> SetBranchAddress("nComponent"                     , nComponent.data()                      );
	Tin -> SetBranchAddress("eta"                            , eta.data()                             );
	Tin -> SetBranchAddress("phi"                            , phi.data()                             );
	Tin -> SetBranchAddress("e"                              , e.data()                               );
	Tin -> SetBranchAddress("pt"                             , pt.data()                              );
	Tin -> SetBranchAddress("matched_truthID"                , matched_truthID.data()                 );
	Tin -> SetBranchAddress("matched_truthNComponent"        , matched_truthNComponent.data()         );
	Tin -> SetBranchAddress("matched_truthEta"               , matched_truthEta.data()                );
	Tin -> SetBranchAddress("matched_truthPhi"               , matched_truthPhi.data()                );
	Tin -> SetBranchAddress("matched_truthE"                 , matched_truthE.data()                  );
	Tin -> SetBranchAddress("matched_truthPt"                , matched_truthPt.data()                 );
	Tin -> SetBranchAddress("matched_Constituent_truthPID"   ,_matched_Constituent_truthPID           );
	Tin -> SetBranchAddress("matched_Constituent_truthCharge",_matched_Constituent_truthCharge        );
	Tin -> SetBranchAddress("matched_Constituent_truthEta"   ,_matched_Constituent_truthEta           );
	Tin -> SetBranchAddress("matched_Constituent_truthPhi"   ,_matched_Constituent_truthPhi           );
	Tin -> SetBranchAddress("matched_Constituent_truthPt"    ,_matched_Constituent_truthPt            );
	Tin -> SetBranchAddress("matched_Constituent_truthE"     ,_matched_Constituent_truthE             );
	Tin -> SetBranchAddress("matched_charged_truthNComponent", matched_charged_truthNComponent.data() );
	Tin -> SetBranchAddress("matched_charged_truthEta"       , matched_charged_truthEta.data()        );
	Tin -> SetBranchAddress("matched_charged_truthPhi"       , matched_charged_truthPhi.data()        );
	Tin -> SetBranchAddress("matched_charged_truthE"         , matched_charged_truthE.data()          );
	Tin -> SetBranchAddress("matched_charged_truthPt"        , matched_charged_truthPt.data()         );
	Tin -> SetBranchAddress("all_truthID"                    , all_truthID.data()                     );
	Tin -> SetBranchAddress("all_truthNComponent"            , all_truthNComponent.data()             );
	Tin -> SetBranchAddress("all_truthEta"                   , all_truthEta.data()                    );
	Tin -> SetBranchAddress("all_truthPhi"                   , all_truthPhi.data()                    );
	Tin -> SetBranchAddress("all_truthE"                     , all_truthE.data()                      );
	Tin -> SetBranchAddress("all_truthPt"                    , all_truthPt.data()                     );
	Tin -> SetBranchAddress("all_Constituent_truthPID"       ,_all_Constituent_truthPID               );
	Tin -> SetBranchAddress("all_Constituent_truthCharge"    ,_all_Constituent_truthCharge            );
	Tin -> SetBranchAddress("all_Constituent_truthEta"       ,_all_Constituent_truthEta               );
	Tin -> SetBranchAddress("all_Constituent_truthPhi"       ,_all_Constituent_truthPhi               );
	Tin -> SetBranchAddress("all_Constituent_truthPt"        ,_all_Constituent_truthPt                );
	Tin -> SetBranchAddress("all_Constituent_truthE"         ,_all_Constituent_truthE                 );
	Tin -> SetBranchAddress("electron_truthEta"              ,&electron_truthEta                      );
	Tin -> SetBranchAddress("electron_truthPhi"              ,&electron_truthPhi                      );
	Tin -> SetBranchAddress("electron_truthE"                ,&electron_truthE                        );
	Tin -> SetBranchAddress("electron_truthPt"               ,&electron_truthPt                       );
	Tin -> SetBranchAddress("electron_truthpX"               ,&electron_truthpX                       );
	Tin -> SetBranchAddress("electron_truthpY"               ,&electron_truthpY                       );
	Tin -> SetBranchAddress("electron_truthpZ"               ,&electron_truthpZ                       );
	Tin -> SetBranchAddress("electron_truthPID"              ,&electron_truthPID                      );
	int nEntries = Tin -> GetEntries();

	// output file
	TFile * Fout = new TFile(dir+"skimmed_"+common_name,"recreate");
	TTree * Tout = new TTree("T","skimmed tree");
	Tout->Branch("event"                          ,&event                                 , "event/I"                                         );
	Tout->Branch("njets"                          ,&njets                                 , "njets/I"                                         );
	Tout->Branch("ntruthjets"                     ,&ntruthjets                            , "ntruthjets/I"                                    );
	Tout->Branch("nAlltruthjets"                  ,&nAlltruthjets                         , "nAlltruthjets/I"                                 );
	Tout->Branch("id"                             , id.data()                             , "id[njets]/I"                                     );
	Tout->Branch("nComponent"                     , nComponent.data()                     , "nComponent[njets]/I"                             );
	Tout->Branch("eta"                            , eta.data()                            , "eta[njets]/F"                                    );
	Tout->Branch("phi"                            , phi.data()                            , "phi[njets]/F"                                    );
	Tout->Branch("e"                              , e.data()                              , "e[njets]/F"                                      );
	Tout->Branch("pt"                             , pt.data()                             , "pt[njets]/F"                                     );
	Tout->Branch("matched_truthID"                , matched_truthID.data()                , "matched_truthID[ntruthjets]/I"                   );
	Tout->Branch("matched_truthNComponent"        , matched_truthNComponent.data()        , "matched_truthNComponent[ntruthjets]/I"           );
	Tout->Branch("matched_truthEta"               , matched_truthEta.data()               , "matched_truthEta[ntruthjets]/F"                  );
	Tout->Branch("matched_truthPhi"               , matched_truthPhi.data()               , "matched_truthPhi[ntruthjets]/F"                  );
	Tout->Branch("matched_truthE"                 , matched_truthE.data()                 , "matched_truthE[ntruthjets]/F"                    );
	Tout->Branch("matched_truthPt"                , matched_truthPt.data()                , "matched_truthPt[ntruthjets]/F"                   );
	Tout->Branch("matched_Constituent_truthPID"   ,&matched_Constituent_truthPID          ,"matched_Constituent_truthPID[njets][100]/F"       );
	Tout->Branch("matched_Constituent_truthCharge",&matched_Constituent_truthCharge       ,"matched_Constituent_truthCharge[njets][100]/F"    );
	Tout->Branch("matched_Constituent_truthEta"   ,&matched_Constituent_truthEta          ,"matched_Constituent_truthEta[njets][100]/F"       );
	Tout->Branch("matched_Constituent_truthPhi"   ,&matched_Constituent_truthPhi          ,"matched_Constituent_truthPhi[njets][100]/F"       );
	Tout->Branch("matched_Constituent_truthPt"    ,&matched_Constituent_truthPt           ,"matched_Constituent_truthPt[njets][100]/F"        ); 
	Tout->Branch("matched_Constituent_truthE"     ,&matched_Constituent_truthE            ,"matched_Constituent_truthE[njets][100]/F"         );
	Tout->Branch("matched_charged_truthNComponent", matched_charged_truthNComponent.data(),"matched_charged_truthNComponent[ntruthjets]/I"    );
	Tout->Branch("matched_charged_truthEta"       , matched_charged_truthEta.data()       ,"matched_charged_truthEta[ntruthjets]/F"           );
	Tout->Branch("matched_charged_truthPhi"       , matched_charged_truthPhi.data()       ,"matched_charged_truthPhi[ntruthjets]/F"           );
	Tout->Branch("matched_charged_truthE"         , matched_charged_truthE.data()         ,"matched_charged_truthE[ntruthjets]/F"             );
	Tout->Branch("matched_charged_truthPt"        , matched_charged_truthPt.data()        ,"matched_charged_truthPt[ntruthjets]/F"            );
	Tout->Branch("all_truthID"                    , all_truthID.data()                    ,"all_truthID[nAlltruthjets]/I"                     );
	Tout->Branch("all_truthNComponent"            , all_truthNComponent.data()            ,"all_truthNComponent[nAlltruthjets]/I"             );
	Tout->Branch("all_truthEta"                   , all_truthEta.data()                   ,"all_truthEta[nAlltruthjets]/F"                    );
	Tout->Branch("all_truthPhi"                   , all_truthPhi.data()                   ,"all_truthPhi[nAlltruthjets]/F"                    );
	Tout->Branch("all_truthE"                     , all_truthE.data()                     ,"all_truthE[nAlltruthjets]/F"                      );
	Tout->Branch("all_truthPt"                    , all_truthPt.data()                    ,"all_truthPt[nAlltruthjets]/F"                     );
	Tout->Branch("all_Constituent_truthPID"       ,&all_Constituent_truthPID              ,"All_Constituent_truthPID[nAlltruthjets][100]/F"   );
	Tout->Branch("all_Constituent_truthCharge"    ,&all_Constituent_truthCharge           ,"All_Constituent_truthCharge[nAlltruthjets][100]/F");
	Tout->Branch("all_Constituent_truthEta"       ,&all_Constituent_truthEta              ,"All_Constituent_truthEta[nAlltruthjets][100]/F"   );
	Tout->Branch("all_Constituent_truthPhi"       ,&all_Constituent_truthPhi              ,"All_Constituent_truthPhi[nAlltruthjets][100]/F"   );
	Tout->Branch("all_Constituent_truthPt"        ,&all_Constituent_truthPt               ,"All_Constituent_truthPt[nAlltruthjets][100]/F"    );
	Tout->Branch("all_Constituent_truthE"         ,&all_Constituent_truthE                ,"All_Constituent_truthE[nAlltruthjets][100]/F"     );
	Tout->Branch("electron_truthEta"              ,&electron_truthEta                     , "electron_truthEta/F"                             );
	Tout->Branch("electron_truthPhi"              ,&electron_truthPhi                     , "electron_truthPhi/F"                             );
	Tout->Branch("electron_truthE"                ,&electron_truthE                       , "electron_truthE/F"                               );
	Tout->Branch("electron_truthPt"               ,&electron_truthPt                      , "electron_truthPt/F"                              );
	Tout->Branch("electron_truthpX"               ,&electron_truthpX                      , "electron_truthpX/F"                              );
	Tout->Branch("electron_truthpY"               ,&electron_truthpY                      , "electron_truthpY/F"                              );
	Tout->Branch("electron_truthpZ"               ,&electron_truthpZ                      , "electron_truthpZ/F"                              );
	Tout->Branch("electron_truthPID"              ,&electron_truthPID                     , "electron_truthPID/I"                             );
                                                                                               
	// looping over entries
	for(int evt = 0 ; evt < nEntries ; evt++){
		if(evt%10000==0) cout << evt << "\tout of " << nEntries << endl;	
		Tin -> GetEntry(evt);
		
		for(int i = 0 ; i < 20 ; i++){ // Loop over jets
			for(int j = 0 ; j < 100 ; j++){ // Loop over constituents	
				matched_Constituent_truthPID   [i][j] = _matched_Constituent_truthPID   [i][j];
        			matched_Constituent_truthCharge[i][j] = _matched_Constituent_truthCharge[i][j];
        			matched_Constituent_truthEta   [i][j] = _matched_Constituent_truthEta   [i][j];
        			matched_Constituent_truthPhi   [i][j] = _matched_Constituent_truthPhi   [i][j];
        			matched_Constituent_truthPt    [i][j] = _matched_Constituent_truthPt    [i][j];
		        	matched_Constituent_truthE     [i][j] = _matched_Constituent_truthE     [i][j];

				all_Constituent_truthPID    [i][j] = _all_Constituent_truthPID    [i][j];
                                all_Constituent_truthCharge [i][j] = _all_Constituent_truthCharge [i][j];
                                all_Constituent_truthEta    [i][j] = _all_Constituent_truthEta    [i][j];
                                all_Constituent_truthPhi    [i][j] = _all_Constituent_truthPhi    [i][j];
                                all_Constituent_truthPt     [i][j] = _all_Constituent_truthPt     [i][j];
                                all_Constituent_truthE      [i][j] = _all_Constituent_truthE      [i][j];	
			}
		}

		Tout -> Fill();
	}
	Tout -> Write();
	Fout -> Close();
	Fin -> Close();

	return 0;
}
