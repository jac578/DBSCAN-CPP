#include"nv/cluster.h"
#include "dbscan_vp_cosine.h"
#include<string>
#include<iostream>
#include "glog/logging.h"

using namespace clustering;

int main(){

    const string filelist="../tests/txt/feat_filelist.txt";
    const float eps=0.5;
    const int num_pts=2;
    Dataset::Ptr dset = Dataset::create();
    Cluster cluster;
    cluster.load_file_txt(filelist);
    dset->load_features(cluster.feat_total);
    dset->L2_normalize();
    const int print_len = (int) dset->rows();
    
    DBSCAN_VP_COSINE::Ptr dbs = boost::make_shared<DBSCAN_VP_COSINE>(dset);
    dbs->fit();
    int numcl = dbs->predict(eps, num_pts);
    LOG(INFO) << "CPU  cluster=" << numcl << ", fit time=" << dbs->get_fit_time()
                << "s, predict time=" << dbs->get_predict_time()<<"s.\n";

    const DBSCAN_VP_COSINE::Labels &l2 = dbs->get_labels();

    for (size_t i = 0; i < print_len; ++i){
        LOG(INFO) << "Element = " << i << " cluster = " << l2[i];
    }
    
    return 0;

}
