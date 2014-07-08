//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jul  7 20:53:13 2014 by ROOT version 5.34/14
// from TTree HSParticles/A tree containing reconstructed particles
// found on file: /home/dglazier/Work/Research/HaSpect/data/g11pippippim_missn_HS/inp2_50.root
//////////////////////////////////////////////////////////

#ifndef ThreePiDecay_h
#define ThreePiDecay_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include "THSOutput.h"

// Header file for the classes stored in the TTree if any.
#include <TLorentzVector.h>

// Fixed size dimensions of array or collections stored in the TTree if any.

class ThreePiDecay : public TSelector, public THSOutput {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   THSParticle     *pim_0;
   THSParticle     *pip_0;
   THSParticle     *pip_1;
   THSParticle     *beam;
   TLorentzVector  *miss;
   TLorentzVector  *ThreePi;
   TLorentzVector  *TwoPiF;
   TLorentzVector  *TwoPiS;
   Float_t         t;
  //data members for new branches
  //you must define how they are processed for each event
  //e.g.   TLorentzVector  *fp1;
   TLorentzVector  *fTwoPiCM;
   TLorentzVector  *fOnePiCM;

   // List of branches
   TBranch        *b_pim_0;   //!
   TBranch        *b_pip_0;   //!
   TBranch        *b_pip_1;   //!
   TBranch        *b_beam;   //!
   TBranch        *b_miss;   //!
   TBranch        *b_ThreePi;   //!
   TBranch        *b_TwoPiF;   //!
   TBranch        *b_TwoPiS;   //!
   TBranch        *b_ft;   //!

   ThreePiDecay(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~ThreePiDecay() { SafeDelete(fTwoPiCM);SafeDelete(fOnePiCM);}
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

   ClassDef(ThreePiDecay,0);
};

#endif

#ifdef ThreePiDecay_cxx
void ThreePiDecay::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   pim_0 = 0;
   pip_0 = 0;
   pip_1 = 0;
   beam = 0;
   miss = 0;
   ThreePi = 0;
   TwoPiF = 0;
   TwoPiS = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("pim_0", &pim_0, &b_pim_0);
   fChain->SetBranchAddress("pip_0", &pip_0, &b_pip_0);
   fChain->SetBranchAddress("pip_1", &pip_1, &b_pip_1);
   fChain->SetBranchAddress("beam", &beam, &b_beam);
   fChain->SetBranchAddress("miss", &miss, &b_miss);
   fChain->SetBranchAddress("ThreePi", &ThreePi, &b_ThreePi);
   fChain->SetBranchAddress("TwoPiF", &TwoPiF, &b_TwoPiF);
   fChain->SetBranchAddress("TwoPiS", &TwoPiS, &b_TwoPiS);
   fChain->SetBranchAddress("t", &t, &b_ft);
}

Bool_t ThreePiDecay::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.
   THSOutput::HSNotify(fChain);

   return kTRUE;
}

#endif // #ifdef ThreePiDecay_cxx
