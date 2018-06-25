#include"nv/cluster.h"
// #include "dbscan_vp_cosine.h"
#include<string>
#include<iostream>
#include<dirent.h>
#include<unistd.h>
#include<fstream>
#include <sys/stat.h>
using namespace clustering;
template<class T>
string int2str(const T value){
    string str;
    stringstream ss;
    ss<<value;
    ss>>str;
    return str;
}


template<class T>
T str2num(const string& str_value){
    T value;
    stringstream ss;
    ss<<str_value;
    ss>>value;
    return value;
}

int main(int argc,char** argv){
    string filelist="../tests/feats_bin_v3/feat_filelist.txt"; // "../tests/feats_bin/feat_filelist.txt";
    string imagelist="../tests/imgs_aligned_112x112/img_filelist.txt";
    string root="./cluster_result/";
    float eps=0.5;
    int num_pts=2;
    if (argc>2){
    for (int i=2;  i<argc; i+=2)
    {
      cout << "argv[" << i << "]: " << argv[i]  << endl;
      string t_str(argv[i]);
      if (t_str=="--feat-list")
      {
        filelist = argv[i+1];
      }
      else if (t_str=="--image-list")
       {
        imagelist = argv[i+1];
       }
      else if (t_str=="--save-root")
       {
        root = argv[i+1];
       }
      else if (t_str=="--eps")
       {
        eps = str2num<float>(argv[i+1]);
       }
      else if (t_str=="--num_pts")
       {
        num_pts = str2num<int>(argv[i+1]);
       }
    }
  }
 
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
    DIR *dir;   
    if((dir=opendir(root.c_str())) == NULL){   
        mkdir(root.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);      //system( s.c_str() );
    }   
    ifstream fin(imagelist);
    string str_line="";
    int num=-1;
    string path("");
    while(getline(fin,str_line)){
        if(++num==0){
            const int index1=str_line.rfind("/");
            const int index2=str_line.substr(0,index1).rfind("/");
	    const string substr=str_line.substr(0,index1);
             const string folder=substr.substr(substr.rfind("/")+1,substr.size());
	    cout<<"substr="<<str_line.substr(0,index1)<<"\n";
	    cout<<"floder="<<folder<<"\n";
            cout<<"index1="<<index1<<",index2="<<index2<<"\n";       
            path=root+folder;
            if((dir=opendir(path.c_str())) == NULL){   
                mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); 
            } 
            for( int i=0;i<labels.size(); ++i){
                string cluster_num_folder("");
                cluster_num_folder=path+"/"+int2str(labels[i]);
                if((dir=opendir(cluster_num_folder.c_str())) == NULL){   
                    mkdir(cluster_num_folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); 
                } 
            } 
        }
         // copy images to the folder
         const string str_copy_op="cp -r " + str_line + " "+ path + "/" + int2str(labels[num]);
         cout<<"copy op:"<<str_copy_op<<"\n";
         cout<<"num="<<num<<"\n" ;
 	 system( str_copy_op.c_str() );
   
    }
    fin.close();

    return 0;

}
