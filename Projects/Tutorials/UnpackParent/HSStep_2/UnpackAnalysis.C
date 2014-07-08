//Execute this macro to unpack the source code used to analyse this data file, it needs a directory to write to as an arguement
//Usage : root data.root
//        root [0] UnpackAnalysis(_file0,destination_dir)

void UnpackAnalysis(TDirectory *dir, TString BaseDir){
  TDirectory *dirsav = gDirectory;
  TIter next(dir->GetListOfKeys());
  TKey *key;
  //Note code will wait on the next line of directory name is not good, i.e. it already exists, or its parent doesn't, difficult to ouput this message to screen
  while(gSystem->MakeDirectory(BaseDir)!=0){cout<<"This is not an appropriate directory, it either exists already or its parent does not"<<endl;}
  while ((key = (TKey*)next())) {
    if (key->GetClassName()==TString("TDirectoryFile")) {
      cout<<key->GetName()<<endl;
      //Make the directory in real file system
      dir->cd(key->GetName());
      TDirectory *subdir = gDirectory;
      UnpackAnalysis(subdir,BaseDir+"/"+key->GetName());
      dirsav->cd();
      continue;
    }
    if(key->GetClassName()==TString("TMacro")){
      TMacro *macro = (TMacro*)key->ReadObj();
      cout<<"  "<<macro->GetName()<<endl;
      macro->SaveSource(BaseDir+"/"+macro->GetName());
      delete macro;
    }
  }
}
