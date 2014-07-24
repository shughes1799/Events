//--Author      A Celantano, DI Glazier 2014
//--Rev
//--Update
//--Description
//HASPECT Event Reconstruction
//THSEventWeighter
//Description
//To be used as part of a TSelector analysis. The selector must be given the 
//actual chain being analysed, it will then loop over the chain for each event
//being analysed and find the nearest neighbours. It will then fill a tree 
//with a discriminatory variable and fit it using RooFit. This is done for
//each event.
//The nearest neighbours search can be sped up by supplying entry lists of 
//kinematic binned events. Otherwise the speed of the code will be proportional
//to N^2
//The default fit funciton is a Gaussian with 2nd order Chebychev polynomial.
//Users can overwrite the SetupRooFit and RunRooFit functions to configure
//there own model.

#ifndef THSEventWeighter_h
#define THSEventWeighter_h

#include <TVectorD.h>
#include <TChain.h>
#include <TFile.h>
#include <TList.h>
#include <iostream>
#include "RooWorkspace.h"
using namespace RooFit;

using namespace std;

class THSEventWeighter {
 public :
  
  THSEventWeighter();
  virtual ~THSEventWeighter() {SafeDelete(fNNEvTree);SafeDelete(fRooFit);SafeDelete(fNNVdisVarP);SafeDelete(fEventEntryList);}
  
  //Nearest Neighbours functions
  Bool_t fIsDiagonal; //quick calculation if metric is diagonal
  TMatrixDSym Dmetric; //Non diagonal metric
  TVectorD    Dmetric_diagonal;  //diagonal metric
  Int_t fNcoord; //number of coordinates in metric (variables in distance calculation)
  Int_t fNdisc; //number of discriminating variables to use in signal/background fit

  UInt_t fNmax; //Maximum number of neighbours to be kept
  TVectorD fCoord0;
  TVectorD fCoordi;
  TVectorD fDisVar0;
  vector< TVectorD > fVcoord; //vector of coordinates to store all events
  TVectorD fDiscVar; //Discriminatory variables for event
  vector< TVectorD > fVdisVar; //vector of disc. variables to store for all events
  vector<TVectorD> *fNNVdisVarP;//vector of disc. variables to load for NN events;
  
  vector< TVectorD > fNNVdisVar; //vector of disc. variables to store for NN events
  
  TChain *fNNChain; //chan connecting to full event list being anlsysed
  TChain *fNNChainLoad; //chain containing saved nearest neighbour trees
  Long64_t fOffNNChain;//offset entry number for start of current tree
  TTree *fSaveChain; // temp pointer to local tree (only contains subset of full events
  TTree* fNNEvTree; //Tree to store the fNMax nearest neighbours for an event
  TTree* fTofT; //Tree to store NNEv trees for each event
  map<Float_t,Int_t> fNNmap; //std::map orders entries based on the first (key)
  Int_t fSufficient; //if we have enough events within this distance then go on with fit

  virtual void InitNN(TList* input); //initialise nearest neighbours algorithm
  Float_t Distance2(const TVectorD &vi,const TVectorD &vj); //use metric to find distance between two points

  virtual void FillCoord(TVectorD &coordV) =0; //Define how coordinates are to be filled
  virtual void FillDiscVar(Long64_t nni) =0; //define discriminatory variables
  virtual void FillNNEvBranches(Long64_t id)=0; //define how to fill the NN tree branches
  virtual void GetNNBranches(Long64_t nni)=0; //get the branches needed to fill coordinates and discriminatry variables
  virtual void CreateNNTree(); //create tree and set branches for RooFit
  void SetCoord0(TVectorD &coordV){fCoord0=coordV;};

  virtual void SetMetric();  //Define the metric used to normalise distance variables
  void MakeNNMap(); //loop over chain and find nearest neighbours
  void FillNNEvTree(Long64_t entry); //Fill the near neighbours tree for fitting
  void  LoadNNEvTree(Long64_t entry); //Load previously saved event
  void SaveNNTree(TFile* file,TString OutName); //save each events nearest neighbour tree
  void NotifyNNTree(TTree* tree); //find the entry offset when loading NN trees
  Bool_t fIsSaveNN; //are we going to save the nearest neighours tree for each event
  Bool_t fIsLoadNN; //are we going to load the nearest neighours tree for each event
  //EntryLists for resticting nearest neighbours hunt
  TList* fEventEntryList;
  TH1* fKinBins;
  TEntryList* fCurKinBinList;
  void LoadEventEntryLists(TString filename,TString hbname);
  void SetEventEntryList(Float_t ev1,Float_t ev2=0,Float_t ev3=0);
  //Fitting functions
  virtual void  SetupRooFit(); //define your roofit model
  virtual void  RunRooFit();   // do the fit


  //RooFit members
  RooWorkspace* fRooFit; //keeps hold of all the RooFit stuff
  Bool_t fIsPlot; //show the fit plotfor each event

 protected:
  //variables for writing
  Float_t fQval;
  Float_t fSB;
  Float_t fSigWidth;
  Float_t fSigMean;
  
   //For NN fit tree
  Float_t         fMM;

   
};


#endif 
