//--Author      DI Glazier 25/01/2015
//--Rev
//--Update
//--Description
//HASPECT Event Reconstruction
//THSLongPS
//Class to perform general Longitudinal Phasespace Analysis
#include "THSLongPS.h"
#include <TLorentzRotation.h>

THSLongPS::THSLongPS(Int_t Np){
  //PARTICLE 0 MUST BE BARYON!!!!!!!!!!!!!!!!!!!!!!!!
  fNpart=Np;
  fP4s.reserve(fNpart);
  fCM.SetXYZT(0,0,0,0);
  fSector=0;
  fSize=0;
  //fSectName.reserve(Nsector);
   for(Int_t i=1;i<fNpart;i++){//loop over numbers at top,bottom vertex (1->fNpart-1) must always have at least 1 on top and bottom!!
    //increment the sector number
    Int_t NT=fNpart-i; //number of particles on top 
    Int_t NB=fNpart-NT;  //number of particles on bottom
    MakeIndices(Np,NT);  //make vectors with Top and Botton particle indices
 
   }//end topologies

  cout<<"For "<<Np <<"  particles there are "<<fSector<< "valid sectors"<<endl; 
  fNSector=fSector;
  fSector=0;
}
void THSLongPS::MakeIndices(Int_t Np,Int_t Nt){//number of particle, number of particles top
  //makes all combinations of particles on top and bottom branches
  //saves in vectors fITop and fIBot
  //index of these vectors corresponds to unique Sector number
  //Note number of combinations choosing Nt from Np and ignoring order :
  // Number = Np!/(Np-Nt)!/Nt! or TMath::Factorial(N)/TMath::Factorial(N-K)/TMath::Factorial(K)

  vector<Int_t> donethis;//use to reject equivalnet permutations (not ordering)
  vector<Int_t >IP(Np); //vector containing particle numbers from 0->Np
  for(Int_t i=0;i<Np;i++)IP[i]=i; 
 vector<TString> sPart(Np);
  for(Int_t isp=0;isp<Np;isp++)sPart[isp].Form("%d",isp);//A number for each particle
  
  vector<Int_t>::iterator ite = IP.begin(); 
  do{//loop over all possible combinations
    vector<Int_t> sub(&IP[0],&IP[Nt]); //make a new vector length Nt with this permutation
    std::sort(sub.begin(),sub.end()); //order the vector (ignore permutation order)
    if(!(std::find(fITop.begin(), fITop.end(), sub) != fITop.end())){
      fITop.push_back(sub); //If these particles already used for top then do not save
      vector<Int_t> subB(&IP[Nt],&IP[Np]); 
      fIBot.push_back(subB);
    fSectName.push_back("F_");
    //   sBot.Clear();
    vector<Bool_t> isFor(Np);
    for(UInt_t isp=0;isp<Nt;isp++){
      fSectName[fSector]+=sPart[fITop[fSector][isp]]+"_";//make name
      isFor[fITop[fSector][isp]]=kTRUE;//make sector condition
    }
    fSectName[fSector]+="B_";
    for(UInt_t isp=0;isp<Np-Nt;isp++){
      fSectName[fSector]+=sPart[fIBot[fSector][isp]]+"_";
      isFor[fIBot[fSector][isp]]=kFALSE;//make sector condition
    }
    fIsForward.push_back(isFor);
    cout<<"Sector NAme "<<fSector<<" "<<fSectName[fSector]<<endl;
     fSector++;
    }
 }while(std::next_permutation(ite,ite+Np));

 }

void THSLongPS::PrintVector(vector<Int_t> vecI){
  for(Int_t i=0;i<vecI.size();i++)cout<<vecI[i]<<" ";
  cout<<endl;
}
void THSLongPS::PrintVector(vector<Bool_t> vecI){
  for(Int_t i=0;i<vecI.size();i++)cout<<vecI[i]<<" ";
  cout<<endl;
}

void THSLongPS::AddParticle(TLorentzVector p4){//add 4 vectors should be done for each particle for each event
  if(fSize<fNpart){
    fP4s[fSize++]=p4;
    fCM+=p4;
  }
}

void THSLongPS::Analyse(){
   //Construct the CM boosts
   TVector3 CMboost=-fCM.BoostVector();
   TLorentzRotation CMRot(CMboost);
 
  //loop over particles and check if they are going forward in CM
  vector< Bool_t > whichForward(fSize); //whichForward[2]=kTRUE means particle 2 is going forward
  for(Int_t ip=0;ip<fSize;ip++){
    fP4s[ip]=(CMRot*(fP4s[ip])); //cm boost
    whichForward[ip]=fP4s[ip].Z()>0;
  }
  for(fSector=0;fSector<fNSector;fSector++)
    if(fIsForward[fSector]==whichForward) break;
  
  //Now calculate masses of Top and Bottom particles
  TLorentzVector pTop;
  for(Int_t ip=0;ip<fITop[fSector].size();ip++) pTop+=fP4s[fITop[fSector][ip]];
  fMTop=pTop.M();
  fCosTh=pTop.CosTheta();

  TLorentzVector pBot;
  for(Int_t ip=0;ip<fIBot[fSector].size();ip++) pBot+=fP4s[fIBot[fSector][ip]];
  fMBot=pBot.M();
}
