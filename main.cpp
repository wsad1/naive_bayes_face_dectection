#include <iostream>
#include<cmath>
#include <fstream>
#include <string>
#include <stdio.h>
using namespace std;
class Data
{
public:
	//Data();
	int a[451][4201];
	//~Data();
   void train()
   {
   	ifstream in("facedatatrain",ios::in);
   	int i;
   	for(i=0;i<451;i++)
   	{
   		for(int j=0;j<4201;j++)
   		{
   			a[i][j]=0;
   		}
   	}
   	string line;
   	for(i=0;i<451;i++)
   	{
   	for(int j=0;j<70;j++)
   	{
   	getline(in,line,'\n');
   	//cout<<line<<'\n';
   	for(int k=0;k<60;k++)
   	{

   		if(line[k]=='#')
   		a[i][(j*60)+k]=1;
   		else
   			a[i][(j*60)+k]=0;
   	}

   }
	}
	ifstream in2("facedatatrainlabels",ios::in);
	for(i=0;i<451;i++)
	{
	getline(in2,line,'\n');
	if(line[0]=='0')
		a[i][4200]=0;

	else
		a[i][4200]=1;
}

   }
   void test()
   {
   	ifstream in("facedatatest",ios::in);
   	int i;
   	for(i=0;i<150;i++)
   	{
   		for(int j=0;j<4201;j++)
   		{
   			a[i][j]=0;
   		}
   	}
   	string line;
   	for(i=0;i<150;i++)
   	{
   	for(int j=0;j<70;j++)
   	{
   	getline(in,line,'\n');
   	//cout<<line<<'\n';
   	for(int k=0;k<60;k++)
   	{

   		if(line[k]=='#')
   		a[i][(j*60)+k]=1;
   		else
   			a[i][(j*60)+k]=0;
   	}

   }
	}
	ifstream in2("facedatatestlabels",ios::in);
	for(i=0;i<150;i++)
	{
	getline(in2,line,'\n');
	if(line[0]=='0')
		a[i][4200]=0;

	else
		a[i][4200]=1;

}

   }

};
class N_B
{
  public:
  int false_pos,false_neg,true_pos,true_neg;
  double P_attr_1_pos[60*70+2];//P(x='#'|face)
  double P_attr_1_neg[60*70+2];//P(x='#'|not a face)
  long tot_train,tot_test,pixels;
  long m;// virtual sample size
  double error,accu;
  double P_1,P_0;
  int *in;
  int out;
  N_B()
  {
      int i=0;
      m=2;//CHANGE M VALUES HERE
      tot_train=451;tot_test=150;pixels=60*70;P_1=0.0;P_0=0.0;error=0.0;accu=0.0;
      false_pos=0;false_neg=0;true_pos=0;true_neg=0;
      for(i=0;i<pixels;i++)
      {
          P_attr_1_pos[i]=0;
          P_attr_1_neg[i]=0;
      }
  }
  void compute_freq()// stores frequecies of occurences of '#' given face and not face
  {
      Data *D=new Data();
      long i;
      D->train();
      for(i=0;i<tot_train;i++)
      {
          out=D->a[i][60*70];
          in=D->a[i];
          update();
      }
  }
  void test()// accuracy over test data
  {
      long i,j;
      double pos,neg;
      Data *D=new Data();
      D->test();
      for(i=0;i<tot_test;i++)
      {
          pos=0.0;neg=0.0;
          out=D->a[i][70*60];
          in=D->a[i];
          for(j=0;j<pixels;j++)
          {
              if(in[j]==1)
              {
                  pos+=log((P_attr_1_pos[j]+m*0.5)/(P_1+m));
              }
              else
              {
                  pos+=log((P_1-P_attr_1_pos[j]+m*0.5)/(P_1+m));
              }
          }
          pos+=log(P_1);
          for(j=0;j<pixels;j++)
          {
              if(in[j]==1)
              {
                  neg+=log((P_attr_1_neg[j]+m*0.5)/(P_0+m));
              }
              else
              {
                  neg+=log((P_0-P_attr_1_neg[j]+m*0.5)/(P_0+m));
              }
          }
          neg+=log(P_0);
          //cout<<pos<<" "<<neg<<"\n";
          if((pos>neg && out==1) || (pos<neg && out==0))//coorect classification
          {
              if(out==1)
                true_pos++;
              else
                true_neg++;
          }
          else
          {

              if(out==1)
              {
                  //cout<<i<<"-false_neg\n";
                  false_neg++;
              }
              else
              {
                  //cout<<i<<"-false_pos\n";
                  false_pos++;
              }
              error+=1.0;
          }

    }
    accu=((tot_test-error)/tot_test)*100;
}
  void update()
  {
      long i=0;
      if(out==0)
      {
          P_0+=1.0;
          for(i=0;i<pixels;i++)
          {
              if(in[i]==1)
              P_attr_1_neg[i]+=1.0;
          }
      }
      else
      {
          P_1+=1.0;
          for(i=0;i<pixels;i++)
          {
              if(in[i]==1)
                P_attr_1_pos[i]+=1.0;
          }
      }
    }
};
int main()
{
    int i;
    N_B *b=new N_B();
    b->compute_freq();
    b->test();
    cout<<"Accuracy over 150 testing samples="<<b->accu<<"\n";
    cout<<"true +ve's="<<b->true_pos<<"\n";
    cout<<"true -ve's="<<b->true_neg<<"\n";
    cout<<"false -ve's="<<b->false_neg<<"\n";
    cout<<"false +ve's="<<b->false_pos<<"\n";
    cin>>i;
}
