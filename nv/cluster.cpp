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
/* Conversion little endian float data to big endian  */
float ReverseFloat(const float inFloat)
{
    float retVal;
    char *floatToConvert = (char*) & inFloat;
    char *returnFloat = (char*) & retVal;

    // swap the bytes into a temporary buffer
    returnFloat[0] = floatToConvert[3];
    returnFloat[1] = floatToConvert[2];
    returnFloat[2] = floatToConvert[1];
    returnFloat[3] = floatToConvert[0];

    return retVal;
}

void Cluster::load_features(){

     dset->load_features(feat_total);
}

bool IsBigEndian()  
{  
    union NUM  
    {  
        int a;  
        char b;  
    }num;  
    num.a = 0x1234;  
    if( num.b == 0x12 )  
    {  
        return true;  
    }  
    return false;  
}

typedef union FLOAT_CONV
{
    float f;
    char c[4];
}float_conv;


float BLEndianFloat(float value)
{
    float_conv d1,d2;
    d1.f = value;
    d2.c[0] = d1.c[3];
    d2.c[1] = d1.c[2];
    d2.c[2] = d1.c[1];
    d2.c[3] = d1.c[0];
    return d2.f;
}

int Cluster::load_file_bin(const string& filelist,const int bigEndian){
    cout<<"file list="<<filelist<<"\n";
    ifstream fin(filelist);
    if(!fin){
        cout<<filelist<<" not exist,exit...\n";
        return  -1;
    }
    string str_name("");
    bool isBigendian=IsBigEndian();
    cout<<"IsBigEndian="<<isBigendian<<"\n";
    while(getline(fin,str_name)){  
        ifstream fin2(str_name,std::ios::binary);
        cout<<str_name<<"\n";
        if(!fin2){
           cout<<str_name<<" not exist,exit...\n";
           return -1;
        }
           // get size of file
        fin2.seekg (0,fin2.end);
        const long size = fin2.tellg();
        fin2.seekg (0);

        // allocate memory for file content
        char* buffer = new char[size];
        // read content of infile
        fin2.read (buffer,size);
       // cout<<"length="<<size<<"\n";
        const int feat_size=size / sizeof(float);
        float featTemp[feat_size];
        vector<float> feat;
        memcpy(featTemp,buffer,size);
        delete [] buffer;
        for(int i=0;i<feat_size;++i){
            float value=0.0;
            if(!isBigendian)
                value=BLEndianFloat(featTemp[i]);
            else 
                 value=featTemp[i];
            feat.push_back(value);
        }
        feat_total.push_back(feat);
        feat.clear();
	fin2.close();
    }
    fin.close();
    load_features();

    return 0;
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
