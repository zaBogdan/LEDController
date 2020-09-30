#include <iostream>
#include <cstring>
using namespace std;

//Check if everything is alright before upload it 
void handleUDPPayload(char incPkt[],int &r,int &g,int &b){
  char *p, v[10][10];
  p = strtok(incPkt, ":");
  int i=0;
  while(p){
    strcpy(v[i++], p);
    p = strtok(NULL, ":");
  }
  for(int i=0;i<8;i+=2){
    if(v[i] && v[i][0]=='r')
      r = atoi(v[i+1])*4;
    if(v[i] && v[i][0]=='g')
      g = atoi(v[i+1])*4;
    if(v[i] && v[i][0]=='b')
      b = atoi(v[i+1])*4;
    if(v[i] && !strcmp(v[i], "cmd")){
      if(v[i+1] && !strcmp(v[i+1], "shutdown")){
      //do sth
      }
      if(v[i+1] && !strcmp(v[i+1], "start")){
        //do sth
      }
    }
  }
}
int main(){
    char incPkt[255] = "r:256:g:0:b:0:cmd:xxxx";
    int r=0,g=0,b=0;
    handleUDPPayload(incPkt,r,g,b);
    printf("R: %d , G: %d B: %d", r,g,b);
    cout << endl;
    return 0;
}