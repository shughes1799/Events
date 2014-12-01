//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Nov  5 14:45:23 2014 by ROOT version 5.34/14
// from TTree sWeights/A tree containing reconstructed particles
// found on file: /home/dglazier/Work/Research/HaSpect/data/g11pippippim_missn_HSID2/Weighted/sWeights/inp1_50.root
//////////////////////////////////////////////////////////

#ifndef MergeWeights_h
#define MergeWeights_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include "THSOutput.h"
#include "Simplify.h"

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class MergeWeights : public TSelector, public THSOutput {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Simplify   fParent;

   // Declaration of leaf types
   Float_t         SigW;
   Float_t         BckW;
   Int_t           fgID;
  //data members for new branches
  //you must define how they are processed for each event
  //e.g.   TLorentzVector  *fp1;

   // List of branches
   TBranch        *b_SigW;   //!
   TBranch        *b_BckW;   //!
   TBranch        *b_fgID;   //!

   MergeWeights(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~MergeWeights() { }
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

   ClassDef(MergeWeights,0);
};

#endif

#ifdef MergeWeights_cxx
void MergeWeights::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
    // (once per file to be processed).
  b_SigW=0;
  b_BckW=0;
  // b_fgID=0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("SigW", &SigW, &b_SigW);
   fChain->SetBranchAddress("BckW", &BckW, &b_BckW);
   fChain->SetBranchAddress("fgID", &fgID, &b_fgID);
}

Bool_t MergeWeights::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.
   THSOutput::HSNotify(fChain);
   THSOutput::InitParent(fChain,"HSStep_2/HSStep_1");
   fParent.Init(fParentTree);
  //If we want to clone the input tree we have to do it here as fChain does not exist in SlaveBegin on PROOF
  if(!fOutTree){//First file, clone the input tree and add branches
   Int_t buff=32000;
   Int_t split=0;//note split is important in the TSelector framework, if increased branches in subsequent selectors will be data members of the THSParticle object rather than the whole object (this can lead to name conflicts)

   fOutTree=fChain->CloneTree(0);
    //e.g. fp1=new TLorentzVector(); //should be declared as class data member
    //e.g. fOutTree->Branch("p1",&fp1,buff,split);
   fOutTree->Branch("beam",&(fParent.beam),buff,split);
   fOutTree->Branch("ThreePi",&(fParent.ThreePi),buff,split);
   fOutTree->Branch("TwoPiF",&(fParent.TwoPiF),buff,split);
   fOutTree->Branch("TwoPiS",&(fParent.TwoPiS),buff,split);
   fOutTree->Branch("miss",&(fParent.miss),buff,split);
  }
  else {fChain->CopyAddresses(fOutTree);}//reset the branch addresses of the cloned tree to the new file tree
  THSOutput::InitOutTree();
  return kTRUE;
}

#endif // #ifdef MergeWeights_cxx
