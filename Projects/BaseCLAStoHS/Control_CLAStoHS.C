{
  //Set the input files
   TChain* tree=new TChain("h10","datachain");
   // tree->Add("~dglazier/Work/Research/HaSpect/data/g11pippippim_missn/inp*_50.root");

   //Full Statistics 4th Filter Itteration

   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/*.root");
   tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_1.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_2.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_3.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_4.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_5.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_6.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_7.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_8.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_9.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_10.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_11.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_12.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_13.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_14.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_15.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_16.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_17.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_18.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_19.root");
   // tree->Add("/Disk/ecdf_thehub/thehub6/shughes/Haspect/OmegaPiPi/FullStatsFilter4/data_filtered_20.root");
  
   //Set the ouput directory or file (if full file name is given just 1 file is created, while if a directory it will be created and filled with files of the same name as the input 
   //  gSystem->Setenv("HSOUT",
   //		   "/home/dglazier/Work/Research/HaSpect/data/g11pippippim_missn_HS/");

   gSystem->Setenv("HSOUT",
		   "/phys/linux/s0677668/Haspect/OmegaPiPPiMPDataPortion/OutputTest200/Test1file3WithProof.root");
   
   //Configure the analysis (i.e PROOF, source compilation...)
   TString HSANA=gSystem->Getenv("HSANA");
   gROOT->LoadMacro(HSANA+"/HSControl.C");
   //HSControl(kFALSE); //where the proof stuff is set, kTRUE means use PROOF
   HSControl(kTRUE);
   
   //Use an event list from a previous analysis 
   //HSEntryList("~dglazier/Work/Research/HaSpect/data/pippippimMn_HS2/ParentEventList.root");
   
   //RUN
   // tree->Process("CLAStoHS.C++","pi+:pi+:pi-");
   tree->Process("CLAStoHS.C++","pi+:pi+:pi-:pi-:proton");
}
