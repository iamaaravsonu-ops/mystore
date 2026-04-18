#include <iostream>
 using namespace std; int main(){
     int a,b,c;
     cout<<"type 1st side length : ";
     cin>> a;
     cout<<"type 2nd side length :";
     cin>> b;
     cout<<"type 3rd side length :";
     cin>> c;
    if((a+b)>c && (b+c)>a && (c+a)>b) {cout<<"this is triangle";}
    else{cout<<"this is not triangle";}
 }
