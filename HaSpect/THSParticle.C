//--Author      DI Glazier 30/06/2014
//--Rev
//--Update
//--Description
//HASPECT Event Reconstruction
//THSParticle
//Persistant Data structure
//Contains reconstructed information required for data analysis

#include "THSParticle.h"

ClassImp(THSParticle)

THSParticle::THSParticle() : fP4(), fVertex(), fPDGCode(0), fPDGMass(0), fMeasMass(0) {

}
THSParticle::THSParticle(int code) : fP4(), fVertex(), fPDGCode(0), fPDGMass(0), fMeasMass(0) {
  SetPDGcode(code);
}
