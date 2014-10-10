//--Author      A Celantano,DI Glazier 2014
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

#define THSEventWeighter_cxx

#include "THSEventWeighter.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TText.h>
#include <TLine.h>
#include <TSelector.h>
#include <TEntryList.h>
#include <TKey.h>
#include <TROOT.h>
#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooPlot.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooChebychev.h"
#include "RooFitResult.h"

THSEventWeighter::THSEventWeighter(){
  //default constructor
  fNcoord=0;
  fNmax=0;
  fNdisc=0;
  fSufficient=0;
  fIsPlot=kFALSE;
  fIsSaveNN=kFALSE;
  fIsLoadNN=kFALSE;
  fNNVdisVarP=0;
  fNNChain=0;
  fOffNNChain=0;
  fNNChainLoad=0;
  fSaveChain=0;
  fNNEvTree=0;
  fTofT=0;
  fRooFit=0;
  fQval=fSigWidth=fSB=fSigMean=0;
  fEventEntryList=0;
  fKinBins=0;
  fCurKinBinList=0;
}
void THSEventWeighter::InitNN(TList* input){
  if(!fNcoord) Error("THSEventWeighter::InitNN","fNcoord is not set, so so in SlaveBegin()");
  if(!fNmax) fNmax=200; //default number of nearest neighbours=200
  // need to set in Slavebegin fNcoord=2;
 // need to set in Slavebegin  fNmax=200;
 // need to set in Slavebegin  fSufficient=0.0001;
  //Initialise the vectors etc. used in the nearest neigbours algorithm
  
  //make the matrices and vectors the correct size
  Dmetric.ResizeTo(fNcoord,fNcoord);
  Dmetric.Zero();
  Dmetric_diagonal.ResizeTo(fNcoord);
  Dmetric_diagonal.Zero();
  fCoord0.ResizeTo(fNcoord);
  fCoordi.ResizeTo(fNcoord);
   //define the metric for distance calculation
  //users should define their own
  SetMetric();

  //now get the full chain from the input list,
  //as if using proof you only get part of a tree not a chain
  //the user must set add the chain to the input list before processing
  //Also add NNChainName to input to give name of tree
  TNamed* chname=dynamic_cast<TNamed*>(input->FindObject("NNChainName"));
  if(chname)fNNChain=dynamic_cast<TChain*>(input->FindObject(chname->GetTitle()));
  if(!fNNChain){cout<<"EventWeighter::InitNN() No valid chain in input list"<<endl; exit(1);}
  fNNChain->SetProof(kFALSE); //the scan through the chain should not be parellised
  cout<<" Number of Entries in Chain "<<fNNChain->GetEntries()<<endl;
  //the chain has the same structure as the current selector
  //so we can use the current Init function to set branches
  (dynamic_cast<TSelector*>(this))->Init(fNNChain);
  //wow this last piece of code actually worked, even though I wanted the derived class Init and we are not in a selector class
  fDiscVar.ResizeTo(fNdisc);
  
  //reserve the NN  vectors space
  fNNVdisVar.reserve(fNmax);
  //fVcoord.reserve(fNmax);
  if(fIsLoadNN) fVdisVar.reserve(fNmax);//Know size when loading saved event
  //cache the useful variables, much quicker than reading tree each event
  TEntryList* el=0;
  Long64_t Nch=0;
  TNamed* elname=dynamic_cast<TNamed*>(input->FindObject("NNEntryList"));
  if(elname) el=dynamic_cast<TEntryList*>(input->FindObject(elname->GetTitle()));
  if(el){cout<<"got elist " <<el->GetN()<<endl;fNNChain->SetEntryList(el); Nch=el->GetN(); }//if chain has entry list
  else Nch=fNNChain->GetEntries();
  cout<<"NCH "<<Nch<<endl;
  Int_t treenum=0;
  Long64_t treeEntry =0;
  Long64_t chainEntry=0;
  for(Int_t nnentry=0;nnentry<Nch;nnentry++){
    if(el){//if there is an entrylist get the event number from there
      treeEntry = el->GetEntryAndTree(nnentry,treenum);
      chainEntry = treeEntry+fNNChain->GetTreeOffset()[treenum]; 
      fNNChain->LoadTree(chainEntry);
    }//if not just use loop index over all events
    else treeEntry = fNNChain->LoadTree(nnentry);
    GetNNBranches(treeEntry);
    FillCoord(fCoordi); //The fixed coordinate for THE possible neighbour  event
    fVcoord.push_back(fCoordi);
  
    FillDiscVar(nnentry); //The discriminatory variable e.g. missing mass
    fVdisVar.push_back(fDiscVar);
  }

  //Trees
  TDirectory* savedir=gDirectory;
  //tree with discriminating variable to fill each nearest neighbour
  TDirectory::AddDirectory(kFALSE); //do not add to current directory
  CreateNNTree(); //user should define in their seelctor class
  if(fIsSaveNN){//going to save the NN tree for each event
    fTofT=new TTree("TreeOfNNTrees","Tree of nearest neigbour trees");
    fTofT->Branch("NNTree",&fNNVdisVar); 
  }
  else if (fIsLoadNN){//going to load apreviously saved NN tree 
    chname=dynamic_cast<TNamed*>(input->FindObject("NNChainLoad"));
    if(chname)fNNChainLoad=dynamic_cast<TChain*>(input->FindObject(chname->GetTitle()));
    if(!fNNChainLoad){cout<<"EventWeighter::InitNN() No valid load chain in input list"<<endl; exit(1);}
    if(fNNChainLoad->GetEntries()!=fNNChainLoad->GetEntries()){cout<<"EventWeighter::InitNN() Load chain mismatch with selector chain"<<endl; exit(1);}
    fNNChainLoad->SetBranchAddress("NNTree",&fNNVdisVarP);
    // fNNVdisVar=*pointvec;
    cout<<"Loaded "<<fNNChainLoad->GetName()<<endl;
  }
  TDirectory::AddDirectory(kTRUE); //back to normal

  //Load saved kinematic bins for restricting nearest neighbours searc
  TNamed* kbfname=dynamic_cast<TNamed*>(input->FindObject("NNKinBins"));
  TNamed* kbhname=dynamic_cast<TNamed*>(input->FindObject("NNKinBinsHis"));
  if(kbfname&&kbhname) LoadEventEntryLists(kbfname->GetTitle(),kbhname->GetTitle());
  else cout<<"Note no kinematic bins given, if you want to use them give file (NNKinBins) and histogram(NNKinBinsHis)  name to input list"<<endl;
  savedir->cd();
}
void THSEventWeighter::SetMetric(){
  //User must define their metric in their own SetMetric then call 
  //THSEventWeighter::SetMetric() for the following

  //Check if symmetric and diagonal
  for (int ii=0;ii<fNcoord;ii++){
    for (int jj=0;jj<ii;jj++){
      if (ii==jj) continue;
      double valU,valD;
      valU=Dmetric[ii][jj];
      valD=Dmetric[jj][ii];
      if (valD!=valU){
	cout<<"Error, the metrics matrics should be symmetric"<<endl;
	Dmetric.Print();
	exit(1); 
      }
      if (valD!=0) fIsDiagonal=kFALSE;
    }
  }
  
  cout<<"Is diagonal: "<<fIsDiagonal<<endl;
  if (fIsDiagonal)//maybe the access to a TVectorD is faster than to a TMatrixD?
    for (int ii=0;ii<fNcoord;ii++) Dmetric_diagonal[ii]=Dmetric[ii][ii]; 
 
}

Float_t  THSEventWeighter::Distance2(const TVectorD &vi,const TVectorD &vj){
  //use metric to find distance between two points
  //  Dist^2= xT * D * x, where x is a vector and D the metric
  //the metric accounts for the scale of different variable types
  TVectorD vdelta(fNcoord);
  Double_t distance2;
  vdelta=(vi-vj);
  if (fIsDiagonal){
    distance2=0;
    for (int qq=0;qq<fNcoord;qq++){distance2+=vdelta[qq]*vdelta[qq]*Dmetric_diagonal[qq];}
  }
  else{	
    vdelta*=Dmetric;
    distance2=vdelta*(vi-vj);
  }	 	
  return distance2;
}
void  THSEventWeighter::MakeNNMap(){
  if (fIsLoadNN) return;
  //Make a map of the fNmax nearest neigbours to this event
  FillCoord(fCoord0); //The fixed coordinate for THE event whose neighbours are being found
  map<Float_t,Int_t>::iterator it; //iterator for fNNMap
  fNNmap.clear(); //Clear previous event map
  Float_t curr_dist=0;
  //loop over ALL the events in the users chain
  Long64_t Nch=0;
  //If we have loaded and set a kinmatic bin entry list, use it
  // cout<< fCurKinBinList->GetName()<<" "<<fCurKinBinList->GetN()<<" "<<fNNChain<<" "<<fCurKinBinList->GetLists()<<" "<<endl;
  if(fCurKinBinList) {
     Nch=fCurKinBinList->GetN(); 
     fNNChain->SetEntryList(fCurKinBinList);
     if(fCurKinBinList->GetLists()){//if only 1 file lists not created
       if(fCurKinBinList->GetLists()->GetEntries()!=fNNChain->GetNtrees()){
	 Nch=0;//differnt number of files in entry list to chain, so recalc number of events
         for(Int_t itree=0;itree<fNNChain->GetNtrees();itree++){
	   Nch+=fCurKinBinList->GetEntryList(fCurKinBinList->GetTreeName(),fNNChain->GetListOfFiles()->At(itree)->GetTitle())->GetN();
	 }
       }
     }	
   }//if chain has entry list
   else Nch=fNNChain->GetEntries();
   Long64_t nnentry=0;
   Int_t treenum=0;
   for(Long64_t chentry=0;chentry<Nch;chentry++){
    if(fCurKinBinList) {//got to get the chain entry number from the entrylist
      //or else it just gives the local tree number
      //Long64_t treeEntry =fCurKinBinList->GetEntryAndTree(chentry,treenum);
      // nnentry = treeEntry+fNNChain->GetTreeOffset()[treenum];
      nnentry=fNNChain->GetEntryNumber(chentry); 
    }
    else nnentry=chentry;
   //calculate the distance squared 
    curr_dist=Distance2(fCoord0,fVcoord[nnentry]);
    //if(nnentry<68) cout<<nnentry<<" "<<curr_dist<<" "<<fNNmap.size()<<" "<<fNmax<<" "<<fVcoord.size()<<" "<<fVcoord[nnentry][0]<<endl;
    //now fill map
    if(fNNmap.size()<fNmax){//fill map if not got enough entries
      fNNmap[curr_dist]=nnentry;
      continue; //continue loop until reach NN entries
    }   
    it = fNNmap.end();//move to one past last element ("furthest stored neighbour")
    it--; //move back to last element
     if(curr_dist<it->first){ //fill map if this distance is less than the largest in the map
      fNNmap.erase(it);
      fNNmap[curr_dist]=nnentry;
    }
     //map filled
     if(fSufficient){//if set a sufficient distance check for it
       it = fNNmap.end();//move to one past last element ("furthest stored neighbour")
       it--; //move back to last element
       if(fSufficient>it->first) break; //we have enough events close enough
     }
  }//end NN loop
}
void  THSEventWeighter::FillNNEvTree(Long64_t entry){
  fNNEvTree->Reset(); //get rid of previous event
  if(fIsLoadNN) {LoadNNEvTree(entry); return;}
  //fNNMap->first = distance; fNNMap->second= event id
  map<Float_t,Int_t>::iterator it; //iterator for fNNMap
  Int_t count=0;
  // Bool_t doFill=0;
  //if(fNNEvTree->GetEntries()>0) {doFill=kFALSE;fNNEvTree->Reset();}
  // RooRealVar* RFvx=fRooFit->var("MM");
  //RooDataSet *nn_data=(RooDataSet*)(fRooFit->data("NNset"));
  //nn_data->reset();
  if(fIsSaveNN)fNNVdisVar.clear();
  for(it =fNNmap.begin();it !=fNNmap.end();it++){
    FillNNEvBranches(it->second); //users must define this function
    //Fill nn tree for fitting    
     fNNEvTree->Fill(); 
     //*RFvx =fMM ;
     //nn_data->add(RooArgSet(*RFvx));
    if(fIsSaveNN){//if writing NN tree for each event to file
    //put it in nn vector for writing to tree
      if(fIsSaveNN)fNNVdisVar.push_back(fVdisVar[it->second]);
    }
    //do some histogramming
    //fCoordi=fVcoord[it->second];
    // FillHistograms("CutNN",0);
    count++;
  }//tree should now be filled with fNmax events
  //Save to tree of trees
  if(fIsSaveNN)fTofT->Fill();
}
void  THSEventWeighter::LoadNNEvTree(Long64_t entry){
  //Load sa saved tree of nearest neignbours for fitting
  fNNChainLoad->GetEntry(fOffNNChain+entry);
  //  Bool_t doFill=0;
  //if(fNNEvTree->GetEntries()>0) {doFill=kFALSE;fNNEvTree->Reset();}
  //if(fVdisVar[0]!=0)return;
  for(UInt_t ii=0;ii<fNNVdisVarP->size();++ii){
     fVdisVar[ii]=fNNVdisVarP->at(ii);//to keep consistent FillNNEvBranch functionwith standard NN operation
     // cout<<ii<<" "<< fVdisVar[ii][0]<<" "<<endl;
     FillNNEvBranches(ii); //users must define this function
    //Fill nn tree for fitting    
    //Note this line causes the processing to slow when also proceed to fitting
    //wihtout fitting it runs very fast ~50kHz
    //without this fitting runs fast ~1kHz
    //with both the rate is 100Hz.....
     fNNEvTree->Fill(); 
    //   cout<<fMM<<endl;
     // if(!doFill) continue;
  }
 
}
void  THSEventWeighter::SetupRooFit(){
  //a default/example model for fitting with
  //users can interface via the fRooFit workspace
  //Fits a Gaussian + polynomial to a missing mass distribution centred on the nucleon mass

  cout<<"Setup up the RooFit Model"<<endl;
  //Make RooFit silent....
  RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL) ;
  RooMsgService::instance().setSilentMode(true);
  //Once off initialisation of RooFit
  fRooFit=new RooWorkspace("rfwspace");
  //Setup the variables and models for RooFit
  //variable(s) to be fitted
  TString fitv1=fNNEvTree->GetListOfBranches()->At(0)->GetName();
  Float_t minv1=0.8;
  Float_t maxv1=1.04;
  RooRealVar RFvx(fitv1,fitv1,minv1,maxv1);
  RFvx.Print();
  //Make fit model
  // --- Parameters ---
  RooRealVar sigmean("sigmean","miss mass",0.94,minv1,maxv1) ;
  RooRealVar sigwidth1("sigwidth","miss mass width1",0.03,0.005,0.1) ;
  RooGaussian RFs_model("smodel","signal1 PDF",RFvx,sigmean,sigwidth1) ;

 //construct background as Chebychev polynomial
  RooRealVar c0("c0","coefficient #0", 1.0,-1.0,1.0) ;
  RooRealVar c1("c1","coefficient #1", 0.1,-1.0,1.0) ;
  RooRealVar c2("c2","coefficient #2",-0.1,-1.0,1.0) ;
  RooRealVar c3("c3","coefficient #3",-0.1,-1.0,1.0) ;
  RooChebychev RFb_model("bmodel","background p.d.f.",RFvx,RooArgList(c0,c1)) ;

  //combine signal + background
  RooRealVar RFs_Yield("fSigYield","fitted yield for signal",fNmax/2,0.,fNmax*1.2); 
  RooRealVar RFb_Yield("fBckYield","fitted yield for background",fNmax/2,0.,fNmax*1.2 ); 
  // model(x) = fsig*sig(x) + (1-fsig)*bkg(x)
  RooRealVar RFsig("fsig","signal fraction",0.5,0.05,0.95) ;
  RooAddPdf RFmodel("model","fitted signal and background model",
		    RooArgList(RFs_model,RFb_model), 
		    RooArgList(RFs_Yield,RFb_Yield));
  
  RFmodel.Print();

  fRooFit->import(RFmodel);
  
  //data set container
  
  // RooDataSet dset("NNset","NNset",RooArgSet(RFvx)) ;
  // fRooFit->import(dset);
}
void  THSEventWeighter::RunRooFit(){
  //a default/example model for fitting with
  //users can interface via the fRooFit workspace
  //Fits a Gaussian + polynomial to a missing mass distribution centred on the nucleon mass
  RooRealVar* RFvx=fRooFit->var(fNNEvTree->GetListOfBranches()->At(0)->GetName());
  RooAddPdf* RFmodel=dynamic_cast<RooAddPdf*>(fRooFit->pdf("model"));

  //Do the fit (to be called each event)
  //load data from nearest neighbour tree

  RooDataSet nn_data(TString("Data Set"),TString("Data Set"),RooArgSet(*RFvx),Import(*fNNEvTree));
  // nn_data.Print() ;
  //RooDataSet *nn_data=(RooDataSet*)(fRooFit->data("NNset"));
  //initilaise parameters;
  fRooFit->var("sigmean")->setVal(0.94);
  fRooFit->var("sigwidth")->setVal(0.03);
  fRooFit->var("c0")->setVal(1.0);
  fRooFit->var("c1")->setVal(0.1);
  //fRooFit->var("c2")->setVal(0.0);
  //fRooFit->var("c3")->setVal(0.0);
  RooFitResult* rf_res = RFmodel->fitTo(nn_data,Extended());

  //To get the Q value must get values of PDFs at mass=mm
  RFvx->setVal(fDiscVar[0]);
  //fRooFit->var("MM")->setVal(miss->M());
  RooRealVar* RFs_Yield=fRooFit->var("fSigYield");
  RooRealVar* RFb_Yield=fRooFit->var("fBckYield");

  Double_t valT=RFmodel->getVal(*RFvx)*(RFs_Yield->getVal()+RFb_Yield->getVal()); //Total PDF
  Double_t valS=RFs_Yield->getVal()*fRooFit->pdf("smodel")->getVal(*RFvx); //signal pdf * sigyield
  Double_t valB=RFb_Yield->getVal()*fRooFit->pdf("bmodel")->getVal(*RFvx);//background pdf *byield
  Double_t Q=valS/(valB+valS);

  if(fIsPlot){
   TDirectory* savedir=gDirectory;
    gROOT->cd();
     //make some plots
    TCanvas *c=new TCanvas("c","c");
    RooPlot* frame = RFvx->frame(Title("vx")) ;
    nn_data.plotOn(frame,Binning(50));
    //note Visualise error does not seem to do anything?
    RFmodel->plotOn(frame,VisualizeError(*rf_res,1,kFALSE),DrawOption("L"),LineWidth(2),LineColor(kRed));
    RFmodel->plotOn(frame,Components(*(fRooFit->pdf("smodel"))),LineStyle(kDashed)) ; 
    RFmodel->plotOn(frame,Components(*(fRooFit->pdf("bmodel"))),LineStyle(kDashed)) ; 
    
    RFmodel->paramOn(frame,
		     FillColor(kRed),
		     Label("Global Fit parameters:"),
		     Layout(0.1, 0.4, 0.9),
		     Format("NEU",AutoPrecision(3)),
		     ShowConstants());
    nn_data.statOn(frame) ;
    TLine arrow(fDiscVar[0],0,fDiscVar[0],1000) ;
    arrow.SetLineColor(kRed) ;
    arrow.SetLineWidth(3) ;
    frame->addObject(&arrow) ;
    frame->Draw();
    
    
    cout<<"Tot "<< valT<<" "<<valB+valS<<endl;
    cout<<"Signal "<< valS<<" "<<fRooFit->pdf("smodel")->getVal(*RFvx)<<endl;
    cout<<"Background "<< valB<<" "<<fRooFit->pdf("bmodel")->getVal(*RFvx)<<endl;
    cout<<" At "<<fDiscVar[0]<<endl;
    cout<<" Q "<<Q<<endl;
    
    //Add text to frame
   TString sQ;
   sQ.Form("Q = %f",Q);
   TText txt(1.01,15,sQ) ;
   txt.SetTextSize(0.04) ;
   txt.SetTextColor(kRed) ;
   frame->addObject(&txt) ;
   frame->Draw();

   c->Modified();
   c->Update();
   cin.get();
   delete c;
   savedir->cd();
   
  }
  //Note could attemtp to calculate errors for signal and background based on RooAbsReal::plotOnWithErrorBand( algorithm.
  fQval=Q;
  
  //some fit parameters
  fSigWidth=fRooFit->var("sigwidth")->getVal(); //signal width
  fSigMean=fRooFit->var("sigmean")->getVal(); //signal width
  if(RFb_Yield->getVal())fSB=RFs_Yield->getVal()/RFb_Yield->getVal();//note actually signal fraction
  else fSB=0;  
  //caclulate the signal/background ratio
  //note this code has memeory leak and causes code to slow down a lot
  // RFvx->setRange("underpeak",fSigMean-3*fSigWidth,fSigMean+3*fSigWidth);
  // RooAbsReal* intBackX = fRooFit->pdf("bmodel")->createIntegral(*RFvx,NormSet(*RFvx),Range("underpeak")); 
  // RooAbsReal* intSigX = fRooFit->pdf("smodel")->createIntegral(*RFvx,NormSet(*RFvx),Range("underpeak")); 
  // fSB=intSigX->getVal()*RFs_Yield->getVal()/(intSigX->getVal()*RFs_Yield->getVal()+intBackX->getVal()*RFb_Yield->getVal());

}
void THSEventWeighter::CreateNNTree(){
  // fNNEvTree=new TTree("NN tree","tree with NN fit variables");
  //fNNEvTree->Branch("MM",&fMM,"MM/F");
}
void THSEventWeighter::SaveNNTree(TFile* file,TString OutName){
  //function to save tree 
  //needs to be called in notify, before THSOutput::Notify  to save previous files and SlaveTerminate to sae the last file
  if (file) {
    TDirectory *savedir = gDirectory;
    file->cd();
    cout<<"Write tree to file "<<fTofT->GetEntries()<<endl;
    fTofT->Write(0, TObject::kOverwrite);
    if(!OutName.EndsWith(".root"))fTofT->Reset(); //Empty tree of trees to start new file
    fTofT->SetDirectory(0);
    savedir->cd();
  }
}
void THSEventWeighter::NotifyNNTree(TTree* tree){
  //Get the offset for the chain entry number in the current tree
  TObjArray* files=fNNChain->GetListOfFiles();
  //ListoFile->At()->GetName() returns the treename not the filename which is in the title!
  Int_t treenum=0;
  for(treenum=0;treenum<files->GetEntries();treenum++){
    if(TString(files->At(treenum)->GetTitle())==TString(tree->GetCurrentFile()->GetName()))break;
  }
  fOffNNChain = fNNChain->GetTreeOffset()[treenum];//got offset

}
void THSEventWeighter::LoadEventEntryLists(TString filename,TString hbname){
  //Given filename and hbname, will look in filename for directory 
  //HSKinBinEntries, then read in the TEntryLists from there
  //Will also read histgram hbname which should define the kinematic bins
  //as done in THSHisto
  TDirectory* savedir=gDirectory;
  TFile* file=new TFile(filename);
  fKinBins=(TH1*)file->Get(hbname);
  fEventEntryList=new TList();
  TDirectoryFile* listdir=(TDirectoryFile*)file->Get("HSKinBinEntries");
   TIter next(listdir->GetListOfKeys());
  TKey* key=0;
  while((key=(TKey*)next())) fEventEntryList->Add(listdir->Get(key->GetName()));
  cout<<"void THSEventWeighter::LoadEventEntryLists( Added "<<fEventEntryList->GetEntries()<<" from "<<filename<<endl;
  savedir->cd();
}
void THSEventWeighter::SetEventEntryList(Float_t ev1,Float_t ev2,Float_t ev3){
  //Function that finds the bin number and uses it to find the correct entrylist
  //For the current event. This must be called in Process before MakeNNMap.
  Int_t evBin=0;
  if(fKinBins){
    evBin=fKinBins->FindBin(ev1,ev2,ev3); //find bin
    if(fEventEntryList) fCurKinBinList=(TEntryList*)fEventEntryList->At(evBin);
  }
}
