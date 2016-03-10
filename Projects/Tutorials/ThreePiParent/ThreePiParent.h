//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jul 22 09:12:37 2014 by ROOT version 5.34/14
// from TTree HSParticles/A tree containing reconstructed particles
// found on file: /home/dglazier/Work/Research/HaSpect/data/g11pippippim_missn_HSID/Decay/inp2_50.root
//////////////////////////////////////////////////////////

#ifndef ThreePiParent_h
#define ThreePiParent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include "THSOutput.h"
#include "ThreePiDecay.h"

// Header file for the classes stored in the TTree if any.
#include <TLorentzVector.h>

// Fixed size dimensions of array or collections stored in the TTree if any.

class ThreePiParent : public TSelector, public THSOutput {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   ThreePiDecay   fParent;

   // Declaration of leaf types
   TLorentzVector  *TwoPiCM;
   TLorentzVector  *OnePiCM;
   Int_t           fgID;

   // List of branches
   TBranch        *b_TwoPiCM;   //!
   TBranch        *b_OnePiCM;   //!
   TBranch        *b_fgID;   //!

   ThreePiParent(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~ThreePiParent() { }
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
//Add THSHisto functions
   virtual void HistogramList(TString sLabel);
   virtual void FillHistograms(TString sCut,Int_t bin);

   ClassDef(ThreePiParent,0);
};

#endif

#ifdef ThreePiParent_cxx
void ThreePiParent::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   TwoPiCM = 0;
   OnePiCM = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("TwoPiCM", &TwoPiCM, &b_TwoPiCM);
   fChain->SetBranchAddress("OnePiCM", &OnePiCM, &b_OnePiCM);
   fChain->SetBranchAddress("fgID", &fgID, &b_fgID);
}

Bool_t ThreePiParent::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.
   THSOutput::HSNotify(fChain);
   THSOutput::InitOutTree();
   THSOutput::InitParent(fChain,"HSStep_1");
   fParent.Init(fParentTree);

   return kTRUE;
}

#endif // #ifdef ThreePiParent_cxx
