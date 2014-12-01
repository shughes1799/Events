//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Nov 26 14:36:51 2014 by ROOT version 5.34/14
// from TTree Simple/A tree containing reconstructed variables
// found on file: /home/dglazier/Work/Research/HaSpect/data/g11pippippim_missn_11b_14/Weighted/inp1_50.root
//////////////////////////////////////////////////////////

#ifndef sWeighter_h
#define sWeighter_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include "THSOutput.h"
// use this order for safety on library loading
#include "RooGlobalFunc.h"
#include "RooStats/SPlot.h"
using namespace RooFit ;
using namespace RooStats ;

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class sWeighter : public TSelector, public THSOutput {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Float_t         Mmiss;
   Float_t         M2pif;
   Float_t         M2pis;
   Float_t         M3pi;
   Float_t         Eg;
   Float_t         t;
   Int_t           fgID;
  //data members for new branches
  //you must define how they are processed for each event
  //e.g.   TLorentzVector  *fp1;

   // List of branches
   TBranch        *b_Mmiss;   //!
   TBranch        *b_M2pif;   //!
   TBranch        *b_M2pis;   //!
   TBranch        *b_M3pi;   //!
   TBranch        *b_Eg;   //!
   TBranch        *b_t;   //!
   TBranch        *b_fgID;   //!

   sWeighter(TTree * /*tree*/ =0) : fChain(0),fCurrSW(0),fSWFile(0),fsPlotList(0),fSWKinBins(0) { }
   virtual ~sWeighter() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();
   //Data members for reading sPlots
   RooStats::SPlot* fCurrSW;//sPlot object for current event, depends on kinematic binning
   TFile *fSWFile;//File containing the sPlots produced by THS_sWeights.C
   TObjArray* fsPlotList;//Array of sPlots for each kinematic bin
   vector<Long64_t> fSEntry;//Counters for each sPlot kinematic bin, needed to synch chain with splots
   Float_t fSigW;//signal weight, note in general splots may produce more than 2 types of event in which case additional weights will be needed here
   Float_t fBckW;//background weight
   TH1* fSWKinBins;//Histogram defining kinematic bins (if used) for sPlots
   Int_t fSWBin; //ID for current SPlot kinematic bin
   void SetsPlot(Float_t ev1,Float_t ev2=0,Float_t ev3=0); //Function to find the sPlot for the event
   Bool_t GetsWeight(); //function which asigns the weight for this event from sPlot object
//Add THSHisto functions
   virtual void HistogramList(TString sLabel);
   virtual void FillHistograms(TString sCut,Int_t bin);

   ClassDef(sWeighter,0);
};

#endif

#ifdef sWeighter_cxx
void sWeighter::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Mmiss", &Mmiss, &b_Mmiss);
   fChain->SetBranchAddress("M2pif", &M2pif, &b_M2pif);
   fChain->SetBranchAddress("M2pis", &M2pis, &b_M2pis);
   fChain->SetBranchAddress("M3pi", &M3pi, &b_M3pi);
   fChain->SetBranchAddress("Eg", &Eg, &b_Eg);
   fChain->SetBranchAddress("t", &t, &b_t);
   fChain->SetBranchAddress("fgID", &fgID, &b_fgID);
}

Bool_t sWeighter::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.
  THSOutput::HSNotify(fChain);
  THSOutput::InitOutTree();

   return kTRUE;
}
void sWeighter::SetsPlot(Float_t ev1,Float_t ev2,Float_t ev3){
  //Function that finds the bin number and uses it to find the correct sPlot   
   fSWBin=0;
      if(fSWKinBins){//can only find bin if have histogram which defines them
      fSWBin=fSWKinBins->FindBin(ev1,ev2,ev3); //find bin
   if(fsPlotList) fCurrSW=dynamic_cast<RooStats::SPlot*>(fsPlotList->At(fSWBin));//note dynamic cast returns NULL if object can't be cast
     }
  //if no bins defined get just the first sPlot from the list once 
    else if(!fCurrSW)  fCurrSW=dynamic_cast<RooStats::SPlot*>(fsPlotList->At(0));
}

#endif // #ifdef sWeighter_cxx
