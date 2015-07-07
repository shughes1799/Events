{
  //Set the input files
   TChain* tree=new TChain("h10","datachain");
   // tree->Add("~dglazier/Work/Research/HaSpect/data/g11pippippim_missn/inp*_50.root");

   tree->Add("/Disk/ds-sopa-group/np/thehubdata/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_1.root");
   tree->Add("/Disk/ds-sopa-group/np/thehubdata/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_2.root");
   tree->Add("/Disk/ds-sopa-group/np/thehubdata/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_3.root");
  
   //Set the ouput directory or file (if full file name is given just 1 file is created, while if a directory it will be created and filled with files of the same name as the input 
   // gSystem->Setenv("HSOUT",
   //		   "/home/dglazier/Work/Research/HaSpect/data/g11pippippim_missn_HS/");
   gSystem->Setenv("HSOUT",
		  "/phys/linux/s0677668/Haspect/OmegaPiPPiMPDataPortion/2015/July/Junk/TestStats19.root");
   //Configure the analysis (i.e PROOF, source compilation...)
   TString HSANA=gSystem->Getenv("HSANA");
   gROOT->LoadMacro(HSANA+"/HSControl.C");
   // HSControl(kFALSE); //where the proof stuff is set, kTRUE means use PROOF
   HSControl(kTRUE);
   //Use an event list from a previous analysis 
   //HSEntryList("~dglazier/Work/Research/HaSpect/data/pippippimMn_HS2/ParentEventList.root");
   
   //RUN
   // tree->Process("CLAStoHS.C++","pi+:pi+:pi-");
   tree->Process("CLAStoHS.C++","pi+:pi+:pi-:pi-:proton");
}
