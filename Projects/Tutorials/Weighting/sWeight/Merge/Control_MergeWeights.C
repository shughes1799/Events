{
  //Set up the input chain to be analysed
   TChain* tree=new TChain("sWeights","datachain");
   tree->Add("/home/dglazier/Work/Research/HaSpect/data/g11pippippim_missn_11_14/Weighted/sWeights/inp*root");
   //Set the ouput directory or file (if full file name is given just 1 file is created, while if a directory it will be created and filled with files of the same name as the input 
   gSystem->Setenv("HSOUT",
		   "/home/dglazier/Work/Research/HaSpect/data/g11pippippim_missn_11_14/Weighted/sWeights/Merged");
   
   //Configure the analysis (i.e PROOF, source compilation...)
   TString HSANA=gSystem->Getenv("HSANA");
   gROOT->LoadMacro(HSANA+"/HSControl.C");
   TProof* prf=HSControl(kTRUE); //where the proof stuff is set, kTRUE means use PROOF
   
   //Use an event list from a previous analysis 
   //If only 1 file was output the event list will be in its top directory, so just give the file name
   //If many files were ouput the event list will be in the same directory with the name ParentEventList.root
   //HSEntryList("/PATH TO PERVIOUS EVENTLIST/ParentEventList.root");
  //we are linking to a parent selector so need to load its source code here
  gROOT->LoadMacro("Simplify.C++");
  if(prf) prf->Load("Simplify.C+");

   tree->Process("MergeWeights.C++");

   //Not actually sure if this is needed or not...
   if(prf)prf->ClearData(TProof::kUnregistered | TProof::kForceClear);
 }
