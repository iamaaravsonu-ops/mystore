#include <iostream>
 using namespace std; int main(){
     int a,b,c;
     cout<<"type 1st no  : ";
     cin>> a;
     cout<<"type 2nd no :";
     cin>> b;
     cout<<"type 3rd no  :";
     cin>> c;
     if(a>b) { if (a>c) {cout<<a<<" is greatest";
    } else {cout<<c<< " is greatest";} }

    else { if (b>c) {cout<<b<< " is greatest";}
    else {cout<<c<< " is greatest";}
    }

    
 }
