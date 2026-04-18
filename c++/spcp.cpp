#include <iostream>
 using namespace std; int main() { 
        int cp;
        cout<<"Enter cost price -:";
        cin>>cp;
        int sp;
        cout<<"Enter selling price -:";
        cin>>sp;
        if(cp>=sp) {cout<<"you are in loss by  ";
        cout<<sp - cp; cout<<" ruppes ";}
        else{ cout<<" you are in profit by  ";
        cout<<sp-cp; cout<<" ruppes ";
        }


       }