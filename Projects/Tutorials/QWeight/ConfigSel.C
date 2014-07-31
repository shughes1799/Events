{
  //You need to have the enviroment variable HSANA defined to link to the THS source
  TString HSANA(gSystem->Getenv("HSANA"));
  gROOT->LoadMacro(HSANA+"/MakeHSSelector.C");

  //Set arguments for MakeHSSelector
 IsHisto=kFALSE;  //use THSHisto?
 IsAppendTree=kTRUE; //Append branches to the input tree
 IsNewTree=kFALSE;  //Output a brand new tree
 IsHSTree=kFALSE;   //Use THSOuput to THSParticle interface (probably not)
 IsQval=kTRUE;   //Use Qvalue event weighting algorithm

 FileName="/home/dglazier/Work/Research/HaSpect/data/g11pippippim_missn_HSID/inp1_50.root";   // The input filename containing the tree
 TreeName="HSParticles";   // The name of the tree
 OutName="/home/dglazier/Work/Research/HaSpect/data/test";   // The name of the output directory or file
 SelName="QWeight";    // The name of the selector to be produced

 //IsParent=kTRUE; //use connection with parent tree
 //StepN=0;        // If connecting with parent, which step number is its selector in
 //ParSel="";

 //Make the selector skeleton + control files in this directory 
 //This is based on the info given above
 MakeHSSelector();

}
