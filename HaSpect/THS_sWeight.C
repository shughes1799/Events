//--Author      DI Glazier 29/10/2014
//--Rev
//--Update
//--Description
//HASPECT Event Reconstruction
//THS_sWeight.C
//Macro to implement RooStat::sPlot Method
//Should be configured to the input tree and data models
//Users must 
//(0)Create an ouput file TFile* outPlots=new TFile("outfile.root","recreate");
//(1)setup input chain in  THS_sWeight() and deal with kinematic bins if using
//(2)convert chain to RooFit dataset in AddData(), you will need to define the variable(s) you are fitting
//(3)define their fit model in AddModel(), as a funciton of the variable(s) defined in AddData
//(4)configure the DoSPlot funciton with the model PDFs and variables you have defined
//(5) Run with .x THS_sWeight.C++()
#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooStats/SPlot.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooAddition.h"
#include "RooProduct.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "RooAbsPdf.h"
#include "RooFit.h"
#include "RooFitResult.h"
#include "RooWorkspace.h"
#include "RooConstVar.h"
#include <iostream>
//for drawing parameters on plots
#include "TChain.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TText.h"
#include "TEntryList.h"
#include "TKey.h"
#include "TH1.h"
#include "TObjArray.h"

using namespace std;

// use this order for safety on library loading
using namespace RooFit ;
using namespace RooStats ;


// Some Functions
// see below for implementation
void Run_sWeight(TTree* schain,TString fel="",TString hbname="");
void Add1DModel(RooWorkspace*);
Bool_t AddData(RooWorkspace*,TTree*);
RooStats::SPlot*  DoSPlot(RooWorkspace*);
void LoadEventEntryLists(TString filename,TString hbname);


//Some global varibles
TList* fEventEntryList=0;//Entry lists for kinematic bins if using(read from file)
TH1* fKinBins=0; //Histogram definingthe kinematic bins (read from file)
TString dataSetName="swDS"; //Name tag for RooFit dataset
TString pdfName="swPDF";  //Name tag for RooFit model PDF
Long64_t EventCounter=0; // Total event counter
TCanvas *canvas=new TCanvas(); //Canvas for plotting fit results
  TTree* cop=0;
//////////////////////////////////////////////////////////////////////
//You must create your output file here....
TFile* outPlots=new TFile("outname.root","recreate"); //File to write sPlots to
/////////////////////////////////////////////////////////////////////

void THS_sWeight(){
  //Set up the input chain to be analysed
   TChain* tree=new TChain("TREENAME_HERE","datachain");
   tree->Add("ADD_TREEFILES_HERE");

   //Do fitting
   //with no kinematic bins
   Run_sWeight(tree);
   //with kinematic bins
   //the second arguement is the file containing the kinematic bins and the 3rd the name of the hisbins histogram
   //Kinematic bins can be created using THSHisto functionality on a TChain through a selector
   //Once bins are defined in THSHisto a list of TEntryLists can be produced corresponding to the 
   //different bins and used here. KinBinHistName will be the name of fHisBins given in the selector that produced the entrylists
   //Run_sWeight(tree,"SWeightKINBINFILE.root","KinBinHistName");

}
void Run_sWeight(TTree* schain,TString fel,TString hbname){
  //Make an array to hold the sPlot objects, one for each defined kinematic bin
  //Note if no bins are defined (fel="") only 1 total sPlot is produced and added to sPlots
  TDirectory *sDir=outPlots->mkdir("HSsPlots");//This name will be used to retrieve the list in the TSelector class

  //If given an entrylist files check it and load entry lists
  if(fel.Contains(".root")) LoadEventEntryLists(fel,hbname);
     
  if(!fEventEntryList){//Just do fit once
    // Create a new workspace to manage the project.
    RooWorkspace* wspace = new RooWorkspace("myWS");
    // add some data to the workspace
    AddData(wspace,schain);
    // add the signal and background models to the workspace.
    // Inside this function you will find a discription our model.
    // This only considers a 1D PDF
    Add1DModel(wspace);
    //Calculate the sweights and add to the list
    TDirectory* savedir=gDirectory;
    sDir->cd();
    DoSPlot(wspace)->Write(dataSetName);
    savedir->cd();

    delete wspace;
  }
  else{
    //loop over kinematic bin lists and do an splot for each
    for(Int_t iel=0;iel<fEventEntryList->GetEntries();iel++){
      // Create a new workspace to manage the project.
      RooWorkspace* wspace = new RooWorkspace("myWS");
      schain->SetEntryList((TEntryList*)fEventEntryList->At(iel));
      // add some data to the workspace
      if(AddData(wspace,schain)){
	// add the signal and background models to the workspace.
	// Inside this function you will find a discription our model.
	// This only considers a 1D PDF
	Add1DModel(wspace);
	//Calculate the sweights and save it to file
	TDirectory* savedir=gDirectory;
	sDir->cd();
	TString SPName;
	SPName.Form("SPlotBin%d",iel);
	DoSPlot(wspace)->Write(SPName);
	savedir->cd();
	if(wspace){ delete wspace;}
  	if(cop)delete cop;
      }
      else {
	TDirectory* savedir=gDirectory;
	sDir->cd();
	TString SPName;
	SPName.Form("SPlotBin%d",iel);
	TNamed(SPName.Data(),"dummy entry list").Write(); //place holder
  	savedir->cd();
 	if(wspace){ delete wspace;}
  	if(cop)delete cop;
      }
    }
  }
  cout<<" fitted "<<EventCounter<<" events"<<endl;
  //Now write sPlots to ouput file
  if(fKinBins){//If using kinematic bins also write the histogram defining it
    fKinBins->SetTitle(fKinBins->GetName());
    fKinBins->SetName("HSsPlotsBins");//This name will be used to retrieve it from TSelector class
    fKinBins->Write();
  }
  outPlots->Close();
}
void Add1DModel(RooWorkspace* ws){
  //Add your own model here, it should be fitting variables given in AddData() (here it is ML)
  //I will leave example code here for now, but much will have to be rewritten for the users own analysis

  //////////////////////////////////////////////////
  Int_t Nevents=ws->data(dataSetName)->numEntries();//get number of entries to help set initial parameters
  pdfName=dataSetName;
  pdfName.ReplaceAll("DS","PDF");
  //////////////////////////////////////////////////
  //create signal PDF shape
  RooRealVar sl_mean1("Lambda Mean1","Lambda Mean1",0,-0.01,0.01);
  RooRealVar sl_sigma1("Lambda Sigma1","Lambda Width1",0.002,0.0001,0.02);
  RooRealVar sl_mean2("Lambda Mean2","Lambda Mean2",0,-0.01,0.01);
  RooRealVar sl_sigma2("Lambda Sigma2","Lambda Width2",0.005,0.0001,0.02);
  //Note, ws->var("ML") gets the already declared (AddData) vairiable ML whih is part of the input tree
  RooGaussian sl1("LambdaPDF1","LambdaPDF1",*(ws->var("ML")),sl_mean1,sl_sigma1);
  RooGaussian sl2("LambdaPDF2","LambdaPDF2",*(ws->var("ML")),sl_mean2,sl_sigma2);
  RooRealVar Ysl1("Ysl1","Ysl1",Nevents/20,0.,Nevents) ; 
  RooRealVar Ysl2("Ysl2","Ysl2",Nevents/10,0.,Nevents) ; 

  RooAddPdf sl("LambdaPDF","LambdaPDF",RooArgList(sl1, sl2), RooArgList(Ysl1,Ysl2));


  /////////////////////////////////////////////////
  //create background shape
  RooRealVar l0("l0","lcoefficient #0", 0.5,-1.,1.) ;
  RooRealVar l1("l1","lcoefficient #1", 0.1,-1.,1.) ;
  RooRealVar l2("l2","lcoefficient #2",-0.1,-1.,1.) ;
  // RooRealVar l3("l3","coefficient #3",-0.1,-1.,1.) ;
  RooChebychev bl("LambdaBackPDF","Lambda background p.d.f.",*(ws->var("ML")),RooArgList(l0)) ;//Could use higher order poly
 
  //////////////////////////////////////////////
  // combined model
  // These variables represent the number of Signal or Background events
  // They will be fitted.
  // Note In general we could have more than 2 types of event
  RooRealVar s_Yield("SigYield","fitted yield for signal",Nevents/2,0.,Nevents*1.2) ; 
  RooRealVar b_Yield("BckYield","fitted yield for background", Nevents/2 ,0.,Nevents*1.2) ; 

  // now make the combined model
  std::cout << "make full model" << std::endl;
   RooAddPdf model(pdfName,"signal and background model",
		  RooArgList(sl,bl), 
		  RooArgList(s_Yield,b_Yield));


  // interesting for debugging and visualizing the model
  model.graphVizTree("fullModel.dot");

  std::cout << "import model" << std::endl;

  ws->import(model);
  // ws->import(model,RenameAllVariablesExcept(pdfName,"ML"));
}

//____________________________________
Bool_t AddData(RooWorkspace* ws, TTree* schain){
  //import variables from your tree that you would like to fit
  //I will leave example code here, but users will need to edit for their own analysis
  cout<<"Got schain with "<<schain->GetEntries()<<" events"<<endl;
  if(schain->GetEntryList()){//if chain has an entry list use this to filter dataset now, note ROOFIT data set do not currently use TEntryLists
    cop = schain->CloneTree(0);
    for(Int_t i=0;i<schain->GetEntryList()->GetN();i++){
      //break in case the current chain has less files than KinBins chain
      if(schain->GetEntry(schain->GetEntryNumber(i))<1) break;
      cop->Fill();
    }
    if(cop->GetEntries()<50)return kFALSE; //Fit will struggle with few events, but this can be removed if appropriate
  }
  else cop=schain;//no entry list
  EventCounter+=cop->GetEntries();
  // Make tree in to RooDataSet
  // Declare tree branches as RooRealVars
  RooRealVar MK0("MK0","MK0",-0.04,0.04) ;//Note selecting these ranges cuts on the rest of the data in the tree, so fits will only be to do within this range
   //non fit variables
  RooRealVar ML("ML","ML",-0.03,0.03) ;
  RooRealVar MN("MN","MN",0.8,1.3) ;
  RooRealVar EG("Eg","Eg",1.1,1.3) ;
  RooRealVar CT("cthCM","cthCM",-0.2,0.2);
 //Event ID number....note the max number of events is implied 1E12
  //Keep this in for synchronisation purposes
  RooRealVar ID("fgID","fgID",0,1E12);

  if( schain->GetEntryList()) {
    //If there is an entry list give a unique name to the datset
    //Here I am only actually going to add the varibale ML to the dataset
    RooDataSet data(dataSetName=TString(schain->GetEntryList()->GetName())+"DS","Total data",RooArgSet(ML,ID),Import(*cop));
    cout<<"Import data to workspace"<<endl;
    // import data into workspace
    ws->import(data);
  }
  else{
    
    RooDataSet data(dataSetName,"Total data",RooArgSet(ML,ID),Import(*cop));
    cout<<"Import data to workspace"<<endl;
    // import data into workspace
    ws->import(data);
  }  
   return kTRUE;
}

//____________________________________
RooStats::SPlot*  DoSPlot(RooWorkspace* ws){
  //Users will have to edit this functioon to account for changes made for their own model
  //This will mainly involve matching the variable names to those defined in AddModel and AddData

  std::cout << "Calculate sWeights" << std::endl;

  // get what we need out of the workspace to do the fit
  RooAbsPdf* model = ws->pdf(pdfName);
  RooDataSet* data = (RooDataSet*) ws->data(dataSetName);

  // fit the model to the data.
  model->fitTo(*data, Extended() );

  // The sPlot technique requires that we fix the parameters
  // of the model that are not yields after doing the fit.
  ws->var(TString("Lambda Sigma1"))->setConstant();
  ws->var(TString("Lambda Mean1"))->setConstant();  
  ws->var(TString("Lambda Sigma2"))->setConstant();
  ws->var(TString("Lambda Mean2"))->setConstant(); 
  ws->var(TString("Ysl1"))->setConstant();  
  ws->var(TString("Ysl2"))->setConstant();  
  ws->var(TString("l0"))->setConstant();
  //The only 2 free parameters will now be the signal and background yields
  //Note, if we have more than 2 types of event we would need to get additional yeilds here  
  RooRealVar* s_Yield = ws->var(TString("SigYield"));
  RooRealVar* b_Yield = ws->var(TString("BckYield"));


  RooMsgService::instance().setSilentMode(true);

  // Now we use the SPlot class to add SWeights to our data set
  // based on our model and our yield variables
  RooStats::SPlot* sData = new RooStats::SPlot(pdfName+"SW","An SPlot",
		            *data, model, RooArgList(*s_Yield,*b_Yield) );

  //Check that our weights have the desired properties

  std::cout << "Check SWeights:" << std::endl;


  std::cout << std::endl <<  "Yield of Signal is " 
  	    << s_Yield->getVal() << ".  From sWeights it is "
  	    << sData->GetYieldFromSWeight(TString("SigYield")) << std::endl;


  std::cout << "Yield of Background is " 
  	    << b_Yield->getVal() << ".  From sWeights it is "
  	    << sData->GetYieldFromSWeight(TString("BckYield")) << std::endl
  	    << std::endl;

  for(Int_t i=0; i < 10; i++)
    {
      std::cout << "signal Weight   " << sData->GetSWeight(i,TString("SigYield")) 
  		<< " background Weight   " << sData->GetSWeight(i,TString("BckYield")) 
  		<< "  Total Weight   " << sData->GetSumOfEventSWeight(i) 
  		<< std::endl;
    }

  std::cout << std::endl;

  //Now plot the fit resluts for checking 
  //Users will have to edit this for the variables they are using
  RooRealVar* vML = ws->var("ML");
  RooAbsPdf* b_Model = ws->pdf("LambdaBackPDF"); 
  RooPlot* frame = vML->frame() ; 
  data->plotOn(frame, DataError(RooAbsData::SumW2) ) ; //plot the data
  model->plotOn(frame,LineStyle(kDashed), LineColor(kRed)) ; //model = signal + back fit result   
  model->plotOn(frame,Components(*b_Model),LineStyle(kDashed),LineColor(kGreen)) ; //just the back fit result  
  model->paramOn(frame,
		 FillColor(kRed),
		 Label("Global Fit parameters:"),
		 Layout(0.1, 0.4, 0.9),
		 Format("NEU",AutoPrecision(3)),
		 ShowConstants());
  frame->SetTitle("M#Lambda distribution fit");
  frame->Draw() ;
  canvas->Modified();
  canvas->Update();
  cin.get();//wait here until users ready to continue
  //Write fit plot canvas to output file
  TDirectory* savedir=gDirectory;
  outPlots->cd();
  canvas->SetName(pdfName);
  canvas->Write();
  savedir->cd();

  //return the finished RooStats::sPlot object 
  return sData;

}
//THSEventWEighter functions for handling kin bins entry lists
void LoadEventEntryLists(TString filename,TString hbname){
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
  cout<<"LoadEventEntryLists( Added "<<fEventEntryList->GetEntries()<<" from "<<filename<<endl;
  savedir->cd();
}
