//--Author      DI Glazier 30/06/2014
//--Rev
//--Update
//--Description
//HASPECT Event Reconstruction
//THSParticle
//Persistant Data structure
//Contains reconstructed information required for data analysis

#ifndef __THSParticle_h__
#define __THSParticle_h__

#include "TLorentzVector.h"
#include "TVector3.h"
#include "TDatabasePDG.h"

class THSParticle {
 private:
 protected:
  TLorentzVector fP4;  //4-vector 
  TVector3 fVertex;     //particle vertex position
  //TVector3 fPol;     //polarisation vector
  Int_t fPDGCode;           //PDG number
  Double_t fPDGMass;
  Double_t fMeasMass;
  Double_t fTime;
 
 public:
  THSParticle();  	        //Constructor
  THSParticle(int code);  	        //Constructor
  virtual ~THSParticle(){};	     		//Destructor


  //Setting functions
  void SetPDGcode(Int_t code){fPDGMass = TDatabasePDG::Instance()->GetParticle(fPDGCode=code)->Mass();}
  void SetP4(TLorentzVector v){fP4=v;}
  void SetVectPDG(TLorentzVector v){fP4.SetVectM(v.Vect(),fPDGMass);}
  void SetP4(TLorentzVector *v){fP4=*v;}
  void SetXYZT(Double_t X,Double_t Y,Double_t Z,Double_t T){fP4.SetXYZT(X,Y,Z,T);}
  void SetXYZM(Double_t X,Double_t Y,Double_t Z,Double_t M){fP4.SetXYZM(X,Y,Z,M);}
  void SetVertex(TVector3 v){fVertex=v;}
  void SetVertex(Double_t X,Double_t Y,Double_t Z){fVertex.SetXYZ(X,Y,Z);}
  //void SetPol(TVector3 p){fPol=p;}
  //void SetPol(Double_t X,Double_t Y,Double_t Z){fPol.SetXYZ(X,Y,Z);}
  void SetTime(Double_t time){fTime=time;};
  void SetMeasMass(Double_t mass){fMeasMass=mass;};

  //Getting functions
  TLorentzVector P4(){return fP4;}
  TLorentzVector* P4p(){return &fP4;}
  TVector3 Vertex(){return fVertex;}
  //TVector3 Pol(){return fPol;}
  Int_t PDG(){return fPDGCode;}
  Double_t PDGMass(){return fPDGMass;}
  Double_t MassDiff(){return fPDGMass-fMeasMass;}
  Double_t Time(){return fTime;}
  //Utility functions


  ClassDef(THSParticle,1) //class THSParticle
};
#endif
