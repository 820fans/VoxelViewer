//
// Created by yida on 18-8-19.
//

#include<stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
using namespace std;

int main(){

    int k=1, n= 29;

    vector<int> digits;
    if (n==0) digits.push_back(n);
    while(n>0){
        digits.push_back(n%10);
        n/=10;
    }

    int sum = 0;
    for(int i=digits.size()-1;i>=0;i--){
        if (i>0){
            sum += digits[i]*i*pow(10, i-1);
            if(digits[i] == k){
                int val = 0;
                for(int j=i-1;j>=0;j--)
                    val = val*10 + digits[j];
                sum+=val;
                sum++;
            }
        }
        else{
            sum += digits[i] > k;
        }
    }
    printf("%d\n", sum);

    return 0;
}
