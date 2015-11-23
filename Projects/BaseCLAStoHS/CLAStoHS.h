//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jun  2 15:22:29 2014 by ROOT version 5.34/14
// from TTree h10/PART
// found on file: /home/dglazier/Work/Research/HaSpect/data/pippippimMn/ntpd_43582_pass1.one-third-of-full.pippippimMn.root
//////////////////////////////////////////////////////////

#ifndef CLAStoHS_h
#define CLAStoHS_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH2.h>
#include <TF1.h>
//#include <TLorentzVector.h>
#include <TSelector.h>
#include <iostream>

#include "THSOutput.h"

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class CLAStoHS : public TSelector, public  THSOutput {
  //private:
  //Additional ouput branches
  THSParticle* fHSgamma;  //the photon beam
  TLorentzVector* fMissing; // The missing 4-vector
  TLorentzVector* fOmegaCand1; // The omega Candidate 1 4-vector The faster PiM and the faster PiP
  TLorentzVector* fOmegaCand2; // The omega Candidate 2 4-vector The faster PiM and the slower PiP
  TLorentzVector* fOmegaCand3; // The omega Candidate 3 4-vector The slower PiM and the faster PiP
  TLorentzVector* fOmegaCand4; // The omega Candidate 4 4-vector The slower PiM and the slower PiP
  TLorentzVector* fPionComb1;  // The Pion Combination 1 4-vector The slower PiM and the slower PiP
  TLorentzVector* fPionComb2;  // The Pion Combination 2 4-vector The slower PiM and the faster PiP
  TLorentzVector* fPionComb3;  // The Pion Combination 3 4-vector The faster PiM and the slower PiP
  TLorentzVector* fPionComb4;  // The Pion Combination 4 4-vector The faster PiM and the faster PiP
  TLorentzVector* fBeamEnergySelection; //Variable for converting the THSParticle to a TLorentzVector to extract the energy.
  TLorentzVector* fProton; //The scattered proton from the gamma interaction

  TLorentzVector* fPiMFast0; // The Fast Pi Minus
  TLorentzVector* fPiMSlow1; // The Slow Pi Minus
  TLorentzVector* fPiPFast2; // The Fast Pi Plus
  TLorentzVector* fPiPSlow3; // The Slow Pi Plus

  //Dalitz Variables
  TLorentzVector* fFastPiMPi0; //Combination of the Fast Pi Minus and the Pi0
  TLorentzVector* fSlowPiMPi0; //Combination of the Slow Pi Minus and the Pi0
  TLorentzVector* fFastPiPPi0; //Combination of the Fast Pi Plus and the Pi0
  TLorentzVector* fSlowPiPPi0; //Combination of the Slow Pi Plus and the Pi0

  TLorentzVector* fOmegaCand1PiMSlow1; //Combination of the Omega and Slow Pi Minus
  TLorentzVector* fOmegaCand1PiPSlow3; //Combination of the Omega and Slow Pi Minus
  TLorentzVector* fOmegaCand2PiMSlow1; //Combination of the Omega and Slow Pi Plus
  TLorentzVector* fOmegaCand2PiPFast2; //Combination of the Omega and Fast Pi Plus
  TLorentzVector* fOmegaCand3PiMFast0; //Combination of the Omega and Fast Pi Minus
  TLorentzVector* fOmegaCand3PiPSlow3; //Combination of the Omega and Slow Pi Minus
  TLorentzVector* fOmegaCand4PiMFast0; //Combination of the Omega and Fast Pi Plus
  TLorentzVector* fOmegaCand4PiPFast2; //Combination of the Omega and Fast Pi Plus

  //public:
  //Functions used to process data
  void GetEventPartBranches(Int_t evi);         //get the required branches
  void MakeParticle(THSParticle* hsp,Int_t ip); //convert to THSParticle using index ip e.g. cx[ip] etc.
  Bool_t IsGoodEvent();                         // initial test of event
  Bool_t IsGoodEventGhosts();                   // initial test of event allowing ghost tracks
  void MakeDetected();                          // convert the input data to THSParticles
  Bool_t MakeBeam(Float_t Tmid,Float_t Tcut);   // convert the beam branches into a THSParticle

  //Simon Functions
  Double_t GetWeight(Double_t *par, Double_t x); //Weighting histograms based on function parameters

 public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain

  //Histograms for the GetWeight Function
  TF1* total;
  TF1* g2;


  //Variables controlling the selection cuts on particles on interest for the different combinations of pions
  
  //General
  Float_t MissingMCLow;
  Float_t MissingMCHigh;
  Float_t OmegaMCLow;
  Float_t OmegaMCHigh;
  Float_t EtaMCLow;
  Float_t EtaMChigh;

  //Combination 1
  Float_t Comb1MissingMCLow;
  Float_t Comb1MissingMCHigh;
  Float_t Comb1OmegaMCLow;
  Float_t Comb1OmegaMCHigh;
  Float_t Comb1EtaMCLow;
  Float_t Comb1EtaMChigh;


  //Combination 2
  Float_t Comb2MissingMCLow;
  Float_t Comb2MissingMCHigh;
  Float_t Comb2OmegaMCLow;
  Float_t Comb2OmegaMCHigh;
  Float_t Comb2EtaMCLow;
  Float_t Comb2EtaMChigh;

  //Combination 3
  Float_t Comb3MissingMCLow;
  Float_t Comb3MissingMCHigh;
  Float_t Comb3OmegaMCLow;
  Float_t Comb3OmegaMCHigh;
  Float_t Comb3EtaMCLow;
  Float_t Comb3EtaMChigh;

  //Combination 4
  Float_t Comb4MissingMCLow;
  Float_t Comb4MissingMCHigh;
  Float_t Comb4OmegaMCLow;
  Float_t Comb4OmegaMCHigh;
  Float_t Comb4EtaMCLow;
  Float_t Comb4EtaMChigh;

  //Delta Vextex variables

  TVector3 fOmegaCand1DeltaVertex;
  TVector3 fOmegaCand2DeltaVertex;
  TVector3 fOmegaCand3DeltaVertex;
  TVector3 fOmegaCand4DeltaVertex;
  
  //Fitting Parameter Variables

  Double_t OmegaComb1Par[8];
  Double_t OmegaComb2Par[8];
  Double_t OmegaComb3Par[8];
  Double_t OmegaComb4Par[8];

  // Declaration of leaf types
  UShort_t        run_num;
  UInt_t          evntid;
  Char_t          evstat;
  Char_t          evntrip;
  UChar_t         l1bit;
  Float_t         tr_time;
  Float_t         rf_time;
  UChar_t         helicity;
  UChar_t         npart;
  Int_t           inl1b;
  Int_t           L1bit_v[16];   //[inl1b]
  Int_t           gpart;
  Short_t         id[20];   //[gpart]
  Char_t          stat[20];   //[gpart]
  UChar_t         dc[20];   //[gpart]
  UChar_t         cc[20];   //[gpart]
  UChar_t         sc[20];   //[gpart]
  UChar_t         ec[20];   //[gpart]
  UChar_t         lec[20];   //[gpart]
  UChar_t         st[20];   //[gpart]
  Float_t         p[20];   //[gpart]
  Float_t         m[20];   //[gpart]
  Char_t          q[20];   //[gpart]
  Float_t         b[20];   //[gpart]
  Float_t         cx[20];   //[gpart]
  Float_t         cy[20];   //[gpart]
  Float_t         cz[20];   //[gpart]
  Float_t         vx[20];   //[gpart]
  Float_t         vy[20];   //[gpart]
  Float_t         vz[20];   //[gpart]
  Int_t           dc_part;
  UChar_t         dc_sect[20];   //[dc_part]
  UChar_t         dc_trk[20];   //[dc_part]
  Char_t          dc_stat[20];   //[dc_part]
  Int_t           tb_st[20];   //[dc_part]
  Float_t         dc_xsc[20];   //[dc_part]
  Float_t         dc_ysc[20];   //[dc_part]
  Float_t         dc_zsc[20];   //[dc_part]
  Float_t         dc_cxsc[20];   //[dc_part]
  Float_t         dc_cysc[20];   //[dc_part]
  Float_t         dc_czsc[20];   //[dc_part]
  Float_t         dc_vx[20];   //[dc_part]
  Float_t         dc_vy[20];   //[dc_part]
  Float_t         dc_vz[20];   //[dc_part]
  Float_t         dc_vr[20];   //[dc_part]
  Float_t         dc_c2[20];   //[dc_part]
  Int_t           ec_part;
  Int_t           ec_stat[20];   //[ec_part]
  UChar_t         ec_sect[20];   //[ec_part]
  Int_t           ec_whol[20];   //[ec_part]
  Int_t           ec_inst[20];   //[ec_part]
  Int_t           ec_oust[20];   //[ec_part]
  Float_t         etot[20];   //[ec_part]
  Float_t         ec_ei[20];   //[ec_part]
  Float_t         ec_eo[20];   //[ec_part]
  Float_t         ec_t[20];   //[ec_part]
  Float_t         ec_r[20];   //[ec_part]
  Float_t         ech_x[20];   //[ec_part]
  Float_t         ech_y[20];   //[ec_part]
  Float_t         ech_z[20];   //[ec_part]
  Float_t         ec_m2[20];   //[ec_part]
  Float_t         ec_m3[20];   //[ec_part]
  Float_t         ec_m4[20];   //[ec_part]
  Float_t         ec_c2[20];   //[ec_part]
  Int_t           sc_part;
  UChar_t         sc_sect[20];   //[sc_part]
  UChar_t         sc_hit[20];   //[sc_part]
  UChar_t         sc_pd[20];   //[sc_part]
  Int_t           sc_stat[20];   //[sc_part]
  Float_t         edep[20];   //[sc_part]
  Float_t         sc_t[20];   //[sc_part]
  Float_t         sc_r[20];   //[sc_part]
  Float_t         sc_c2[20];   //[sc_part]
  Int_t           st_part;
  Int_t           st_sector[20];   //[st_part]
  Int_t           st_ihit[20];   //[st_part]
  Int_t           st_trkno[20];   //[st_part]
  Float_t         st_time[20];   //[st_part]
  Float_t         st_rtrk[20];   //[st_part]
  Int_t           st_status[20];   //[st_part]
  Int_t           taghit;
  Float_t         E_gamma[30];   //[taghit]
  Float_t         T_gamma_norf[30];   //[taghit]
  Float_t         T_gamma[30];   //[taghit]
  Int_t           tagstat[30];   //[taghit]
  UChar_t         Tid[30];   //[taghit]
  UShort_t        Eid[30];   //[taghit]
  Int_t           taggoodhit;
  Int_t           tag_ptr[20];   //[taggoodhit]
  Float_t         vertex_time[20];   //[taggoodhit]
  Float_t         tag_energy[20];   //[taggoodhit]
  Float_t         dt_st_tag[20];   //[taggoodhit]
  Int_t           tag_stat[20];   //[taggoodhit]
  Int_t           nstr;
  UShort_t        idstr[24];   //[nstr]
  UShort_t        Str_trk[24];   //[nstr]
  Float_t         Str_t[24];   //[nstr]
  Float_t         Str_l[24];   //[nstr]
  Float_t         Str_z[24];   //[nstr]
  UChar_t         Strstat[24];   //[nstr]
  Int_t           ntber;
  Float_t         qptber[100];   //[ntber]
  Float_t         lbtber[100];   //[ntber]
  Float_t         phitber[100];   //[ntber]
  Float_t         d0tber[100];   //[ntber]
  Float_t         z0tber[100];   //[ntber]
  Float_t         c11tber[100];   //[ntber]
  Float_t         c12tber[100];   //[ntber]
  Float_t         c13tber[100];   //[ntber]
  Float_t         c14tber[100];   //[ntber]
  Float_t         c15tber[100];   //[ntber]
  Float_t         c22tber[100];   //[ntber]
  Float_t         c23tber[100];   //[ntber]
  Float_t         c24tber[100];   //[ntber]
  Float_t         c25tber[100];   //[ntber]
  Float_t         c33tber[100];   //[ntber]
  Float_t         c34tber[100];   //[ntber]
  Float_t         c35tber[100];   //[ntber]
  Float_t         c44tber[100];   //[ntber]
  Float_t         c45tber[100];   //[ntber]
  Float_t         c55tber[100];   //[ntber]
  Float_t         chi2tber[100];   //[ntber]
  Int_t           lay1tber[100];   //[ntber]
  Int_t           lay2tber[100];   //[ntber]
  Int_t           vidmvrt;
  Int_t           ntrmvrt;
  Float_t         xmvrt;
  Float_t         ymvrt;
  Float_t         zmvrt;
  Float_t         ch2mvrt;
  Float_t         cxxmvrt;
  Float_t         cxymvrt;
  Float_t         cxzmvrt;
  Float_t         cyymvrt;
  Float_t         cyzmvrt;
  Int_t           stamvrt;
  Int_t           nprt;
  Int_t           pidpart[20];   //[nprt]
  Float_t         xpart[20];   //[nprt]
  Float_t         ypart[20];   //[nprt]
  Float_t         zpart[20];   //[nprt]
  Float_t         epart[20];   //[nprt]
  Float_t         pxpart[20];   //[nprt]
  Float_t         pypart[20];   //[nprt]
  Float_t         pzpart[20];   //[nprt]
  Float_t         qpart[20];   //[nprt]
  Char_t          trkpart[20];   //[nprt]
  Float_t         qpipart[20];   //[nprt]
  Float_t         qtrpart[20];   //[nprt]
  Int_t           flgpart[20];   //[nprt]
  Int_t           ntbid;
  UChar_t         trktbid[100];   //[ntbid]
  UChar_t         sectbid[100];   //[ntbid]
  Float_t         btbid[100];   //[ntbid]
  Float_t         vttbid[100];   //[ntbid]
  UChar_t         scstbid[100];   //[ntbid]
  UChar_t         scitbid[100];   //[ntbid]
  Float_t         scttbid[100];   //[ntbid]
  Float_t         scqtbid[100];   //[ntbid]
  Float_t         scvtbid[100];   //[ntbid]
  Float_t         scbtbid[100];   //[ntbid]
  UChar_t         ecstbid[100];   //[ntbid]
  UChar_t         ecitbid[100];   //[ntbid]
  Float_t         ecttbid[100];   //[ntbid]
  Float_t         ecqtbid[100];   //[ntbid]
  Float_t         ecvtbid[100];   //[ntbid]
  Float_t         ecbtbid[100];   //[ntbid]
  UChar_t         ststbid[100];   //[ntbid]
  UChar_t         stitbid[100];   //[ntbid]
  Float_t         stttbid[100];   //[ntbid]
  Float_t         stqtbid[100];   //[ntbid]
  Float_t         stvtbid[100];   //[ntbid]
  Float_t         stbtbid[100];   //[ntbid]
  UChar_t         lcstbid[100];   //[ntbid]
  UChar_t         lcitbid[100];   //[ntbid]
  Float_t         lcttbid[100];   //[ntbid]
  Float_t         lcqtbid[100];   //[ntbid]
  Float_t         lcvtbid[100];   //[ntbid]
  Float_t         lcbtbid[100];   //[ntbid]
  Int_t           nschit;
  UChar_t         scsect[20];   //[nschit]
  UChar_t         schid[20];   //[nschit]
  UChar_t         scpid[20];   //[nschit]
  Float_t         sce[20];   //[nschit]
  Float_t         sct[20];   //[nschit]
  Float_t         scx[20];   //[nschit]
  Float_t         scy[20];   //[nschit]
  Float_t         scz[20];   //[nschit]
  Float_t         scdy[20];   //[nschit]
  Int_t           scstat[20];   //[nschit]
  Float_t         scde[20];   //[nschit]
  Float_t         scdt[20];   //[nschit]

  // List of branches
  TBranch        *b_run_num;   //!
  TBranch        *b_evntid;   //!
  TBranch        *b_evstat;   //!
  TBranch        *b_evntrip;   //!
  TBranch        *b_l1bit;   //!
  TBranch        *b_tr_time;   //!
  TBranch        *b_rf_time;   //!
  TBranch        *b_helicity;   //!
  TBranch        *b_npart;   //!
  TBranch        *b_inl1b;   //!
  TBranch        *b_L1bit_v;   //!
  TBranch        *b_gpart;   //!
  TBranch        *b_id;   //!
  TBranch        *b_stat;   //!
  TBranch        *b_dc;   //!
  TBranch        *b_cc;   //!
  TBranch        *b_sc;   //!
  TBranch        *b_ec;   //!
  TBranch        *b_lec;   //!
  TBranch        *b_st;   //!
  TBranch        *b_p;   //!
  TBranch        *b_m;   //!
  TBranch        *b_q;   //!
  TBranch        *b_b;   //!
  TBranch        *b_cx;   //!
  TBranch        *b_cy;   //!
  TBranch        *b_cz;   //!
  TBranch        *b_vx;   //!
  TBranch        *b_vy;   //!
  TBranch        *b_vz;   //!
  TBranch        *b_dc_part;   //!
  TBranch        *b_dc_sect;   //!
  TBranch        *b_dc_trk;   //!
  TBranch        *b_dc_stat;   //!
  TBranch        *b_tb_st;   //!
  TBranch        *b_dc_xsc;   //!
  TBranch        *b_dc_ysc;   //!
  TBranch        *b_dc_zsc;   //!
  TBranch        *b_dc_cxsc;   //!
  TBranch        *b_dc_cysc;   //!
  TBranch        *b_dc_czsc;   //!
  TBranch        *b_dc_vx;   //!
  TBranch        *b_dc_vy;   //!
  TBranch        *b_dc_vz;   //!
  TBranch        *b_dc_vr;   //!
  TBranch        *b_dc_c2;   //!
  TBranch        *b_ec_part;   //!
  TBranch        *b_ec_stat;   //!
  TBranch        *b_ec_sect;   //!
  TBranch        *b_ec_whol;   //!
  TBranch        *b_ec_inst;   //!
  TBranch        *b_ec_oust;   //!
  TBranch        *b_etot;   //!
  TBranch        *b_ec_ei;   //!
  TBranch        *b_ec_eo;   //!
  TBranch        *b_ec_t;   //!
  TBranch        *b_ec_r;   //!
  TBranch        *b_ech_x;   //!
  TBranch        *b_ech_y;   //!
  TBranch        *b_ech_z;   //!
  TBranch        *b_ec_m2;   //!
  TBranch        *b_ec_m3;   //!
  TBranch        *b_ec_m4;   //!
  TBranch        *b_ec_c2;   //!
  TBranch        *b_sc_part;   //!
  TBranch        *b_sc_sect;   //!
  TBranch        *b_sc_hit;   //!
  TBranch        *b_sc_pd;   //!
  TBranch        *b_sc_stat;   //!
  TBranch        *b_edep;   //!
  TBranch        *b_sc_t;   //!
  TBranch        *b_sc_r;   //!
  TBranch        *b_sc_c2;   //!
  TBranch        *b_st_part;   //!
  TBranch        *b_st_sector;   //!
  TBranch        *b_st_ihit;   //!
  TBranch        *b_st_trkno;   //!
  TBranch        *b_st_time;   //!
  TBranch        *b_st_rtrk;   //!
  TBranch        *b_st_status;   //!
  TBranch        *b_taghit;   //!
  TBranch        *b_E_gamma;   //!
  TBranch        *b_T_gamma_norf;   //!
  TBranch        *b_T_gamma;   //!
  TBranch        *b_tagstat;   //!
  TBranch        *b_Tid;   //!
  TBranch        *b_Eid;   //!
  TBranch        *b_taggoodhit;   //!
  TBranch        *b_tag_ptr;   //!
  TBranch        *b_vertex_time;   //!
  TBranch        *b_tag_energy;   //!
  TBranch        *b_dt_st_tag;   //!
  TBranch        *b_tag_stat;   //!
  TBranch        *b_nstr;   //!
  TBranch        *b_idstr;   //!
  TBranch        *b_Str_trk;   //!
  TBranch        *b_Str_t;   //!
  TBranch        *b_Str_l;   //!
  TBranch        *b_Str_z;   //!
  TBranch        *b_Strstat;   //!
  TBranch        *b_ntber;   //!
  TBranch        *b_qptber;   //!
  TBranch        *b_lbtber;   //!
  TBranch        *b_phitber;   //!
  TBranch        *b_d0tber;   //!
  TBranch        *b_z0tber;   //!
  TBranch        *b_c11tber;   //!
  TBranch        *b_c12tber;   //!
  TBranch        *b_c13tber;   //!
  TBranch        *b_c14tber;   //!
  TBranch        *b_c15tber;   //!
  TBranch        *b_c22tber;   //!
  TBranch        *b_c23tber;   //!
  TBranch        *b_c24tber;   //!
  TBranch        *b_c25tber;   //!
  TBranch        *b_c33tber;   //!
  TBranch        *b_c34tber;   //!
  TBranch        *b_c35tber;   //!
  TBranch        *b_c44tber;   //!
  TBranch        *b_c45tber;   //!
  TBranch        *b_c55tber;   //!
  TBranch        *b_chi2tber;   //!
  TBranch        *b_lay1tber;   //!
  TBranch        *b_lay2tber;   //!
  TBranch        *b_vidmvrt;   //!
  TBranch        *b_ntrmvrt;   //!
  TBranch        *b_xmvrt;   //!
  TBranch        *b_ymvrt;   //!
  TBranch        *b_zmvrt;   //!
  TBranch        *b_ch2mvrt;   //!
  TBranch        *b_cxxmvrt;   //!
  TBranch        *b_cxymvrt;   //!
  TBranch        *b_cxzmvrt;   //!
  TBranch        *b_cyymvrt;   //!
  TBranch        *b_cyzmvrt;   //!
  TBranch        *b_stamvrt;   //!
  TBranch        *b_nprt;   //!
  TBranch        *b_pidpart;   //!
  TBranch        *b_xpart;   //!
  TBranch        *b_ypart;   //!
  TBranch        *b_zpart;   //!
  TBranch        *b_epart;   //!
  TBranch        *b_pxpart;   //!
  TBranch        *b_pypart;   //!
  TBranch        *b_pzpart;   //!
  TBranch        *b_qpart;   //!
  TBranch        *b_trkpart;   //!
  TBranch        *b_qpipart;   //!
  TBranch        *b_qtrpart;   //!
  TBranch        *b_flgpart;   //!
  TBranch        *b_ntbid;   //!
  TBranch        *b_trktbid;   //!
  TBranch        *b_sectbid;   //!
  TBranch        *b_btbid;   //!
  TBranch        *b_vttbid;   //!
  TBranch        *b_scstbid;   //!
  TBranch        *b_scitbid;   //!
  TBranch        *b_scttbid;   //!
  TBranch        *b_scqtbid;   //!
  TBranch        *b_scvtbid;   //!
  TBranch        *b_scbtbid;   //!
  TBranch        *b_ecstbid;   //!
  TBranch        *b_ecitbid;   //!
  TBranch        *b_ecttbid;   //!
  TBranch        *b_ecqtbid;   //!
  TBranch        *b_ecvtbid;   //!
  TBranch        *b_ecbtbid;   //!
  TBranch        *b_ststbid;   //!
  TBranch        *b_stitbid;   //!
  TBranch        *b_stttbid;   //!
  TBranch        *b_stqtbid;   //!
  TBranch        *b_stvtbid;   //!
  TBranch        *b_stbtbid;   //!
  TBranch        *b_lcstbid;   //!
  TBranch        *b_lcitbid;   //!
  TBranch        *b_lcttbid;   //!
  TBranch        *b_lcqtbid;   //!
  TBranch        *b_lcvtbid;   //!
  TBranch        *b_lcbtbid;   //!
  TBranch        *b_nschit;   //!
  TBranch        *b_scsect;   //!
  TBranch        *b_schid;   //!
  TBranch        *b_scpid;   //!
  TBranch        *b_sce;   //!
  TBranch        *b_sct;   //!
  TBranch        *b_scx;   //!
  TBranch        *b_scy;   //!
  TBranch        *b_scz;   //!
  TBranch        *b_scdy;   //!
  TBranch        *b_scstat;   //!
  TBranch        *b_scde;   //!
  TBranch        *b_scdt;   //!

  //fOutName - if directory will fill with filtered files of the same name as input
  //fOutName - if full filename will just fill the single file with output of all input

//Place to null pointers in the contructor
 CLAStoHS(TTree * /*tree*/ =0) : THSOutput(), fHSgamma(0), fMissing(0), fChain(0), fOmegaCand1(0), fOmegaCand2(0), fOmegaCand3(0), fOmegaCand4(0), fPionComb1(0), fPionComb2(0), fPionComb3(0), fPionComb4(0), fBeamEnergySelection(0), fProton(0), total(0), g2(0), fFastPiMPi0(0), fSlowPiMPi0(0),fFastPiPPi0(0), fSlowPiPPi0(0), fPiMFast0(0), fPiMSlow1(0), fPiPFast2(0), fPiPSlow3(0), fOmegaCand1PiMSlow1(0), fOmegaCand1PiPSlow3(0), fOmegaCand2PiMSlow1(0), fOmegaCand2PiPFast2(0), fOmegaCand3PiMFast0(0), fOmegaCand3PiPSlow3(0), fOmegaCand4PiMFast0(0), fOmegaCand4PiPFast2(0) { }
  //CLAStoHS(TTree * /*tree*/ =0) : THSOutput(), fHSgamma(0), fMissing(0), fChain(0) { fOutName="/home/dglazier/Work/Research/HaSpect/data/pippippimMn_HS2/OneFile.root";}

  virtual ~CLAStoHS();
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

  //Add THSHistoFunctions
  virtual void HistogramList(TString sLabel);
  virtual void FillHistograms(TString sCut,Int_t bin);

  ClassDef(CLAStoHS,0);
};

#endif

#ifdef CLAStoHS_cxx
void CLAStoHS::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("run_num", &run_num, &b_run_num);
  fChain->SetBranchAddress("evntid", &evntid, &b_evntid);
  fChain->SetBranchAddress("evstat", &evstat, &b_evstat);
  fChain->SetBranchAddress("evntrip", &evntrip, &b_evntrip);
  fChain->SetBranchAddress("l1bit", &l1bit, &b_l1bit);
  fChain->SetBranchAddress("tr_time", &tr_time, &b_tr_time);
  fChain->SetBranchAddress("rf_time", &rf_time, &b_rf_time);
  fChain->SetBranchAddress("helicity", &helicity, &b_helicity);
  fChain->SetBranchAddress("npart", &npart, &b_npart);
  fChain->SetBranchAddress("inl1b", &inl1b, &b_inl1b);
  fChain->SetBranchAddress("L1bit_v", L1bit_v, &b_L1bit_v);
  fChain->SetBranchAddress("gpart", &gpart, &b_gpart);
  fChain->SetBranchAddress("id", id, &b_id);
  fChain->SetBranchAddress("stat", stat, &b_stat);
  fChain->SetBranchAddress("dc", dc, &b_dc);
  fChain->SetBranchAddress("cc", cc, &b_cc);
  fChain->SetBranchAddress("sc", sc, &b_sc);
  fChain->SetBranchAddress("ec", ec, &b_ec);
  fChain->SetBranchAddress("lec", lec, &b_lec);
  fChain->SetBranchAddress("st", st, &b_st);
  fChain->SetBranchAddress("p", p, &b_p);
  fChain->SetBranchAddress("m", m, &b_m);
  fChain->SetBranchAddress("q", q, &b_q);
  fChain->SetBranchAddress("b", b, &b_b);
  fChain->SetBranchAddress("cx", cx, &b_cx);
  fChain->SetBranchAddress("cy", cy, &b_cy);
  fChain->SetBranchAddress("cz", cz, &b_cz);
  fChain->SetBranchAddress("vx", vx, &b_vx);
  fChain->SetBranchAddress("vy", vy, &b_vy);
  fChain->SetBranchAddress("vz", vz, &b_vz);
  fChain->SetBranchAddress("dc_part", &dc_part, &b_dc_part);
  fChain->SetBranchAddress("dc_sect", dc_sect, &b_dc_sect);
  fChain->SetBranchAddress("dc_trk", dc_trk, &b_dc_trk);
  fChain->SetBranchAddress("dc_stat", dc_stat, &b_dc_stat);
  fChain->SetBranchAddress("tb_st", tb_st, &b_tb_st);
  fChain->SetBranchAddress("dc_xsc", dc_xsc, &b_dc_xsc);
  fChain->SetBranchAddress("dc_ysc", dc_ysc, &b_dc_ysc);
  fChain->SetBranchAddress("dc_zsc", dc_zsc, &b_dc_zsc);
  fChain->SetBranchAddress("dc_cxsc", dc_cxsc, &b_dc_cxsc);
  fChain->SetBranchAddress("dc_cysc", dc_cysc, &b_dc_cysc);
  fChain->SetBranchAddress("dc_czsc", dc_czsc, &b_dc_czsc);
  fChain->SetBranchAddress("dc_vx", dc_vx, &b_dc_vx);
  fChain->SetBranchAddress("dc_vy", dc_vy, &b_dc_vy);
  fChain->SetBranchAddress("dc_vz", dc_vz, &b_dc_vz);
  fChain->SetBranchAddress("dc_vr", dc_vr, &b_dc_vr);
  fChain->SetBranchAddress("dc_c2", dc_c2, &b_dc_c2);
  fChain->SetBranchAddress("ec_part", &ec_part, &b_ec_part);
  fChain->SetBranchAddress("ec_stat", ec_stat, &b_ec_stat);
  fChain->SetBranchAddress("ec_sect", ec_sect, &b_ec_sect);
  fChain->SetBranchAddress("ec_whol", ec_whol, &b_ec_whol);
  fChain->SetBranchAddress("ec_inst", ec_inst, &b_ec_inst);
  fChain->SetBranchAddress("ec_oust", ec_oust, &b_ec_oust);
  fChain->SetBranchAddress("etot", etot, &b_etot);
  fChain->SetBranchAddress("ec_ei", ec_ei, &b_ec_ei);
  fChain->SetBranchAddress("ec_eo", ec_eo, &b_ec_eo);
  fChain->SetBranchAddress("ec_t", ec_t, &b_ec_t);
  fChain->SetBranchAddress("ec_r", ec_r, &b_ec_r);
  fChain->SetBranchAddress("ech_x", ech_x, &b_ech_x);
  fChain->SetBranchAddress("ech_y", ech_y, &b_ech_y);
  fChain->SetBranchAddress("ech_z", ech_z, &b_ech_z);
  fChain->SetBranchAddress("ec_m2", ec_m2, &b_ec_m2);
  fChain->SetBranchAddress("ec_m3", ec_m3, &b_ec_m3);
  fChain->SetBranchAddress("ec_m4", ec_m4, &b_ec_m4);
  fChain->SetBranchAddress("ec_c2", ec_c2, &b_ec_c2);
  fChain->SetBranchAddress("sc_part", &sc_part, &b_sc_part);
  fChain->SetBranchAddress("sc_sect", sc_sect, &b_sc_sect);
  fChain->SetBranchAddress("sc_hit", sc_hit, &b_sc_hit);
  fChain->SetBranchAddress("sc_pd", sc_pd, &b_sc_pd);
  fChain->SetBranchAddress("sc_stat", sc_stat, &b_sc_stat);
  fChain->SetBranchAddress("edep", edep, &b_edep);
  fChain->SetBranchAddress("sc_t", sc_t, &b_sc_t);
  fChain->SetBranchAddress("sc_r", sc_r, &b_sc_r);
  fChain->SetBranchAddress("sc_c2", sc_c2, &b_sc_c2);
  fChain->SetBranchAddress("st_part", &st_part, &b_st_part);
  fChain->SetBranchAddress("st_sector", st_sector, &b_st_sector);
  fChain->SetBranchAddress("st_ihit", st_ihit, &b_st_ihit);
  fChain->SetBranchAddress("st_trkno", st_trkno, &b_st_trkno);
  fChain->SetBranchAddress("st_time", st_time, &b_st_time);
  fChain->SetBranchAddress("st_rtrk", st_rtrk, &b_st_rtrk);
  fChain->SetBranchAddress("st_status", st_status, &b_st_status);
  fChain->SetBranchAddress("taghit", &taghit, &b_taghit);
  fChain->SetBranchAddress("E_gamma", E_gamma, &b_E_gamma);
  fChain->SetBranchAddress("T_gamma_norf", T_gamma_norf, &b_T_gamma_norf);
  fChain->SetBranchAddress("T_gamma", T_gamma, &b_T_gamma);
  fChain->SetBranchAddress("tagstat", tagstat, &b_tagstat);
  fChain->SetBranchAddress("Tid", Tid, &b_Tid);
  fChain->SetBranchAddress("Eid", Eid, &b_Eid);
  fChain->SetBranchAddress("taggoodhit", &taggoodhit, &b_taggoodhit);
  fChain->SetBranchAddress("tag_ptr", tag_ptr, &b_tag_ptr);
  fChain->SetBranchAddress("vertex_time", vertex_time, &b_vertex_time);
  fChain->SetBranchAddress("tag_energy", tag_energy, &b_tag_energy);
  fChain->SetBranchAddress("dt_st_tag", dt_st_tag, &b_dt_st_tag);
  fChain->SetBranchAddress("tag_stat", tag_stat, &b_tag_stat);
  fChain->SetBranchAddress("nstr", &nstr, &b_nstr);
  fChain->SetBranchAddress("idstr", idstr, &b_idstr);
  fChain->SetBranchAddress("Str_trk", Str_trk, &b_Str_trk);
  fChain->SetBranchAddress("Str_t", Str_t, &b_Str_t);
  fChain->SetBranchAddress("Str_l", Str_l, &b_Str_l);
  fChain->SetBranchAddress("Str_z", Str_z, &b_Str_z);
  fChain->SetBranchAddress("Strstat", Strstat, &b_Strstat);
  fChain->SetBranchAddress("ntber", &ntber, &b_ntber);
  fChain->SetBranchAddress("qptber", qptber, &b_qptber);
  fChain->SetBranchAddress("lbtber", lbtber, &b_lbtber);
  fChain->SetBranchAddress("phitber", phitber, &b_phitber);
  fChain->SetBranchAddress("d0tber", d0tber, &b_d0tber);
  fChain->SetBranchAddress("z0tber", z0tber, &b_z0tber);
  fChain->SetBranchAddress("c11tber", c11tber, &b_c11tber);
  fChain->SetBranchAddress("c12tber", c12tber, &b_c12tber);
  fChain->SetBranchAddress("c13tber", c13tber, &b_c13tber);
  fChain->SetBranchAddress("c14tber", c14tber, &b_c14tber);
  fChain->SetBranchAddress("c15tber", c15tber, &b_c15tber);
  fChain->SetBranchAddress("c22tber", c22tber, &b_c22tber);
  fChain->SetBranchAddress("c23tber", c23tber, &b_c23tber);
  fChain->SetBranchAddress("c24tber", c24tber, &b_c24tber);
  fChain->SetBranchAddress("c25tber", c25tber, &b_c25tber);
  fChain->SetBranchAddress("c33tber", c33tber, &b_c33tber);
  fChain->SetBranchAddress("c34tber", c34tber, &b_c34tber);
  fChain->SetBranchAddress("c35tber", c35tber, &b_c35tber);
  fChain->SetBranchAddress("c44tber", c44tber, &b_c44tber);
  fChain->SetBranchAddress("c45tber", c45tber, &b_c45tber);
  fChain->SetBranchAddress("c55tber", c55tber, &b_c55tber);
  fChain->SetBranchAddress("chi2tber", chi2tber, &b_chi2tber);
  fChain->SetBranchAddress("lay1tber", lay1tber, &b_lay1tber);
  fChain->SetBranchAddress("lay2tber", lay2tber, &b_lay2tber);
  fChain->SetBranchAddress("vidmvrt", &vidmvrt, &b_vidmvrt);
  fChain->SetBranchAddress("ntrmvrt", &ntrmvrt, &b_ntrmvrt);
  fChain->SetBranchAddress("xmvrt", &xmvrt, &b_xmvrt);
  fChain->SetBranchAddress("ymvrt", &ymvrt, &b_ymvrt);
  fChain->SetBranchAddress("zmvrt", &zmvrt, &b_zmvrt);
  fChain->SetBranchAddress("ch2mvrt", &ch2mvrt, &b_ch2mvrt);
  fChain->SetBranchAddress("cxxmvrt", &cxxmvrt, &b_cxxmvrt);
  fChain->SetBranchAddress("cxymvrt", &cxymvrt, &b_cxymvrt);
  fChain->SetBranchAddress("cxzmvrt", &cxzmvrt, &b_cxzmvrt);
  fChain->SetBranchAddress("cyymvrt", &cyymvrt, &b_cyymvrt);
  fChain->SetBranchAddress("cyzmvrt", &cyzmvrt, &b_cyzmvrt);
  fChain->SetBranchAddress("stamvrt", &stamvrt, &b_stamvrt);
  fChain->SetBranchAddress("nprt", &nprt, &b_nprt);
  fChain->SetBranchAddress("pidpart", pidpart, &b_pidpart);
  fChain->SetBranchAddress("xpart", xpart, &b_xpart);
  fChain->SetBranchAddress("ypart", ypart, &b_ypart);
  fChain->SetBranchAddress("zpart", zpart, &b_zpart);
  fChain->SetBranchAddress("epart", epart, &b_epart);
  fChain->SetBranchAddress("pxpart", pxpart, &b_pxpart);
  fChain->SetBranchAddress("pypart", pypart, &b_pypart);
  fChain->SetBranchAddress("pzpart", pzpart, &b_pzpart);
  fChain->SetBranchAddress("qpart", qpart, &b_qpart);
  fChain->SetBranchAddress("trkpart", trkpart, &b_trkpart);
  fChain->SetBranchAddress("qpipart", qpipart, &b_qpipart);
  fChain->SetBranchAddress("qtrpart", qtrpart, &b_qtrpart);
  fChain->SetBranchAddress("flgpart", flgpart, &b_flgpart);
  fChain->SetBranchAddress("ntbid", &ntbid, &b_ntbid);
  fChain->SetBranchAddress("trktbid", trktbid, &b_trktbid);
  fChain->SetBranchAddress("sectbid", sectbid, &b_sectbid);
  fChain->SetBranchAddress("btbid", btbid, &b_btbid);
  fChain->SetBranchAddress("vttbid", vttbid, &b_vttbid);
  fChain->SetBranchAddress("scstbid", scstbid, &b_scstbid);
  fChain->SetBranchAddress("scitbid", scitbid, &b_scitbid);
  fChain->SetBranchAddress("scttbid", scttbid, &b_scttbid);
  fChain->SetBranchAddress("scqtbid", scqtbid, &b_scqtbid);
  fChain->SetBranchAddress("scvtbid", scvtbid, &b_scvtbid);
  fChain->SetBranchAddress("scbtbid", scbtbid, &b_scbtbid);
  fChain->SetBranchAddress("ecstbid", ecstbid, &b_ecstbid);
  fChain->SetBranchAddress("ecitbid", ecitbid, &b_ecitbid);
  fChain->SetBranchAddress("ecttbid", ecttbid, &b_ecttbid);
  fChain->SetBranchAddress("ecqtbid", ecqtbid, &b_ecqtbid);
  fChain->SetBranchAddress("ecvtbid", ecvtbid, &b_ecvtbid);
  fChain->SetBranchAddress("ecbtbid", ecbtbid, &b_ecbtbid);
  fChain->SetBranchAddress("ststbid", ststbid, &b_ststbid);
  fChain->SetBranchAddress("stitbid", stitbid, &b_stitbid);
  fChain->SetBranchAddress("stttbid", stttbid, &b_stttbid);
  fChain->SetBranchAddress("stqtbid", stqtbid, &b_stqtbid);
  fChain->SetBranchAddress("stvtbid", stvtbid, &b_stvtbid);
  fChain->SetBranchAddress("stbtbid", stbtbid, &b_stbtbid);
  fChain->SetBranchAddress("lcstbid", lcstbid, &b_lcstbid);
  fChain->SetBranchAddress("lcitbid", lcitbid, &b_lcitbid);
  fChain->SetBranchAddress("lcttbid", lcttbid, &b_lcttbid);
  fChain->SetBranchAddress("lcqtbid", lcqtbid, &b_lcqtbid);
  fChain->SetBranchAddress("lcvtbid", lcvtbid, &b_lcvtbid);
  fChain->SetBranchAddress("lcbtbid", lcbtbid, &b_lcbtbid);
  fChain->SetBranchAddress("nschit", &nschit, &b_nschit);
  fChain->SetBranchAddress("scsect", scsect, &b_scsect);
  fChain->SetBranchAddress("schid", schid, &b_schid);
  fChain->SetBranchAddress("scpid", scpid, &b_scpid);
  fChain->SetBranchAddress("sce", sce, &b_sce);
  fChain->SetBranchAddress("sct", sct, &b_sct);
  fChain->SetBranchAddress("scx", scx, &b_scx);
  fChain->SetBranchAddress("scy", scy, &b_scy);
  fChain->SetBranchAddress("scz", scz, &b_scz);
  fChain->SetBranchAddress("scdy", scdy, &b_scdy);
  fChain->SetBranchAddress("scstat", scstat, &b_scstat);
  fChain->SetBranchAddress("scde", scde, &b_scde);
  fChain->SetBranchAddress("scdt", scdt, &b_scdt);
}

Bool_t CLAStoHS::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.
  //cleanup previouos file
  //Use THSOutput functions to look after files
  THSOutput::HSNotify(fChain);
  THSOutput::InitOutTree();

  return kTRUE;
}

#endif // #ifdef CLAStoHS_cxx
