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
		cout << "--------------------------------------\nRun this code as:\n./skimmer /path/to/file/ filename A\n\n";
		cout << "A = 1 -> Include both momentum and vertex information\n  = 2 -> Only include momentum information";
		cout << "\n  = 3 -> Include information needed for projections";
		cout << "\n  = 4 -> Include everything in the 'tracks' tree" << endl;
		cout << "--------------------------------------" << endl;
		return 0;
	}

	// input file
	TString common_name = argv[1];
	common_name += argv[2];
	cout << "Skimming file: " << common_name << endl;

	TFile * Fin = new TFile(common_name);
	TTree * Tin = (TTree*) Fin -> Get("tracks");
	float gpx, gpy, gpz, px, py, pz, gvz, pcaz, dca2d;
	Tin -> SetBranchAddress("gpx"  ,&gpx  );
	Tin -> SetBranchAddress("gpy"  ,&gpy  );
	Tin -> SetBranchAddress("gpz"  ,&gpz  );
	Tin -> SetBranchAddress("px"   ,&px   );
	Tin -> SetBranchAddress("py"   ,&py   );
	Tin -> SetBranchAddress("pz"   ,&pz   );
	Tin -> SetBranchAddress("gvz"  ,&gvz  );
	Tin -> SetBranchAddress("pcaz" ,&pcaz );
	Tin -> SetBranchAddress("dca2d",&dca2d);

	int trackID;
	float DIRC_px, DIRC_py, DIRC_pz, DIRC_proj_px, DIRC_proj_py, DIRC_proj_pz,
	      FOR_px, FOR_py, FOR_pz, FOR_proj_px, FOR_proj_py, FOR_proj_pz,
	      BACK_px, BACK_py, BACK_pz, BACK_proj_px, BACK_proj_py, BACK_proj_pz;

	if(atoi(argv[3])>=3){
		Tin -> SetBranchAddress("trackID"      ,&trackID      );
		Tin -> SetBranchAddress("DIRC_px"      ,&DIRC_px      );
		Tin -> SetBranchAddress("DIRC_py"      ,&DIRC_py      );
		Tin -> SetBranchAddress("DIRC_pz"      ,&DIRC_pz      );
		Tin -> SetBranchAddress("DIRC_proj_px" ,&DIRC_proj_px );
		Tin -> SetBranchAddress("DIRC_proj_py" ,&DIRC_proj_py );
		Tin -> SetBranchAddress("DIRC_proj_pz" ,&DIRC_proj_pz );
		Tin -> SetBranchAddress("FOR_px"       ,&FOR_px       );
		Tin -> SetBranchAddress("FOR_py"       ,&FOR_py       );
		Tin -> SetBranchAddress("FOR_pz"       ,&FOR_pz       );
		Tin -> SetBranchAddress("FOR_proj_px"  ,&FOR_proj_px  );
		Tin -> SetBranchAddress("FOR_proj_py"  ,&FOR_proj_py  );
		Tin -> SetBranchAddress("FOR_proj_pz"  ,&FOR_proj_pz  );
		Tin -> SetBranchAddress("BACK_px"      ,&BACK_px      );
		Tin -> SetBranchAddress("BACK_py"      ,&BACK_py      );
		Tin -> SetBranchAddress("BACK_pz"      ,&BACK_pz      );
		Tin -> SetBranchAddress("BACK_proj_px" ,&BACK_proj_px );
		Tin -> SetBranchAddress("BACK_proj_py" ,&BACK_proj_py );
		Tin -> SetBranchAddress("BACK_proj_pz" ,&BACK_proj_pz );
	}

	int nEntries = Tin -> GetEntries();

	// output file
	TString outname = "skimmed_";
	outname += argv[2];
	TFile * Fout = new TFile(outname,"recreate");
	TTree * Tout = new TTree("tracks","skimmed tree");
	Tout -> Branch("gpx"  ,&gpx  ,"gpx/F"  );
	Tout -> Branch("gpy"  ,&gpy  ,"gpy/F"  );
	Tout -> Branch("gpz"  ,&gpz  ,"gpz/F"  );
	Tout -> Branch("px"   ,&px   ,"px/F"   );
	Tout -> Branch("py"   ,&py   ,"py/F"   );
	Tout -> Branch("pz"   ,&pz   ,"pz/F"   );
	if(atoi(argv[3])==1){
		Tout -> Branch("gvz"  ,&gvz  ,"gvz/F"  );
		Tout -> Branch("pcaz" ,&pcaz ,"pcaz/F" );
		Tout -> Branch("dca2d",&dca2d,"dca2d/F");
	}
	else if(atoi(argv[3])==3){
		Tout -> Branch("trackID"      ,&trackID     ,"trackID/I"     );
		Tout -> Branch("DIRC_px"      ,&DIRC_px     ,"DIRC_px/F"     );
		Tout -> Branch("DIRC_py"      ,&DIRC_py     ,"DIRC_py/F"     );
		Tout -> Branch("DIRC_pz"      ,&DIRC_pz     ,"DIRC_pz/F"     );
		Tout -> Branch("DIRC_proj_px" ,&DIRC_proj_px,"DIRC_proj_px/F");
		Tout -> Branch("DIRC_proj_py" ,&DIRC_proj_py,"DIRC_proj_py/F");
		Tout -> Branch("DIRC_proj_pz" ,&DIRC_proj_pz,"DIRC_proj_pz/F");
		Tout -> Branch("FOR_px"       ,&FOR_px      ,"FOR_px/F"      );
		Tout -> Branch("FOR_py"       ,&FOR_py      ,"FOR_py/F"      );
		Tout -> Branch("FOR_pz"       ,&FOR_pz      ,"FOR_pz/F"      );
		Tout -> Branch("FOR_proj_px"  ,&FOR_proj_px ,"FOR_proj_px/F" );
		Tout -> Branch("FOR_proj_py"  ,&FOR_proj_py ,"FOR_proj_py/F" );
		Tout -> Branch("FOR_proj_pz"  ,&FOR_proj_pz ,"FOR_proj_pz/F" );
		Tout -> Branch("BACK_px"      ,&BACK_px     ,"BACK_px/F"     );
		Tout -> Branch("BACK_py"      ,&BACK_py     ,"BACK_py/F"     );
		Tout -> Branch("BACK_pz"      ,&BACK_pz     ,"BACK_pz/F"     );
		Tout -> Branch("BACK_proj_px" ,&BACK_proj_px,"BACK_proj_px/F");
		Tout -> Branch("BACK_proj_py" ,&BACK_proj_py,"BACK_proj_py/F");
		Tout -> Branch("BACK_proj_pz" ,&BACK_proj_pz,"BACK_proj_pz/F");
	}
	else if(atoi(argv[3])==4){
		Tout -> Branch("gvz"  ,&gvz  ,"gvz/F"  );
		Tout -> Branch("pcaz" ,&pcaz ,"pcaz/F" );
		Tout -> Branch("dca2d",&dca2d,"dca2d/F");
		Tout -> Branch("trackID"      ,&trackID     ,"trackID/I"     );
		Tout -> Branch("DIRC_px"      ,&DIRC_px     ,"DIRC_px/F"     );
		Tout -> Branch("DIRC_py"      ,&DIRC_py     ,"DIRC_py/F"     );
		Tout -> Branch("DIRC_pz"      ,&DIRC_pz     ,"DIRC_pz/F"     );
		Tout -> Branch("DIRC_proj_px" ,&DIRC_proj_px,"DIRC_proj_px/F");
		Tout -> Branch("DIRC_proj_py" ,&DIRC_proj_py,"DIRC_proj_py/F");
		Tout -> Branch("DIRC_proj_pz" ,&DIRC_proj_pz,"DIRC_proj_pz/F");
		Tout -> Branch("FOR_px"       ,&FOR_px      ,"FOR_px/F"      );
		Tout -> Branch("FOR_py"       ,&FOR_py      ,"FOR_py/F"      );
		Tout -> Branch("FOR_pz"       ,&FOR_pz      ,"FOR_pz/F"      );
		Tout -> Branch("FOR_proj_px"  ,&FOR_proj_px ,"FOR_proj_px/F" );
		Tout -> Branch("FOR_proj_py"  ,&FOR_proj_py ,"FOR_proj_py/F" );
		Tout -> Branch("FOR_proj_pz"  ,&FOR_proj_pz ,"FOR_proj_pz/F" );
		Tout -> Branch("BACK_px"      ,&BACK_px     ,"BACK_px/F"     );
		Tout -> Branch("BACK_py"      ,&BACK_py     ,"BACK_py/F"     );
		Tout -> Branch("BACK_pz"      ,&BACK_pz     ,"BACK_pz/F"     );
		Tout -> Branch("BACK_proj_px" ,&BACK_proj_px,"BACK_proj_px/F");
		Tout -> Branch("BACK_proj_py" ,&BACK_proj_py,"BACK_proj_py/F");
		Tout -> Branch("BACK_proj_pz" ,&BACK_proj_pz,"BACK_proj_pz/F");
	}

	// looping over entries
	for(int evt = 0 ; evt < nEntries ; evt++){
		if(evt%1000000==0) cout << evt << "\tout of " << nEntries << endl;
		Tin -> GetEntry(evt);
		Tout -> Fill();
	}
	Tout -> Write();
	Fout -> Close();
	Fin -> Close();

	return 0;
}
