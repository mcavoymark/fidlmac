/* 9/20/22 fidl_vox_ass.cxx */

#include <iostream>
#include <fstream>
#include <sstream>

//#include <unordered_map>
#include <map>

//#include <string>

#include "file.h" 
#include "stack.h"
#include "subs_nifti.h"

int help0(){
    std::cout<<"Assign new values to voxels."<<std::endl;
    std::cout<<"  -f --file  .4dfp.img's, .nii's, .nii.gz's, conc(s) or list(s)."<<std::endl;
    std::cout<<"  -d --dat   First column is the current value, second column in the new value. The rest are assigned zero."<<std::endl;
    return 1;
    }

class dat{
    public:
        //dat();
        //virtual ~dat(){};

        //std::unordered_map<int,int> DAT;
        std::map<int,int> DAT;

        int readdat(char* file);
    };
int dat::readdat(char* file){
    std::ifstream ifile(file);
    if(!ifile.is_open()){
        std::cout<<"fidlError: Cannot open "<<file<<std::endl;
        return 0;
        }
    std::string str;
    int cur,nex;
    while(getline(ifile,str)){
        if(str.empty() || !str.find("#"))continue;
        std::istringstream iss(str);
        iss>>cur>>nex;
        DAT[cur]=nex;
        }
    return 1;
    }



int main(int argc,char **argv)
{
if(argc<5){
    help0();
    exit(-1);
    }
int nfile=0;

//Files_Struct *file=NULL;
//START221029
file f0;
//dat d0;

//START221104
char* datf=NULL;

for(int i=1;i<argc;i++){
    //std::cout<<"here0 argv["<<i<<"]="<<argv[i]<<std::endl;
    if(!strcmp(argv[i],"-h")||!strcmp(argv[i],"-help")){
        help0();
        exit(-1);
        }

    if((!strcmp(argv[i],"-f")||!strcmp(argv[i],"-file")) && argc > i+1){
        //std::cout<<"here1"<<std::endl;
        for(int j=1; i+j < argc && strchr(argv[i+j],'-') != argv[i+j]; j++)++nfile;

        //if(!(file=read_files(nfile,&argv[i+1])))exit(-1);
        //START221029
        if(!(f0.read_files(nfile,&argv[i+1])))exit(-1);
        i+=nfile;
        }

    //START221104
    if((!strcmp(argv[i],"-d")||!strcmp(argv[i],"-dat")) && argc > i+1){
        datf=argv[++i];
        //d0.readdat(argv[++i]);
        }
    }

//START221104
if(!nfile){
    std::cout<<"fidlError: Need to provide -f --file  .4dfp.img's, .nii's, .nii.gz's, conc(s) or list(s)."<<std::endl;
    exit(-1);
    }
if(!datf){
    std::cout<<"fidlError: Need to provide -d --dat   First column is the current value, second column in the new value. The rest are assigned zero."<<std::endl;
    exit(-1);
    }


dat d0;
if(!d0.readdat(datf)){
    std::cout<<"fidlError: unable to read "<<datf<<std::endl;
    exit(-1);
    }
//std::cout<<"datf="<<datf<<std::endl;
std::cout<<"datf="<<datf<<"\t"<<d0.DAT.size()<<std::endl;
for(auto it=d0.DAT.begin();it!=d0.DAT.end();++it){
    std::cout<<it->first<<"\t"<<it->second<<std::endl;
    }

std::cout<<"d0.DAT.begin()->first="<<d0.DAT.begin()->first<<std::endl;
//std::cout<<"--d0.DAT.end()->first="<<--d0.DAT.end()->first<<std::endl;
std::cout<<"d0.DAT.rbegin()->first="<<d0.DAT.rbegin()->first<<std::endl;


std::cout<<"f0.files.size()="<<f0.files.size()<<std::endl;

#if 0
for(int i=0;i<f0.files.size();i++){
    std::cout<<"f0.files["<<i<<"]="<<f0.files[i]<<std::endl;
    }
#endif
float* tf;
for(auto it=f0.files.begin();it!=f0.files.end();++it){
    std::cout<<*it<<std::endl;
    stack s0;
    //if(!(tf=s0.stack0((char*)*it)))exit(-1);
    //if(!(tf=s0.stack0((char*)*it.c_str())))exit(-1);
    //if(!(tf=s0.stack0((char*)*it->c_str())))exit(-1);
    if(!(tf=s0.stack0((char*)it->c_str())))exit(-1);

    std::cout<<"s0.vol="<<s0.vol<<std::endl;

    //for(int i=0;i<s0.vol;++i)std::cout<<tf[i]<<" ";std::cout<<std::endl;

    std::vector<float> vf(s0.vol);
    for(int i=0;i<s0.vol;++i){
        if(tf[i]<d0.DAT.begin()->first||tf[i]>d0.DAT.rbegin()->first)continue;
        std::cout<<tf[i]<<" ";
        for(auto it=d0.DAT.begin();it!=d0.DAT.end();++it){
            if(tf[i]==it->first){
                vf[i]=it->second;
                break;
                }
            }
        }
    std::cout<<std::endl;

    std::string str(*it);
    str=str.substr(str.find_last_of("/")+1,str.find_last_of(".4dfp.img")-str.find_last_of("/")-9)+"_reassigned.nii";
    std::cout<<"str="<<str<<std::endl;

    std::cout<<"s0.dim64="<<s0.dim64[0]<<" "<<s0.dim64[1]<<" "<<s0.dim64[2]<<" "<<s0.dim64[3]<<std::endl;
    std::cout<<"s0.dim="<<s0.dim[0]<<" "<<s0.dim[1]<<" "<<s0.dim[2]<<" "<<s0.dim[3]<<std::endl;

    //if(!nifti_write((char*)str.c_str(),s0.dim64,s0.centerf,s0.mmppixf,&vf[0]))exit(-1);
    if(!nifti_write((char*)str.c_str(),s0.dim64,s0.centerf,s0.mmppixf,vf.data()))exit(-1);
    std::string str2="gzip -f "+str;
    if(system(str2.c_str())==-1)std::cout<<"fidlError: "<<str2<<std::endl;else std::cout<<"Output written to "<<str+".gz"<<std::endl;

#if 0
        std::string str(petf->files[i]);
        //str=pwd+"/"+str.substr(str.find_last_of("/")+1,str.find_last_of(".nii.gz")-str.find_last_of("/")-7)+"_"+outstr+".nii";
        str=str.substr(str.find_last_of("/")+1,str.find_last_of(".nii.gz")-str.find_last_of("/")-7)+"_"+outstr+".nii";

        if(!nifti_write((char*)str.c_str(),dims,centerf,mmppixf,&tf[0]))exit(-1);
        std::string str2="gzip -f "+str;
        if(system(str2.c_str())==-1)std::cout<<"fidlError: "<<str2<<std::endl;else std::cout<<"Output written to "<<str+".gz"<<std::endl;
#endif


    }

}
