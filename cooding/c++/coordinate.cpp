#include <iostream>
 using namespace std; int main(){
     int x,y;
     cout<<"type x co-ordinate : ";
     cin>> x;
     cout<<"type y co-ordinate :";
     cin>> y;
    if( x>0 and y>0){ 
        cout<<" the co-ordinate lies in 1st quadrant";
    }
    else if(x<0 and y>0  ){
         cout<<" the co-ordinate lies in 2nd quadrant";
    }
    else if( x<0 ){ 
        cout<<" the co-ordinate lies in 3rd quadrant";
    }
   else if(x>0){
         cout<<" the co-ordinate lies in 4th quadrant";
    }
    }