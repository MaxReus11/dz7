#include <iostream>
#include<random>
#include<thread>
#include<vector>
#include<iterator>
#include"timer.h"
using namespace std;
void MONTE( int N, vector<int> &k, int j){
    random_device rd;
    mt19937 RD(rd());
    for(int i=(N/6*j+1); i<N/6*(j+1); i++){
        uniform_real_distribution<> dis(10.0/6.0*j, 10.0/6.0*(j+1));
        uniform_real_distribution<> dis1(0.0, 10.0);
        if(sqrt(pow(dis(RD),2)+pow(dis1(RD),2))<10.0){
            k[j]++;
        }
    }
}
int main() {

    long long int N=100000000;

    vector<int> k(6,0);
    vector <thread> thr;
    //pair<double,double> a;
    Timer parallel;
    for(int j=0; j<5; j++) {thr.emplace_back(thread(MONTE, N, ref(k), j));}
    random_device rd;
    mt19937 RD(rd());
    for(int i=N/6*5+1; i<N+1; i++){
        uniform_real_distribution<> dis(10.0/6.0*5, 10.0);
        uniform_real_distribution<> dis1(0.0, 10.0);
        if(sqrt(pow(dis(RD),2)+pow(dis1(RD),2))<10.0){
            k[5]++;
        }
    }
    for(int j=0; j<5; j++) {thr[j].join();}
    double sum = accumulate(k.begin(), k.end(), 0);
    double pi = 4*sum/N;
    cout<<"Thread pi calculation: "<<pi<<endl;
    cout<<"Time of thread calculation: "<<parallel.print()<<endl;
    sum = 0;
    Timer line;
    for(int i=0; i<N; i++){
        uniform_real_distribution<> dis(0.0, 10.0);
        uniform_real_distribution<> dis1(0.0, 10.0);
        if(sqrt(pow(dis(RD),2)+pow(dis1(RD),2))<10.0){
            sum++;
        }
    }
    cout<<"Thread pi calculation: "<<4*sum/N<<endl;
    cout<<"Time of thread calculation: "<<line.print()<<endl;
    return 0;
}
