#include<iostream>
#include<string>
#include<fstream>
#define key "kolinko"
#define engal 26
using namespace std;
typedef unsigned short int usi;

bool gff(string& ifn,string& dst/*,string& key*/){
	ifstream fpi; ofstream fpo;
    fpi.open(ifn.c_str(),ios::binary);
    if(!fpi.is_open()) {cout<<"\nerr: gff no input file"; return 0;}
    else{
    	fpo.open(dst.c_str(),ios::binary);
    	if(!fpo.is_open()) {cout<<"\nerr: gff no input file"; return 0;}
    	else{
    		unsigned char ch='\0'; unsigned long ulcnt=0; bool uc=0;
			while(1){
				fpi>>noskipws>>ch;
				if(fpi.eof()) break;
				if(((usi)ch<'A')||((usi)ch>'z')||((usi)ch>'Z'&&(usi)ch<'a'))//skip letters that is not in alphabet;
	    			fpo<<ch;
	    		else{
	    			if(isupper(ch)){uc=1; ch=tolower(ch);}
					else uc=0;
	    			unsigned char tmpch=(ch+key[ulcnt%(sizeof(key)-1)]-2*'a')%engal+'a';
	    			/*cout<<"\nch+key[ulcnt%(sizeof(key)-1)]-2*'a')%engal+'a'="<<
	    			(unsigned char)ch<<'+'<<key[ulcnt%(sizeof(key)-1)]<<"-2*a%"<<engal<<"+a=\n"<<
	    			(usi)ch<<'+'<<(usi)key[ulcnt%(sizeof(key)-1)]<<"-2*"<<(usi)'a'<<'%'<<engal<<'+'<<(usi)'a'<<"=\n"<<
	    			(usi)((ch+key[ulcnt%(sizeof(key)-1)]-2*'a')%engal+'a')<<'='<<
					(unsigned char)((ch+key[ulcnt%(sizeof(key)-1)]-2*'a')%engal+'a');*/
	    			if(uc) tmpch=toupper(tmpch);
	    			fpo<<tmpch;
	    			ulcnt++;
	    		}
			}
	     fpi.close(); fpo.close();
		}    
    }
    return 1;
}

int main(){
	string srs="q.txt",dst="qoup.txt";
	//string key;	cout<<"enter key\n"; cin>>key; 
	cout<<"\nkey="<<key;//<<"\nfdt="<<fdt;
	gff(srs,dst/*,key*/);
	return 0;
}
