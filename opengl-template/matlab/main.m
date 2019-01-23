clear
clc
[x,y,z]=textread('../output/1.txt','%f%f%f');
plot3(x,y,z,'*');
xlabel('x');
ylabel('y');