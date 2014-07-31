#define QWeight_cxx
// The class definition in QWeight.h has been generated automatically
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
// Root > T->Process("QWeight.C")
// Root > T->Process("QWeight.C","some options")
// Root > T->Process("QWeight.C+")
//

#include "QWeight.h"
#include <TH2.h>
#include <TStyle.h>


void QWeight::Begin(TTree * /*tree*/)
{
   THSOutput::HSBegin(fInput,fOutput);
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void QWeight::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   THSOutput::HSSlaveBegin(fInput,fOutput);
// Event Weighting initialisation
   fNcoord=2;//Need to set the correct number of varibles in distance calc.
   fNdisc=1;//Need to set the correct number of varibles signal/back fit
   fIsPlot=kFALSE;    //Show the fit to the nearest neigbours
   fIsSaveNN=kFALSE; //Save all the nearest nieghbours tree
   fIsLoadNN=kTRUE; //Load previously save nearest neigbours trees
   InitNN(fInput);
   if(fIsSaveNN) fOutput->Add(fTofT);
   SetupRooFit();
// End Event Weighting initialisation
  //Initialise the ouput tree
   Int_t buff=32000;
   Int_t split=0;//note split is important in the TSelector framework, if increased branches in subsequent selectors will be data members of the THSParticle object rather than the whole object (this can lead to name conflicts)
   //Only done once. THSOutput::FinishOutput Resets the tree before new files
   //If you want to save additional branches to a new file, do it here
   //If you are creating a HSParticle tree via fStrParticles then do not create a new tree here, just add the extra branches
   fOutTree=new TTree("HSParticles","A tree containing reconstructed particles"); 
  //e.g. Make some objects to make branch with, these should be declared in the .h file 
 //e.g. fp1=new TLorentzVector();
  //e.g.  fOutTree->Branch("p1",&fp1,buff,split);
    //EventWeighter make new output tree
    fOutTree->Branch("Qval",&fQval,"Qval/F");
    fOutTree->Branch("SigWidth",&fSigWidth,"SigWidth/F");
    fOutTree->Branch("SigMean",&fSigMean,"SigMean/F");
    fOutTree->Branch("SB",&fSB,"SB/F");
}

Bool_t QWeight::Process(Long64_t entry)
{
   THSOutput::HSProcessStart(entry);
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either QWeight::GetEntry() or TBranch::GetEntry()
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
   GetNNBranches(entry);//just get branches needed for weighting
   //Ready to do some analysis here, before the Fill
    MakeNNMap(); //EventWeighter make map of nearest neighbours for this event
    FillNNEvTree(entry); //EventWeighter fill tree for fitting
    FillDiscVar(entry);//get the discriminatory variable value for thsi event
    RunRooFit();  //EventWeighter do the fit
   
   THSOutput::HSProcessFill(); 


   return kTRUE;
}

void QWeight::SlaveTerminate()
{
    if(fIsSaveNN) SaveNNTree(fFile,fOutName);//EventWeighter save nearest neighbour trees
   THSOutput::HSSlaveTerminate();
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void QWeight::Terminate()
{
   THSOutput::HSTerminate();
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}
void QWeight::GetNNBranches(Long64_t nni){
 //EventWeighter function to get the branches needed for the nearest neighbour search and fit
  b_beam->GetEntry(nni);
  b_ft->GetEntry(nni);
}
void QWeight::FillCoord(TVectorD &coordV){//define the variables used in the distance calculation
 //There should be fNcoord entries
 coordV[0]=beam->P4().E();
 coordV[1]=t;
}
void QWeight::FillDiscVar(Long64_t nni){//define the variables used in the distance calculationsignal/background fit
 //There should be fNdisc entries
 b_miss->GetEntry(nni);
fDiscVar[0]=(miss->M());//missing mass
}
void QWeight::CreateNNTree(){// create the tree fNNEvTree used in the RooFit
  fNNEvTree=new TTree("NN tree","tree with NN fit variables");
  fNNEvTree->Branch("MM",&fMM,"MM/F");
}
void QWeight::FillNNEvBranches(Long64_t id){//define how the branches in the NN tree match the discriminatory variables vector
 fMM=fVdisVar[id][0];//as fDiscVar[0]=(miss->M());
}
void QWeight::SetMetric(){//define the metric use to scale the variables in the distance calculation
 //There should be fNcoord rows and columns
 fIsDiagonal=kTRUE;
 Dmetric[0][0]=1;
 Dmetric[1][1]=(1./2.)*(1./2.);
 THSEventWeighter::SetMetric();
}

