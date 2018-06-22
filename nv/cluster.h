#pragma once 

#include<iostream>
#include<string>
#include<vector>
#include"dataset.h"
using namespace std;

class Cluster{

public:
    explicit Cluster(){}
    ~Cluster(){}

    int load_file_txt(const string& filelist);
    int normalization();
    int predict();
    int get_lables();
  

    vector<vector<float> > feat_total;
    
private:

};