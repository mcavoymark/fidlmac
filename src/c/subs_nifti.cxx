/* Copyright 7/14/15 Washington University.  All Rights Reserved.
   subs_nifti.cxx  $Revision: 1.9 $*/
//#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include "CiftiException.h"
#include "NiftiIO.h"
#include "VolumeSpace.h"
#include "subs_nifti.h"
#include <cstdio>
#include <iostream>
using namespace std;
using namespace cifti;
void* nifti_openRead(char* file){
    void* ret=NULL;
    NiftiIO* realPtr;
    try{
        ret=new NiftiIO();
        realPtr=(NiftiIO*)ret;
        realPtr->openRead(file);
        }
    catch(CiftiException& e){
        if(ret)delete (NiftiIO*)ret;
        cerr << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << endl;
        return NULL;
        }
    catch(exception& e){
        if(ret)delete (NiftiIO*)ret;
        cerr << "Caught exception: " << e.what() << endl;
        return NULL;
        }
    int nc;
    if((nc=realPtr->getNumComponents())!=1){
        cerr<<"fidlError: "<<file<<" getNumComponents()="<<nc<<" Only one is allowed."<<endl;
        if(ret)delete (NiftiIO*)ret;
        return NULL;
        }
    if(realPtr->getDimensions().size()<3||realPtr->getDimensions().size()>4){
        cerr<<"fidlError: "<<file<<" Not a 3D or 4D volume"<<endl;
        if(ret)delete (NiftiIO*)ret;
        return NULL;
        }
    return ret;
    }
int nifti_getvol(void* niftiPtr,int64_t t,float *stack){
    NiftiIO* realPtr=(NiftiIO*)niftiPtr;

    //cout << "nifti_getvol t=" << t << endl;

    vector<int64_t> mySelect(1,t);
    try{

        //realPtr->readData(stack,3,mySelect);
        //START170620
        realPtr->readData(stack,3,t>=0?mySelect:vector<int64_t>());

        }


    catch(CiftiException& e){
        cerr << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << endl;
        return 0;
        }
    catch(exception& e){
        cerr << "Caught exception: " << e.what() << endl;
        return 0;
        }
    return 1;
    }
void nifti_free(void* niftiPtr){
    NiftiIO* realPtr=(NiftiIO*)niftiPtr;
    if(realPtr)delete realPtr;
    }
int nifti_read(char *file,float *stack){
    try{
        NiftiIO inputFile;
        inputFile.openRead(file);
        if(inputFile.getDimensions().size()<3||inputFile.getDimensions().size()>4){
            cerr << "fidlError: "<<file<<" Not a 3D or 4D volume" << endl;
            return 0;
            }

        //cout << "nifti_read inputFile.getDimensions().size()=" << inputFile.getDimensions().size() << endl;
        //cout << "nifti_read vector<int64_t>()=" << vector<int64_t>() << endl;

        inputFile.readData(stack,inputFile.getDimensions().size(),vector<int64_t>());
        inputFile.close();
        } 
    catch (CiftiException& e) {
        cerr << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << endl;
        return 0;
        }
    return 1;
    }

//START190702
//int nifti_read(char *file,unsigned short int* stack){
int nifti_readus(char *file,unsigned short* stack){
    try{
        NiftiIO inputFile;
        inputFile.openRead(file);
        if(inputFile.getDimensions().size()<3||inputFile.getDimensions().size()>4){
            cerr << "fidlError: "<<file<<" Not a 3D or 4D volume" << endl;
            return 0;
            }
        inputFile.readData(stack,inputFile.getDimensions().size(),vector<int64_t>());
        inputFile.close();
        }
    catch (CiftiException& e) {
        cerr << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << endl;
        return 0;
        }
    return 1;
    }
int nifti_readsi(char *file,int* stack){
    try{
        NiftiIO inputFile;
        inputFile.openRead(file);
        if(inputFile.getDimensions().size()<3||inputFile.getDimensions().size()>4){
            cerr << "fidlError: "<<file<<" Not a 3D or 4D volume" << endl;
            return 0;
            }
        inputFile.readData(stack,inputFile.getDimensions().size(),vector<int64_t>());
        inputFile.close();
        }
    catch (CiftiException& e) {
        cerr << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << endl;
        return 0;
        }
    return 1;
    }

int nifti_write_setdim4to1(char *file,char *file_hdr,float *stack){
    try{
        NiftiIO inputFile;
        inputFile.openRead(file_hdr);

        vector<int64_t> dimsIn(inputFile.getDimensions().begin(),inputFile.getDimensions().end());
        dimsIn[3]=1;

        NiftiHeader superbird=inputFile.getHeader();

        superbird.setDimensions(dimsIn);
        int outVersion=!superbird.canWriteVersion(1)?2:1;
        NiftiIO myIO;
        myIO.writeNew(file,superbird,outVersion);
        myIO.writeData(stack,superbird.getDimensions().size(),vector<int64_t>());
        inputFile.close();
        }
    catch(CiftiException& e){
        cerr << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << endl;
        cout << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << endl;
        return 0;
        }
    catch(exception& e){
        cerr << "Caught exception: " << e.what() << endl;
        cout << "Caught exception: " << e.what() << endl;
        return 0;
        }
    return 1;
    }




int nifti_write3(char *file,char *file_hdr,float *stack,int64_t *dims,float sform13){
    try{
        NiftiIO inputFile;
        inputFile.openRead(file_hdr);
        const NiftiHeader& myHeader=inputFile.getHeader();
        vector<vector<float> > sform=myHeader.getSForm(); 
        
        cout<<"sform before"<<endl;
        for(size_t i=0;i<sform.size();++i){
            for(size_t j=0;j<sform[i].size();++j)cout<<sform[i][j]<<' ';
            cout<<endl;
            }
        sform[1][3]=sform13;
        cout<<"sform after"<<endl;
        for(size_t i=0;i<sform.size();++i){
            for(size_t j=0;j<sform[i].size();++j)cout<<sform[i][j]<<' ';
            cout<<endl;
            }

        NiftiHeader superbird;
        vector<int64_t> dimsIn(dims,dims+4); //in main, int64_t dims[4]={0,0,0,1}; 
        superbird.setDimensions(dimsIn);
        superbird.setSForm(sform);
        int outVersion=!superbird.canWriteVersion(1)?2:1;
        NiftiIO myIO;
        myIO.writeNew(file,superbird,outVersion); 
        myIO.writeData(stack,superbird.getDimensions().size(),vector<int64_t>());
        inputFile.close();
        }
    catch(CiftiException& e){
        cerr << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << endl;
        cout << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << endl;
        return 0;
        }
    catch(exception& e){
        cerr << "Caught exception: " << e.what() << endl;
        cout << "Caught exception: " << e.what() << endl;
        return 0;
        }
    return 1;
    }
int nifti_write2(char *file,char *file_hdr,float *stack){
    try{
        NiftiIO inputFile;
        inputFile.openRead(file_hdr);
        const NiftiHeader& myHeader=inputFile.getHeader();
        inputFile.close();
        int outVersion=!myHeader.canWriteVersion(1)?2:1;
        NiftiIO myIO;
        myIO.writeNew(file,myHeader,outVersion); 
        myIO.writeData(stack,myHeader.getDimensions().size(),vector<int64_t>());
        }
    catch (CiftiException& e){
        cerr << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << endl;
        cout << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << endl;
        return 0;
        }
    return 1;
    }

//START191001
int nifti_write2us(char *file,char *file_hdr,unsigned short *stack){
    try{
        NiftiIO inputFile;
        inputFile.openRead(file_hdr);
        const NiftiHeader& myHeader=inputFile.getHeader();
        inputFile.close();

        #if 0
        int outVersion=!myHeader.canWriteVersion(1)?2:1;
        NiftiIO myIO;
        myIO.writeNew(file,myHeader,outVersion);
        myIO.writeData(stack,myHeader.getDimensions().size(),vector<int64_t>());
        #endif
        //START191009
        NiftiHeader superbird;
        superbird.setDimensions(myHeader.getDimensions());
        superbird.setSForm(myHeader.getSForm());
        superbird.setDataType((int16_t)NIFTI_TYPE_UINT16);
        int outVersion=!superbird.canWriteVersion(1)?2:1;
        NiftiIO myIO;
        myIO.writeNew(file,superbird,outVersion);
        myIO.writeData(stack,superbird.getDimensions().size(),vector<int64_t>());

        }
    catch (CiftiException& e){
        cerr << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << endl;
        cout << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << endl;
        return 0;
        }
    return 1;
    }

#if 0
int nifti_write(char *file,int64_t *dims,float *center,float *mmppix,float *stack){
    try{
        NiftiHeader myHeader;
        vector<int64_t> dimsIn(dims,dims+4); //in main, int64_t dims[4]={0,0,0,1}; 
        myHeader.setDimensions(dimsIn);
        vector<vector<float> > sform(4,vector<float>(4,0.0f));
        sform[3][3]=1.0f;
        sform[0][0]=mmppix[0];sform[0][3]=center[0];
        sform[1][1]=mmppix[1];sform[1][3]=center[1];
        sform[2][2]=mmppix[2];sform[2][3]=center[2];

        //cout<<"sform"<<endl;
        //for(size_t i=0;i<sform.size();++i){
        //    for(size_t j=0;j<sform[i].size();++j)cout<<sform[i][j]<<' ';
        //    cout<<endl;
        //    }

        myHeader.setSForm(sform);
        int outVersion=!myHeader.canWriteVersion(1)?2:1;
        NiftiIO myIO;
        myIO.writeNew(file,myHeader,outVersion); 
        myIO.writeData(stack,myHeader.getDimensions().size(),vector<int64_t>());
        }
    catch(CiftiException& e){
        cerr << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << endl;
        return 0;
        }
    catch(exception& e){
        cerr << "Caught exception: " << e.what() << endl;
        return 0;
        }
    return 1;
    }
#endif
//START210412
int nifti_write(char *file,int64_t *dims,float *center,float *mmppix,float *stack){
    try{
        NiftiHeader myHeader;
        vector<int64_t> dimsIn(dims,dims+4); //in main, int64_t dims[4]={0,0,0,1}; 
        myHeader.setDimensions(dimsIn);
        if(center&&mmppix){
            vector<vector<float> > sform(4,vector<float>(4,0.0f));
            sform[3][3]=1.0f;
            sform[0][0]=mmppix[0];sform[0][3]=center[0];
            sform[1][1]=mmppix[1];sform[1][3]=center[1];
            sform[2][2]=mmppix[2];sform[2][3]=center[2];
            myHeader.setSForm(sform);
            }
        int outVersion=!myHeader.canWriteVersion(1)?2:1;
        NiftiIO myIO;
        myIO.writeNew(file,myHeader,outVersion);
        myIO.writeData(stack,myHeader.getDimensions().size(),vector<int64_t>());
        }
    catch(CiftiException& e){
        cerr << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << endl;
        return 0;
        }
    catch(exception& e){
        cerr << "Caught exception: " << e.what() << endl;
        return 0;
        }
    return 1;
    }



void* nifti_writeNew(char *file,int64_t *dims,float *center,float *mmppix){
    void* ret=NULL;
    NiftiIO* realPtr;
    try{
        NiftiHeader myHeader;

        //vector<int64_t> dimsIn(dims,dims+3);
        //START161214
        vector<int64_t> dimsIn(dims,dims+4); //in main, int64_t dims[4]={0,0,0,1};
        //START170302
        //vector<int64_t> dimsIn(dims,dims+(dims[3]>0?4:3));

        //std::cout<<"here200"<<std::endl;

        myHeader.setDimensions(dimsIn);

        //std::cout<<"here201"<<std::endl;

        vector<vector<float> > sform(4,vector<float>(4,0.0f));
        sform[3][3]=1.0f;
        sform[0][0]=mmppix[0];sform[0][3]=center[0];
        sform[1][1]=mmppix[1];sform[1][3]=center[1];
        sform[2][2]=mmppix[2];sform[2][3]=center[2];
        myHeader.setSForm(sform);
        int outVersion=!myHeader.canWriteVersion(1)?2:1;
        ret=new NiftiIO();
        realPtr=(NiftiIO*)ret;
        realPtr->writeNew(file,myHeader,outVersion);
        }
    catch(CiftiException& e){
        if(ret)delete (NiftiIO*)ret;
        std::cerr << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << std::endl;
        return NULL;
        }
    catch(exception& e){
        if(ret)delete (NiftiIO*)ret;
        std::cerr << "Caught exception: " << e.what() << std::endl;
        return NULL;
        }
    return ret;
    }
int nifti_putvol(void* niftiPtr,int64_t t,float *stack){
    NiftiIO* realPtr=(NiftiIO*)niftiPtr;
    vector<int64_t> mySelect(1,t);
    try{
        realPtr->writeData(stack,3,mySelect);
        }
    catch(CiftiException& e){
        cerr << "Caught CiftiException: " + AString_to_std_string(e.whatString()) << endl;
        return 0;
        }
    catch(exception& e){
        cerr << "Caught exception: " << e.what() << endl;
        return 0;
        }
    return 1;
    }
