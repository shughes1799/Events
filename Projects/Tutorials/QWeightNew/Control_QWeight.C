{
  //Set up the input chain to be analysed
   TChain* tree=new TChain("HSParticles","datachain");
   tree->Add("/home/dglazier/Work/Research/HaSpect/data/g11pippippim_missn_HSID/inp1*root");
   //Make a chain of the previously saved  NN trees for quick Qweigthing
   //The input files should match the files in tree
   TChain* treeNN=new TChain("TreeOfNNTrees","datachain");
   treeNN->Add("/home/dglazier/Work/Research/HaSpect/data/g11pippippim_missn_HSID/Qval/inp1*root");
   //Set the ouput directory or file (if full file name is given just 1 file is created, while if a directory it will be created and filled with files of the same name as the input 
   gSystem->Setenv("HSOUT",
		   "/home/dglazier/Work/Research/HaSpect/data/g11pippippim_missn_HSID/QvalNew");
   
   //Configure the analysis (i.e PROOF, source compilation...)
   TString HSANA=gSystem->Getenv("HSANA");
   gROOT->LoadMacro(HSANA+"/HSControl.C");
   TProof* prf=HSControl(kTRUE); //where the proof stuff is set, kTRUE means use PROOF
   
   //Use an event list from a previous analysis 
   //If only 1 file was output the event list will be in its top directory, so just give the file name
   //If many files were ouput the event list will be in the same directory with the name ParentEventList.root
   //HSEntryList("/PATH TO PERVIOUS EVENTLIST/ParentEventList.root");

//EventWeight needs full chain to find nearest neighbours, so this additional code is necessary!
   if(prf){//proof looks after its own input list
     prf->AddInput(new TNamed("NNChainName",tree->GetName()));
     prf->AddInput(tree);
     //Give the chain of NN trees to the input list
     prf->AddInput(new TNamed("NNChainLoad",treeNN->GetName()));
     prf->AddInput(treeNN);
    tree->Process("QWeight.C++");
   }
  else{ //we have to look after the input list
    gROOT->LoadMacro("QWeight.C++");
    QWeight* nnsel=new QWeight();
     TList* inList=new TList();
     inList->SetOwner();//let the list delete the objects you give it
     inList->Add(new TNamed("NNChainName",tree->GetName()));
     inList->Add(tree->Clone());
     //Give the chain of NN trees to the input list
     inList->Add(new TNamed("NNChainLoad",treeNN->GetName()));
     inList->Add(treeNN);
     nnsel->SetInputList(inList);
     Long64_t first=0;
     tree->LoadTree(first); //This is done in TChain::Process(filename) but not Process(selector)!! 
     tree->Process(nnsel,"");
     delete inList;
   }

   //Not actually sure if this is needed or not...
   if(prf)prf->ClearData(TProof::kUnregistered | TProof::kForceClear);
 }
