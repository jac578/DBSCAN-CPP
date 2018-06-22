#pragma once 

#include<iostream>
#include<string>
#include<vector>
#include"dataset.h"
#include "dbscan_vp_cosine.h"

using namespace std;

class Cluster{

public:
    explicit Cluster(){}
    ~Cluster(){ dset = Dataset::create(); }

    int load_file_txt(const string& filelist);
    int normalization();
    int predict();
    int get_labels(vector<int>& labels);
  
  private:
    vector<vector<float> > feat_total;
    Dataset::Ptr dset;
    DBSCAN_VP_COSINE::Ptr dbs;
    void load_features();
    void fit();
};