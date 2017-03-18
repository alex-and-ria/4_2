#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<cstdint>
#define bsz 8
using namespace std;
typedef unsigned short int usi;
typedef uint8_t ui8;

ui8 pc1[]{57,   49,    41   ,33   ,25    ,17    ,9,
1   ,58    ,50   ,42    ,34    ,26   ,18,
10    ,2    ,59   ,51    ,43    ,35   ,27,
19   ,11     ,3   ,60    ,52    ,44   ,36,
63,   55    ,47   ,39    ,31    ,23   ,15,
7   ,62    ,54   ,46    ,38    ,30   ,22,
14    ,6    ,61   ,53    ,45    ,37   ,29,
21   ,13     ,5   ,28    ,20    ,12    ,4
};

ui8 itrol[]{1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

ui8 pc2[]{14    ,17   ,11    ,24     ,1    ,5,
3    ,28   ,15     ,6    ,21   ,10,
23    ,19   ,12     ,4    ,26    ,8,
16     ,7   ,27    ,20    ,13    ,2,
41    ,52   ,31    ,37    ,47   ,55,
30    ,40   ,51    ,45    ,33   ,48,
44    ,49   ,39    ,56    ,34   ,53,
46    ,42   ,50    ,36    ,29   ,32
};

ui8 ip[]{58    ,50   ,42    ,34    ,26   ,18    ,10    ,2,
60    ,52   ,44    ,36    ,28   ,20    ,12    ,4,
62    ,54   ,46    ,38    ,30   ,22    ,14    ,6,
64    ,56   ,48    ,40    ,32   ,24    ,16    ,8,
57    ,49   ,41    ,33    ,25   ,17     ,9    ,1,
59    ,51   ,43    ,35    ,27   ,19    ,11    ,3,
61    ,53   ,45    ,37    ,29   ,21    ,13    ,5,
63    ,55   ,47    ,39    ,31   ,23    ,15    ,7
};

ui8 ep[]{32,    1,   2,     3,     4,    5,
4 ,   5 ,  6,    7,    8,   9,
8 ,   9 , 10,   11,  12,  13,
12,   13,  14,   15,   16,  17,
16,   17,  18,   19,   20,  21,
20,   21,  22,   23,   24,  25,
24,   25,  26,   27,   28,  29,
28,   29,  30,   31,   32,   1
};

struct b2b{
	unsigned char b0:1;
	unsigned char b1:1;
	unsigned char b2:1;
	unsigned char b3:1;
	unsigned char b4:1;
	unsigned char b5:1;
	unsigned char b6:1;
	unsigned char b7:1;
	ui8 getb(ui8 i){
		switch(i){
			case 0: return b0;
			case 1: return b1;
			case 2: return b2;
			case 3: return b3;
			case 4: return b4;
			case 5: return b5;
			case 6: return b6;
			case 7: return b7;
		}
	}
	void putb(ui8 i,ui8 j){
		switch(i){
			case 0: b0=j;
			case 1: b1=j;
			case 2: b2=j;
			case 3: b3=j;
			case 4: b4=j;
			case 5: b5=j;
			case 6: b6=j;
			case 7: b7=j;
		}
	}
};

union dtb{
	unsigned char ucarr[bsz];
	b2b lb[bsz/2];
	b2b rb[bsz/2];
	b2b barr[bsz];
	void getarr(vector<bool>& bvecl,vector<bool>& bvecr){
		for(ui8 k=0;k<bsz*8;k++){
			ui8 nbyte=k/8, nbit=k%8;
			if(k<(ui8)56/2)
				bvecl.push_back(barr[nbyte].getb(nbit));
			else if(k>=(ui8)56/2&&k<56)
				bvecr.push_back(barr[nbyte].getb(nbit));
		}
	}
	void getbarr(vector<bool>& bvecl,vector<bool>& bvecr){
		for(ui8 k=0;k<bsz*8;k++){
			ui8 nbyte=k/8, nbit=k%8;
			if(k<(ui8)56/2)
				barr[nbyte].putb(nbit,bvecl.at(k));
			else if(k>=(ui8)56/2&&k<56)
				barr[nbyte].putb(nbit,bvecr.at(k-(usi)56/2));
			else
				barr[nbyte].putb(nbit,0);
		}
	}
	void getarrd(vector<bool>& bvecl,vector<bool>& bvecr){
		for(ui8 k=0;k<bsz*8;k++){
			ui8 nbyte=k/8, nbit=k%8;
			if(k<(ui8)bsz*8/2)
				bvecl.push_back(barr[nbyte].getb(nbit));
			else if(k>=bsz*8/2)
				bvecr.push_back(barr[nbyte].getb(nbit));
		}
	}
	void getbarrd(vector<bool>& bvecl,vector<bool>& bvecr){
		for(ui8 k=0;k<bsz*8;k++){
			ui8 nbyte=k/8, nbit=k%8;
			if(k<(ui8)bsz*8/2)
				barr[nbyte].putb(nbit,bvecl.at(k));
			else if(k>=(ui8)bsz*8/2)
				barr[nbyte].putb(nbit,bvecr.at(k-(usi)bsz*8/2));
		}
	}
};

bool gff(string& ifn,dtb& fdt,bool& strt,ifstream& fpi){
	usi cnt=0;
	for(ui8 i=0;i<bsz;i++) fdt.ucarr[i]=0;
	if(strt==true){
		strt=false;
    	fpi.open(ifn.c_str(),ios::binary);
    	if(!fpi.is_open()) {cout<<"\nerr: gff no input file"; system("pause"); return 0;}
	}
	unsigned char ch='\0';
	while(1){
		fpi>>noskipws>>ch;
		if(fpi.eof()) break;
		fdt.ucarr[cnt]=ch; cnt++;
		//cout<<"\nch="<<ch<<" fdt.ucarr[cnt-1]="<<fdt.ucarr[cnt];
		if(cnt==bsz) return 0;
	}
	fpi.close();    
    return 1;
}

bool ptf(string& ofn,dtb& fdt,bool& strt,ofstream& fpo){
	if(strt==true){
    	fpo.open(ofn.c_str(),ios::binary);
    	if(!fpo.is_open()) {cout<<"\nerr:ptff no output file"; system("pause"); return 0;}
	}
	while(1){
		for(ui8 i=0;i<bsz;i++){
			//fpo<<' '; cout<<' '<<hex<<(usi)fdt.ucarr[i]<<' ';
			cout<<' ';
			for(ui8 j=0;j<8;j++){
				cout<<(usi)fdt.barr[i].getb(j);
			}
		}
		strt=false;
		if(strt==false) break;
	}
	fpo.close();    
    return 1;
}

int main(){
	string srs="q.txt",dst="q1.txt"; dtb fdt,key={19,52,87,121,155,188,223,241};
	bool strt=true,strt1=true; ifstream fpi; ofstream fpo; ui8 gffres;
	cout<<'\n';
	//do{
		gffres=gff(srs,fdt,strt,fpi);dtb apc1,apc2,aip;
		for(ui8 i=0;i<sizeof(pc1);i++){
			ui8 nbyte=i/8, nbit=i%8, nbyte1=(pc1[i]-1)/8,nbit1=(pc1[i]-1)%8;
			apc1.barr[nbyte].putb(nbit,fdt.barr[nbyte1].getb(7-nbit1));
		}
		for(ui8 i=0;i<sizeof(ip);i++){
			ui8 nbyte=i/8, nbit=i%8, nbyte1=(ip[i]-1)/8,nbit1=(ip[i]-1)%8;
			aip.barr[nbyte].putb(nbit,fdt.barr[nbyte1].getb(7-nbit1));
		}
		for(ui8 i=0;i<sizeof(itrol);i++){//round;
			for(ui8 j=0;j<itrol[i];j++){//num of shifts;
				vector<bool> bvecl,bvecr; apc1.getarr(bvecl,bvecr);
				/* cout<<'\n';
				for(ui8 l=0;l<bvecl.size();l++){
					cout<<bvecl.at(l);
				} cout<<'\t';
				for(ui8 l=0;l<bvecr.size();l++){
					cout<<bvecr.at(l);
				}
				cout<<'\n';*/ 
				ui8 u8tmpl=bvecl.at(0), u8tmpr=bvecr.at(0);
				for(ui8 k=0;k<bvecl.size()-1;k++){
					bvecl.at(k)=bvecl.at(k+1);
					bvecr.at(k)=bvecr.at(k+1);
				} 
				bvecl.at(bvecl.size()-1)=u8tmpl; bvecr.at(bvecr.size()-1)=u8tmpr;
				apc1.getbarr(bvecl,bvecr);
			}
			for(ui8 k=0;k<sizeof(pc2);k++){
				ui8 nbyte=k/8, nbit=k%8, nbyte1=(pc2[k]-1)/8,nbit1=(pc2[k]-1)%8;
				apc2.barr[nbyte].putb(nbit,apc1.barr[nbyte1].getb(nbit1));
			}//apc2 is round key; apc1 is Ci, Di;
			vector<bool> li,ri;	aip.getarrd(li,ri); cout<<'\n';
			/*for(ui8 j=0;j<li.size();j++) cout<<li.at(j); cout<<' ';
			for(ui8 j=0;j<li.size();j++) cout<<ri.at(j); cout<<'\n'; system("pause");*/
			
		}
		cout<<'\n';
	return 0;
}
