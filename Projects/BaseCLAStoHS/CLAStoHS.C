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
  SafeDelete(fPionComb); //Added for fPionComb 1
  SafeDelete(fPiP); //Pi plus
  SafeDelete(fPiM); //Pi Minus
  SafeDelete(f2Pi); //Both pions
  SafeDelete(fBeamEnergySelection); //Added for selecting beam energies of interest
  SafeDelete(fProton); //Added for looking at the scattered proton
  SafeDelete(fMissOmegaPiP); //Adding for the Pi+ missing combination
  SafeDelete(fMissOmegaPiM); //Adding for the Pi- missing combination
  SafeDelete(fProtonPiP); //Proton and Pi+ Combination
  SafeDelete(fProtonPiM); //Proton and Pi- Combination
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
  fPiP=new TLorentzVector(); //PiP
  fPiM=new TLorentzVector(); //PiM
  f2Pi=new TLorentzVector(); //2 Pions (Pi+ and Pi-)
  fPionComb=new TLorentzVector(); //Pion Combination
  fBeamEnergySelection=new TLorentzVector();
  fProton=new TLorentzVector(); //Scattered Proton
  fMissOmegaPiP=new TLorentzVector(); //Pi+ and Missing 4 Vector
  fMissOmegaPiM=new TLorentzVector(); //Pi- and Missing 4 Vector
  fProtonPiP=new TLorentzVector(); //Proton and Pi+
  fProtonPiM=new TLorentzVector(); //Proton and Pi-

  fOutTree->Branch("beam",&fHSgamma,buff,split);
  fOutTree->Branch("miss",&fMissing,buff,split);
  fOutTree->Branch("PiM",&fPiM,buff,split);
  fOutTree->Branch("PiP",&fPiP,buff,split);
  fOutTree->Branch("2Pi",&f2Pi,buff,split);
  fOutTree->Branch("Pion Comb",&fPionComb,buff,split);
  fOutTree->Branch("Beam Energy Selection",&fBeamEnergySelection,buff,split);
  fOutTree->Branch("Scattered Proton",&fProton,buff,split);
  fOutTree->Branch("Mmiss",&Mmiss,buff,split);
  fOutTree->Branch("fgID",&fgID,buff,split);

 // //sWeighter make new output tree
 //   fOutTree->Branch("SigW",&fSigW,"SigW/F");
 //   fOutTree->Branch("BckW",&fBckW,"BckW/F");

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

  // Testing DeltaT Function
  //  DeltaT();
  // DeltaBeta();

 
  
  if(!IsGoodEventGhosts())
     return kTRUE; //not the event we want so exit

  //if(!GetsWeight()) return kTRUE; 
    
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
  *fPiM=fDetParticle[0]->P4(); // The Pi Minus
  *fPiP=fDetParticle[1]->P4(); // The Pi Plus
 
  *fPionComb=fDetParticle[0]->P4()+fDetParticle[1]->P4();
  *f2Pi=fDetParticle[0]->P4()+fDetParticle[1]->P4();
 
  //Ouputting the energy of the tagged photon
  *fBeamEnergySelection = fHSgamma->P4();

  //Considering the properties of the scattered proton
  *fProton = /*gTarget-*/fDetParticle[2]->P4();

  //Sweights Variables
  Mmiss=fMissing->M();
  MPiP=fPiP->M();
  MPiM=fPiM->M();
  M2Pi=f2Pi->M();

  //Dalitz Plot variables
  *fMissOmegaPiP=*fPiP+*fMissing;
  *fMissOmegaPiM=*fPiM+*fMissing;

  *fProtonPiP=*fProton+*fPiP;
  *fProtonPiM=*fProton+*fPiM;


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
  else{    
    return kTRUE; //it is a good event
  }
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
  if(!(std::equal(EventState.begin(),EventState.end(),fFinalState.begin()))){
 return kFALSE; // return if IDs are not those requested
  }
  else{
 return kTRUE; //it is a good event
  }
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
  // fOutput->Add(MapHist(new TH2F("histo_n11"+sLabel,"Missing Mass vs #omega MassCand1;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",200,-0.2,1.20,200,0.0,2.0)));			    
  // fOutput->Add(MapHist(new TH2F("histo_n12"+sLabel,"Missing Mass vs #omega MassCand2;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));
  // fOutput->Add(MapHist(new TH2F("histo_n13"+sLabel,"Missing Mass vs #omega MassCand3;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));
  // fOutput->Add(MapHist(new TH2F("histo_n14"+sLabel,"Missing Mass vs #omega MassCand4;Missing Mass(GeV/c^{2});#omega Candidate Mass(GeV/c^{2})",50,0.05,0.20,50,0.4,1.0)));

  fOutput->Add(MapHist(new TH1F("Missing_Mass"+sLabel,"Missing Mass (GeV/c^2);Missing Mass(GeV/c^{2}))",200,-0.2,1.20)));
  
  fOutput->Add(MapHist(new TH2F("DalitzTest"+sLabel,"MissingMassPiP vs MissingMassPiM;M_{1,2}^{2}(GeV/c^{2})^{2}; M_{1,3}^{2}(GeV/c^{2})^{2}",200,-1,3,200,-1,3)));
  fOutput->Add(MapHist(new TH2F("DalitzTest2"+sLabel,"MissingMassPiP vs MissingMassPiM;M_{1,2}^{2}(GeV/c^{2})^{2}; M_{1,3}^{2}(GeV/c^{2})^{2}",200,-1,3,200,-1,3)));
  
  fOutput->Add(MapHist(new TH2F("DalitzSignal"+sLabel,"MissingMassPiP vs MissingMassPiM;M_{1,2}^{2}(GeV/c^{2})^{2}; M_{1,3}^{2}(GeV/c^{2})^{2}",200,0.5,3,200,0.5,3)));
  fOutput->Add(MapHist(new TH2F("DalitzLeftSide"+sLabel,"MissingMassPiP vs MissingMassPiM;M_{1,2}^{2}(GeV/c^{2})^{2}; M_{1,3}^{2}(GeV/c^{2})^{2}",200,0.5,3,200,0.5,3)));
  fOutput->Add(MapHist(new TH2F("DalitzRightSide"+sLabel,"MissingMassPiP vs MissingMassPiM;M_{1,2}^{2}(GeV/c^{2})^{2}; M_{1,3}^{2}(GeV/c^{2})^{2}",200,0.5,3,200,0.5,3)));
  fOutput->Add(MapHist(new TH2F("DalitzBothSides"+sLabel,"MissingMassPiP vs MissingMassPiM;M_{1,2}^{2}(GeV/c^{2})^{2}; M_{1,3}^{2}(GeV/c^{2})^{2}",200,0.5,3,200,0.5,3)));
  fOutput->Add(MapHist(new TH2F("DalitzBackSub"+sLabel,"MissingMassPiP vs MissingMassPiM;M_{1,2}^{2}(GeV/c^{2})^{2}; M_{1,3}^{2}(GeV/c^{2})^{2}",200,0.5,3,200,0.5,3)));

  fOutput->Add(MapHist(new TH2F("DalitzSignalProton"+sLabel,"ProtonPiP vs ProtonPiM;M_{1,2}^{2}(GeV/c^{2})^{2}; M_{1,3}^{2}(GeV/c^{2})^{2}",200,0.5,3,200,0.5,3)));
  fOutput->Add(MapHist(new TH2F("DalitzLeftSideProton"+sLabel,"ProtonPiP vs ProtonPiM;M_{1,2}^{2}(GeV/c^{2})^{2}; M_{1,3}^{2}(GeV/c^{2})^{2}",200,0.5,3,200,0.5,3)));
  fOutput->Add(MapHist(new TH2F("DalitzRightSideProton"+sLabel,"ProtonPiP vs ProtonPiM;M_{1,2}^{2}(GeV/c^{2})^{2}; M_{1,3}^{2}(GeV/c^{2})^{2}",200,0.5,3,200,0.5,3)));
  fOutput->Add(MapHist(new TH2F("DalitzBothSidesProton"+sLabel,"ProtonPiP vs ProtonPiM;M_{1,2}^{2}(GeV/c^{2})^{2}; M_{1,3}^{2}(GeV/c^{2})^{2}",200,0.5,3,200,0.5,3)));
  fOutput->Add(MapHist(new TH2F("DalitzBackSubProton"+sLabel,"ProtonPiP vs ProtonPiM;M_{1,2}^{2}(GeV/c^{2})^{2}; M_{1,3}^{2}(GeV/c^{2})^{2}",200,0.5,3,200,0.5,3)));


//Signal distributions
// fOutput->Add(MapHist(new TH1F("SigMPiP"+sLabel,"Sig M_{#Pi+}"+sLabel,100,0.2,1.5)));
// fOutput->Add(MapHist(new TH1F("SigMPiM"+sLabel,"Sig M_{#Pi-}"+sLabel,100,0.2,1.5)));
// fOutput->Add(MapHist(new TH1F("SigM2Pi"+sLabel,"Sig M_{2#Pi}"+sLabel,100,0.5,2)));
//  //Background distributions
// fOutput->Add(MapHist(new TH1F("SigMPiP"+sLabel,"Sig M_{#Pi+}"+sLabel,100,0.2,1.5)));
// fOutput->Add(MapHist(new TH1F("SigMPiM"+sLabel,"Sig M_{#Pi-}"+sLabel,100,0.2,1.5)));
// fOutput->Add(MapHist(new TH1F("SigM2Pi"+sLabel,"Sig M_{2#Pi}"+sLabel,100,0.5,2)));


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
  // FindHist("histo_n11"+sLabel)->Fill(fMissing->M(), fOmegaCand1->M());
  // FindHist("histo_n12"+sLabel)->Fill(fMissing->M(), fOmegaCand2->M());
  // FindHist("histo_n13"+sLabel)->Fill(fMissing->M(), fOmegaCand3->M());
  // FindHist("histo_n14"+sLabel)->Fill(fMissing->M(), fOmegaCand4->M());

   FindHist("Missing_Mass"+sLabel)->Fill(fMissing->M());


   FindHist("DalitzTest"+sLabel)->Fill((Mmiss+MPiP)*(Mmiss+MPiP),(Mmiss+MPiM)*(Mmiss+MPiM));
   FindHist("DalitzTest2"+sLabel)->Fill(fMissOmegaPiP->M2(),fMissOmegaPiM->M2());

   if(Mmiss >= 0.762 && Mmiss<= 0.802){
     FindHist("DalitzSignal"+sLabel)->Fill(fMissOmegaPiP->M2(),fMissOmegaPiM->M2());
     FindHist("DalitzSignalProton"+sLabel)->Fill(fProtonPiP->M2(),fProtonPiM->M2());
   }
   if(Mmiss >= 0.722 && Mmiss<= 0.762){
     FindHist("DalitzLeftSide"+sLabel)->Fill(fMissOmegaPiP->M2(),fMissOmegaPiM->M2());
     FindHist("DalitzBothSides"+sLabel)->Fill(fMissOmegaPiP->M2(),fMissOmegaPiM->M2());
     FindHist("DalitzLeftSideProton"+sLabel)->Fill(fProtonPiP->M2(),fProtonPiM->M2());
     FindHist("DalitzBothSidesProton"+sLabel)->Fill(fProtonPiP->M2(),fProtonPiM->M2());

   }
   if(Mmiss >= 0.802 && Mmiss<= 0.842){
     FindHist("DalitzRightSide"+sLabel)->Fill(fMissOmegaPiP->M2(),fMissOmegaPiM->M2());
     FindHist("DalitzBothSides"+sLabel)->Fill(fMissOmegaPiP->M2(),fMissOmegaPiM->M2());
     FindHist("DalitzRightSideProton"+sLabel)->Fill(fProtonPiP->M2(),fProtonPiM->M2());
     FindHist("DalitzBothSidesProton"+sLabel)->Fill(fProtonPiP->M2(),fProtonPiM->M2());
   }


 
   //Histograms with signal weight
   // FindHist("SigMPiP"+sLabel)->Fill(MPiP,fSigW);
   // FindHist("SigMPiM"+sLabel)->Fill(MPiM,fSigW);
   // FindHist("SigM2Pi"+sLabel)->Fill(M2Pi,fSigW);

   // //Histograms with background weight
   // FindHist("BckMPiP"+sLabel)->Fill(MPiP,fBckW);
   // FindHist("BckMPiM"+sLabel)->Fill(MPiM,fBckW);
   // FindHist("BckM2Pi"+sLabel)->Fill(M2Pi,fBckW);
  
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
