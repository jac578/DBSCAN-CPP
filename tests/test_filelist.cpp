#include"nv/cluster.h"
// #include "dbscan_vp_cosine.h"
#include<string>
#include<iostream>
#include<direct.h>
#include<unistd.h>

using namespace clustering;
<template T>
string int2str(const T value){
    string str;
    stringstream ss;
    ss<<value;
    ss>>str;
    return str;
}

int main(){
    const string filelist="../tests/feats_bin_v3/feat_filelist.txt"; // "../tests/feats_bin/feat_filelist.txt";
    const string imagelist="../tests/imgs_aligned_112x112/img_filelist.txt"
    const float eps=0.5;
    const int num_pts=2;
    vector<int> labels;
    // Dataset::Ptr dset = Dataset::create();
    Cluster cluster(eps,num_pts);
    cluster.load_file_bin(filelist);
    cluster.normalization();
    cluster.predict();
    cluster.get_labels(labels);
    for(int i=0;i<labels.size();++i){
        std::cout << "Element = " << i << ", cluster = " << labels[i]<<"\n";
    }
    // image clustering
    const string root="./cluster_result/";
    DIR *dir;   
    if((dir=opendir(root.c_str())) == NULL){   
        _mkdir(root);      //system( s.c_str() );
    }   
    fstrem fin(imagelist);
    string str_line="";
    int num=0;
    while(getline(fin,str_line)){
        if(num++==0){
            const int index1=str_line.rfind("/");
            const int index2=str_line.substr(0,index1).rfind("/");
            const string folder=str_line.substr(index2,index1);
            cou<<"floder="<<folder<<"\n";
        
            cosnt string path=root+folder;
            if((dir=opendir(path.c_str())) == NULL){   
                mkdir(path); 
            } 
            for( int i=0;i<label.size(); ++i){
                const string cluster_num_folder("");
                cluster_num_folder=path+"/"+int2str(label[i]);
                if((dir=opendir(cluster_num_folder.c_str())) == NULL){   
                    mkdir(cluster_num_folder); 
                } 
            } 
        }
         // copy images to the folder
         const string str_copy_op="cp -r " + str_line + " "+ path + "/" + int2str(label[num]);
         cout<<"copy op:"<<str_copy_op<<"\n";
         system( str_copy_op.c_str() );
   
    }
    fin.close();

    return 0;

}
