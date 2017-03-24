#include<iostream>
#include<vector>
#include<algorithm>
#define dltst 30

using namespace std;
typedef unsigned short int usi;

struct usib{
	double val;
	usi num;
	usib(){}
	usib(double nval, usi nnum): val(nval), num(nnum){}
};

bool dscsrt(usib i,usib j){
	return(i.val>j.val);
}

void getubv(double darr[],vector<usib>& ubarr,usi n){
	ubarr.clear();
	for(usi i=0;i<n;i++){
		usib ubtmp; ubtmp.val=darr[i];
		ubarr.push_back(ubtmp);
	}
}

int main(){
	usi cnt=0;
	cout<<cnt<<" pairwise comparison method;\n";
	vector<usi> z; usi z1[]={33,100,68,40}, maxi=0;
	z.assign(z1,z1+(sizeof(z1)/sizeof(z1[0])));
	for(usi i=0;i<z.size();i++){
		for(usi j=1;j<z.size();j++){
			if(i>j){
				if(z.at(i)<z.at(j)){
					maxi=i;
				}
			}
		}
	}
	cout<<"maxi="<<maxi+1<<"\n\n"; cnt++;
	//system("pause");
	
	cout<<cnt<<" sequential comparison method;\n";
	vector<usib> zz;
	for(usi i=0;i<z.size();i++){
		usib usibtmp; usibtmp.val=z.at(i); usibtmp.num=i;
		zz.push_back(usibtmp);
	}
	stable_sort(zz.begin(),zz.end(),dscsrt);
	cout<<"before correction:\n";
	for(usi i=0;i<zz.size(); i++){
		cout<<zz.at(i).val<<' '<<zz.at(i).num<<'\n';
	}
	usi dlt=dltst;
	for(usi i=0;i<zz.size();i++){
		for(usi j=1;j<zz.size();j++){
			for(usi k=2;k<zz.size();k++){
				if(i<j&&j<k&&zz.at(i).val<(zz.at(j).val+zz.at(k).val)){
					zz.at(i).val+=dlt; dlt/=2;
				}
			}
		}
	}
	unsigned int tots=0;
	cout<<"after correction:\n";
	for(usi i=0;i<zz.size();i++){
		cout<<zz.at(i).val<<' '<<zz.at(i).num<<'\n';
		tots+=zz.at(i).val;
	}
	for(usi i=0;i<zz.size();i++){
		zz.at(i).val/=tots;
	}
	cout<<"resulting weights:\n"; for(usi i=0;i<zz.size();i++) cout<<zz.at(i).val<<' '<<zz.at(i).num<<'\n';
	cout<<'\n'; cnt++;
	
	cout<<cnt<<" weighted estimation method;\n";
	vector<usib> iz1,iz2; double r1=8,r2=8.5,totr,tmps=0; bool isfst=1;
	totr=r1+r2; r1/=totr; r2/=totr;
	double dz1[]={0.2,0.14,0.16,0.5},dz2[]={0.4,0.25,0.15,0.2};
	for(usi i=0;i<sizeof(dz1)/sizeof(dz1[0]);i++){
		usib usibtmp; usibtmp.val=dz1[i]; usibtmp.num=i;
		iz1.push_back(usibtmp);
		usibtmp.val=dz2[i];
		iz2.push_back(usibtmp);
	}
	/*for(usi i=0;i<iz1.size();i++){
		cout<<iz1.at(i).val<<' '<<iz2.at(i).val<<'\n';
	}*/
	for(usi i=0;i<iz1.size();i++){
		if(isfst){
			tmps+=iz1.at(i).val;
			if(i==iz1.size()-1){
				if(tmps!=1) {cout<<"\nerr: tmps1!=1\n"; system("pause");}
				isfst=0; i=-1; tmps=0;
			}
		}
		else{
			tmps+=iz2.at(i).val;
			if(i==iz2.size()-1){
				if(tmps!=1) {cout<<"\nerr: tmps2!=1\n"; system("pause");}
				isfst=1; tmps=0;
			}
		}
	}
	for(usi i=0;i<iz1.size();i++){
		iz1.at(i).val=iz1.at(i).val*r1+iz2.at(i).val*r2;
	}
	stable_sort(iz1.begin(),iz1.end(),dscsrt);
	cout<<"result:\n"; for(usi i=0;i<iz1.size();i++) cout<<iz1.at(i).val<<' '<<iz1.at(i).num<<'\n';
	cout<<'\n'; cnt++;
	
	cout<<cnt<<" advantages method;\n";
	vector<vector<usib>> ez; vector<usib> vutmp; double darr[3],totk=0;
	darr[0]=1;darr[1]=2;darr[2]=3;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez.push_back(vutmp);
	darr[0]=2;darr[1]=1;darr[2]=3;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez.push_back(vutmp);
	darr[0]=2;darr[1]=3;darr[2]=1;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez.push_back(vutmp);
	darr[0]=3;darr[1]=2;darr[2]=1;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez.push_back(vutmp);
	darr[0]=3;darr[1]=1;darr[2]=2;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez.push_back(vutmp);
	darr[0]=1;darr[1]=2;darr[2]=3;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez.push_back(vutmp);
	for(usi i=0;i<ez.size();i++){
		for(usi j=0; j<ez.at(i).size();j++){
			cout<<ez.at(i).at(j).val<<' ';
			ez.at(i).at(j).val=ez.at(i).size()-ez.at(i).at(j).val;
			ez.at(i).at(j).num=j;
			vutmp.at(j).val=0; vutmp.at(j).num=j;
			//cout<<" q "<<ez.at(i).at(j).val<<' ';
		}
		cout<<'\n';
	}
	for(usi i=0;i<ez.size();i++){
		for(usi j=0; j<ez.at(i).size();j++){
			vutmp.at(j).val+=ez.at(i).at(j).val;
		}
	}
	cout<<" K=";
	for(usi j=0; j<vutmp.size();j++){
		cout<<vutmp.at(j).val<<' ';
		totk+=vutmp.at(j).val;
	}
	for(usi i=0;i<vutmp.size();i++){
		vutmp.at(i).val/=totk;
	}
	stable_sort(vutmp.begin(),vutmp.end(),dscsrt);
	cout<<"\nresult:\n"; for(usi i=0;i<vutmp.size();i++) cout<<vutmp.at(i).val<<' '<<vutmp.at(i).num<<'\n';
	cout<<'\n'; cnt++;
	
	cout<<cnt<<" rank method;\n";
	ez.clear(); vutmp.clear(); totk=0;
	darr[0]=1;darr[1]=2;darr[2]=3;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez.push_back(vutmp);
	darr[0]=2;darr[1]=1;darr[2]=3;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez.push_back(vutmp);
	darr[0]=2;darr[1]=3;darr[2]=1;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez.push_back(vutmp);
	darr[0]=3;darr[1]=2;darr[2]=1;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez.push_back(vutmp);
	darr[0]=3;darr[1]=1;darr[2]=2;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez.push_back(vutmp);
	darr[0]=1;darr[1]=2;darr[2]=3;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez.push_back(vutmp);
	for(usi i=0;i<ez.size();i++){
		totk=0;
		for(usi j=0; j<ez.at(i).size();j++){
			cout<<ez.at(i).at(j).val<<' ';
			totk+=ez.at(i).at(j).val;
			ez.at(i).at(j).num=j;
			vutmp.at(j).val=0; vutmp.at(j).num=j;
		}
		for(usi j=0; j<ez.at(i).size();j++){
			ez.at(i).at(j).val/=totk;
		}
		cout<<'\n';
	}
	for(usi j=0; j<ez.at(0).size();j++){
		totk=0;
		for(usi i=0;i<ez.size();i++){
			totk+=ez.at(i).at(j).val;
		}
		vutmp.at(j).val=totk/ez.size();
	}
	stable_sort(vutmp.begin(),vutmp.end(),dscsrt);
	cout<<"result:\n"; for(usi i=0;i<vutmp.size();i++) cout<<vutmp.at(i).val<<' '<<vutmp.at(i).num<<'\n';
	cout<<'\n'; cnt++;

	cout<<cnt<<" full pairwise comparison method;\n";
	ez.clear(); vutmp.clear(); totk=0; vector<vector<usib>> ez1; double totk1=0; usi n=3;
	darr[0]=0;darr[1]=13.0/16;darr[2]=1.0/16;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez.push_back(vutmp);
	darr[0]=3.0/16;darr[1]=0;darr[2]=7.0/16;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez.push_back(vutmp);
	darr[0]=15.0/16;darr[1]=9.0/16;darr[2]=0;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez.push_back(vutmp);
	darr[0]=0;darr[1]=11.0/16;darr[2]=4.0/16;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez1.push_back(vutmp);
	darr[0]=5.0/16;darr[1]=0;darr[2]=5.0/16;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez1.push_back(vutmp);
	darr[0]=12.0/16;darr[1]=11.0/16;darr[2]=0;
	getubv(darr,vutmp,sizeof(darr)/sizeof(darr[0]));
	ez1.push_back(vutmp);
	for(usi i=0;i<ez.size();i++){
		for(usi j=0; j<ez.at(i).size();j++){
			cout<<ez.at(i).at(j).val<<' ';
			ez.at(i).at(j).num=j;
			vutmp.at(j).val=0; vutmp.at(j).num=j;
		}
		cout<<'\n';
	}
	for(usi i=0;i<ez1.size();i++){
		for(usi j=0; j<ez1.at(i).size();j++){
			cout<<ez1.at(i).at(j).val<<' ';
			ez1.at(i).at(j).num=j;
		}
		cout<<'\n';
	}
	for(usi i=0;i<ez.size();i++){
		totk=0; totk1=0;
		for(usi j=0; j<ez.at(i).size();j++){
			totk+=ez.at(i).at(j).val;
			totk1+=ez1.at(i).at(j).val;
		}
		ez.at(i).push_back(usib(totk/(n*(n-1)),i));
		ez1.at(i).push_back(usib(totk1/(n*(n-1)),i));
	}
	totk=0;
	for(usi i=0;i<ez.size();i++) totk+=ez.at(i).at(ez.at(i).size()-1).val+ez1.at(i).at(ez1.at(i).size()-1).val;
	for(usi i=0;i<vutmp.size();i++){
		vutmp.at(i).val=(ez.at(i).at(ez.at(i).size()-1).val+ez1.at(i).at(ez1.at(i).size()-1).val)/totk;
	}
	stable_sort(vutmp.begin(),vutmp.end(),dscsrt);
	cout<<"result:\n"; for(usi i=0;i<vutmp.size();i++) cout<<vutmp.at(i).val<<' '<<vutmp.at(i).num<<'\n';
	cout<<'\n'; cnt++;
	
	
	
	
	
	
	
	return 0;
}
