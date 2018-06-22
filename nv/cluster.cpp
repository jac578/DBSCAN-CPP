#include"cluster.h"
#include<fstream>

template <class Type>  
Type stringToNum(const string& str)  
{  
    istringstream iss(str);  
    Type num;  
    iss >> num;  
    return num;      
}  

char *strsep(char **stringp, const char *delim)  
{  
    //判断stringp是否为空,提高程序的容错性  
    char *s;  
    const char *spanp;  
    int c, sc;  
    char *tok;  
    if ((s = *stringp)== NULL)  
        return (NULL);  
    for (tok = s;;) {  
        c = *s++;  
        spanp = delim;  
        do {  
            if ((sc =*spanp++) == c) {  
                if (c == 0)  
                    s = NULL;  
                else  
                    s[-1] = 0;  
                *stringp = s;  
                return (tok);  
            }  
        } while (sc != 0);  
    }  
}  

void pushToVec(std::vector<float>&obj,const char *param,const std::string& token)  
{  
    char *p = (char*)param;  
    char *key_point;  
    while(p){  
        while (key_point = strsep(&p,token.c_str())){  
            if (*key_point == 0)  
                continue;  
            else  
                break;  
        }  
        string str_temp=key_point;
        const float temp=stringToNum<float>(str_temp);
        if (key_point)  
            obj.push_back(temp);  
    }  
}  

void Cluster::load_features(){

     dset->load_features(feat_total);
}


int Cluster::load_file_txt(const string& filelist){
    cout<<"file list="<<filelist<<"\n";
    ifstream fin(filelist);
    if(!fin){
        cout<<filelist<<" not exist,exit...\n";
        return  -1;
    }
    string str_name("");
    const string token(" ");
    while(getline(fin,str_name)){  
        ifstream fin2(str_name);
        if(!fin2){
           cout<<str_name<<" not exist,exit...\n";
           return -1;
        }
        string str_fin2_name("");
        vector<float> feat;
        while(getline(fin2,str_fin2_name)){
            feat.clear();
            pushToVec(feat,str_fin2_name.c_str(),token); 
            feat_total.push_back(feat);
        }
        fin2.close();
    }
    fin.close();
    load_features();

    return 0;
}

int Cluster::normalization(){
    dset->L2_normalize();
    fit();
    return 0;
}

void Cluster::fit(){
    dbs = boost::make_shared<DBSCAN_VP_COSINE>(dset);
    dbs->fit();
}

int Cluster::predict(){
    cout<<"eps="<<eps<<",num_pts="<<num_pts<<"\n";
    const int num=dbs->predict(eps, num_pts);
    return num;
}

int Cluster::get_labels(vector<int>& labels){
    const int print_len = (int) dset->rows();
    const DBSCAN_VP_COSINE::Labels &l2 = dbs->get_labels();
    for (size_t i = 0; i < print_len; ++i){
        labels.push_back((int)l2[i]);
    }
    return 0;

}
