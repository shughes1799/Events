#define CLAStoHS_cxx
// The class definition in CLAStoHS.h has been generated automatically
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
// Root > T->Process("CLAStoHS.C")
// Root > T->Process("CLAStoHS.C","some options")
// Root > T->Process("CLAStoHS.C+")
//

#include "CLAStoHS.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
using namespace std;

TLorentzVector gTarget(0,0,0,9.38271999999999995e-01);
/*
  Note, I can choose to either ignore or count ghost tracks when considering events in the final state
  By default it ignores them by calling the function IsGoodEventGhosts();
  To count them you should comment this out and use IsGoodEvent() instead in Process()
  //if(!IsGoodEvent()) return kTRUE; //not the event we want so exit
  if(!IsGoodEventGhosts()) return kTRUE; //not the event we want so exit
*/
CLAStoHS::~CLAStoHS(){

  SafeDelete(fHSgamma);
  SafeDelete(fMissing);

  SafeDelete(fOmegaCand1); //Added for fOmega Candidate 1
  SafeDelete(fOmegaCand2); //Added for fOmega Candidate 2
  SafeDelete(fOmegaCand3); //Added for fOmega Candidate 3
  SafeDelete(fOmegaCand4); //Added for fOmega Candidate 4
  SafeDelete(fPionComb1); //Added for fPionComb 1
  SafeDelete(fPionComb2); //Added for fPionComb 2
  SafeDelete(fPionComb3); //Added for fPionComb 3
  SafeDelete(fPionComb4); //Added for fPionComb 4
  SafeDelete(fBeamEnergySelection); //Added for selecting beam energies of interest
  SafeDelete(fProton); //Added for looking at the scattered proton

  SafeDelete(fPiMFast0); // The Fast Pi Minus
  SafeDelete(fPiMSlow1); // The Slow Pi Minus
  SafeDelete(fPiPFast2); // The Fast Pi Plus
  SafeDelete(fPiPSlow3); // The Slow Pi Plus

  SafeDelete(fFastPiMPi0); //Combination of the Fast Pi Minus and the Pi0
  SafeDelete(fSlowPiMPi0); //Combination of the Slow Pi Minus and the Pi0
  SafeDelete(fFastPiPPi0); //Combination of the Fast Pi Plus and the Pi0
  SafeDelete(fSlowPiPPi0); //Combination of the Slow Pi Plus and the Pi0

  SafeDelete(fOmegaCand1PiMSlow1); //Combination of the Omega and Slow Pi Minus
  SafeDelete(fOmegaCand1PiPSlow3); //Combination of the Omega and Slow Pi Minus
  SafeDelete(fOmegaCand2PiMSlow1); //Combination of the Omega and Slow Pi Plus
  SafeDelete(fOmegaCand2PiPFast2); //Combination of the Omega and Fast Pi Plus
  SafeDelete(fOmegaCand3PiMFast0); //Combination of the Omega and Fast Pi Minus
  SafeDelete(fOmegaCand3PiPSlow3); //Combination of the Omega and Slow Pi Minus
  SafeDelete(fOmegaCand4PiMFast0); //Combination of the Omega and Fast Pi Plus
  SafeDelete(fOmegaCand4PiPFast2); //Combination of the Omega and Fast Pi Plus
 
  SafeDelete(total); //Histograms for the getweight function
  SafeDelete(g2);    //Histograms for the getweight function

}
void CLAStoHS::Begin(TTree * /*tree*/)
{
  
  TString option = GetOption();
  THSOutput::HSBegin(fInput,fOutput);
}

void CLAStoHS::SlaveBegin(TTree * /*tree*/)
{
  nTot       = 0;
  nGoodOne   = 0;    
  nGoodTwo   = 0;  
  nGoodThree = 0;
  nGoodFour  = 0;
  nGoodFive  = 0;
  nGoodSix   = 0;
  
  TString option = GetOption();
  fStrParticles=option;
  //fStrParticles="pi+:pi+:pi-";
  THSOutput::HSSlaveBegin(fInput,fOutput);
  
  //Output Tree is initialised can now add further branches if needed
  //note you are creating these yourself and so are responsible for deleting them
  //You will also have to declare the objects in the .h file
  Int_t buff=32000;
  Int_t split=0;//note split is important in the TSelector framework, if increased branches in subsequent selectors will be data members of the THSParticle object rather than the whole object (this can lead to name conflicts)
  fHSgamma=new THSParticle(22); //beam
  fMissing=new TLorentzVector(); //missing
  fOmegaCand1=new TLorentzVector(); //omega candidate1
  fOmegaCand2=new TLorentzVector(); //omega candidate2
  fOmegaCand3=new TLorentzVector(); //omega candidate3
  fOmegaCand4=new TLorentzVector(); //omega candidate4
  fPionComb1=new TLorentzVector(); //Other Pion Combination 1
  fPionComb2=new TLorentzVector(); //Other Pion Combination 2
  fPionComb3=new TLorentzVector(); //Other Pion Combination 3
  fPionComb4=new TLorentzVector(); //Other Pion Combination 4
  fBeamEnergySelection=new TLorentzVector();
  fProton=new TLorentzVector(); //Scattered Proton

  fPiMFast0=new TLorentzVector(); // The Fast Pi Minus
  fPiMSlow1=new TLorentzVector(); // The Slow Pi Minus
  fPiPFast2=new TLorentzVector(); // The Fast Pi Plus
  fPiPSlow3=new TLorentzVector(); // The Slow Pi Plus

  fFastPiMPi0=new TLorentzVector(); //Combination of the Fast Pi Minus and the Pi0
  fSlowPiMPi0=new TLorentzVector(); //Combination of the Slow Pi Minus and the Pi0
  fFastPiPPi0=new TLorentzVector(); //Combination of the Fast Pi Plus and the Pi0
  fSlowPiPPi0=new TLorentzVector(); //Combination of the Slow Pi Plus and the Pi0

  fOmegaCand1PiMSlow1=new TLorentzVector(); //Combination of the Omega and Slow Pi Minus
  fOmegaCand1PiPSlow3=new TLorentzVector(); //Combination of the Omega and Slow Pi Minus
  fOmegaCand2PiMSlow1=new TLorentzVector(); //Combination of the Omega and Slow Pi Plus
  fOmegaCand2PiPFast2=new TLorentzVector(); //Combination of the Omega and Fast Pi Plus
  fOmegaCand3PiMFast0=new TLorentzVector(); //Combination of the Omega and Fast Pi Minus
  fOmegaCand3PiPSlow3=new TLorentzVector(); //Combination of the Omega and Slow Pi Minus
  fOmegaCand4PiMFast0=new TLorentzVector(); //Combination of the Omega and Fast Pi Plus
  fOmegaCand4PiPFast2=new TLorentzVector(); //Combination of the Omega and Fast Pi Plus


  //Histograms for the getweight function
  total = new TF1("total","pol4(0)+gaus(5)",0.63,0.95);
  g2    = new TF1("g2","gaus",0.74,0.82);


  //General Cut Variables
  MissingMCLow = 0.105;
  MissingMCHigh = 0.170;
  OmegaMCLow = 0.65;
  OmegaMCHigh = 0.92;
  EtaMCLow = 0;
  EtaMChigh = 0;

  //Combination 1 variables
  Comb1MissingMCLow = 0.11;
  Comb1MissingMCHigh = 0.16;
  Comb1OmegaMCLow = 0.76;
  Comb1OmegaMCHigh = 0.80;
  Comb1EtaMCLow = 0;
  Comb1EtaMChigh = 0;

  //Combination 2 variables
  Comb2MissingMCLow = 0.11;
  Comb2MissingMCHigh = 0.16;
  Comb2OmegaMCLow = 0.76;
  Comb2OmegaMCHigh = 0.80;
  Comb2EtaMCLow = 0;
  Comb2EtaMChigh = 0;

  //Combination 3 variables
  Comb3MissingMCLow = 0.11;
  Comb3MissingMCHigh = 0.16;
  Comb3OmegaMCLow = 0.76;
  Comb3OmegaMCHigh = 0.80;
  Comb3EtaMCLow = 0;
  Comb3EtaMChigh = 0;

  //Combination 4 variables
  Comb4MissingMCLow = 0.11;
  Comb4MissingMCHigh = 0.16;
  Comb4OmegaMCLow = 0.76;
  Comb4OmegaMCHigh = 0.80;
  Comb4EtaMCLow = 0.53;
  Comb4EtaMChigh = 0.57;


  // //Combination Parameters
  // double Par1[7] = {93577.99,-23139.8,43067.9,-23129.5,982.942,0.783338,0.0139013}; //Cand 1
  // double Par2[7] = {-16970.9,57122.7,-57869.2,17910.2,1116.62,0.782595,0.0117021};  //Cand 2
  // double Par3[7] = {-9192.09,25178.3,-15389.6,-440.733,1262.17,0.782384,0.0119449}; //Cand 3
  // double Par4[7] = {-38164.1,147453,-181252,72151.7,1207.61,0.781922,0.0109179};    //Cand 4

  // for (int i=0; i<7; i++) {
  //   OmegaComb1Par[i] = Par1[i];
  //   OmegaComb2Par[i] = Par2[i];
  //   OmegaComb3Par[i] = Par3[i];
  //   OmegaComb4Par[i] = Par4[i];
  // }

  //Combination Parameters 2
  double Par1[8] = {108207,-566306,1.0931e+06,-918964,284628,965.042,0.783282,0.0132217}; //Cand 1
  double Par2[8] = {163056,-873153,1.73257e+06,-1.50321e+06,481393,1093.93,0.782476,-0.0108865};  //Cand 2
  double Par3[8] = {198466,-1.0494e+06,2.05555e+06,-1.76209e+06,558163,1234.66,0.78228,0.0110629}; //Cand 3
  double Par4[8] = {-11518.2,10737.4,80001.1,-148221,69247.4,1204.53,0.781906,0.0108166};    //Cand 4
  
  for (int i=0; i<8; i++) {
    OmegaComb1Par[i] = Par1[i];
    OmegaComb2Par[i] = Par2[i];
    OmegaComb3Par[i] = Par3[i];
    OmegaComb4Par[i] = Par4[i];
  }

  fOutTree->Branch("beam",&fHSgamma,buff,split);
  fOutTree->Branch("miss",&fMissing,buff,split);
  fOutTree->Branch("omega candidate1",&fOmegaCand1,buff,split);
  fOutTree->Branch("omega candidate2",&fOmegaCand2,buff,split);
  fOutTree->Branch("omega candidate3",&fOmegaCand3,buff,split);
  fOutTree->Branch("omega candidate4",&fOmegaCand4,buff,split);
  fOutTree->Branch("Other Pion Comb 1",&fPionComb1,buff,split);
  fOutTree->Branch("Other Pion Comb 2",&fPionComb2,buff,split);
  fOutTree->Branch("Other Pion Comb 3",&fPionComb3,buff,split);
  fOutTree->Branch("Other Pion Comb 4",&fPionComb4,buff,split);
  fOutTree->Branch("Beam Energy Selection",&fBeamEnergySelection,buff,split);
  fOutTree->Branch("Scattered Proton",&fProton,buff,split);

  //Histogram Additions
  //if you want kinematic bins you must define fHisbins here
  //  fHisbins=new TH2F("My Bins","E and t",10,1.5,3.5,10,0,5);
  if(fHisbins) fOutput->Add(fHisbins);
  //  fHisbins->SetXTitle("Eg");//give useful axis name
  //  fHisbins->SetYTitle("t");//give useful axis name
  THSHisto::ChangeNames();
  // THSHisto::LoadCut("mass_gt_0");
  THSHisto::LoadCut("NoCut");
  THSHisto::LoadHistograms();
  
  

}

Bool_t CLAStoHS::Process(Long64_t entry)
{
  THSOutput::HSProcessStart(entry);
  
  //Check if this event has the correct final state, see fFinalState
  /// if(!IsGoodEvent()) return kTRUE; //not the event we want so exit
  
  nTot++;

  // Testing DeltaT Function
  DeltaT();
  DeltaBeta();

  cout << "Tof Measured " << tof_meas << endl;
  cout << "Trigger Time " << tr_time << endl;
  cout << "Tof Calculated " << tof_calc << endl;
  cout << "Path Length " << sc_r[0] << endl;
  cout << "Momentum " << p[0] << endl;
  cout << "ID 0 " << id[0] << endl;
  cout << "ID 1 " << id[1] << endl;
  cout << "ID 2 " << id[2] << endl;
  cout << "Status " << (double)stat[0] << endl;
  cout << "Status " << (double)stat[1] << endl;
  cout << "Status " << (double)stat[2] << endl;
  cout << "Charge " << (double)q[0] << endl;
  cout << "Charge " << (int)q[1] << endl;
  cout << "Charge " << (int)q[2] << endl;
  cout << "DeltaT[0] " << delta_tof[0] << endl;
  cout << "DeltaT[1] " << delta_tof[1] << endl;
  cout << "DeltaT[2] " << delta_tof[2] << endl;
  cout << "Beta " << b[0] << endl;
  cout << "Beta " << b[1] << endl;
  cout << "Beta " << b[2] << endl;
  cout << "Beta Meas " << beta_meas << endl;
  cout << "Beta Calc " << beta_calc << endl;
  cout << "DeltaB[0] " << delta_beta[0] << endl;
  cout << "DeltaB[1] " << delta_beta[1] << endl;
  cout << "DeltaB[2] " << delta_beta[2] << endl;
  
  if(!IsGoodEventGhosts())
     return kTRUE; //not the event we want so exit

 
    
  //check for a good photon
  GetEventPartBranches(entry); //get just the branches required
  //look for photons in 1.5ns window, note 2ns looks a bit too large a window
  //first arguement is centre position, 2nd is window (half size)
  if(!MakeBeam(0,1.5)) return kTRUE;
  //now arrange the detected particles into THSParticles
  MakeDetected();

  //Finished THSOutput stuff

  //can now make further reconstructed particles and make cuts etc.
  *fMissing=gTarget+fHSgamma->P4();
  for(UInt_t ip=0;ip<fNdet;ip++)*fMissing-=fDetParticle[ip]->P4();

  //note if I wanted to make a K0, I can look in the output message for which
  //fDetParticle[?] indice corresponds to the pi- and pi+
  //Then fK0=fDetParticle[x]->P4()+fDetParticle[y]->P4();

  //example cut, missing mass must be within 50MeV of target mass
  // if(TMath::Abs(fMissing.M()-gTarget.M())>0.05) return kTRUE; //failed the cut, don't fill
  //all particles reconstructed, all cuts past, fill the output tree


  //Converting the particle entries to lorentz vectors
  *fPiMFast0=fDetParticle[0]->P4(); // The Fast Pi Minus
  *fPiMSlow1=fDetParticle[1]->P4(); // The Slow Pi Minus
  *fPiPFast2=fDetParticle[2]->P4(); // The Fast Pi Plus
  *fPiPSlow3=fDetParticle[3]->P4(); // The Slow Pi Plus

  // cout << "Slow PiM " << fPiMSlow1->M2() << " Slow PiP " << fPiPSlow3->M2() << endl;
  // cout << "Fast PiM " << fPiMFast0->M2() << " Fast PiP " << fPiPFast2->M2() << endl;

  //functions for determining Omega Candidates
  *fOmegaCand1=*fMissing+fDetParticle[0]->P4()+fDetParticle[2]->P4();
  *fOmegaCand2=*fMissing+fDetParticle[0]->P4()+fDetParticle[3]->P4();
  *fOmegaCand3=*fMissing+fDetParticle[1]->P4()+fDetParticle[2]->P4();
  *fOmegaCand4=*fMissing+fDetParticle[1]->P4()+fDetParticle[3]->P4();
   
  //Functions for determining properties of different combinations of 2 detected pions particles [1]-[4]
  *fPionComb1=fDetParticle[1]->P4()+fDetParticle[3]->P4();
  *fPionComb2=fDetParticle[1]->P4()+fDetParticle[2]->P4();
  *fPionComb3=fDetParticle[0]->P4()+fDetParticle[3]->P4();
  *fPionComb4=fDetParticle[0]->P4()+fDetParticle[2]->P4();
   
  //Looking at the seperation of the vertexes of different combinations of pions
   
  fOmegaCand1DeltaVertex=fDetParticle[0]->Vertex()-fDetParticle[2]->Vertex();
  fOmegaCand2DeltaVertex=fDetParticle[0]->Vertex()-fDetParticle[3]->Vertex();
  fOmegaCand3DeltaVertex=fDetParticle[1]->Vertex()-fDetParticle[2]->Vertex();
  fOmegaCand4DeltaVertex=fDetParticle[1]->Vertex()-fDetParticle[3]->Vertex();

  //Ouputting the energy of the tagged photon

  *fBeamEnergySelection = fHSgamma->P4();

  //Considering the properties of the scattered proton

  *fProton = gTarget-fDetParticle[4]->P4();

  //Dalitz Plot Objects

  *fFastPiMPi0=*fMissing+fDetParticle[0]->P4();
  *fSlowPiMPi0=*fMissing+fDetParticle[1]->P4();
  *fFastPiPPi0=*fMissing+fDetParticle[2]->P4();
  *fSlowPiPPi0=*fMissing+fDetParticle[3]->P4();

  *fOmegaCand1PiMSlow1=*fOmegaCand1+fDetParticle[1]->P4(); //Combination of the Omega and Slow Pi Minus
  *fOmegaCand1PiPSlow3=*fOmegaCand1+fDetParticle[3]->P4(); //Combination of the Omega and Slow Pi Minus
  *fOmegaCand2PiMSlow1=*fOmegaCand2+fDetParticle[1]->P4(); //Combination of the Omega and Slow Pi Plus
  *fOmegaCand2PiPFast2=*fOmegaCand2+fDetParticle[2]->P4(); //Combination of the Omega and Fast Pi Plus
  *fOmegaCand3PiMFast0=*fOmegaCand3+fDetParticle[0]->P4(); //Combination of the Omega and Fast Pi Minus
  *fOmegaCand3PiPSlow3=*fOmegaCand3+fDetParticle[3]->P4(); //Combination of the Omega and Slow Pi Minus
  *fOmegaCand4PiMFast0=*fOmegaCand4+fDetParticle[0]->P4(); //Combination of the Omega and Fast Pi Plus
  *fOmegaCand4PiPFast2=*fOmegaCand4+fDetParticle[2]->P4(); //Combination of the Omega and Fast Pi Plus


  //Histogram Additions
   
  //  Int_t kinBin=GetKinBin(beam->P4().E(),t);//if fHisbins is defined need to give this meaningful arguments
   
   FillHistograms("NoCut",0);
  //FillHistograms("Cut1",kinBin);
  //if(fDetParticle[0]->P4().M()>0) FillHistograms("mass_gt_0",0);
  //if(beam->P4().E()>2) FillHistograms("Eg_gt_2",kinBin);

 
  THSOutput::HSProcessFill();
  return kTRUE;
}

void CLAStoHS::SlaveTerminate()
{
  cout << endl;
  cout << " nTot       = " << nTot     << endl; 
  cout << " nGoodOne   = " << nGoodOne << endl;
  cout << " nGoodTwo   = " << nGoodTwo << endl;
  cout << " nGoodThree = " << nGoodThree << endl;
  cout << " nGoodFour = " << nGoodFour << endl;
  cout << " nGoodFive = " << nGoodFive << endl;
  
  HSSlaveTerminate();
}

void CLAStoHS::Terminate()
{
  HSTerminate();  
}


//Functions used in Process
Bool_t CLAStoHS::IsGoodEvent(){
  //this algorithm uses std::vec to compare the particle ids of the event
  //and those defined in fFinalState, event is good if they match

  //only get branches used
  b_gpart->GetEntry(fEntry);
  if(gpart!=(Int_t)fNdet) return kFALSE;//only analyse event with correct final state
  
 

  //Check the tracking status is OK for all tracks (Must need to apply an efficiency correction for this perhaps MC already does)
  b_stat->GetEntry(fEntry);
  for(UInt_t istat=0;istat<fNdet;istat++) if(stat[istat]<1) return kFALSE;
  
  
  
  b_id->GetEntry(fEntry); //particle id branch
  //store the particle IDs in vector to compare with fFinalState
  vector<Int_t>EventState;
  for(UInt_t ifs=0;ifs<fNdet;ifs++) EventState.push_back(id[ifs]);
  //put in order for comparison with requested fFinalState
  std::sort(EventState.begin(),EventState.begin()+gpart);
  //now compare with the particles defined in fFinalState 
  if(!(std::equal(EventState.begin(),EventState.end(),fFinalState.begin()))) return kFALSE; // return if IDs are not those requested
  else{ 
   
    return kTRUE; //it is a good event
  }
}
Bool_t CLAStoHS::IsGoodEventGhosts(){
  //this algorithm uses std::vec to compare the particle ids of the event
  //and those defined in fFinalState, event is good if they match
nGoodOne++;
  //only get branches used
  b_gpart->GetEntry(fEntry);
  //  if(gpart!=(Int_t)fNdet) return kFALSE;//only analyse event with correct final state
  //Check the tracking status is OK for all tracks (Must need to apply an efficiency correction for this perhaps MC already does)
  b_stat->GetEntry(fEntry);
  // for(UInt_t istat=0;istat<fNdet;istat++) if(stat[istat]<1) return kFALSE;

  b_id->GetEntry(fEntry); //particle id branch
  //store the particle IDs in vector to compare with fFinalState
  vector<Int_t>EventState;
  Int_t nreal=0;
  for(UInt_t ifs=0;ifs<gpart;ifs++) {
    if(stat[ifs]>0) {EventState.push_back(id[ifs]);nreal++;}
  }

nGoodTwo++;

  if(nreal!=(Int_t)fNdet) return kFALSE;//only analyse event with correct final state

 nGoodThree++;
  //put in order for comparison with requested fFinalState
  std::sort(EventState.begin(),EventState.begin()+nreal);
  //now compare with the particles defined in fFinalState 
  if(!(std::equal(EventState.begin(),EventState.end(),fFinalState.begin()))){
    nGoodFour++;
 return kFALSE; // return if IDs are not those requested
  }
  else{
 nGoodFive++;
 return kTRUE; //it is a good event
  }
  nGoodSix++;
}


void CLAStoHS::MakeDetected(){
  //this function controls the interfaciing of the reconstructed data to THSParticles
  Int_t iIDall[gpart]; //array containing order of particle IDs in id array for all tracks (incl ghosts) 
  Int_t iID[fNdet]; //array containing order of particle IDs in id array for "real" tracks
  TMath::Sort((Int_t)gpart,id,iIDall,kFALSE); //order the array in asscending order(kFALSE), e.g. -211,211,211
  //write only the indexes of the real tracks to the ID array
  UInt_t Ndet=0;
  for(Int_t ireal=0;ireal<gpart;ireal++) if(stat[iIDall[ireal]]>0)iID[Ndet++]=iIDall[ireal];
  //the ordering in iID[] should now match fFinalState and fEventSate
  //loop over different particle types
  Ndet=0;
  for(UInt_t itype=0;itype<fNtype.size();itype++){ 
    if(fNtype[itype]==1)MakeParticle(fDetParticle[Ndet],iID[Ndet]); 
    else{//order particles of same type fastest first
      //get an array with the momentum of particles of this type
      Double_t typemom[fNtype[itype]];
      Int_t imom[fNtype[itype]];
      for(UInt_t intype=0;intype<fNtype[itype];intype++) typemom[intype]=p[iID[Ndet+intype]];
      TMath::Sort((Int_t)fNtype[itype],typemom,imom,kTRUE); //order the array in decreasing order(kTRUE)
      for(UInt_t intype=0;intype<fNtype[itype];intype++) MakeParticle(fDetParticle[Ndet+intype],iID[Ndet+imom[intype]]);  //function which maps the original variables into THSParticle 
    }
    Ndet+=fNtype[itype];//move on to the next particle type
  }//end itype loop
}

void CLAStoHS::MakeParticle(THSParticle* hsp,Int_t ip){
  //set the vertex
  hsp->SetVertex(vx[ip],vy[ip],vz[ip]);
  //set the intitial Lorentz Vector
  hsp->SetXYZM(p[ip]*cx[ip],p[ip]*cy[ip],p[ip]*cz[ip],hsp->PDGMass());
  //set the measured mass
  hsp->SetMeasMass(sqrt(m[ip]));
  //hsp->SetMeasMass((m[ip]));
  //calculate the vertex time
  hsp->SetTime(sc_t[sc[ip]-1]-sc_r[sc[ip]-1]/hsp->P4().Beta()/29.9792458-tr_time);

}
Bool_t CLAStoHS::MakeBeam(Float_t Tmid,Float_t Tcut){
  //Find a photon within a Tcut window of Tmid  
  //and throw away events with more than one photon
  Bool_t gotGamma=kFALSE;
  for(Int_t im=0;im<taggoodhit;im++) if(TMath::Abs(dt_st_tag[im])+Tmid<Tcut){
      if(gotGamma==kTRUE)return kFALSE; //ambigous photon event throw away for now
      fHSgamma->SetXYZT(0,0,tag_energy[im],tag_energy[im]);
      fHSgamma->SetTime(vertex_time[im]);//=-tr_time
      gotGamma=kTRUE;
    }
  if(!gotGamma) return kFALSE; //didn't get a photon event no good
  return kTRUE; //got one good photon
}

//Histogram Functions
void CLAStoHS::HistogramList(TString sLabel){
  TDirectory::AddDirectory(kFALSE); //do not add to current directory
  //now define all histograms and add to Output
  //label includes kinematic bin and additional cut name
  // e.g fOutput->Add(MapHist(new TH1F("Mp1"+sLabel,"M_{p1}"+sLabel,100,0,2)));
  
  //Simon Histograms
  fOutput->Add(MapHist(new TH2F("histo_n11"+sLabel,"Missing Mass vs #omega MassCand1;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",200,-0.2,1.20,200,0.0,2.0)));			    
  fOutput->Add(MapHist(new TH2F("histo_n12"+sLabel,"Missing Mass vs #omega MassCand2;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("histo_n13"+sLabel,"Missing Mass vs #omega MassCand3;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("histo_n14"+sLabel,"Missing Mass vs #omega MassCand4;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));

  //Selected Region Histograms
  fOutput->Add(MapHist(new TH2F("OmegaCandComb1"+sLabel,"Missing Mass vs #omega MassCand1;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.1,0.175,50,0.75,0.82)));		    
  fOutput->Add(MapHist(new TH2F("OmegaCandComb1Other2Pions"+sLabel,"Missing Mass vs #omega MassCand4;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.1,0.175,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb1NotSelected"+sLabel,"Missing Mass vs #omega MassCand1;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb1BeamEnergy"+sLabel,"Beam Energy vs #omega MassCand1;Beam Energy(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,1.75,4.0,50,0.75,0.82)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb1DeltaZ"+sLabel,"#DeltaZ vs #omega MassCand1;#DeltaZ Vextex(cm);#omega Candidate Mass(GeV/c^{2})",200,-4,4,50,0.75,0.82)));
 
  fOutput->Add(MapHist(new TH2F("OmegaCandComb2"+sLabel,"Missing Mass vs #omega MassCand2;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.1,0.175,50,0.75,0.82)));	           
  fOutput->Add(MapHist(new TH2F("OmegaCandComb2Other2Pions"+sLabel,"Missing Mass vs #omega MassCand3;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.1,0.175,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb2NotSelected"+sLabel,"Missing Mass vs #omega MassCand2;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb2BeamEnergy"+sLabel,"Beam Energy vs #omega MassCand2;Beam Energy(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,1.75,4.0,50,0.75,0.82)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb2DeltaZ"+sLabel,"#DeltaZ vs #omega MassCand2;#DeltaZ Vextex(cm);#omega Candidate Mass(GeV/c^{2})",200,-4,4,50,0.75,0.82)));

  fOutput->Add(MapHist(new TH2F("OmegaCandComb3"+sLabel,"Missing Mass vs #omega MassCand3;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.1,0.175,50,0.75,0.82)));		    
  fOutput->Add(MapHist(new TH2F("OmegaCandComb3Other2Pions"+sLabel,"Missing Mass vs #omega MassCand2;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.1,0.175,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb3NotSelected"+sLabel,"Missing Mass vs #omega MassCand3;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb3BeamEnergy"+sLabel,"Beam Energy vs #omega MassCand3;Beam Energy(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,1.75,4.0,50,0.75,0.82)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb3DeltaZ"+sLabel,"#DeltaZ vs #omega MassCand3;#DeltaZ Vextex(cm);#omega Candidate Mass(GeV/c^{2})",200,-4,4,50,0.75,0.82)));

  fOutput->Add(MapHist(new TH2F("OmegaCandComb4"+sLabel,"Missing Mass vs #omega MassCand4;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.1,0.175,50,0.75,0.82)));		    
  fOutput->Add(MapHist(new TH2F("OmegaCandComb4Other2Pions"+sLabel,"Missing Mass vs #omega MassCand1;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.1,0.175,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb4NotSelected"+sLabel,"Missing Mass vs #omega MassCand4;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb4BeamEnergy"+sLabel,"Beam Energy vs #omega MassCand4;Beam Energy(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,1.75,4.0,50,0.75,0.82)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb4DeltaZ"+sLabel,"#DeltaZ vs #omega MassCand4;#DeltaZ Vextex(cm);#omega Candidate Mass(GeV/c^{2})",200,-4,4,50,0.75,0.82)));


  //Plots for determining fitting parameters for the data

  fOutput->Add(MapHist(new TH1F("OmegaComb1FittingPlot"+sLabel,"#omega MassCand1;#omega Candidate Mass(GeV/c^{2})",200,0.4,1.0)));
  fOutput->Add(MapHist(new TH1F("OmegaComb2FittingPlot"+sLabel,"#omega MassCand2;#omega Candidate Mass(GeV/c^{2})",200,0.4,1.0)));
  fOutput->Add(MapHist(new TH1F("OmegaComb3FittingPlot"+sLabel,"#omega MassCand3;#omega Candidate Mass(GeV/c^{2})",200,0.4,1.0)));
  fOutput->Add(MapHist(new TH1F("OmegaComb4FittingPlot"+sLabel,"#omega MassCand4;#omega Candidate Mass(GeV/c^{2})",200,0.4,1.0)));

  fOutput->Add(MapHist(new TH1F("OmegaCombNoWeightFittingPlot"+sLabel,"#omega MassCand weighting;#omega Candidate Mass(GeV/c^{2})",50,0.7,0.9)));
  fOutput->Add(MapHist(new TH1F("OmegaCombWeightFittingPlot"+sLabel,"#omega MassCand weighting;#omega Candidate Mass(GeV/c^{2})",50,0.7,0.9)));
 
  fOutput->Add(MapHist(new TH2F("OmegaComb1FittingPlot2D"+sLabel,"#omega MassCand1;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0,0.3,200,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaComb2FittingPlot2D"+sLabel,"#omega MassCand2;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0,0.3,200,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaComb3FittingPlot2D"+sLabel,"#omega MassCand3;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0,0.3,200,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaComb4FittingPlot2D"+sLabel,"#omega MassCand4;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0,0.3,200,0.4,1.0)));



  //Weighted Plots
  
  fOutput->Add(MapHist(new TH2F("WeightedOmegaCandvsBeamEnergy"+sLabel,"Beam Energy vs #omega#pi^{-}#pi^{+};Beam Energy(GeV/c^{2});#omega#pi^{-}#pi^{+}(GeV/c^{2})",50,1.75,4.0,100,1.0,2.0)));
  //fOutput->Add(MapHist(new TH2F("tDepedencevsOmegaPiPi"+sLabel,"t dependence vs #omega#pi^{-}#pi^{+};t(GeV/c^{2});#omega#pi^{-}#pi^{+}(GeV/c^{2})",50,-5,0,100,1.0,2.0)));

  //Determining the t dependence 

  fOutput->Add(MapHist(new TH1F("tDependence"+sLabel,"t dependence;t",50,-5,0)));
  fOutput->Add(MapHist(new TH1F("tDependence2"+sLabel,"-t dependence;-t",50,0,5)));

  fOutput->Add(MapHist(new TH2F("tDepedencevsOmegaPiPi"+sLabel,"t dependence vs #omega#pi^{-}#pi^{+};t(GeV/c^{2});#omega#pi^{-}#pi^{+}(GeV/c^{2})",50,0,5,100,1.0,2.0)));

  //Plots for accounting for tagger acceptance

  fOutput->Add(MapHist(new TH1F("TaggerAcceptance"+sLabel, "Beam Energy for Tagger Acceptance; Beam Energy(GeV/c^{2})",50,1.75,4.0))); 
  fOutput->Add(MapHist(new TH1F("TaggerAcceptance2"+sLabel, "Beam Energy for Tagger Acceptance; Beam Energy(GeV/c^{2})",200,0,4.0))); 

  //Dalitz Plots Tests

  // fOutput->Add(MapHist(new TH2F("OmegaDalitzTest1"+sLabel,"#omega to #pi^{-}#pi^{+}#pi^{0} Dalitz;#pi^{-}#pi^{0}(GeV/c^{2});#pi^{+}#pi^{0}(GeV/c^{2})",200,0,1,200,0,1)));
  // fOutput->Add(MapHist(new TH2F("OmegaDalitzTest2"+sLabel,"#omega to #pi^{-}#pi^{+}#pi^{0} Dalitz;#pi^{-}#pi^{0}(GeV/c^{2});#pi^{+}#pi^{0}(GeV/c^{2})",200,0,1,200,0,1)));
  // fOutput->Add(MapHist(new TH2F("OmegaDalitzTest3"+sLabel,"#omega to #pi^{-}#pi^{+}#pi^{0} Dalitz;#pi^{-}#pi^{0}(GeV/c^{2});#pi^{+}#pi^{0}(GeV/c^{2})",200,0,1,200,0,1)));
  // fOutput->Add(MapHist(new TH2F("OmegaDalitzTest4"+sLabel,"#omega to #pi^{-}#pi^{+}#pi^{0} Dalitz;#pi^{-}#pi^{0}(GeV/c^{2});#pi^{+}#pi^{0}(GeV/c^{2})",200,0,1,200,0,1)));

  //New Dalitz plot tests
  fOutput->Add(MapHist(new TH2F("OmegaDalitzTestComb1NoCut"+sLabel,"Comb 1 #omega#pi^{-} vs #omega#pi^{+} Dalitz;#omega#pi^{-}(GeV/c^{2});#omega#pi^{+}(GeV/c^{2})",200,-1,3,200,-1,3)));
  fOutput->Add(MapHist(new TH2F("OmegaDalitzTestComb2NoCut"+sLabel,"Comb 2 #omega#pi^{-} vs #omega#pi^{+} Dalitz;#omega#pi^{-}(GeV/c^{2});#omega#pi^{+}(GeV/c^{2})",200,-1,3,200,-1,3)));
  fOutput->Add(MapHist(new TH2F("OmegaDalitzTestComb3NoCut"+sLabel,"Comb 3 #omega#pi^{-} vs #omega#pi^{+} Dalitz;#omega#pi^{-}(GeV/c^{2});#omega#pi^{+}(GeV/c^{2})",200,-1,3,200,-1,3)));
  fOutput->Add(MapHist(new TH2F("OmegaDalitzTestComb4NoCut"+sLabel,"Comb 4 #omega#pi^{-} vs #omega#pi^{+} Dalitz;#omega#pi^{-}(GeV/c^{2});#omega#pi^{+}(GeV/c^{2})",200,-1,3,200,-1,3)));

  // fOutput->Add(MapHist(new TH2F("OmegaDalitzTestComb1NoCutTest"+sLabel,"Comb 1 #omega#pi^{-} vs #omega#pi^{+} Dalitz;#omega#pi^{-}(GeV/c^{2});#omega#pi^{+}(GeV/c^{2})",200,-2,2,200,-2,2)));
  // fOutput->Add(MapHist(new TH2F("OmegaDalitzTestComb2NoCutTest"+sLabel,"Comb 2 #omega#pi^{-} vs #omega#pi^{+} Dalitz;#omega#pi^{-}(GeV/c^{2});#omega#pi^{+}(GeV/c^{2})",200,-2,2,200,-2,2)));
  // fOutput->Add(MapHist(new TH2F("OmegaDalitzTestComb3NoCutTest"+sLabel,"Comb 3 #omega#pi^{-} vs #omega#pi^{+} Dalitz;#omega#pi^{-}(GeV/c^{2});#omega#pi^{+}(GeV/c^{2})",200,-2,2,200,-2,2)));
  // fOutput->Add(MapHist(new TH2F("OmegaDalitzTestComb4NoCutTest"+sLabel,"Comb 4 #omega#pi^{-} vs #omega#pi^{+} Dalitz;#omega#pi^{-}(GeV/c^{2});#omega#pi^{+}(GeV/c^{2})",200,-2,2,200,-2,2)));

  fOutput->Add(MapHist(new TH2F("OmegaDalitzComb1"+sLabel,"Comb 1 #omega#pi^{-} vs #omega#pi^{+} Dalitz;#omega#pi^{-}(GeV/c^{2});#omega#pi^{+}(GeV/c^{2})",80,0.8,2.2,80,0.8,2.2)));
  fOutput->Add(MapHist(new TH2F("OmegaDalitzComb2"+sLabel,"Comb 2 #omega#pi^{-} vs #omega#pi^{+} Dalitz;#omega#pi^{-}(GeV/c^{2});#omega#pi^{+}(GeV/c^{2})",80,0.8,2.2,80,0.8,2.2)));
  fOutput->Add(MapHist(new TH2F("OmegaDalitzComb3"+sLabel,"Comb 3 #omega#pi^{-} vs #omega#pi^{+} Dalitz;#omega#pi^{-}(GeV/c^{2});#omega#pi^{+}(GeV/c^{2})",80,0.8,2.2,80,0.8,2.2)));
  fOutput->Add(MapHist(new TH2F("OmegaDalitzComb4"+sLabel,"Comb 4 #omega#pi^{-} vs #omega#pi^{+} Dalitz;#omega#pi^{-}(GeV/c^{2});#omega#pi^{+}(GeV/c^{2})",80,0.8,2.2,80,0.8,2.2)));


  //Delta T Histograms
  fOutput->Add(MapHist(new TH1D("DeltaT"+sLabel,"#Delta T Proton,#pi^{+} and #pi^{-}; #Delta T(ns)",1000,-50,120)));
  fOutput->Add(MapHist(new TH1D("DeltaTPro"+sLabel,"#Delta T Proton; #Delta T(ns)",1000,-50,120)));
  fOutput->Add(MapHist(new TH1D("DeltaTPiP"+sLabel,"#Delta T #pi^{+}; #Delta T(ns)",1000,-50,120)));
  fOutput->Add(MapHist(new TH1D("DeltaTPiM"+sLabel,"#Delta T #pi^{-}; #Delta T(ns)",1000,-50,120)));

  //DeltaT vs Momentum for particles
  // fOutput->Add(MapHist(new TH2D("DeltaTProvsMom"+sLabel,"#Delta T Proton vs P;Proton #Delta T(ns); P (GeV)",200,-20,20,200,0,4)));
  // fOutput->Add(MapHist(new TH2D("DeltaTPiP1vsMom"+sLabel,"#Delta T PiP1 vs P;PiP1 #Delta T(ns); P (GeV)",200,-20,20,200,0,2)));
  // fOutput->Add(MapHist(new TH2D("DeltaTPiP2vsMom"+sLabel,"#Delta T PiP2 vs P;PiP2 #Delta T(ns); P (GeV)",200,-20,20,200,0,2)));
  // fOutput->Add(MapHist(new TH2D("DeltaTPiM1vsMom"+sLabel,"#Delta T PiM1 vs P;PiM1 #Delta T(ns); P (GeV)",200,-20,20,200,0,2)));
  // fOutput->Add(MapHist(new TH2D("DeltaTPiM2vsMom"+sLabel,"#Delta T PiM2 vs P;PiM2 #Delta T(ns); P (GeV)",200,-20,20,200,0,2)));

  fOutput->Add(MapHist(new TH2D("DeltaTMomvsPro"+sLabel,"P vs #Delta T Proton;P (GeV); Proton #Delta T(ns)",200,0,4,200,-20,20)));
  fOutput->Add(MapHist(new TH2D("DeltaTMomvsPiP1"+sLabel,"P vs #Delta T #pi^{+}1;#pi^{+}1 #Delta T(ns);P (GeV)",200,0,4,200,-20,20)));
  fOutput->Add(MapHist(new TH2D("DeltaTMomvsPiP2"+sLabel,"P vs #Delta T #pi^{+}2;#pi^{+}2 #Delta T(ns);P (GeV)",200,0,4,200,-20,20)));
  fOutput->Add(MapHist(new TH2D("DeltaTMomvsPiM1"+sLabel,"P vs #Delta T #pi^{-}1;#pi^{-}1 #Delta T(ns);P (GeV)",200,0,4,200,-20,20)));
  fOutput->Add(MapHist(new TH2D("DeltaTMomvsPiM2"+sLabel,"P vs #Delta T #pi^{-}2;#pi^{-}2 #Delta T(ns);P (GeV)",200,0,4,200,-20,20)));

  
  //DeltaT one particle vs another
  fOutput->Add(MapHist(new TH2D("DeltaTProPiP1"+sLabel,"#Delta T Proton vs #pi^{+}1;Proton #Delta T(ns); #pi^{+}1 #Delta T (ns)",200,-20,20,200,-20,20)));
  fOutput->Add(MapHist(new TH2D("DeltaTProPiP2"+sLabel,"#Delta T Proton vs #pi^{+}2;Proton #Delta T(ns); #pi^{+}2 #Delta T (ns)",200,-20,20,200,-20,20)));
  
  fOutput->Add(MapHist(new TH2D("DeltaTProPiM1"+sLabel,"#Delta T Proton vs #pi^{-}1;Proton #Delta T(ns); #pi^{-}1 #Delta T (ns)",200,-20,20,200,-20,20)));
  fOutput->Add(MapHist(new TH2D("DeltaTProPiM2"+sLabel,"#Delta T Proton vs #pi^{-}2;Proton #Delta T(ns); #pi^{-}2 #Delta T (ns)",200,-20,20,200,-20,20)));

  fOutput->Add(MapHist(new TH2D("DeltaTPiP1PiP2"+sLabel,"#Delta T #pi^{+}1 vs #pi^{+}2; #pi^{+}1 #Delta T(ns); #pi^{+}2 #Delta T (ns)",200,-20,20,200,-20,20)));
  fOutput->Add(MapHist(new TH2D("DeltaTPiM1PiM2"+sLabel,"#Delta T #pi^{-}1 vs #pi^{-}2; #pi^{-}1 #Delta T(ns); #pi^{-}2 #Delta T (ns)",200,-20,20,200,-20,20)));
  fOutput->Add(MapHist(new TH2D("DeltaTPiP1PiM2"+sLabel,"#Delta T #pi^{+}1 vs #pi^{-}2; #pi^{+}1 #Delta T(ns); #pi^{-}2 #Delta T (ns)",200,-20,20,200,-20,20)));
  fOutput->Add(MapHist(new TH2D("DeltaTPiM1PiP2"+sLabel,"#Delta T #pi^{-}1 vs #pi^{+}2; #pi^{-}1 #Delta T(ns); #pi^{+}2 #Delta T (ns)",200,-20,20,200,-20,20)));


  //DeltaBeta Histograms
  fOutput->Add(MapHist(new TH1D("DeltaB"+sLabel,"#Delta B Proton,#pi^{+} and #pi^{-}; #Delta B",1000,-0.6,0.6)));
  fOutput->Add(MapHist(new TH1D("DeltaBPro"+sLabel,"#Delta B Proton; #Delta T",1000,-0.6,0.6)));
  fOutput->Add(MapHist(new TH1D("DeltaBPiP"+sLabel,"#Delta B #pi^{+}; #Delta T",1000,-0.6,0.6)));
  fOutput->Add(MapHist(new TH1D("DeltaBPiM"+sLabel,"#Delta B #pi^{-}; #Delta T",1000,-0.6,0.6)));

  // fOutput->Add(MapHist(new TH2D("DeltaBProvsMom"+sLabel,"#Delta B Proton vs P;Proton #Delta B; P (GeV)",200,-0.6,0.6,200,0,4)));
  // fOutput->Add(MapHist(new TH2D("DeltaBPiP1vsMom"+sLabel,"#Delta B PiP1 vs P;PiP1 #Delta B; P (GeV)",200,-0.6,0.6,200,0,2)));
  // fOutput->Add(MapHist(new TH2D("DeltaBPiP2vsMom"+sLabel,"#Delta B PiP2 vs P;PiP2 #Delta B; P (GeV)",200,-0.6,0.6,200,0,2)));
  // fOutput->Add(MapHist(new TH2D("DeltaBPiM1vsMom"+sLabel,"#Delta B PiM1 vs P;PiM1 #Delta B; P (GeV)",200,-0.6,0.6,200,0,2)));
  // fOutput->Add(MapHist(new TH2D("DeltaBPiM2vsMom"+sLabel,"#Delta B PiM2 vs P;PiM2 #Delta B; P (GeV)",200,-0.6,0.6,200,0,2)));

  fOutput->Add(MapHist(new TH2D("DeltaBMomvsPro"+sLabel,"P vs #Delta B Proton;P (GeV); Proton #Delta T(ns)",200,0,4,200,-0.6,0.6)));
  fOutput->Add(MapHist(new TH2D("DeltaBMomvsPiP1"+sLabel,"P vs #Delta B #pi^{+}1;#pi^{+}1 #Delta T(ns);P (GeV)",200,0,2,200,-0.6,0.6)));
  fOutput->Add(MapHist(new TH2D("DeltaBMomvsPiP2"+sLabel,"P vs #Delta B #pi^{+}2;#pi^{+}2 #Delta T(ns);P (GeV)",200,0,2,200,-0.6,0.6)));
  fOutput->Add(MapHist(new TH2D("DeltaBMomvsPiM1"+sLabel,"P vs #Delta B #pi^{-}1;#pi^{-}1 #Delta T(ns);P (GeV)",200,0,2,200,-0.6,0.6)));
  fOutput->Add(MapHist(new TH2D("DeltaBMomvsPiM2"+sLabel,"P vs #Delta B #pi^{-}2;#pi^{-}2 #Delta T(ns);P (GeV)",200,0,2,200,-0.6,0.6)));


//end of histogram list
  TDirectory::AddDirectory(kTRUE); //back to normal
}
void CLAStoHS::FillHistograms(TString sCut,Int_t bin) {
   fCurrCut=sCut;
   fCurrBin=bin;
   //Get histogram from list
   //Fill histogram
   TString sLabel;
   sLabel=sCut+fVecBinNames[bin];
   // e.g. FindHist("Mp1"+sLabel)->Fill(fp1->M());
  
  //Simon Histograms
  //  FindHist("histo_n10"+sLabel)->Fill(fMissing->M(),OmegaMassTest);
  FindHist("histo_n11"+sLabel)->Fill(fMissing->M(), fOmegaCand1->M());
  FindHist("histo_n12"+sLabel)->Fill(fMissing->M(), fOmegaCand2->M());
  FindHist("histo_n13"+sLabel)->Fill(fMissing->M(), fOmegaCand3->M());
  FindHist("histo_n14"+sLabel)->Fill(fMissing->M(), fOmegaCand4->M());
  
  //Selecting regions of interest
  
  //Combination 1
  if (fMissing->M() >= Comb1MissingMCLow && fMissing->M() <= Comb1MissingMCHigh && fOmegaCand1->M() >= Comb1OmegaMCLow && fOmegaCand1->M()<= Comb1OmegaMCHigh){
    
    FindHist("OmegaCandComb1"+sLabel)->Fill(fMissing->M(), fOmegaCand1->M()); //Omega Candidate
    FindHist("OmegaCandComb1Other2Pions"+sLabel)->Fill(fMissing->M(),fOmegaCand4->M()); //Other 2 pions with missing mass
    FindHist("OmegaCandComb1BeamEnergy"+sLabel)->Fill(fBeamEnergySelection->E(),fOmegaCand1->M());
    FindHist("OmegaCandComb1DeltaZ"+sLabel)->Fill(fOmegaCand1DeltaVertex.Z(),fOmegaCand1->M());
    // FindHist("OmegaDalitzTest1"+sLabel)->Fill(fFastPiMPi0->M(),fFastPiPPi0->M()); //Dalitz Test
    // FindHist("OmegaDalitzTestComb1"+sLabel)->Fill(fOmegaCand1->M2()+fPiMSlow1->M2(),fOmegaCand1->M2()+fPiPSlow3->M2());
    FindHist("OmegaDalitzComb1"+sLabel)->Fill(fOmegaCand1PiMSlow1->M2(),fOmegaCand1PiPSlow3->M2());
  }
  else{
    FindHist("OmegaCandComb1NotSelected"+sLabel)->Fill(fMissing->M(),fOmegaCand1->M()); //Not selected region
  }
  
  //Combination 2
  if (fMissing->M() >= Comb2MissingMCLow && fMissing->M() <= Comb2MissingMCHigh && fOmegaCand2->M() >= Comb2OmegaMCLow && fOmegaCand2->M()<= Comb2OmegaMCHigh){
    
    FindHist("OmegaCandComb2"+sLabel)->Fill(fMissing->M(), fOmegaCand2->M()); //Omega Candidate
    FindHist("OmegaCandComb2Other2Pions"+sLabel)->Fill(fMissing->M(),fOmegaCand3->M()); //Other 2 pions with missing mass
    FindHist("OmegaCandComb2BeamEnergy"+sLabel)->Fill(fBeamEnergySelection->E(),fOmegaCand2->M());
    FindHist("OmegaCandComb2DeltaZ"+sLabel)->Fill(fOmegaCand2DeltaVertex.Z(),fOmegaCand2->M());
    //  FindHist("OmegaDalitzTest2"+sLabel)->Fill(fFastPiMPi0->M(),fSlowPiPPi0->M()); //Dalitz Test
    //  FindHist("OmegaDalitzTestComb2"+sLabel)->Fill(fOmegaCand2->M2()+fPiMSlow1->M2(),fOmegaCand1->M2()+fPiPFast2->M2());
    FindHist("OmegaDalitzComb2"+sLabel)->Fill(fOmegaCand2PiMSlow1->M2(),fOmegaCand2PiPFast2->M2());
  }
  else{
    FindHist("OmegaCandComb2NotSelected"+sLabel)->Fill(fMissing->M(),fOmegaCand2->M()); //Not selected region
  }
    
  //Combination 3
  if (fMissing->M() >= Comb3MissingMCLow && fMissing->M() <= Comb3MissingMCHigh && fOmegaCand3->M() >= Comb3OmegaMCLow && fOmegaCand3->M()<= Comb3OmegaMCHigh){
      
    FindHist("OmegaCandComb3"+sLabel)->Fill(fMissing->M(), fOmegaCand3->M()); //Omega Candidate
    FindHist("OmegaCandComb3Other2Pions"+sLabel)->Fill(fMissing->M(),fOmegaCand2->M()); //Other 2 pions with missing mass
    FindHist("OmegaCandComb3BeamEnergy"+sLabel)->Fill(fBeamEnergySelection->E(),fOmegaCand3->M());
    FindHist("OmegaCandComb3DeltaZ"+sLabel)->Fill(fOmegaCand3DeltaVertex.Z(),fOmegaCand3->M());
    // FindHist("OmegaDalitzTest3"+sLabel)->Fill(fSlowPiMPi0->M(),fFastPiPPi0->M()); //Dalitz Test
    // FindHist("OmegaDalitzTestComb3"+sLabel)->Fill(fOmegaCand3->M2()+fPiMFast0->M2(),fOmegaCand1->M2()+fPiPSlow3->M2());
    FindHist("OmegaDalitzComb3"+sLabel)->Fill(fOmegaCand3PiMFast0->M2(),fOmegaCand3PiPSlow3->M2());
  }
  else{
    FindHist("OmegaCandComb3NotSelected"+sLabel)->Fill(fMissing->M(),fOmegaCand3->M()); //Not selected region
  }
    
  //Combination 4
  if (fMissing->M() >= Comb4MissingMCLow && fMissing->M() <= Comb4MissingMCHigh && fOmegaCand4->M() >= Comb4OmegaMCLow && fOmegaCand4->M()<= Comb4OmegaMCHigh){
      
    FindHist("OmegaCandComb4"+sLabel)->Fill(fMissing->M(), fOmegaCand4->M()); //Omega Candidate
    FindHist("OmegaCandComb4Other2Pions"+sLabel)->Fill(fMissing->M(),fOmegaCand1->M()); //Other 2 pions with missing mass
    FindHist("OmegaCandComb4BeamEnergy"+sLabel)->Fill(fBeamEnergySelection->E(),fOmegaCand4->M());
    FindHist("OmegaCandComb4DeltaZ"+sLabel)->Fill(fOmegaCand4DeltaVertex.Z(),fOmegaCand4->M());
    // FindHist("OmegaDalitzTest4"+sLabel)->Fill(fSlowPiMPi0->M(),fSlowPiPPi0->M()); //Dalitz Test
    // FindHist("OmegaDalitzTestComb4"+sLabel)->Fill(fOmegaCand4->M2()+fPiMFast0->M2(),fOmegaCand1->M2()+fPiPFast2->M2());
    FindHist("OmegaDalitzComb4"+sLabel)->Fill(fOmegaCand4PiMFast0->M2(),fOmegaCand4PiPFast2->M2());
  }
  else{
    FindHist("OmegaCandComb4NotSelected"+sLabel)->Fill(fMissing->M(),fOmegaCand4->M()); //Not selected region
  }

  // Fitting Function Histograms
  if (fMissing->M() >= Comb1MissingMCLow && fMissing->M() <= Comb1MissingMCHigh){
    FindHist("OmegaComb1FittingPlot"+sLabel)->Fill(fOmegaCand1->M());
    FindHist("OmegaCombNoWeightFittingPlot"+sLabel)->Fill(fOmegaCand1->M());
    FindHist("OmegaCombWeightFittingPlot"+sLabel)->Fill(fOmegaCand1->M(),GetWeight(OmegaComb1Par,fOmegaCand1->M()));
  }
  if (fMissing->M() >= Comb2MissingMCLow && fMissing->M() <= Comb2MissingMCHigh){
    FindHist("OmegaComb2FittingPlot"+sLabel)->Fill(fOmegaCand2->M());
    FindHist("OmegaCombNoWeightFittingPlot"+sLabel)->Fill(fOmegaCand2->M());
    FindHist("OmegaCombWeightFittingPlot"+sLabel)->Fill(fOmegaCand1->M(),GetWeight(OmegaComb2Par,fOmegaCand2->M()));
  }
  if (fMissing->M() >= Comb3MissingMCLow && fMissing->M() <= Comb3MissingMCHigh){
    FindHist("OmegaComb3FittingPlot"+sLabel)->Fill(fOmegaCand3->M());
    FindHist("OmegaCombNoWeightFittingPlot"+sLabel)->Fill(fOmegaCand3->M());
    FindHist("OmegaCombWeightFittingPlot"+sLabel)->Fill(fOmegaCand1->M(),GetWeight(OmegaComb3Par,fOmegaCand3->M()));
  }
  if (fMissing->M() >= Comb4MissingMCLow && fMissing->M() <= Comb4MissingMCHigh){
    FindHist("OmegaComb4FittingPlot"+sLabel)->Fill(fOmegaCand4->M());
    FindHist("OmegaCombNoWeightFittingPlot"+sLabel)->Fill(fOmegaCand4->M());
    FindHist("OmegaCombWeightFittingPlot"+sLabel)->Fill(fOmegaCand1->M(),GetWeight(OmegaComb4Par,fOmegaCand4->M()));
  }

  //Weighted Histograms
  
  if (fMissing->M() >= MissingMCLow && fMissing->M() <= MissingMCHigh && fOmegaCand1->M() >= OmegaMCLow && fOmegaCand1->M()<= OmegaMCHigh){
    ((TH2F*)FindHist("WeightedOmegaCandvsBeamEnergy"+sLabel))->Fill(fBeamEnergySelection->E(),(*fOmegaCand1+*fPionComb1).M(),GetWeight(OmegaComb1Par,fOmegaCand1->M()));
    ((TH2F*)FindHist("OmegaComb1FittingPlot2D"+sLabel))->Fill(fMissing->M(),fOmegaCand1->M(),GetWeight(OmegaComb1Par,fOmegaCand1->M()));
    ((TH2F*)FindHist("tDepedencevsOmegaPiPi"+sLabel))->Fill(fProton->M2(),(*fOmegaCand1+*fPionComb1).M(),GetWeight(OmegaComb1Par,fOmegaCand1->M()));
  }
  if (fMissing->M() >= MissingMCLow && fMissing->M() <= MissingMCHigh && fOmegaCand2->M() >= OmegaMCLow && fOmegaCand2->M()<= OmegaMCHigh){
    ((TH2F*)FindHist("WeightedOmegaCandvsBeamEnergy"+sLabel))->Fill(fBeamEnergySelection->E(),(*fOmegaCand2+*fPionComb2).M(),GetWeight(OmegaComb2Par,fOmegaCand2->M()));
    ((TH2F*)FindHist("OmegaComb2FittingPlot2D"+sLabel))->Fill(fMissing->M(),fOmegaCand2->M(),GetWeight(OmegaComb2Par,fOmegaCand2->M()));
    ((TH2F*)FindHist("tDepedencevsOmegaPiPi"+sLabel))->Fill(fProton->M2(),(*fOmegaCand2+*fPionComb2).M(),GetWeight(OmegaComb2Par,fOmegaCand2->M()));
  }
  if (fMissing->M() >= MissingMCLow && fMissing->M() <= MissingMCHigh && fOmegaCand3->M() >= OmegaMCLow && fOmegaCand3->M()<= OmegaMCHigh){
    ((TH2F*)FindHist("WeightedOmegaCandvsBeamEnergy"+sLabel))->Fill(fBeamEnergySelection->E(),(*fOmegaCand3+*fPionComb3).M(),GetWeight(OmegaComb3Par,fOmegaCand3->M()));
    ((TH2F*)FindHist("OmegaComb3FittingPlot2D"+sLabel))->Fill(fMissing->M(),fOmegaCand3->M(),GetWeight(OmegaComb3Par,fOmegaCand3->M()));
    ((TH2F*)FindHist("tDepedencevsOmegaPiPi"+sLabel))->Fill(fProton->M2(),(*fOmegaCand3+*fPionComb3).M(),GetWeight(OmegaComb3Par,fOmegaCand3->M()));
  }
  if (fMissing->M() >= MissingMCLow && fMissing->M() <= MissingMCHigh && fOmegaCand4->M() >= OmegaMCLow && fOmegaCand4->M()<= OmegaMCHigh){
    ((TH2F*)FindHist("WeightedOmegaCandvsBeamEnergy"+sLabel))->Fill(fBeamEnergySelection->E(),(*fOmegaCand4+*fPionComb4).M(),GetWeight(OmegaComb4Par,fOmegaCand4->M()));
    ((TH2F*)FindHist("OmegaComb4FittingPlot2D"+sLabel))->Fill(fMissing->M(),fOmegaCand4->M(),GetWeight(OmegaComb4Par,fOmegaCand4->M()));
    ((TH2F*)FindHist("tDepedencevsOmegaPiPi"+sLabel))->Fill(fProton->M2(),(*fOmegaCand4+*fPionComb4).M(),GetWeight(OmegaComb4Par,fOmegaCand4->M()));
  }
  
  //TDependence Histograms
  if(fMissing->M() >= MissingMCLow && fMissing->M() <= MissingMCHigh){
    FindHist("tDependence"+sLabel)->Fill(fProton->M2());
    FindHist("tDependence2"+sLabel)->Fill(fProton->M2()*(-1));
  }

  //Tagger Acceptance Histograms

  FindHist("TaggerAcceptance"+sLabel)->Fill(fBeamEnergySelection->E());
  FindHist("TaggerAcceptance2"+sLabel)->Fill(fBeamEnergySelection->E());

  //Dalitz Test Histograms
 
  // FindHist("OmegaDalitzTestComb1NoCutTest"+sLabel)->Fill(fOmegaCand1->M2()+fPiMSlow1->M2(),fOmegaCand1->M2()+fPiPSlow3->M2());
  // FindHist("OmegaDalitzTestComb2NoCutTest"+sLabel)->Fill(fOmegaCand2->M2()+fPiMSlow1->M2(),fOmegaCand1->M2()+fPiPFast2->M2());
  // FindHist("OmegaDalitzTestComb3NoCutTest"+sLabel)->Fill(fOmegaCand3->M2()+fPiMFast0->M2(),fOmegaCand1->M2()+fPiPSlow3->M2());
  // FindHist("OmegaDalitzTestComb4NoCutTest"+sLabel)->Fill(fOmegaCand4->M2()+fPiMFast0->M2(),fOmegaCand1->M2()+fPiPFast2->M2());

  FindHist("OmegaDalitzTestComb1NoCut"+sLabel)->Fill(fOmegaCand1PiMSlow1->M2(),fOmegaCand1PiPSlow3->M2());
  FindHist("OmegaDalitzTestComb2NoCut"+sLabel)->Fill(fOmegaCand2PiMSlow1->M2(),fOmegaCand2PiPFast2->M2());
  FindHist("OmegaDalitzTestComb3NoCut"+sLabel)->Fill(fOmegaCand3PiMFast0->M2(),fOmegaCand3PiPSlow3->M2());
  FindHist("OmegaDalitzTestComb4NoCut"+sLabel)->Fill(fOmegaCand4PiMFast0->M2(),fOmegaCand4PiPFast2->M2());
 

  DeltaT_PiP1 = 110;
  DeltaT_PiP2 = 110;
  DeltaT_Proton = 100;
  DeltaT_PiM1 = 90;
  DeltaT_PiM2 = 90;

  //DeltaT
  for(Int_t dt = 0; dt < gpart; dt++){

    if(stat[dt] <= 0){
      continue;
    }
    FindHist("DeltaT"+sLabel)->Fill(delta_tof[dt]);
    
    if(id[dt] == 2212){
      FindHist("DeltaTPro"+sLabel)->Fill(delta_tof[dt]);
      DeltaT_Proton = delta_tof[dt];
      FindHist("DeltaTMomvsPro"+sLabel)->Fill(p[dt],DeltaT_Proton);
    }
    if(id[dt] == 211){
      FindHist("DeltaTPiP"+sLabel)->Fill(delta_tof[dt]);
      
      if(DeltaT_PiP1 == 110){
	DeltaT_PiP1 = delta_tof[dt];
	FindHist("DeltaTMomvsPiP1"+sLabel)->Fill(p[dt],DeltaT_PiP1);
      }
      else if(DeltaT_PiP1 != 110){
	DeltaT_PiP2 = delta_tof[dt];
	FindHist("DeltaTMomvsPiP2"+sLabel)->Fill(p[dt],DeltaT_PiP2);
      }
    }
    if(id[dt] == -211){
      FindHist("DeltaTPiM"+sLabel)->Fill(delta_tof[dt]);
      
      if(DeltaT_PiM1 == 90){
	DeltaT_PiM1 = delta_tof[dt];
	FindHist("DeltaTMomvsPiM1"+sLabel)->Fill(p[dt],DeltaT_PiM1);
      }
      else if(DeltaT_PiM1 != 90){
	DeltaT_PiM2 = delta_tof[dt];
	FindHist("DeltaTMomvsPiM2"+sLabel)->Fill(p[dt],DeltaT_PiM2);
      }
    }
  }

 

  FindHist("DeltaTProPiP1"+sLabel)->Fill(DeltaT_Proton,DeltaT_PiP1);
  FindHist("DeltaTProPiP2"+sLabel)->Fill(DeltaT_Proton,DeltaT_PiP2);

  FindHist("DeltaTProPiM1"+sLabel)->Fill(DeltaT_Proton,DeltaT_PiM1);
  FindHist("DeltaTProPiM2"+sLabel)->Fill(DeltaT_Proton,DeltaT_PiM2);

  FindHist("DeltaTPiP1PiP2"+sLabel)->Fill(DeltaT_PiP1,DeltaT_PiP2);
  FindHist("DeltaTPiM1PiM2"+sLabel)->Fill(DeltaT_PiM1,DeltaT_PiM2);
  FindHist("DeltaTPiP1PiM2"+sLabel)->Fill(DeltaT_PiP1,DeltaT_PiM2);
  FindHist("DeltaTPiM1PiP2"+sLabel)->Fill(DeltaT_PiM1,DeltaT_PiP2);


  DeltaT_PiP1 = 110;
  DeltaT_PiP2 = 110;
  DeltaT_Proton = 100;
  DeltaT_PiM1 = 90;
  DeltaT_PiM2 = 90;

  DeltaB_PiP1 = 110;
  DeltaB_PiP2 = 110;
  DeltaB_Proton = 100;
  DeltaB_PiM1 = 90;
  DeltaB_PiM2 = 90;

 //DeltaBeta
  for(Int_t db = 0; db < gpart; db++){

    if(stat[db] <= 0){
      continue;
    }
    FindHist("DeltaB"+sLabel)->Fill(delta_beta[db]);
    
    if(id[db] == 2212){
      FindHist("DeltaBPro"+sLabel)->Fill(delta_beta[db]);
      DeltaB_Proton = delta_beta[db];
      FindHist("DeltaBMomvsPro"+sLabel)->Fill(p[db],DeltaB_Proton);
    }
    if(id[db] == 211){
      FindHist("DeltaBPiP"+sLabel)->Fill(delta_beta[db]);
      
      if(DeltaB_PiP1 == 110){
	DeltaB_PiP1 = delta_beta[db];
	FindHist("DeltaBMomvsPiP1"+sLabel)->Fill(p[db],DeltaB_PiP1);
      }
      else if(DeltaB_PiP1 != 110){
	DeltaB_PiP2 = delta_beta[db];
	FindHist("DeltaBMomvsPiP2"+sLabel)->Fill(p[db],DeltaB_PiP2);
      }
    }
    if(id[db] == -211){
      FindHist("DeltaBPiM"+sLabel)->Fill(delta_beta[db]);
      
      if(DeltaB_PiM1 == 90){
	DeltaB_PiM1 = delta_beta[db];
	FindHist("DeltaBMomvsPiM1"+sLabel)->Fill(p[db],DeltaB_PiM1);
      }
      else if(DeltaB_PiM1 != 90){
	DeltaB_PiM2 = delta_beta[db];
	FindHist("DeltaBMomvsPiM2"+sLabel)->Fill(p[db],DeltaB_PiM2);
      }
    }
  }

  DeltaB_PiP1 = 110;
  DeltaB_PiP2 = 110;
  DeltaB_Proton = 100;
  DeltaB_PiM1 = 90;
  DeltaB_PiM2 = 90;

}

//Lorenzo Functions

Double_t CLAStoHS::GetWeight(Double_t *par, Double_t x) {
  // *par need to be an array with the 7 parameters that you get from the previous fit)                                                                                              
  // x is the invariant mass that need to have a wieghting factor                                                                                                                    
  // You will feed your weighted plots with  Fill(mass,GetWeight(par,mass)) for 1 dimension and Fill(whatever,mass,GetWeight(par,mass)) for 2 dimensions                             
  // Par need the one correct for this combination of particles                                                                                                                      
      
   // TF1 *total = new TF1("total","pol3(0)+gaus(4)",0.63,0.95);
  total->SetParameters(par);
  // TF1 *g2    = new TF1("g2","gaus",0.74,0.82);
  g2->SetParameters(&par[5]);
    
  Double_t weight_v =  g2->Eval(x)/total->Eval(x);
  
  //  delete total;
  //  delete g2;
  
  return weight_v;
}

void CLAStoHS::GetEventPartBranches(Int_t evi){
  b_cx->GetEvent(evi);
  b_cy->GetEvent(evi);
  b_cz->GetEvent(evi);
  b_vx->GetEvent(evi);
  b_vy->GetEvent(evi);
  b_vz->GetEvent(evi);
  b_p->GetEvent(evi);
  b_m->GetEvent(evi);
  b_q->GetEvent(evi);
  //timing
  b_sc->GetEvent(evi);
  b_sc_t->GetEvent(evi);
  b_tr_time->GetEvent(evi);

  //tagger
  b_taggoodhit->GetEvent(evi);
  b_tag_energy->GetEvent(evi);
  b_dt_st_tag->GetEvent(evi);
  b_vertex_time->GetEvent(evi);
}

Double_t* CLAStoHS::DeltaT(){

  b_sc->GetEntry(fEntry); // Variable describing which bit of the array of each event information is refering to.
  b_sc_t->GetEntry(fEntry);  //start counter time from ToF paddles, measured with respect to the trigger to tr_time
  b_sc_r->GetEntry(fEntry); //The flight path of each particle (Path length?)

  b_tr_time->GetEntry(fEntry); //The trigger time, or event start time, choice of trigger electron is reconstructed from those available before my code.

  b_p->GetEntry(fEntry); //momentum p for each particle
  b_id->GetEntry(fEntry); //ntuple ID for each particle
  b_q->GetEntry(fEntry); //ntuple charge for each particle
  b_b->GetEntry(fEntry); //beta measured

  b_gpart->GetEntry(fEntry); //number of particle by gpart method
  b_stat->GetEntry(fEntry); //wether the particle is a valid track, if 0 


  //Things to be added
  //If statement looking at the charge on a particle
  //IDing, Pi+, Pi-, then eventually general
  //Getting expected masses directly from PDG lookup possible?
  //Allow for variable 
  //Function should accept or reject and correct IDs before passing to is good event ghosts.

   //Delta t, difference between the predicted and measured 
  
  // tof_meas = sc_t; //Time difference between the interaction vertex and the ToF scintilation paddles
  
  // tof_calc = (sc_r/c)*(1 + (mass/p)^2)^0.5  //formula for tof_calc, from mike williams thesis
  tof_meas = 0;
  tof_calc = 0;


  for(UChar_t ifs=0;ifs<gpart;ifs++){  //looping over the values of gpart in each entry
   tof_meas = 0;
   tof_calc = 0;
   delta_tof[ifs] = 0;
   //Need to correlate gpart with sc entries

   // if(stat[ifs] <= 0){
   //  continue;
   //	}

   tof_meas = sc_t[sc[ifs]]-tr_time; //sc_t is measured relative to the trigger time tr, so this must be subtracted. 

   // tof_meas = (sc_t[sc[ifs]]-tr_time)/ b[ifs];  //updated version correcting for momentum. Might be wrong
    
    //need some if statements to consider different cases.

    // SC correlates gpart blocks to SC ones.

   if((int)q[ifs] == 1){
     //The 29.9792458 is speed of light per 100 nanoseconds to match units with cm and ns.
     if(id[ifs] == 2212){
       tof_calc = (sc_r[sc[ifs]]/29.9792458)*TMath::Sqrt((1 + TMath::Power(((0.938272046)/(p[ifs])),2))); 
     }
     else if(id[ifs] == 211){
       tof_calc = (sc_r[sc[ifs]]/29.9792458)*TMath::Sqrt((1 + TMath::Power(((0.13957018)/(p[ifs])),2)));
     }
     else{
       delta_tof[ifs] = 110;
       continue;
	 }    
   }
   else if((int)q[ifs] == -1){
     if(id[ifs]== -211){
       tof_calc = (sc_r[sc[ifs]]/29.9792458)*TMath::Sqrt((1 + TMath::Power(((0.13957018)/(p[ifs])),2)));
     }
     else{
       delta_tof[ifs] = 90;
       continue;
     }
   }
   else if((int)q[ifs] == 0){
     delta_tof[ifs] = 100;
     continue;
   }
   else{
     delta_tof[ifs] = 115;
     continue;
   }
   delta_tof[ifs] = tof_meas - tof_calc; 
  }
  return delta_tof;
  
}


Double_t* CLAStoHS::DeltaBeta(){

  b_sc->GetEntry(fEntry); // Variable describing which bit of the array of each event information is refering to.
  b_sc_t->GetEntry(fEntry);  //start counter time from ToF paddles, measured with respect to the trigger to tr_time
  b_sc_r->GetEntry(fEntry); //The flight path of each particle (Path length?)

  b_tr_time->GetEntry(fEntry); //The trigger time, or event start time, choice of trigger electron is reconstructed from those available before my code.

  b_p->GetEntry(fEntry); //momentum p for each particle
  b_id->GetEntry(fEntry); //ntuple ID for each particle
  b_q->GetEntry(fEntry); //ntuple charge for each particle
  b_b->GetEntry(fEntry); //beta measured

  b_gpart->GetEntry(fEntry); //number of particle by gpart method
  b_stat->GetEntry(fEntry); //wether the particle is a valid track, if 0 

  
  // tof_meas = sc_t; //Time difference between the interaction vertex and the ToF scintilation paddles
  
  // tof_calc = (sc_r/c)*(1 + (mass/p)^2)^0.5  //formula for tof_calc, from mike williams thesis
  beta_meas = 0;
  beta_calc = 0;
  Mass_Proton = 0.938272046;  // from pdg
  Mass_Pion   = 0.13957018;    // from pdg
  
  
  for(UChar_t idb=0;idb<gpart;idb++){  //looping over the values of gpart in each entry
   beta_meas = 0;
   beta_calc = 0;
   delta_beta[idb] = 0;
   

   //Need to correlate gpart with sc entries

   
   // if(stat[ifs] <= 0){
   // continue;
   //	}

   beta_meas = b[idb];
    //need some if statements to consider different cases.

    // SC correlates gpart blocks to SC ones.

   if((int)q[idb] == 1){
     //  The 0.299792458 is speed of light in meters per nanosecond
     
     //  sqrt (P^2c^2/m^2c^4+p^2c^2)
     
     if(id[idb] == 2212){
       //  beta_calc =  TMath::Sqrt(   (TMath::Power(p[idb],2)*TMath::Power(0.299792458,2)) / ((TMath::Power(Mass_Proton,2)*TMath::Power(0.299792458,4))+(TMath::Power(p[idb],2)*TMath::Power(0.299792458,2)) )   );
       beta_calc = TMath::Sqrt((p[idb]*p[idb])/((Mass_Proton*Mass_Proton)+(p[idb]*p[idb])));
     }
     else if(id[idb] == 211){
       // beta_calc = TMath::Sqrt(   (  TMath::Power(p[idb],2)*TMath::Power(0.299792458,2)  ) / (  TMath::Power(Mass_Pion,2)*TMath::Power(0.299792458,4)+TMath::Power(p[idb],2)*TMath::Power(0.299792458,2) )   );
       beta_calc = TMath::Sqrt((p[idb]*p[idb])/((Mass_Pion*Mass_Pion)+(p[idb]*p[idb])));
     }
     else{
       delta_beta[idb] = 110;
       continue;
     }    
   }
   else if((int)q[idb] == -1){
     if(id[idb]== -211){
       // beta_calc = TMath::Sqrt(   (  TMath::Power(p[idb],2)*TMath::Power(0.299792458,2)  ) / (  TMath::Power(Mass_Pion,2)*TMath::Power(0.299792458,4)+TMath::Power(p[idb],2)*TMath::Power(0.299792458,2) )   );
       beta_calc = TMath::Sqrt((p[idb]*p[idb])/((Mass_Pion*Mass_Pion)+(p[idb]*p[idb])));
     }
     else{
       delta_beta[idb] = 90;
       continue;
     }
   }
   else if((int)q[idb] == 0){
     delta_beta[idb] = 100;
     continue;
   }
   else{
     delta_beta[idb] = 115;
     continue;
   }
   delta_beta[idb] = beta_meas - beta_calc; 
  }
  return delta_beta;
  
}
