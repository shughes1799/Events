//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jul 22 13:09:45 2014 by ROOT version 5.34/14
// from TTree HSParticles/A tree containing reconstructed particles
// found on file: /home/dglazier/Work/Research/HaSpect/data/g11pippippim_missn_HSID/inp1_50.root
//////////////////////////////////////////////////////////

#ifndef QWeight_h
#define QWeight_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include "THSEventWeighter.h"
#include "THSOutput.h"

// Header file for the classes stored in the TTree if any.
#include <TLorentzVector.h>

// Fixed size dimensions of array or collections stored in the TTree if any.

class QWeight : public TSelector, public THSEventWeighter , public THSOutput {
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
   Int_t           fgID;
  //data members for new branches
  //you must define how they are processed for each event
  //e.g.   TLorentzVector  *fp1;

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
   TBranch        *b_fgID;   //!

   QWeight(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~QWeight() { }
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
   //Event Weighter functions to be defined by user
   virtual void FillCoord(TVectorD &coordV); //Define how coordinates are to be filled
   virtual void FillDiscVar(Long64_t nni); //define discriminatory variables
virtual void CreateNNTree();// create the tree fNNEvTree used in the RooFit
   virtual void FillNNEvBranches(Long64_t id); //define how to fill the NN tree branches
   virtual void GetNNBranches(Long64_t nni); //get the branches needed to fill coordinates and discriminatry variables
   virtual void SetMetric();  //Define the metric used to normalise distance variables;

   ClassDef(QWeight,0);
};

#endif

#ifdef QWeight_cxx
void QWeight::Init(TTree *tree)
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
   fChain->SetBranchAddress("fgID", &fgID, &b_fgID);
}

Bool_t QWeight::Notify()
{
    if(fIsSaveNN) SaveNNTree(fFile,fOutName);//EventWeighter save nearest neighbour trees
  if(fIsLoadNN) NotifyNNTree(fChain);//EventWeighter load nearest neighbour trees, get the entry offset for this tree fChain
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.
   THSOutput::HSNotify(fChain);
  //if it exists give the tree to the file
  if(fIsSaveNN){
     fTofT->SetDirectory(fFile);
     fTofT->AutoSave();
  }

   return kTRUE;
}

#endif // #ifdef QWeight_cxx
