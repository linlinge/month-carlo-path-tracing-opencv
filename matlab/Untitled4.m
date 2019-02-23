clear
clc

syms A B C D
dat=[
    0.184751 0.033212 0.133627
0.243361 0.033212 0.021234
0.179809 0.023836 0.019695];

rst=solve([dat(1,1)*A+dat(1,2)*B+dat(1,3)*C+D ==0 ,dat(2,1)*A+dat(2,2)*B+dat(2,3)*C+D ==0,dat(3,1)*A+dat(3,2)*B+dat(3,3)*C+D ==0],[A,B,C,D]);

