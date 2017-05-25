#include "stdafx.h"

#include "MathHelpers.h"


void SoftSmooth(int *input, int *output, int n, int window)
{
   int i,j,z,k1,k2,hw;
   double tmp;
   if(fmod(window,2)==0) window++;
   hw=(window-1)/2;
   output[0]=input[0];

   for (i=1;i<n;i++){
       tmp=0;
       if(i<hw){
           k1=0;
           k2=2*i;
           z=k2+1;
       }
       else if((i+hw)>(n-1)){
           k1=i-n+i+1;
           k2=n-1;
           z=k2-k1+1;
       }
       else{
           k1=i-hw;
           k2=i+hw;
           z=window;
       }

       for (j=k1;j<=k2;j++){
           tmp=tmp+input[j];
       }
       output[i]=tmp/z;
   }
}
