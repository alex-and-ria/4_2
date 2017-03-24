#include<iostream>
#include<vector>
#include<algorithm>
#define dltst 30

using namespace std;
typedef unsigned short int usi;

struct usib{
	double val;
	usi num;
};

bool dscsrt(usib i,usib j){
	return(i.val>j.val);
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
	
	
	
	return 0;
}
