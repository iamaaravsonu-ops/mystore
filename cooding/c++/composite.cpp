// #include <iostream>
//  using namespace std; int main() {
//      int n;
//      cout<<"enter the number:- ";
//      cin>>n;
//      for( int i=2;i<=n/2;i++)  
//         {if( n%i==0){ cout<<" composite ";
//             break;26
//          } }
//     }
// another step
#include <iostream>
 using namespace std; int main() {
     int n;
     cout<<"enter the number:- ";
     cin>>n;
     bool flag = true; // true means it is prime
     for( int i=2;i<=n/2;i++)  
        {if( n%i==0){flag = false;
            break;
         } 
        }
        if ( n==1) cout<<"nither prime nor coposite  ";
        else if ( flag == true) cout<<"prime  ";
        else cout<<"composite  ";
    }