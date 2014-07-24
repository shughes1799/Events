
Bool_t IsHisto=kFALSE;  //use THSHisto?
Bool_t IsAppendTree=kFALSE; //Append branches to the input tree
Bool_t IsNewTree=kFALSE;  //Output a brand new tree
Bool_t IsHSTree=kFALSE;   //Use THSOuput to THSParticle interface (probably not)
Bool_t IsQval=kFALSE;   //Use Qvale event weighting algorithm

TString FileName;   // The input filename containing the tree
TString TreeName;   // The name of the tree
TString OutName;   // The name of the output directory or file
TString SelName;    // The name of the selector to be produced
TFile* File; //The file object
TTree* Tree; // the tree object
Bool_t IsParent=kFALSE; //use connection with parent tree
//The following are only required if IsParent==TRUE
Int_t StepN;        // If connecting with parent, which step number is its selector in
TString ParSel;    // The name of the parent selector


void MakeHSSelector(){

  //Get the tree
  File=new TFile(FileName);
  if(!File) {cout<<"No file "<<FileName<<" found"<<endl; return;}
  else if (File && File->IsZombie()) {cout<<"No file "<<FileName<<" found Z"<<endl; return;}
  Tree=(TTree*)File->Get(TreeName);
  if(!Tree){cout<<"No tree "<<TreeName<<" found in "<<FileName<<endl;return;}

  //First make the selector in the standard way
  Tree->MakeSelector(SelName);

  //now add in THSOuput functionality
  HSit_h();
  HSit_C();


  //Combine with a parent tree
  if(IsParent)ConnectParent();

  //Make the Control macro
  //First set the variable to the HASPECT source code
  ControlMacro();
}
void ControlMacro(){
  TString HSANA=gSystem->Getenv("HSANA");
  //Open a template file 
  TMacro macro(HSANA+"/Control_Template.C");
  TList *lines=macro.GetListOfLines();
  //set tree name
  TObjString* objs=(TObjString*)macro.GetLineWith("TChain* tree=new TChain");
  objs->String().ReplaceAll("TREENAME",TreeName);
  //Assume the input files are all in the same directory as the current tree
  objs=(TObjString*)macro.GetLineWith("tree->Add(\"INPUT TREE DIRECTORY/FILENAMES\");");
  objs->String().ReplaceAll("INPUT TREE DIRECTORY",gSystem->DirName(FileName));
  //Output destination
  objs=(TObjString*)macro.GetLineWith("OUTDIRNAME_FULLPATH");
  objs->String().ReplaceAll("/OUTDIRNAME_FULLPATH/",OutName);
  //HSANA
  //objs=(TObjString*)macro.GetLineWith("gROOT->LoadMacro(HSANA+\"/HSControl.C\");");
  // objs->String().ReplaceAll("HSANA",HSANA);

//Process this new selector
   TObjString* objs=(TObjString*)macro.GetLineWith("tree->Process(\"SELECTOR\");");
   objs->String().ReplaceAll("SELECTOR",SelName+".C++");
  
   //If using a parent need to load its selector class
 //Now include parent selector to combine with 
   if(IsParent){
     TObject* obj=macro.GetLineWith("tree->Process("); 
     Int_t place=lines->IndexOf(obj); //get line number
     lines->AddAt(new TObjString("  //we are linking to a parent selector so need to load its source code here"),place-1); 
     lines->AddAt(new TObjString(TString("  gROOT->LoadMacro(\"")+ParSel+TString(".C++\");") ),place); 
     lines->AddAt(new TObjString(TString("  if(prf) prf->Load(\"")+ParSel+TString(".C+\");") ),place+1); 

   }

   if(IsQval){
     //If you want to load save NN trees need to define its chain
     TObject* obj=macro.GetLineWith("tree->Add(");
     Int_t place=lines->IndexOf(obj); //get line number
     place++;
     lines->AddAt(new TObjString("   //Load a chain of the previously saved  NN trees for quick Qweigthing"),place++); 
     lines->AddAt(new TObjString("   //The input files should match the files in tree"),place++); 
     lines->AddAt(new TObjString("   //TChain* treeNN=new TChain(\"TreeOfNNTrees\",\"datachain\");"),place++); 
     lines->AddAt(new TObjString("   //treeNN->Add(\"/home/dglazier/Work/Research/HaSpect/data/g11pippippim_missn_HSID/Qval/inp1*root\");"),place++); 


    //need to add significant code to pass the Chain to the selector
     //for cases of using PROOF or not
     TObject* obj=macro.GetLineWith("tree->Process(");
     place=lines->IndexOf(obj); //get line number
     //remove process line
     lines->Remove(obj);
     //Add Control Qvalue stuff
     lines->AddAt(new TObjString("//EventWeight needs full chain to find nearest neighbours, so this additional code is necessary!"),place++); 
     lines->AddAt(new TObjString("   if(prf){//proof looks after its own input list"),place++); 
     lines->AddAt(new TObjString("     prf->AddInput(new TNamed(\"NNChainName\",tree->GetName()));"),place++); 
     lines->AddAt(new TObjString("     prf->AddInput(tree);"),place++); 
     lines->AddAt(new TObjString("    //Give the chain of NN trees to the input list id using LoadNNTree"),place++); 
     lines->AddAt(new TObjString("   // prf->AddInput(new TNamed(\"NNChainLoad\",treeNN->GetName()));"),place++); 
     lines->AddAt(new TObjString("    //prf->AddInput(treeNN);"),place++);

     lines->AddAt(new TObjString("    //Give the name of the file and histogram containing the kinematic bins"),place++); 
     lines->AddAt(new TObjString("   // prf->AddInput(new TNamed(\"NNKinBins\",KINBINFILENAME));"),place++); 
     lines->AddAt(new TObjString("   // prf->AddInput(new TNamed(\"NNKinBinsHis\",KINBINFILENAME));"),place++); 
 
     lines->AddAt(new TObjString(TString("    tree->Process(\"")+SelName+".C++\");"),place++); 
     lines->AddAt(new TObjString("   }"),place++); 
     lines->AddAt(new TObjString("  else{ //we have to look after the input list"),place++); 
     lines->AddAt(new TObjString(TString("    gROOT->LoadMacro(\"")+SelName+".C++\");"),place++); 
     lines->AddAt(new TObjString(TString("    ")+SelName+"* nnsel=new "+SelName+"();"),place++); 
     lines->AddAt(new TObjString("     TList* inList=new TList();"),place++); 
     lines->AddAt(new TObjString("     inList->SetOwner();//let the list delete the objects you give it"),place++); 
     lines->AddAt(new TObjString("     inList->Add(new TNamed(\"NNChainName\",tree->GetName()));"),place++); 
     lines->AddAt(new TObjString("     inList->Add(tree->Clone());"),place++); 
     lines->AddAt(new TObjString("    //Give the chain of NN trees to the input list"),place++); 
    lines->AddAt(new TObjString("    //inList->Add(new TNamed(\"NNChainLoad\",treeNN->GetName()));"),place++); 
    lines->AddAt(new TObjString("    //inList->Add(treeNN);"),place++); 

     lines->AddAt(new TObjString("    //Give the name of the file and histogram containing the kinematic bins"),place++); 
     lines->AddAt(new TObjString("   // inList->Add(new TNamed(\"NNKinBins\",KINBINFILENAME));"),place++); 
     lines->AddAt(new TObjString("   // inList->Add(new TNamed(\"NNKinBinsHis\",KINBINFILENAME));"),place++); 
 
       lines->AddAt(new TObjString("     nnsel->SetInputList(inList);"),place++); 
     lines->AddAt(new TObjString("     Long64_t first=0;"),place++); 
     lines->AddAt(new TObjString("     tree->LoadTree(first); //This is done in TChain::Process(filename) but not Process(selector)!! "),place++); 
     lines->AddAt(new TObjString("     tree->Process(nnsel,\"\");"),place++); 
     lines->AddAt(new TObjString("     delete inList;"),place++); 
     lines->AddAt(new TObjString("   }"),place++); 
 
   }
   macro.SaveSource(TString("Control_")+SelName+".C");
}

void HSit_C(){
  TMacro macro(SelName+".C");

  ////////////////////////////////////////////////////
  //First deal with lines that need placed after marker lines
  //Make list of marker lines in TSelector skeleton
  TList* markers=new TList();
  //Make list of lines that need to be added to skeleton TSelector code
  TList* hslines=new TList();
  markers->Add(new TObjString("::Begin(TTree * /*tree*/)"));
  hslines->Add(new TObjString("   THSOutput::HSBegin(fInput,fOutput);"));


  markers->Add(new TObjString("::Process(Long64_t entry)"));
  hslines->Add(new TObjString("   THSOutput::HSProcessStart(entry);"));

  markers->Add(new TObjString("::SlaveTerminate()"));
  hslines->Add(new TObjString("   THSOutput::HSSlaveTerminate();"));

  markers->Add(new TObjString("::Terminate()"));
  hslines->Add(new TObjString("   THSOutput::HSTerminate();"));

//Get list of line in TSelector skeleton
  TList *lines=macro.GetListOfLines();
  TObject* obj=0;
  //loop over new lines
  Int_t i=0;
  while(markers->At(i)){
    //   obj=lines->FindObject(markers->At(i)); //find the marker
    obj=macro.GetLineWith(((TObjString*)markers->At(i))->GetName()); //find the marker
    Int_t place=lines->IndexOf(obj); //get line number
    lines->AddAt(hslines->At(i++),place+2); //add the extraline, +2 for { line
  }
  //Case of SlaveBegin, we want to have TString option=GetOPtion line first
  obj=macro.GetLineWith("::SlaveBegin(TTree * /*tree*/)"); 
  Int_t place=lines->IndexOf(obj); //get line number
  lines->AddAt(new TObjString("   THSOutput::HSSlaveBegin(fInput,fOutput);"),place+8); //+8 to get past option line
  
  //Case of ProcessFill
  obj=macro.GetLineWith("// The return value is currently not used."); 
  Int_t place=lines->IndexOf(obj); //get line number
  lines->AddAt(new TObjString("   GetEntry(entry); //lazy and slow, you can speed the code up by getting the branches you need to use instead"),place+1); 
  lines->AddAt(new TObjString("   //Ready to do some analysis here, before the Fill"),place+2); 
  lines->AddAt(new TObjString("   "),place+3); 
  lines->AddAt(new TObjString("   THSOutput::HSProcessFill(); "),place+4); 



  if(IsHisto){//lines only needed if histogramming, addded at end of .C file
    //Need some configuration lines in SlaveBegin
    obj=macro.GetLineWith("   THSOutput::HSSlaveBegin(fInput,fOutput);"); 
    Int_t place=lines->IndexOf(obj); //get line number
    lines->AddAt(new TObjString("  //if you want kinematic bins you must define fHisbins here"),place+1); 
    lines->AddAt(new TObjString("   //fHisbins=new TH*()"),place+2); 
    lines->AddAt(new TObjString("   if(fHisbins) fOutput->Add(fHisbins);"),place+3);
    lines->AddAt(new TObjString("   //fHisbins->SetXTitle();//give useful axis name"),place+4);
    lines->AddAt(new TObjString("   //fHisbins->SetYTitle();//give useful axis name"),place+5);
    lines->AddAt(new TObjString("   THSHisto::ChangeNames();"),place+6);
    lines->AddAt(new TObjString("   THSHisto::LoadCut(\"Cut1\");"),place+7);
    lines->AddAt(new TObjString("   THSHisto::LoadHistograms();"),place+8);
    //Now fill the histograms in Process
    obj=macro.GetLineWith("   THSOutput::HSProcessFill();"); 
    place=lines->IndexOf(obj); //get line number
    lines->AddAt(new TObjString("   //Int_t kinBin=GetKinBin();//if fHisbins is defined need to give this meaningful arguments"),place);
    lines->AddAt(new TObjString("   //FillHistograms(\"Cut1\",kinBin);"),place+1);
    lines->AddAt(new TObjString("   FillHistograms(\"Cut1\",0);"),place+2);


    //And the user defined functions
    lines->Add(new TObjString(TString("void ")+SelName+"::HistogramList(TString sLabel){"));
    lines->Add(new TObjString("  TDirectory::AddDirectory(kFALSE); //do not add to current directory"));
    lines->Add(new TObjString("  //now define all histograms and add to Output"));
    lines->Add(new TObjString("  //label includes kinematic bin and additional cut name"));
    lines->Add(new TObjString("  // e.g fOutput->Add(MapHist(new TH1F(\"Mp1\"+sLabel,\"M_{p1}\"+sLabel,100,0,2)));"));
    lines->Add(new TObjString("  //end of histogram list"));
    lines->Add(new TObjString("  TDirectory::AddDirectory(kTRUE); //back to normal"));
    lines->Add(new TObjString("}"));

    lines->Add(new TObjString(TString("void ")+SelName+"::FillHistograms(TString sCut,Int_t bin){"));
    lines->Add(new TObjString("  fCurrCut=sCut;"));
    lines->Add(new TObjString("  fCurrBin=bin;"));
    lines->Add(new TObjString("  //Get histogram from list"));
    lines->Add(new TObjString("  //Fill histogram"));
    lines->Add(new TObjString("  TString sLabel;"));
    lines->Add(new TObjString("  sLabel=sCut+fVecBinNames[bin];"));
    lines->Add(new TObjString("  // e.g. FindHist(\"Mp1\"+sLabel)->Fill(fp1->M());"));
    lines->Add(new TObjString("}"));
     
  }
  if(IsNewTree){
    if(IsAppendTree){cout<<"Can't have a new tree and an append tree!"<<endl;exit(0);}
    //we want to create a new tree to output
    //We are going to do this in SlaveBegin
    obj=macro.GetLineWith( "THSOutput::HSSlaveBegin(fInput,fOutput);");
    Int_t place=lines->IndexOf(obj); //get line number
    Int_t poff=0;
    if(IsHSTree){
      lines->AddAt(new TObjString("   fStrParticles=option;"),place-1); 
      lines->AddAt(new TObjString("   //e.g. or hard wire particles here fStrParticles=\"pi+:pi+:pi-\";"),place-1); 
      poff=3;
    }
    else {
    lines->AddAt(new TObjString("  //Initialise the ouput tree"),place+1); 
    lines->AddAt(new TObjString("   Int_t buff=32000;"),place+2); 
    lines->AddAt(new TObjString("   Int_t split=0;//note split is important in the TSelector framework, if increased branches in subsequent selectors will be data members of the THSParticle object rather than the whole object (this can lead to name conflicts)"),place+3); 
    lines->AddAt(new TObjString("   //Only done once. THSOutput::FinishOutput Resets the tree before new files"),place+4); 
    lines->AddAt(new TObjString("   //If you want to save additional branches to a new file, do it here"),place+5); 
    lines->AddAt(new TObjString("   //If you are creating a HSParticle tree via fStrParticles then do not create a new tree here, just add the extra branches"),place+6); 
    if(!IsHSTree) lines->AddAt(new TObjString("   fOutTree=new TTree(\"HSParticles\",\"A tree containing reconstructed particles\"); "),place+7); 
    poff=8;
    }
   lines->AddAt(new TObjString("  //e.g. Make some objects to make branch with, these should be declared in the .h file "),place+poff); 
   lines->AddAt(new TObjString(" //e.g. fp1=new TLorentzVector();"),place+poff+1); 
   lines->AddAt(new TObjString("  //e.g.  fOutTree->Branch(\"p1\",&fp1,buff,split);"),place+poff+2); 
   
   
  }
  if(IsQval){
    //SlaveBegin
    obj=macro.GetLineWith( "THSOutput::HSSlaveBegin(fInput,fOutput);");
    Int_t place=lines->IndexOf(obj); //get line number
    lines->AddAt(new TObjString("// Event Weighting initialisation"),place+1); 
    lines->AddAt(new TObjString("   fNcoord=1;//Need to set the correct number of varibles in distance calc."),place+2); 
    lines->AddAt(new TObjString("   fNdisc=1;//Need to set the correct number of varibles signal/back fit"),place+3); 
    lines->AddAt(new TObjString("   fIsPlot=kTRUE;    //Show the fit to the nearest neigbours"),place+4); 
    lines->AddAt(new TObjString("   fIsSaveNN=kFALSE; //Save all the nearest nieghbours tree"),place+5); 
    lines->AddAt(new TObjString("   fIsLoadNN=kFALSE; //Load previously save nearest neigbours trees"),place+6); 
    lines->AddAt(new TObjString("   InitNN(fInput);"),place+7); 
    lines->AddAt(new TObjString("   if(fIsSaveNN) fOutput->Add(fTofT);"),place+8); 
    lines->AddAt(new TObjString("   SetupRooFit();"),place+9); 
     lines->AddAt(new TObjString("// End Event Weighting initialisation"),place+10); 
     if(IsNewTree){//if creating new file for EventWeight branches
      obj=macro.GetLineWith( "//e.g.  fOutTree->Branch(\"p1\",&fp1,buff,split);");
      place=lines->IndexOf(obj); //get line number
      lines->AddAt(new TObjString("    //EventWeighter make new output tree"),place+1); 
      lines->AddAt(new TObjString("    fOutTree->Branch(\"Qval\",&fQval,\"Qval/F\");"),place+2); 
      lines->AddAt(new TObjString("    fOutTree->Branch(\"SigWidth\",&fSigWidth,\"SigWidth/F\");"),place+3); 
      lines->AddAt(new TObjString("    fOutTree->Branch(\"SigMean\",&fSigMean,\"SigMean/F\");"),place+4); 
      lines->AddAt(new TObjString("    fOutTree->Branch(\"SB\",&fSB,\"SB/F\");"),place+5); 
    }
     //Process  
     TString smarker="GetEntry(entry); //lazy and slow, you can speed the code up by getting the branches you need to use instead";
     TString sline=macro.GetLineWith(smarker)->GetString();
     sline.ReplaceAll(smarker,"GetNNBranches(entry);//just get branches needed for weighting");
     macro.GetLineWith(smarker)->SetString(sline);


    obj=macro.GetLineWith( "//Ready to do some analysis here, before the Fill");
    place=lines->IndexOf(obj); //get line number
    lines->AddAt(new TObjString("   //If using kinematic bins find the correct Entry list"),place+1); 
    lines->AddAt(new TObjString("   SetEventEntryList(beam->P4().fE,t); //correspond to varibles in the NNKinBinsHis histogram "),place+2); 
    lines->AddAt(new TObjString("   MakeNNMap(); //EventWeighter make map of nearest neighbours for this event"),place+3); 
    lines->AddAt(new TObjString("    FillNNEvTree(entry); //EventWeighter fill tree for fitting"),place+4); 
    lines->AddAt(new TObjString("    FillDiscVar(entry);//get the discriminatory variable value for thsi event"),place+5); 

    lines->AddAt(new TObjString("    RunRooFit();  //EventWeighter do the fit"),place+6); 
    //SlaveTerminate
    obj=macro.GetLineWith( "THSOutput::HSSlaveTerminate();");
    place=lines->IndexOf(obj); //get line number
    lines->AddAt(new TObjString("    if(fIsSaveNN) SaveNNTree(fFile,fOutName);//EventWeighter save nearest neighbour trees"),place); 
    
    //virtual functions that must be define by user, added at end of file
    lines->Add(new TObjString(TString("void ")+SelName+"::GetNNBranches(Long64_t nni){"));
    lines->Add(new TObjString(" //EventWeighter function to get the branches needed for the nearest neighbour search and fit"));
    lines->Add(new TObjString(" //e.g. b_beam->GetEntry(nni);"));
    lines->Add(new TObjString("}"));

    lines->Add(new TObjString(TString("void ")+SelName+"::FillCoord(TVectorD &coordV){//define the variables used in the distance calculation"));
    lines->Add(new TObjString(" //There should be fNcoord entries"));
    lines->Add(new TObjString(" //e.g. coordV[0]=beam->P4().E();"));
    lines->Add(new TObjString(" //e.g. coordV[1]=t;"));
    lines->Add(new TObjString("}"));

    lines->Add(new TObjString(TString("void ")+SelName+"::FillDiscVar(Long64_t nni){//define the variables used in the distance calculationsignal/background fit"));
    lines->Add(new TObjString(" //There should be fNdisc entries"));
    lines->Add(new TObjString(" //e.g. b_miss->GetEntry(nni);"));
    lines->Add(new TObjString(" //e.g. fDiscVar[0]=(miss->M());//missing mass"));
    lines->Add(new TObjString("}"));

    lines->Add(new TObjString(TString("void ")+SelName+"::CreateNNTree(){// create the tree fNNEvTree used in the RooFit"));
    lines->Add(new TObjString(" //e.g. fNNEvTree=new TTree(\"NN tree\",\"tree with NN fit variables\");"));
    lines->Add(new TObjString(" //e.g. fNNEvTree->Branch(\"MM\",&fMM,\"MM/F\");"));
    lines->Add(new TObjString("}"));

    lines->Add(new TObjString(TString("void ")+SelName+"::FillNNEvBranches(Long64_t id){//define how the branches in the NN tree match the discriminatory variables vector"));
    lines->Add(new TObjString(" //e.g.  fMM=fVdisVar[id][0];//as fDiscVar[0]=(miss->M());"));
    lines->Add(new TObjString("}"));

    lines->Add(new TObjString(TString("void ")+SelName+"::SetMetric(){//define the metric use to scale the variables in the distance calculation"));
    lines->Add(new TObjString(" //There should be fNcoord rows and columns"));
    lines->Add(new TObjString(" //e.g. fIsDiagonal=kTRUE;"));
    lines->Add(new TObjString(" //e.g. Dmetric[0][0]=1;"));
    lines->Add(new TObjString(" //e.g. Dmetric[1][1]=(1./2.)*(1./2.);"));
    lines->Add(new TObjString(" THSEventWeighter::SetMetric();"));
    lines->Add(new TObjString("}"));
    lines->Add(new TObjString(""));
  }
  macro.SaveSource(SelName+".C");
  
}
void HSit_h(){
  TMacro macro(SelName+".h");

  ////////////////////////////////////////////////////
  //First deal with lines that need placed after marker lines
  //Make list of marker lines in TSelector skeleton
  TList* markers=new TList();
    //Make list of lines that need to be added to skeleton TSelector code
  TList* hslines=new TList();
  markers->Add(new TObjString("#include <TSelector.h>"));
  hslines->Add(new TObjString("#include \"THSOutput.h\""));

  if(IsHisto){//lines only needed if histogramming
    markers->Add(new TObjString("   virtual void    Terminate();"));
    hslines->Add(new TObjString("//Add THSHisto functions"));
    markers->Add(hslines->Last());//adding in more than one line
    hslines->Add(new TObjString("   virtual void HistogramList(TString sLabel);"));
    markers->Add(hslines->Last());//adding in more than one line
    hslines->Add(new TObjString("   virtual void FillHistograms(TString sCut,Int_t bin);"));
  }

  markers->Add(new TObjString("// user if needed. The return value is currently not used."));
  hslines->Add(new TObjString("   THSOutput::HSNotify(fChain);"));



  //Get list of line in TSelector skeleton
  TList *lines=macro.GetListOfLines();
  TObject* obj=0;
  //loop over new lines
  Int_t i=0;
  while(markers->At(i)){
    //   obj=lines->FindObject(markers->At(i)); //find the marker
    obj=macro.GetLineWith(((TObjString*)markers->At(i))->GetName()); //find the marker
    lines->AddAfter(obj,hslines->At(i++)); //add the extraline
  }
  //Done line to be placed after
  //////////////////////////////////////////////////
  /////////////////////////////////////////////////
  //Now lines that need replaced
  markers->Clear();
  hslines->Clear();

  TString smarker="public TSelector {";
  TString sline=macro.GetLineWith(smarker)->GetString();
  sline.ReplaceAll(smarker,"public TSelector, public THSOutput {");
  macro.GetLineWith(smarker)->SetString(sline);
  ////////////////////Done lines that need replaced
  ///////////////////////////////////
  //Any extras
  if(IsAppendTree){
     if(IsNewTree){cout<<"Can't have a new tree and an append tree!"<<endl;exit(0);}
   //we want to add some branches to the input tree
    //put in the lines needed in notify (we need fChain)
    obj=macro.GetLineWith( "THSOutput::HSNotify(fChain);");
    Int_t place=lines->IndexOf(obj); //get line number
    lines->AddAt(new TObjString("  //If we want to clone the input tree we have to do it here as fChain does not exist in SlaveBegin on PROOF"),place+1); 
    lines->AddAt(new TObjString("  if(!fOutTree){//First file, clone the input tree and add branches"),place+2); 
    lines->AddAt(new TObjString("   Int_t buff=32000;"),place+3); 
    lines->AddAt(new TObjString("   Int_t split=0;//note split is important in the TSelector framework, if increased branches in subsequent selectors will be data members of the THSParticle object rather than the whole object (this can lead to name conflicts)
"),place+4); 
    lines->AddAt(new TObjString("    fOutTree=fChain->CloneTree(0);"),place+5);
    lines->AddAt(new TObjString("    //e.g. fp1=new TLorentzVector(); //should be declared as class data member"),place+6);
 
    lines->AddAt(new TObjString("    //e.g. fOutTree->Branch(\"p1\",&fp1,buff,split);"),place+7); 
    lines->AddAt(new TObjString("  }"),place+8); 
    lines->AddAt(new TObjString("  else fChain->CopyAddresses(fOutTree);//reset the branch addresses of the cloned tree to the new file tree"),place+9); 
    
    //Put in an example data member
    obj=macro.GetLineWith( "// List of branches");
    place=lines->IndexOf(obj); //get line number
    lines->AddAt(new TObjString("  //data members for new branches"),place-1); 
    lines->AddAt(new TObjString("  //you must define how they are processed for each event"),place); 
    lines->AddAt(new TObjString("  //e.g.   TLorentzVector  *fp1;"),place+1); 

  }
 if(IsNewTree){
     //Put in an example data member
    obj=macro.GetLineWith( "// List of branches");
    Int_t place=lines->IndexOf(obj); //get line number
    lines->AddAt(new TObjString("  //data members for new branches"),place-1); 
    lines->AddAt(new TObjString("  //you must define how they are processed for each event"),place); 
    lines->AddAt(new TObjString("  //e.g.   TLorentzVector  *fp1;"),place+1); 


 }
 if(IsQval){
   //headre file
   obj=macro.GetLineWith( "#include <TSelector.h>");
   Int_t place=lines->IndexOf(obj); //get line number
   lines->AddAt(new TObjString("#include \"THSEventWeighter.h\""),place+1); 
   //class inheritance
   TString smarker="public TSelector";
   TString sline=macro.GetLineWith(smarker)->GetString();
   sline.ReplaceAll(smarker,"public TSelector, public THSEventWeighter ");
   macro.GetLineWith(smarker)->SetString(sline);
   //Notify
   obj=macro.GetLineWith( "::Notify()");
   place=lines->IndexOf(obj); //get line number
   lines->AddAt(new TObjString("    if(fIsSaveNN) SaveNNTree(fFile,fOutName);//EventWeighter save nearest neighbour trees"),place+2); 
   lines->AddAt(new TObjString("  if(fIsLoadNN) NotifyNNTree(fChain);//EventWeighter load nearest neighbour trees, get the entry offset for this tree fChain"),place+3); 

   obj=macro.GetLineWith( "THSOutput::HSNotify(fChain)");
   place=lines->IndexOf(obj); //get line number
   lines->AddAt(new TObjString("  //if it exists give the tree to the file"),place+1); 
   lines->AddAt(new TObjString("  if(fIsSaveNN){"),place+2); 
   lines->AddAt(new TObjString("     fTofT->SetDirectory(fFile);"),place+3); 
   lines->AddAt(new TObjString("     fTofT->AutoSave();"),place+4); 
   lines->AddAt(new TObjString("  }"),place+5); 

   if(IsAppendTree){//Automiatically add Qval, SigmaMean etc to appened tree
     obj=macro.GetLineWith( "//e.g. fOutTree->Branch(\"p1\",&fp1,buff,split);");
     place=lines->IndexOf(obj); //get line number
     lines->AddAt(new TObjString("    //EventWeighter append branches"),place+1); 
     lines->AddAt(new TObjString("    fOutTree->Branch(\"Qval\",&fQval,\"Qval/F\");"),place+2); 
     lines->AddAt(new TObjString("    fOutTree->Branch(\"SigWidth\",&fSigWidth,\"SigWidth/F\");"),place+3); 
     lines->AddAt(new TObjString("    fOutTree->Branch(\"SigMean\",&fSigMean,\"SigMean/F\");"),place+4); 
     lines->AddAt(new TObjString("    fOutTree->Branch(\"SB\",&fSB,\"SB/F\");"),place+5); 
     
     
   }
   //decalre functions users must define
   obj=macro.GetLineWith( "virtual void    Terminate();");
   place=lines->IndexOf(obj); //get line number
   lines->AddAt(new TObjString("   //Event Weighter functions to be defined by user"),place+1);
   lines->AddAt(new TObjString("   virtual void FillCoord(TVectorD &coordV); //Define how coordinates are to be filled"),place+2);
   lines->AddAt(new TObjString("   virtual void FillDiscVar(Long64_t nni); //define discriminatory variables"),place+3);
   lines->AddAt(new TObjString("   virtual void CreateNNTree();// create the tree fNNEvTree used in the RooFit"),place+4);
   lines->AddAt(new TObjString("   virtual void FillNNEvBranches(Long64_t id); //define how to fill the NN tree branches"),place+5);
   lines->AddAt(new TObjString("   virtual void GetNNBranches(Long64_t nni); //get the branches needed to fill coordinates and discriminatry variables"),place+6);
   lines->AddAt(new TObjString("   virtual void SetMetric();  //Define the metric used to normalise distance variables;"),place+7);

}


  macro.SaveSource(SelName+".h");
}
void ConnectParent(){
  //Add a parent tree selector to this selector
  //users will then be able to access branches and events from the parent
  TString sStep;
  TString addStep;
  sStep.Form("HSStep_%d",StepN);
  while(!File->Get(sStep))  {
    StepN++;
    addStep.Form("HSStep_%d/",StepN);
    sStep.Prepend(addStep);
    if(StepN>10){cout<< "step "<<sStep<<" not found yet, disbale this return line if the step really is greater than 10"<<endl; return;}
  }
  cout<<"Looking for macros in "<<sStep<<endl;
  //Look in the tree File for the step to be connected
  //Make copies of the seelctor files in the current directory
  TMacro* macro=(TMacro*) File->Get(sStep+"/"+ParSel+".C");
  if(!macro){ cout<<"Did not find " <<ParSel<<" in "<<sStep<<endl; return;}
  macro->SaveSource(macro->GetName()) ;
  macro=(TMacro*) File->Get(sStep+"/"+ParSel+".h");
  macro->SaveSource(macro->GetName());

  //now open .h file and add lines
  TMacro macroH(SelName+".h");
  TList *lines=macroH.GetListOfLines();

  //first include file
  TObject*  obj=macroH.GetLineWith("#include \"THSOutput.h\""); //find the marker
  TString sinc;
  sinc=TString("#include \"")+ParSel+".h\"";
  lines->AddAfter(obj,new TObjString(sinc)); //add the parent selector include
 
  //not the parent selector object
  obj=macroH.GetLineWith("TTree          *fChain;");  
  sinc=TString("   ")+ParSel+"   fParent;";
  lines->AddAfter(obj,new TObjString(sinc)); //add the parent selector include

  //Now lines to initialise the parent selector 
  obj=macroH.GetLineWith("THSOutput::HSNotify(fChain);"); 
  sinc="//initialise parent selector";
  Int_t place=lines->IndexOf(obj); //get line number
  sinc=TString("   THSOutput::InitParent(fChain,\"") + sStep+"\");";
  lines->AddAt(new TObjString(sinc),place+1); 
  lines->AddAt(new TObjString("   fParent.Init(fParentTree);"),place+2);


  macroH.SaveSource(TString(macroH.GetName())+".h");
  //now open .C file and add sample lines
  TMacro macroC(SelName+".C");
  TList *lines=macroC.GetListOfLines();
  obj=macroC.GetLineWith("THSOutput::HSProcessStart(entry);");
  place=lines->IndexOf(obj); //get line number
  lines->AddAt(new TObjString("//this selector has been configured to use a parent selector"),place+1);
  lines->AddAt(new TObjString("//get the branch with something like fParent.b_beam->GetEntry(entry)"),place+2);
lines->AddAt(new TObjString("//then use the object like fParent.beam->P4().E();"),place+3);

  obj=macroC.GetLineWith("GetEntry(entry);");
  place=lines->IndexOf(obj); //get line number
  lines->AddAt(new TObjString("   fParent.GetEntry(GetParentEntry(entry)); //lazy and slow, optimise your own analysis by only getting the branches you need"),place+1);

  macroC.SaveSource(TString(macroC.GetName())+".C");

 
}
