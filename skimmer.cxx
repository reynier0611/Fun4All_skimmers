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

	if(argc!=3){
		cout << "--------------------------------------\nRun this code as:\n./skimmer /path/to/file/ filename\n--------------------------------------" << endl;
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
	Tout -> Branch("gvz"  ,&gvz  ,"gvz/F"  );
	Tout -> Branch("pcaz" ,&pcaz ,"pcaz/F" );
	Tout -> Branch("dca2d",&dca2d,"dca2d/F");

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
