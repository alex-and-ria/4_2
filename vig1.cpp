#include<iostream>
#include<string>
#include<fstream>
#define key "kolinko"
#define asciidlt 32
#define engal 26
using namespace std;
typedef unsigned short int usi;

bool gff(string& ifn,string& fdt){
	ifstream fpi;
    fpi.open(ifn.c_str(),ios::binary);
    if(!fpi.is_open()) {cout<<"\nerr: gff no input file"; return 0;}
    else{
    	unsigned char ch='\0';
		while(1){
			fpi>>noskipws>>ch;
			if(fpi.eof()) break;
			fdt+=ch;
		}
     fpi.close();    
    }
    return 1;
}

int main(){
	string srs="q.txt",dst="qoup.txt",fdt=""; gff(srs,fdt);
	//string key;	cout<<"enter key\n"; cin>>key; 
	cout<<"\nkey="<<key;//<<"\nfdt="<<fdt;
	ofstream fpo;
    fpo.open(dst.c_str(),ios::binary);
    if(!fpo.is_open()) {cout<<"\nerr: gff no input file"; return 0;}
    else{
    	unsigned long ulcnt=0; bool uc=0;
    	for(unsigned long int i=0;i<fdt.size();i++){
    		if(((usi)fdt[i]<'A')||((usi)fdt[i]>'z')||((usi)fdt[i]>'Z'&&(usi)fdt[i]<'a'))//skip letters that is not in alphabet;
    			fpo<<fdt[i];
    		else{
    			if(isupper(fdt[i])){uc=1; fdt[i]=tolower(fdt[i]);}
				else uc=0;
    			unsigned char tmpch=(fdt[i]+key[ulcnt%(sizeof(key)-1)]-2*'a')%engal+'a';
    			/*cout<<"\nfdt[i]+key[ulcnt%(sizeof(key)-1)]-2*'a')%engal+'a'="<<
    			(unsigned char)fdt[i]<<'+'<<key[ulcnt%(sizeof(key)-1)]<<"-2*a%"<<engal<<"+a=\n"<<
    			(usi)fdt[i]<<'+'<<(usi)key[ulcnt%(sizeof(key)-1)]<<"-2*"<<(usi)'a'<<'%'<<engal<<'+'<<(usi)'a'<<"=\n"<<
    			(usi)((fdt[i]+key[ulcnt%(sizeof(key)-1)]-2*'a')%engal+'a')<<'='<<
				(unsigned char)((fdt[i]+key[ulcnt%(sizeof(key)-1)]-2*'a')%engal+'a');*/
    			if(uc) tmpch=toupper(tmpch);
    			fpo<<tmpch;
    			ulcnt++;
    		}
		}
	}
	fpo.close();
	return 0;
}
