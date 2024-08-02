#include <fstream>
#include <cstdlib>

using namespace std;

int N=1000, M=499500;

int main()
 {
 ofstream dane("edges1000.in");
 dane<<N<<' '<<M<<endl;
 srand(1);
 for(int i=1; i<=N; ++i)
   for(int j=i+1; j<=N; ++j) dane<<i<<' '<<j<<' '<<rand()%999+1<<endl;
 dane.close();
 return 0;
 }