#if 0

/* 9/20/22 file.h */
#ifndef __FILE_H__
    #define __FILE_H__

    class file {
        public:
            //char **files;
            std::vector<std::string> files;

            char **identify;

            //int *nfileseach;
            std::vector<int> nfileseach;

            size_t nfiles,nstrings,max_length,*strlen_identify;

            //size_t *strlen_files;
            std::vector<size_t> strlen_files;

            file();
            ~file();
            int read_files(int nfiles,char **argv);
            int get_files(int num_region_files,char **argv);
            int init_FS(size_t tnfiles,int nfiles);
            int read_conc(int nfile,char **file);
        };
#endif

#endif

/* 9/20/22 file.cxx */

//START221029
#include <iostream>
#include "file.h"

//START221029
//file::file(){
//    }
//file::~file(){
//    }

int file::read_files(int nfiles,char **argv){
    //char *strptr;
    if(!nfiles){std::cout<<"fidlError: Need to provide *.list, *.conc, *.img, *.nii or *.nii.gz"<<std::endl;return 0;}
    //strptr=strrchr(argv[0],'.');
    char* strptr=strrchr(argv[0],'.');
    if(strptr) if(!strcmp(strptr,".conc")||!strcmp(strptr,".list")){
        //if(!(read_conc(nfiles,&argv[0])))return 0;
        //return 1;
        std::cout<<"fidlError: Need to code read_conc"<<std::endl;return 0;
        }

    //if(!(files=get_files(nfiles,&argv[0])))return 0;
    if(!get_files(nfiles,&argv[0]))return 0;

    return 1;
    }

int file::get_files(int nfiles0,char **argv)
{
    files.insert(files.end(),argv,argv+nfiles0);

    //nfiles = nfiles0;
    //nfileseach.push_back(nfiles0);
    //START221029
    nfileseach.push_back(nfiles0);

    return 1;
}

#if 0
std::ifstream ifile(csvf);
std::string str;
std::getline(ifile,str,'\r');
//std::cout<<str<<"END"<<std::endl;

std::istringstream iss(str);
std::vector<std::string> vstr;

//while(std::getline(iss,str,','))vstr.push_back(str);
while(std::getline(iss,str)){ //stackoverflow.com/questions/35639083/c-csv-line-with-commas-and-strings-within-double-quotes
    const char* mystart=str.c_str();
    bool instring{false};
    for(const char* p=mystart;*p;p++){
        if(*p=='"')
            instring=!instring;
        else if(*p==','&&!instring){
            vstr.push_back(std::string(mystart,p-mystart));
            mystart=p+1;
            }
        }
    vstr.push_back(std::string(mystart));
    }
#endif

