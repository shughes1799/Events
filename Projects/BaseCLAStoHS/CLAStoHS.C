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
  SafeDelete(fBeamEnergySelection);

}
void CLAStoHS::Begin(TTree * /*tree*/)
{

  TString option = GetOption();
  THSOutput::HSBegin(fInput,fOutput);
}

void CLAStoHS::SlaveBegin(TTree * /*tree*/)
{
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

  //General Cut Variables
  MissingMCLow = 0.105;
  MissingMCHigh = 0.170;
  OmegaMCLow = 0.65;
  OmegaMCHigh = 0.92;
  EtaMCLow = 0;
  EtaMChigh = 0;

  //Combination 1 variables
  Comb1MissingMCLow = 0.105;
  Comb1MissingMCHigh = 0.170;
  Comb1OmegaMCLow = 0.76;
  Comb1OmegaMCHigh = 0.81;
  Comb1EtaMCLow = 0;
  Comb1EtaMChigh = 0;

  //Combination 2 variables
  Comb2MissingMCLow = 0.105;
  Comb2MissingMCHigh = 0.170;
  Comb2OmegaMCLow = 0.76;
  Comb2OmegaMCHigh = 0.81;
  Comb2EtaMCLow = 0;
  Comb2EtaMChigh = 0;

  //Combination 3 variables
  Comb3MissingMCLow = 0.105;
  Comb3MissingMCHigh = 0.170;
  Comb3OmegaMCLow = 0.76;
  Comb3OmegaMCHigh = 0.81;
  Comb3EtaMCLow = 0;
  Comb3EtaMChigh = 0;

  //Combination 4 variables
  Comb4MissingMCLow = 0.105;
  Comb4MissingMCHigh = 0.170;
  Comb4OmegaMCLow = 0.76;
  Comb4OmegaMCHigh = 0.81;
  Comb4EtaMCLow = 0.53;
  Comb4EtaMChigh = 0.57;


  //Combination Parameters
  double Par1[7] = {93577.99,-23139.8,43067.9,-23129.5,982.942,0.783338,0.0139013}; //Cand 1
  double Par2[7] = {-16970.9,57122.7,-57869.2,17910.2,1116.62,0.782595,0.0117021};  //Cand 2
  double Par3[7] = {-9192.09,25178.3,-15389.6,-440.733,1262.17,0.782384,0.0119449}; //Cand 3
  double Par4[7] = {-38164.1,147453,-181252,72151.7,1207.61,0.781922,0.0109179};    //Cand 4

  for (int i=0; i<7; i++) {
    OmegaComb1Par[i] = Par1[i];
    OmegaComb2Par[i] = Par2[i];
    OmegaComb3Par[i] = Par3[i];
    OmegaComb4Par[i] = Par4[i];
  }

  //Branches for the Output Tree
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
  if(!IsGoodEventGhosts()) return kTRUE; //not the event we want so exit
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
  else return kTRUE; //it is a good event
}
Bool_t CLAStoHS::IsGoodEventGhosts(){
  //this algorithm uses std::vec to compare the particle ids of the event
  //and those defined in fFinalState, event is good if they match

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
  if(nreal!=(Int_t)fNdet) return kFALSE;//only analyse event with correct final state
  //put in order for comparison with requested fFinalState
  std::sort(EventState.begin(),EventState.begin()+nreal);
  //now compare with the particles defined in fFinalState 
  if(!(std::equal(EventState.begin(),EventState.end(),fFinalState.begin()))) return kFALSE; // return if IDs are not those requested
  else return kTRUE; //it is a good event
}

void CLAStoHS::MakeDetected(){
  //this function controls the interfaciing of the reconstructed data to THSParticles
  Int_t iIDall[gpart]; //array containing order of particle IDs in id array for all tracks (incl ghosts) 
  Int_t iID[fNdet]; //array containing order of particle IDs in id array for "real" tracks
  TMath::Sort((Int_t)gpart,id,iIDall,kFALSE); //order the array in asscending order(kFALSE), e.g. -211,211,211
  //write only the indexes of the real tracks to the ID array
  UInt_t Ndet=0;
  for(Int_t ireal=0;ireal<gpart;ireal++) if(stat[ireal]>0)iID[Ndet++]=iIDall[ireal];
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
  fOutput->Add(MapHist(new TH2F("histo_n11"+sLabel,"Missing Mass vs #omega MassCand1;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));			    
  fOutput->Add(MapHist(new TH2F("histo_n12"+sLabel,"Missing Mass vs #omega MassCand2;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",300,-2,2,300,-2,2)));
  fOutput->Add(MapHist(new TH2F("histo_n13"+sLabel,"Missing Mass vs #omega MassCand3;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("histo_n14"+sLabel,"Missing Mass vs #omega MassCand4;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));

  //Selected Region Histograms
  fOutput->Add(MapHist(new TH2F("OmegaCandComb1"+sLabel,"Missing Mass vs #omega MassCand1;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.1,0.175,50,0.75,0.82)));		    
  fOutput->Add(MapHist(new TH2F("OmegaCandComb1Other2Pions"+sLabel,"Missing Mass vs #omega MassCand4;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.1,0.175,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb1NotSelected"+sLabel,"Missing Mass vs #omega MassCand1;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb1BeamEnergy"+sLabel,"Beam Energy vs #omega MassCand1;Beam Energy(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,1.75,4.0,50,0.75,0.82)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb1DeltaZ"+sLabel,"#DeltaZ vs #omega MassCand1;#DeltaZ Vextex(cm);#omega Candidate Mass(GeV/c^{2})",200,-20,20,50,0.75,0.82)));
 
  fOutput->Add(MapHist(new TH2F("OmegaCandComb2"+sLabel,"Missing Mass vs #omega MassCand2;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.1,0.175,50,0.75,0.82)));	           
  fOutput->Add(MapHist(new TH2F("OmegaCandComb2Other2Pions"+sLabel,"Missing Mass vs #omega MassCand3;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.1,0.175,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb2NotSelected"+sLabel,"Missing Mass vs #omega MassCand2;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb2BeamEnergy"+sLabel,"Beam Energy vs #omega MassCand2;Beam Energy(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,1.75,4.0,50,0.75,0.82)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb2DeltaZ"+sLabel,"#DeltaZ vs #omega MassCand2;#DeltaZ Vextex(cm);#omega Candidate Mass(GeV/c^{2})",200,-20,20,50,0.75,0.82)));

  fOutput->Add(MapHist(new TH2F("OmegaCandComb3"+sLabel,"Missing Mass vs #omega MassCand3;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.1,0.175,50,0.75,0.82)));		    
  fOutput->Add(MapHist(new TH2F("OmegaCandComb3Other2Pions"+sLabel,"Missing Mass vs #omega MassCand2;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.1,0.175,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb3NotSelected"+sLabel,"Missing Mass vs #omega MassCand3;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb3BeamEnergy"+sLabel,"Beam Energy vs #omega MassCand3;Beam Energy(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,1.75,4.0,50,0.75,0.82)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb3DeltaZ"+sLabel,"#DeltaZ vs #omega MassCand3;#DeltaZ Vextex(cm);#omega Candidate Mass(GeV/c^{2})",200,-20,20,50,0.75,0.82)));

  fOutput->Add(MapHist(new TH2F("OmegaCandComb4"+sLabel,"Missing Mass vs #omega MassCand4;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.1,0.175,50,0.75,0.82)));		    
  fOutput->Add(MapHist(new TH2F("OmegaCandComb4Other2Pions"+sLabel,"Missing Mass vs #omega MassCand1;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.1,0.175,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb4NotSelected"+sLabel,"Missing Mass vs #omega MassCand4;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb4BeamEnergy"+sLabel,"Beam Energy vs #omega MassCand4;Beam Energy(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,1.75,4.0,50,0.75,0.82)));
  fOutput->Add(MapHist(new TH2F("OmegaCandComb4DeltaZ"+sLabel,"#DeltaZ vs #omega MassCand4;#DeltaZ Vextex(cm);#omega Candidate Mass(GeV/c^{2})",200,-20,20,50,0.75,0.82)));


  //Plots for determining fitting parameters for the data
  fOutput->Add(MapHist(new TH2F("OmegaComb1FittingPlot"+sLabel,"#omega MassCand1;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0,0.3,200,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaComb2FittingPlot"+sLabel,"#omega MassCand2;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0,0.3,200,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaComb3FittingPlot"+sLabel,"#omega MassCand3;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0,0.3,200,0.4,1.0)));
  fOutput->Add(MapHist(new TH2F("OmegaComb4FittingPlot"+sLabel,"#omega MassCand4;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0,0.3,200,0.4,1.0)));

  //Weighted Dalitz Plots
  
  fOutput->Add(MapHist(new TH2F("WeightedOmegaCandvsBeamEnergy"+sLabel,"Beam Energy vs #omega#pi^{-}#pi^{+};Beam Energy(GeV/c^{2});#omega#pi^{-}#pi^{+}(GeV/c^{2})",50,1.75,4.0,100,1.0,2.0)));
  
  //end of histogram list
  TDirectory::AddDirectory(kTRUE); //back to normal
}
void CLAStoHS::FillHistograms(TString sCut,Int_t bin){
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
  }
  else{
    FindHist("OmegaCandComb4NotSelected"+sLabel)->Fill(fMissing->M(),fOmegaCand4->M()); //Not selected region
  }

  //Weighted Histograms
  
  if (fMissing->M() >= MissingMCLow && fMissing->M() <= MissingMCHigh && fOmegaCand1->M() >= OmegaMCLow && fOmegaCand1->M()<= OmegaMCHigh){
    
    (TH2F)FindHist("WeightedOmegaCandvsBeamEnergy"+sLabel)->Fill(fBeamEnergySelection->E(),(*fOmegaCand1+*fPionComb1).M(),GetWeight(OmegaComb1Par,fOmegaCand1->M()));
    (TH2F)FindHist("OmegaComb1FittingPlot"+sLabel)->Fill(fMissing->M(),fOmegaCand1->M(),GetWeight(OmegaComb1Par,fOmegaCand1->M()));
  }
  if (fMissing->M() >= MissingMCLow && fMissing->M() <= MissingMCHigh && fOmegaCand2->M() >= OmegaMCLow && fOmegaCand2->M()<= OmegaMCHigh){
    
    (TH2F)FindHist("WeightedOmegaCandvsBeamEnergy"+sLabel)->Fill(fBeamEnergySelection->E(),(*fOmegaCand2+*fPionComb2).M(),GetWeight(OmegaComb2Par,fOmegaCand2->M()));
    (TH2F)FindHist("OmegaComb2FittingPlot"+sLabel)->Fill(fMissing->M(),fOmegaCand2->M(),GetWeight(OmegaComb2Par,fOmegaCand2->M()));
  }
  if (fMissing->M() >= MissingMCLow && fMissing->M() <= MissingMCHigh && fOmegaCand3->M() >= OmegaMCLow && fOmegaCand3->M()<= OmegaMCHigh){
    
    (TH2F)FindHist("WeightedOmegaCandvsBeamEnergy"+sLabel)->Fill(fBeamEnergySelection->E(),(*fOmegaCand3+*fPionComb3).M(),GetWeight(OmegaComb3Par,fOmegaCand3->M()));
    (TH2F)FindHist("OmegaComb3FittingPlot"+sLabel)->Fill(fMissing->M(),fOmegaCand3->M(),GetWeight(OmegaComb3Par,fOmegaCand3->M()));
  }
  if (fMissing->M() >= MissingMCLow && fMissing->M() <= MissingMCHigh && fOmegaCand4->M() >= OmegaMCLow && fOmegaCand4->M()<= OmegaMCHigh){
    
    (TH2F)FindHist("WeightedOmegaCandvsBeamEnergy"+sLabel)->Fill(fBeamEnergySelection->E(),(*fOmegaCand4+*fPionComb4).M(),GetWeight(OmegaComb4Par,fOmegaCand4->M()));
    (TH2F)FindHist("OmegaComb4FittingPlot"+sLabel)->Fill(fMissing->M(),fOmegaCand4->M(),GetWeight(OmegaComb4Par,fOmegaCand4->M()));
  }
  
}


//Lorenzo Functions

Double_t CLAStoHS::GetWeight(Double_t *par, Double_t x) {
  // *par need to be an array with the 7 parameters that you get from the previous fit)                                                                                              
  // x is the invariant mass that need to have a wieghting factor                                                                                                                    
  // You will feed your weighted plots with  Fill(mass,GetWeight(par,mass)) for 1 dimension and Fill(whatever,mass,GetWeight(par,mass)) for 2 dimensions                             
  // Par need the one correct for this combination of particles                                                                                                                      
      
  TF1 *total = new TF1("total","pol3(0)+gaus(4)",0.63,0.95);
  total->SetParameters(par);
  TF1 *g2    = new TF1("g2","gaus",0.74,0.82);
  g2->SetParameters(&par[4]);
    
  Double_t weight_v =  g2->Eval(x)/total->Eval(x);
  
  delete total;
  delete g2;
  
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
