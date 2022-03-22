#include<bits/stdc++.h>

using namespace std;

int months[13]={
	0,31,28,31,30,31,30,31,31,30,31,30,31,
};

int is_leap(int year){
	if(year%4==0&&year%100||year%400==0) return 1;
	return 0;
}

int getdays(int year,int month){
	int res=months[month];
	if(month==2) res+=is_leap(month);
	return res;
}

int daysbetdays(string date1,string date2){// 按照YYYY-MM-DD的格式给出 
	if(date1>date2) swap(date1,date2);
	int y_1=stoi(date1.substr(0,4));
	int m_1=stoi(date1.substr(5,2));
	int d_1=stoi(date1.substr(8,2));
	int y_2=stoi(date2.substr(0,4));
	int m_2=stoi(date2.substr(5,2));
	int d_2=stoi(date2.substr(8,2));
	
	int res=0;
	while(true){
		if(y_1==y_2&&m_1==m_2&&d_1==d_2) break;
		res++;
		d_1++;
		if(d_1>getdays(y_1,m_1)) m_1++,d_1=1;
		if(m_1>12) y_1++,m_1=1;
	}
	return res;
}


vector<int> add(vector<int> &a,vector<int> &b){
	vector<int> c;
	for(int i=0,t=0;i<a.size()||i<b.size()||t;i++){
		if(i<a.size()) t+=a[i];
		if(i<b.size()) t+=b[i];
		c.push_back(t%10);
		t/=10;
	}
//	reverse(c.begin(),c.end()); //是否需要翻转待定 
	return c; 
}

bool cmp(vector<int> &a,vector<int> &b){
	if(a.size()!=b.size()) return a.size()>b.size();
	for(int i=a.size()-1;~i;i--)
		if(a[i]!=b[i])
			return a[i]>b[i];
	return true; 
}

vector<int> subtraction(vector<int> &a,vector<int> &b){
	vector<int> c;
	for(int i=0,t=0;i<a.size();i++){//事先通过cmp()保证a>b 
		t=a[i]-t; //t=a[i]-b[i]-t;
		if(i<b.size()) t-=b[i];
		c.push_back((t+10)%10);
		if(t<0) t=1;
		else t=0;
	}
	while(c.size()>1&&!c.back()) c.pop_back();
	return c;
}

vector<int> multi(vector<int> &a,vector<int> &b){
	int n=a.size(),m=b.size();
	vector<int> c(n+m);
	for(int i=0;i<a.size();i++)
		for(int j=0;j<b.size();j++)
			c[i+j]+=a[i]*b[j];
	for(int i=0,t=0;i<c.size();i++){
		t+=c[i];
		c[i]=t%10;
		t/=10;
	}
	while(c.size()>1&&c.back()==0) c.pop_back();
//	reverse(c.begin(),c.end()); // 是否需要翻转待定 
	return c; 
}

vector<int> div(vector<int> &a,vector<int> &b){//用减法去做除法 
//	for(auto &x:a) cout<<x<<' ';
//	for(auto &x:b) cout<<x<<' ';
	int lc=a.size()-b.size()+1;
	vector<int> c(lc,0);
	auto tmp_a=a;
	for(int i=lc-1;~i;i--){
		vector<int> tmp_b;
		for(int j=0;j<i;j++) tmp_b.push_back(0);//这里需要先导入0，因为b是倒序存储的 
		for(auto &x:b) tmp_b.push_back(x);
		while(cmp(tmp_a,tmp_b)){
			c[i]++;
			tmp_a=subtraction(tmp_a,tmp_b);
		}
	}
	while(c.size()>1&&c.back()==0) c.pop_back();
	return c;
}


vector<int> chengf(vector<int> &a,vector<int> &b){
	int k=0;
	for(int i=b.size()-1;~i;i--) k=k*10+b[i];
	vector<int> c;
	auto d=a;
	while(k>1){
		c=multi(a,d);
		d=c;
		k--;
	}
	return c;
}

stack<vector<int>> num;
stack<char> op;

void eval(){
	auto b=num.top(); num.pop();
	auto a=num.top(); num.pop();
	auto c=op.top(); op.pop();
	vector<int> res;
	if(c=='+')  res=add(a,b);
	else if(c=='-') res=subtraction(a,b);
	else if(c=='*') res=multi(a,b);
	else if(c=='/') res=div(a,b);
	else if(c=='^') res=chengf(a,b);
	num.push(res);
}



unordered_map<char,int> mp{{'+',1},{'-',1},{'*',2},{'/',2},{'^',3}};
int main(){
	string select;
	cout<<"请选择您要使用功能：";
	cout<<endl<<"1：计算器功能      2：日期查询功能 :";
	cin>>select;
	if(select=="1"){ 
		cout<<"请输入您要计算的表达式："; 
		string str;
		cin>>str;
		for(int i=0;i<str.size();i++){
			auto c=str[i];
			if(isdigit(c)){
				vector<int> a;
				int j=i;
				while(j<str.size()&&isdigit(str[j])) a.push_back(str[j++]-'0');
				reverse(a.begin(),a.end());
				num.push(a);
				i=j-1;
			}else if(c=='('){
				op.push(c);
			}else if(c==')'){
				while(op.size()&&op.top()!='(') eval();
				op.pop();
			}else {
				while(op.size()&&mp[op.top()]>=mp[c]) eval();
				op.push(c);
			}
		}
		while(op.size()) eval();
		auto res=num.top();
		cout<<"计算的结果是： ";
		for(int i=res.size()-1;~i;i--) cout<<res[i];
		cout<<endl;
	}else{
		string s1,s2;
		cout<<"请输入2个形如\"YYYY-MM-DD\"格式的日期："<<endl;
		cin>>s1>>s2;
		cout<<"日期之间的天数为：";
		cout<<daysbetdays(s1,s2)<<endl;
	}
	return 0;
} 
