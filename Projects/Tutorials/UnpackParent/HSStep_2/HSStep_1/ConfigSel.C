{
  TString HSANA="NEED_TO_SET_THIS"; //Path to the HaSpect directory
  gSystem->Setenv("HSANA",HSANA);
  gROOT->LoadMacro(HSANA+"/MakeHSSelector.C");

  //Set arguments for MakeHSSelector
 IsHisto=kFALSE;  //use THSHisto?
 IsAppendTree=kFALSE; //Append branches to the input tree
 IsNewTree=kFALSE;  //Output a brand new tree
 IsHSTree=kFALSE;   //Use THSOuput to THSParticle interface (probably not)
 FileName="";   // The input filename containing the tree
 TreeName="";   // The name of the tree
 OutName="";   // The name of the output directory or file
 SelName="";    // The name of the selector to be produced

 //Make the selector skeleton + control files in this directory 
 //This is based on the info given above
 MakeHSSelector();

}
