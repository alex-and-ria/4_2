#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<cstdint>
#include"des.h"
#define bsz 8
using namespace std;
typedef unsigned short int usi;
typedef uint8_t ui8;

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
			case 0: b0=j; break;
			case 1: b1=j; break;
			case 2: b2=j; break;
			case 3: b3=j; break;
			case 4: b4=j; break;
			case 5: b5=j; break;
			case 6: b6=j; break;
			case 7: b7=j; break;
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
	void invb(){
		for(ui8 i=0;i<bsz;i++){
			for(ui8 j=0;j<8/2;j++){
				ui8 u8tmp=barr[i].getb(7-j);
				barr[i].putb(7-j,barr[i].getb(j));
				barr[i].putb(j,u8tmp);
			}
		}
	}
};

bool gff(string& ifn,string& dt,ifstream& fpi){
	usi cnt=0;
	fpi.open(ifn.c_str(),ios::binary);
	if(!fpi.is_open()) {cout<<"\nerr: gff no input file"; system("pause"); return 0;}
	else{
		while(1){
			unsigned char ch='\0';
			fpi>>noskipws>>ch;
			if(fpi.eof()) break;
			dt+=ch;
			//cout<<"\nch="<<ch<<" fdt.ucarr[cnt-1]="<<fdt.ucarr[cnt];
		}
	}
	fpi.close();    
    return 1;
}

bool ptf(string& ofn,dtb& fdt,ofstream& fpo,ui8 fl){
	for(ui8 i=0;i<bsz;i++){
		if(fl==2){fpo<<fdt.ucarr[i]; cout<<hex<<(usi)fdt.ucarr[i]<<' ';}
		cout<<' ';
		for(ui8 j=0;j<8;j++){
			if(fl==1) cout<<(usi)fdt.barr[i].getb(7-j);
			else if(fl==0) cout<<(usi)fdt.barr[i].getb(j);
		}
	}
}

void desenc(string& srs, string& dst, dtb key,vector<dtb>& ksh){
	dtb fdt; bool strt=true,strt1=true; ifstream fpi; ofstream fpo; ui8 gffres;
	fpo.open(dst.c_str(),ios::binary);
    if(!fpo.is_open()) {cout<<"\nerr:ptff no output file"; system("pause"); return;}
	cout<<'\n';
	cout<<"\nkey="; ptf(dst,key,fpo,1);
	string dt; gff(srs,dt,fpi);
	while(dt.length()!=0){
		for(ui8 i=0;i<8;i++){
			if(dt.length()>0){
				cout<<"\nsizeof(dt)="<<dt.length()<<' '<<dt<<'\n'; //system("pause");
				fdt.ucarr[i]=(unsigned char)((dt.c_str())[0]);
				cout<<"\ndt="<<hex<<(usi)fdt.ucarr[i]<<dec;
				dt.erase(dt.begin());
			}
			else fdt.ucarr[i]=0;
		}
		dtb apc1,apc2,aip;
		cout<<"\ndata="; ptf(dst,fdt,fpo,1); cout<<'\n';
		for(ui8 i=0;i<sizeof(pc1);i++){
			ui8 nbyte=i/8, nbit=i%8, nbyte1=(pc1[i]-1)/8,nbit1=(pc1[i]-1)%8;
			apc1.barr[nbyte].putb(nbit,key.barr[nbyte1].getb(7-nbit1));
		}
		for(ui8 i=0;i<sizeof(ip);i++){
			ui8 nbyte=i/8, nbit=i%8, nbyte1=(ip[i]-1)/8,nbit1=(ip[i]-1)%8;
			aip.barr[nbyte].putb(nbit,fdt.barr[nbyte1].getb(7-nbit1));
		}
		for(ui8 i=0;i<sizeof(itrol);i++){//round;
			for(ui8 j=0;j<itrol[i];j++){//num of shifts;
				vector<bool> bvecl,bvecr; apc1.getarr(bvecl,bvecr);
				/*cout<<'\n'; for(ui8 l=0;l<bvecl.size();l++) cout<<bvecl.at(l); cout<<'\t';
				for(ui8 l=0;l<bvecr.size();l++) cout<<bvecr.at(l); cout<<'\n'; system("pause");*/
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
			ksh.push_back(apc2);
			cout<<"\nkeysh="; ptf(dst,apc2,fpo,0);
			vector<bool> li,ri;	aip.getarrd(li,ri); dtb aep,asb,app;
			//cout<<'\n'; for(ui8 l=0;l<li.size();l++) cout<<li.at(l); cout<<'\t';
			//for(ui8 l=0;l<ri.size();l++) cout<<ri.at(l); cout<<'\n'; system("pause");
			for(ui8 j=0;j<sizeof(ep);j++){
				ui8 nbyte=j/8, nbit=j%8, nbyte1=(ep[j]-1)/8,nbit1=(ep[j]-1)%8;
				aep.barr[nbyte].putb(nbit,ri.at(ep[j]-1));
			}
			cout<<"\naep="; ptf(dst,aep,fpo,0); cout<<'\n';
			for(ui8 j=0;j<sizeof(ep);j++){//xor ki;
				ui8 nbyte=j/8, nbit=j%8;
				aep.barr[nbyte].putb(nbit,(apc2.barr[nbyte].getb(nbit)^aep.barr[nbyte].getb(nbit)));
			}
			cout<<"\naep xor ki ="; ptf(dst,aep,fpo,0);
			//vector<bool> sbvl,sbvr; aep.getarrd(sbvl,sbvr);
			//for(usi j=0;j<sbvr.size();j++) sbvl.push_back(sbvr.at(j));
			//cout<<"\n\t"; for(usi j=0;j<sbvl.size();j++) cout<<sbvl.at(j); cout<<' ';
			for(ui8 j=0,scnt=0;j<sizeof(ep);j+=6,scnt++){
				ui8 nbyte=j/8, nbit=j%8;
				ui8 nbyte1=(j+1)/8, nbit1=(j+1)%8;
				ui8 nbyte2=(j+2)/8, nbit2=(j+2)%8;
				ui8 nbyte3=(j+3)/8, nbit3=(j+3)%8;
				ui8 nbyte4=(j+4)/8, nbit4=(j+4)%8;
				ui8 nbyte5=(j+5)/8, nbit5=(j+5)%8;
				unsigned char uctmp;
				/*cout<<"x,y="<<aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)<<' '
					<<aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
					aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4);
				cout<<'\n'<<(usi)aep.barr[nbyte].getb(nbit)<<(usi)aep.barr[nbyte1].getb(nbit1)<<(usi)aep.barr[nbyte2].getb(nbit2)
					<<(usi)aep.barr[nbyte3].getb(nbit3)<<(usi)aep.barr[nbyte4].getb(nbit4)<<(usi)aep.barr[nbyte5].getb(nbit5)<<'\n';*/
				switch(scnt){
					case 0: 
						uctmp=sb1[aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)]
							[aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
							aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4)];
					break;
					case 1: 
						uctmp=sb2[aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)]
							[aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
							aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4)];
					break;
					case 2: 
						uctmp=sb3[aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)]
							[aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
							aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4)];
					break;
					case 3: 
						uctmp=sb4[aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)]
							[aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
							aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4)];
					break;
					case 4: 
						uctmp=sb5[aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)]
							[aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
							aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4)];
					break;
					case 5: 
						uctmp=sb6[aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)]
							[aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
							aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4)];
					break;
					case 6: 
						uctmp=sb7[aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)]
							[aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
							aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4)];
					break;
					case 7: 
						uctmp=sb8[aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)]
							[aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
							aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4)];
					break;
				}
				if(scnt%2==0){
					asb.ucarr[scnt/2]=uctmp<<4;
				}
				else{
					asb.ucarr[scnt/2]+=uctmp;
				}
				cout<<"\nuctmp="<<(usi)uctmp<<' '<<hex<<(usi)uctmp<<dec;;
			}
			cout<<"\nasb="; ptf(dst,asb,fpo,1);
			for(ui8 j=0;j<sizeof(pp);j++){
				ui8 nbyte=j/8, nbit=j%8, nbyte1=(pp[j]-1)/8,nbit1=(pp[j]-1)%8;
				app.barr[nbyte].putb(nbit,asb.barr[nbyte1].getb(7-nbit1));
			}
			cout<<"\napp="; ptf(dst,app,fpo,0);
			//cout<<'\n'; for(ui8 j=0;j<li.size();j++) cout<<li.at(j); cout<<' '; //system("pause");
			vector<bool> resr;
			for(ui8 j=0;j<li.size();j++) resr.push_back(app.barr[j/8].getb(j%8)^li.at(j));
			cout<<"\nnli="; for(ui8 j=0;j<li.size();j++) cout<<ri.at(j);
			cout<<"\nnri="; for(ui8 j=0;j<resr.size();j++) cout<<resr.at(j);
			//cout<<"\nsizrof(ri)="<<ri.size()<<" sizeof(resr)="<<resr.size(); cout<<' '; system("pause");
			if(i==15){
				aip.getbarrd(resr,ri); dtb aip1;
				cout<<"\nbinv="; ptf(dst,aip,fpo,0);
				for(ui8 j=0;j<sizeof(ip1);j++){
					ui8 nbyte=j/8, nbit=j%8, nbyte1=(ip1[j]-1)/8,nbit1=(ip1[j]-1)%8;
					aip1.barr[nbyte].putb(nbit,aip.barr[nbyte1].getb(nbit1));
				}
				aip1.invb(); strt1=true; ptf(dst,aip1,fpo,2);
				cout<<"\nainv="; ptf(dst,aip1,fpo,1);
				cout<<'\n'; //system("pause");
			}
			else{
				aip.getbarrd(ri,resr);
				ri.clear(); li.clear();
			}
			cout<<'\n';
		}
		cout<<'\n';
	}
	fpo.close(); cout<<'\n'; //system("pause");
}

void desdec(string& srs, string& dst, dtb key,vector<dtb>& ksh){
	dtb fdt; bool strt=true,strt1=true; ifstream fpi; ofstream fpo; ui8 gffres;
	fpo.open(dst.c_str(),ios::binary);
    if(!fpo.is_open()) {cout<<"\nerr:ptff no output file"; system("pause"); return;}
	cout<<'\n';
	cout<<"\nkey="; ptf(dst,key,fpo,1);
	string dt; gff(srs,dt,fpi);
	while(dt.length()!=0){
		for(ui8 i=0;i<8;i++){
			if(dt.length()>0){
				cout<<"\nsizeof(dt)="<<dt.length()<<' '<<dt<<'\n'; //system("pause");
				fdt.ucarr[i]=(unsigned char)((dt.c_str())[0]);
				cout<<"\ndt="<<hex<<(usi)fdt.ucarr[i]<<dec;
				dt.erase(dt.begin());
			}
			else fdt.ucarr[i]=0;
		}
		dtb apc1,apc2,aip;
		cout<<"\ndata="; ptf(dst,fdt,fpo,1);
		for(ui8 i=0;i<sizeof(pc1);i++){
			ui8 nbyte=i/8, nbit=i%8, nbyte1=(pc1[i]-1)/8,nbit1=(pc1[i]-1)%8;
			apc1.barr[nbyte].putb(nbit,key.barr[nbyte1].getb(7-nbit1));
		}
		for(ui8 i=0;i<sizeof(ip);i++){
			ui8 nbyte=i/8, nbit=i%8, nbyte1=(ip[i]-1)/8,nbit1=(ip[i]-1)%8;
			aip.barr[nbyte].putb(nbit,fdt.barr[nbyte1].getb(7-nbit1));
		}
		for(ui8 i=0;i<sizeof(itrol);i++){//round;
			for(ui8 j=0;j<itrol[i];j++){//num of shifts;
				vector<bool> bvecl,bvecr; apc1.getarr(bvecl,bvecr);
				/*cout<<'\n'; for(ui8 l=0;l<bvecl.size();l++) cout<<bvecl.at(l); cout<<'\t';
				for(ui8 l=0;l<bvecr.size();l++) cout<<bvecr.at(l); cout<<'\n'; system("pause");*/
				ui8 u8tmpl=bvecl.at(bvecl.size()-1), u8tmpr=bvecr.at(bvecr.size()-1);
				for(ui8 k=bvecl.size()-2;k!=0;k--){
					bvecl.at(k+1)=bvecl.at(k);
					bvecr.at(k+1)=bvecr.at(k);
				}
				bvecl.at(0)=u8tmpl; bvecr.at(0)=u8tmpr;
				apc1.getbarr(bvecl,bvecr);
			}
			for(ui8 k=0;k<sizeof(pc2);k++){
				ui8 nbyte=k/8, nbit=k%8, nbyte1=(pc2[k]-1)/8,nbit1=(pc2[k]-1)%8;
				apc2.barr[nbyte].putb(nbit,apc1.barr[nbyte1].getb(nbit1));
			}//apc2 is round key; apc1 is Ci, Di;
			dtb rndkey=ksh.at(ksh.size()-1); ksh.pop_back();
			cout<<"\nkeysh="; ptf(dst,rndkey,fpo,0);
			vector<bool> li,ri;	aip.getarrd(li,ri); dtb aep,asb,app;
			//cout<<'\n'; for(ui8 l=0;l<li.size();l++) cout<<li.at(l); cout<<'\t';
			//for(ui8 l=0;l<ri.size();l++) cout<<ri.at(l); cout<<'\n'; system("pause");
			for(ui8 j=0;j<sizeof(ep);j++){
				ui8 nbyte=j/8, nbit=j%8, nbyte1=(ep[j]-1)/8,nbit1=(ep[j]-1)%8;
				aep.barr[nbyte].putb(nbit,ri.at(ep[j]-1));
			}
			cout<<"\naep="; ptf(dst,aep,fpo,0); cout<<'\n';
			for(ui8 j=0;j<sizeof(ep);j++){//xor ki;
				ui8 nbyte=j/8, nbit=j%8;
				aep.barr[nbyte].putb(nbit,(rndkey.barr[nbyte].getb(nbit)^aep.barr[nbyte].getb(nbit)));
			}
			cout<<"\naep xor ki ="; ptf(dst,aep,fpo,0);
			//vector<bool> sbvl,sbvr; aep.getarrd(sbvl,sbvr);
			//for(usi j=0;j<sbvr.size();j++) sbvl.push_back(sbvr.at(j));
			//cout<<"\n\t"; for(usi j=0;j<sbvl.size();j++) cout<<sbvl.at(j); cout<<' ';
			for(ui8 j=0,scnt=0;j<sizeof(ep);j+=6,scnt++){
				ui8 nbyte=j/8, nbit=j%8;
				ui8 nbyte1=(j+1)/8, nbit1=(j+1)%8;
				ui8 nbyte2=(j+2)/8, nbit2=(j+2)%8;
				ui8 nbyte3=(j+3)/8, nbit3=(j+3)%8;
				ui8 nbyte4=(j+4)/8, nbit4=(j+4)%8;
				ui8 nbyte5=(j+5)/8, nbit5=(j+5)%8;
				unsigned char uctmp;
				/*cout<<"x,y="<<aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)<<' '
					<<aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
					aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4);
				cout<<'\n'<<(usi)aep.barr[nbyte].getb(nbit)<<(usi)aep.barr[nbyte1].getb(nbit1)<<(usi)aep.barr[nbyte2].getb(nbit2)
					<<(usi)aep.barr[nbyte3].getb(nbit3)<<(usi)aep.barr[nbyte4].getb(nbit4)<<(usi)aep.barr[nbyte5].getb(nbit5)<<'\n';*/
				switch(scnt){
					case 0: 
						uctmp=sb1[aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)]
							[aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
							aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4)];
					break;
					case 1: 
						uctmp=sb2[aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)]
							[aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
							aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4)];
					break;
					case 2: 
						uctmp=sb3[aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)]
							[aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
							aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4)];
					break;
					case 3: 
						uctmp=sb4[aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)]
							[aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
							aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4)];
					break;
					case 4: 
						uctmp=sb5[aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)]
							[aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
							aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4)];
					break;
					case 5: 
						uctmp=sb6[aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)]
							[aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
							aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4)];
					break;
					case 6: 
						uctmp=sb7[aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)]
							[aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
							aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4)];
					break;
					case 7: 
						uctmp=sb8[aep.barr[nbyte].getb(nbit)*2+aep.barr[nbyte5].getb(nbit5)]
							[aep.barr[nbyte1].getb(nbit1)*2*2*2+aep.barr[nbyte2].getb(nbit2)*2*2+
							aep.barr[nbyte3].getb(nbit3)*2+aep.barr[nbyte4].getb(nbit4)];
					break;
				}
				if(scnt%2==0){
					asb.ucarr[scnt/2]=uctmp<<4;
				}
				else{
					asb.ucarr[scnt/2]+=uctmp;
				}
				cout<<"\nuctmp="<<(usi)uctmp<<' '<<hex<<(usi)uctmp<<dec;;
			}
			cout<<"\nasb="; ptf(dst,asb,fpo,1);
			for(ui8 j=0;j<sizeof(pp);j++){
				ui8 nbyte=j/8, nbit=j%8, nbyte1=(pp[j]-1)/8,nbit1=(pp[j]-1)%8;
				app.barr[nbyte].putb(nbit,asb.barr[nbyte1].getb(7-nbit1));
			}
			cout<<"\napp="; ptf(dst,app,fpo,0);
			//cout<<'\n'; for(ui8 j=0;j<li.size();j++) cout<<li.at(j); cout<<' '; //system("pause");
			vector<bool> resr;
			for(ui8 j=0;j<li.size();j++) resr.push_back(app.barr[j/8].getb(j%8)^li.at(j));
			cout<<"\nnli="; for(ui8 j=0;j<li.size();j++) cout<<ri.at(j);
			cout<<"\nnri="; for(ui8 j=0;j<resr.size();j++) cout<<resr.at(j);
			//cout<<"\nsizrof(ri)="<<ri.size()<<" sizeof(resr)="<<resr.size(); cout<<' '; system("pause");
			if(i==15){
				aip.getbarrd(resr,ri); dtb aip1;
				cout<<"\nbinv="; ptf(dst,aip,fpo,0);
				for(ui8 j=0;j<sizeof(ip1);j++){
					ui8 nbyte=j/8, nbit=j%8, nbyte1=(ip1[j]-1)/8,nbit1=(ip1[j]-1)%8;
					aip1.barr[nbyte].putb(nbit,aip.barr[nbyte1].getb(nbit1));
				}
				aip1.invb(); strt1=true; ptf(dst,aip1,fpo,2);
				cout<<"\nainv="; ptf(dst,aip1,fpo,1);
				cout<<'\n'; //system("pause");
			}
			else{
				aip.getbarrd(ri,resr);
				ri.clear(); li.clear();
			}
			cout<<'\n';
		}
		cout<<'\n';
	}
	fpo.close();
}

void tdesenc(string tsrs, string tdst,dtb tkey,vector<dtb>& tksh,dtb tkey1,vector<dtb>& tksh1,dtb tkey2,vector<dtb>& tksh2){
	desenc(tsrs, tdst, tkey, tksh);
	desenc(tdst, tsrs, tkey1, tksh1);
	desenc(tsrs, tdst, tkey2, tksh2);
}
void tdesdec(string tdst,string tddst,dtb tkey,vector<dtb>& tksh,dtb tkey1,vector<dtb>& tksh1,dtb tkey2,vector<dtb>& tksh2){
	desdec(tdst, tddst, tkey2,tksh2);
	desdec(tddst, tdst, tkey1,tksh1);
	desdec(tdst, tddst, tkey,tksh);
}

int main(){
	string srs="q.txt",dst="q1.txt",ddst="q2.txt"; dtb key={59,56,152,55,21,32,247,94}; vector<dtb> ksh;
	desenc(srs, dst, key, ksh);
	desdec(dst, ddst, key,ksh);
	
	string tsrs="tq.txt",tdst="tq1.txt",tddst="tq2.txt";
	dtb tkey={59,56,152,55,21,32,247,94}, tkey1={59,56,152,55,21,32,247,94}, tkey2={59,56,152,55,21,32,247,94};
	vector<dtb> tksh,tksh1,tksh2;
	tdesenc(tsrs,tdst,tkey,tksh,tkey1,tksh1,tkey2,tksh2);
	tdesdec(tdst,tddst,tkey,tksh,tkey1,tksh1,tkey2,tksh2);
	
	return 0;
}
