#define sWeighter_cxx
// The class definition in sWeighter.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("sWeighter.C")
// Root > T->Process("sWeighter.C","some options")
// Root > T->Process("sWeighter.C+")
//

#include "sWeighter.h"
#include <TH2.h>
#include <TStyle.h>


void sWeighter::Begin(TTree * /*tree*/)
{
   THSOutput::HSBegin(fInput,fOutput);
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void sWeighter::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   THSOutput::HSSlaveBegin(fInput,fOutput);
   //Get the sPlot, this should be a list containing an splot for each kinematic bin
   //This will have been produced by the THS_sWeight macro
   TDirectory* savedir=gDirectory;
   fSWFile=new TFile(option);//take the filename from the tree->Process() option
   fsPlotList=(TObjArray*)fSWFile->Get("HSsPlots");//List should have name HSsPlots
   if(!fsPlotList){cerr<<"Sorry no sPlots found in SlaveBegin, exiting"<<endl;exit(0);}
   fSEntry.assign(fsPlotList->GetEntries(),0);//initiate kinematic bin counters
   fSWKinBins=(TH1*)fSWFile->Get("HSsPlotsBins");//get histogram defining SW bins
   savedir->cd();
   fSWBin=0;
  //Initialise the ouput tree
   Int_t buff=32000;
   Int_t split=0;//note split is important in the TSelector framework, if increased branches in subsequent selectors will be data members of the THSParticle object rather than the whole object (this can lead to name conflicts)
   //Only done once. THSOutput::FinishOutput Resets the tree before new files
   //If you want to save additional branches to a new file, do it here
   //If you are creating a HSParticle tree via fStrParticles then do not create a new tree here, just add the extra branches
   fOutTree=new TTree("sWeights","A tree containing reconstructed particles"); 
  //e.g. Make some objects to make branch with, these should be declared in the .h file 
 //e.g. fp1=new TLorentzVector();
  //e.g.  fOutTree->Branch("p1",&fp1,buff,split);
   //sWeighter make new output tree
   fOutTree->Branch("SigW",&fSigW,"SigW/F");
   fOutTree->Branch("BckW",&fBckW,"BckW/F");
  //if you want kinematic bins you must define fHisbins here
   //fHisbins=new TH*()
   if(fHisbins) fOutput->Add(fHisbins);
   //fHisbins->SetXTitle();//give useful axis name
   //fHisbins->SetYTitle();//give useful axis name
   THSHisto::ChangeNames();
   THSHisto::LoadCut("Cut1");
   THSHisto::LoadHistograms();
}

Bool_t sWeighter::Process(Long64_t entry)
{
   THSOutput::HSProcessStart(entry);
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either sWeighter::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
   GetEntry(entry); //lazy and slow, you can speed the code up by getting the branches you need to use instead
   //Ready to do some analysis here, before the Fill
   if(!GetsWeight()) return kTRUE; //check if this event is in the sPlot
   
   //Int_t kinBin=GetKinBin();//if fHisbins is defined need to give this meaningful arguments
   //FillHistograms("Cut1",kinBin);
   FillHistograms("Cut1",0);
   //EnterKinBinList(kinBin,entry);//save evente in kinematic bins entry lists
   THSOutput::HSProcessFill(); 


   return kTRUE;
}

void sWeighter::SlaveTerminate()
{
   THSOutput::HSSlaveTerminate();
   fSWFile->Close();
   delete fSWFile;
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void sWeighter::Terminate()
{
   THSOutput::HSTerminate();
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}
void sWeighter::HistogramList(TString sLabel){
  TDirectory::AddDirectory(kFALSE); //do not add to current directory
  //now define all histograms and add to Output
  //label includes kinematic bin and additional cut name
  // e.g fOutput->Add(MapHist(new TH1F("Mp1"+sLabel,"M_{p1}"+sLabel,100,0,2)));
 fOutput->Add(MapHist(new TH1F("SigM2piF"+sLabel,"Sig M_{2#piF}"+sLabel,100,0.2,1.5)));
 fOutput->Add(MapHist(new TH1F("SigM2piS"+sLabel,"Sig M_{2#piS}"+sLabel,100,0.2,1.5)));
 fOutput->Add(MapHist(new TH1F("SigM3pi"+sLabel,"Sig M_{3#pi}"+sLabel,100,0.5,2)));

 fOutput->Add(MapHist(new TH1F("BckM2piF"+sLabel,"Bck M_{2#piF}"+sLabel,100,0.2,1.5)));
 fOutput->Add(MapHist(new TH1F("BckM2piS"+sLabel,"Bck M_{2#piS}"+sLabel,100,0.2,1.5)));
 fOutput->Add(MapHist(new TH1F("BckM3pi"+sLabel,"Bck M_{3#pi}"+sLabel,100,0.5,2)));

 fOutput->Add(MapHist(new TH1F("M2piFgt1"+sLabel,"Bck M_{2#piF}"+sLabel,100,0.2,1.5)));
 fOutput->Add(MapHist(new TH1F("M2piSgt1"+sLabel,"Bck M_{2#piS}"+sLabel,100,0.2,1.5)));
 fOutput->Add(MapHist(new TH1F("M3pigt1"+sLabel,"Bck M_{3#pi}"+sLabel,100,0.5,2)));
   //end of histogram list
  TDirectory::AddDirectory(kTRUE); //back to normal
}
void sWeighter::FillHistograms(TString sCut,Int_t bin){
  fCurrCut=sCut;
  fCurrBin=bin;
  //Get histogram from list
  //Fill histogram
  TString sLabel;
  sLabel=sCut+fVecBinNames[bin];
  // e.g. FindHist("Mp1"+sLabel)->Fill(fp1->M());
  //Histograms with signal weight
  FindHist("SigM2piF"+sLabel)->Fill(M2pif,fSigW);
  FindHist("SigM2piS"+sLabel)->Fill(M2pis,fSigW);
  FindHist("SigM3pi"+sLabel)->Fill(M3pi,fSigW);
  //Histograms with background weight
  FindHist("BckM2piF"+sLabel)->Fill(M2pif,fBckW);
  FindHist("BckM2piS"+sLabel)->Fill(M2pis,fBckW);
  FindHist("BckM3pi"+sLabel)->Fill(M3pi,fBckW);
  if(Mmiss>1){
    FindHist("M2piFgt1"+sLabel)->Fill(M2pif);
    FindHist("M2piSgt1"+sLabel)->Fill(M2pis);
    FindHist("M3pigt1"+sLabel)->Fill(M3pi);
  }
}

Bool_t sWeighter::GetsWeight(){
  //Function to get the correct sPlot
  //Then find the sWeights for this event
   if(fSWKinBins)SetsPlot(0,0,0); //get the SW bin for this event, need to replace 0s by real variable...,
   else SetsPlot(0);//not using kin bins, just 1 sPlot
   // The next 2 lines are required to check synchronisation with parent tree
   // This allows for filtering events while performing the sWeight fit
   // The events output from this selector will all have been included in the fit
    if(fCurrSW->GetSDataSet()->get(fSEntry[fSWBin]))
     if((Int_t)(fCurrSW->GetSDataSet()->get(fSEntry[fSWBin])->getRealValue("fgID"))!=fgID) return kFALSE; //this event is not in the sPlot
   //Now get the weights, by default assume signal and background types only
   if(fCurrSW&&fCurrSW->GetSDataSet()->get(fSEntry[fSWBin])){
     fSigW=fCurrSW->GetSWeight(fSEntry[fSWBin],"SigYield") ;//SigYield is name given in THS_sWeight
     fBckW=fCurrSW->GetSWeight(fSEntry[fSWBin],"BckYield") ;
     fSEntry[fSWBin]++; //increment the sWeight counter for this kinematic bin
   }
   else{
     fSigW=0;fBckW=0;}

   return kTRUE;//got a weight
}
