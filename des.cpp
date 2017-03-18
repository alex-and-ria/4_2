#include<iostream>
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
			for(ui8 j=0;j<8;j++){
				fpo<<(usi)fdt.barr[i].getb(j);
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
	//ptf(dst,key,strt1,fpo);
	cout<<'\n';
	//do{
		/*gffres=gff(srs,fdt,strt,fpi);
		for(ui8 i=0;i<bsz;i++) cout<<hex<<(usi)fdt.ucarr[i]<<' ';*/
		dtb apc1;
		for(ui8 i=0;i<sizeof(pc1);i++){
			ui8 nbyte=i/8, nbit=i%8, nbyte1=(pc1[i]-1)/8,nbit1=(pc1[i]-1)%8;
			apc1.barr[nbyte].putb(nbit,key.barr[nbyte1].getb(7-nbit1));
			/*cout<<dec<<"\ni="<<(usi)i<<" (usi)pc1[i]="<<(usi)pc1[i]
			<<" key.barr["<<(usi)nbyte1<<"].getb("<<(usi)nbit1<<")="<<(usi)key.barr[nbyte1].getb(7-nbit1);*/
			/*if(nbyte==0) {cout<<"\n\ni="<<(usi)i<<'\n';}
			cout<<(usi)pc1[i]<<'\n';
			cout<<(usi)apc1.barr[nbyte].getb(nbit);*/
		}
		for(ui8 i=0;i<sizeof(itrol);i++){
			for(ui8j=0;j<itrol[i];j++){
				ui8 ui8tmp=(usi)apc1.barr[0].getb(0);
				ui8 k;
				for(k=0;k<((ui8)(56/2)-1);k++){
					ui8 nbyte=k/8, nbit=k%8,nbyte1=(k+1)/8, nbit1=(k+1)%8
					apc1.barr[nbyte].putb(nbit,apc1.barr[nbyte1].getb(nbit1));
				}
				apc1.barr[k/8].putb(k%8,ui8tmp);
				cout<<'\n'; system("pause");
			}
		}
		cout<<'\n';
		//ptf(dst,apc1,strt1,fpo);
		
		
	//}while(gffres!=1);
	/*dtb ab; ab.ucarr[0]='A';
	cout<<"sizeof(b2b)="<<sizeof(b2b)<<" sizeof(dtb)="<<sizeof(dtb)<<"\nab='A'="<<(usi)ab.ucarr[0]
	<<"\n(usi)ab.barr[0].bi=";
	cout<<(usi)ab.barr[0].b7<<' '<<(usi)ab.barr[0].b6<<' '<<(usi)ab.barr[0].b5<<' '<<(usi)ab.barr[0].b4<<' '
	<<(usi)ab.barr[0].b3<<' '<<(usi)ab.barr[0].b2<<' '<<(usi)ab.barr[0].b1<<' '<<(usi)ab.barr[0].b0<<'\n';
	for(ui8 i=0;i<8;i++) cout<<(usi)ab.barr[0].getb(7-i)<<' ';*/
	
	return 0;
}
