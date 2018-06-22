#pragma once 

#include<iostream>
#include<string>
#include<vector>
#include"dataset.h"
#include "dbscan_vp_cosine.h"

using namespace std;
using namespace clustering;
class Cluster{

public:
    explicit Cluster(const float _eps,const int _num_pts):eps(_eps),num_pts(_num_pts) { dset = Dataset::create(); }
    ~Cluster(){ }

    int load_file_txt(const string& filelist);
    int normalization();
    int predict();
    int get_labels(vector<int>& labels);
  
  private:
    float eps;
    int num_pts;
    vector<vector<float> > feat_total;
    Dataset::Ptr dset;
    DBSCAN_VP_COSINE::Ptr dbs;
    void load_features();
    void fit();
};
