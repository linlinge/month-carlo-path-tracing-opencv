clear
clc
close all
tl=[0,0,0];
br=[1,1,1];
DrawBox(tl,br,'g')

hold on
P_start=[0.5,0.5,0.5]
P_end=P_start+[0, 0, -3.5]
P=[P_start;P_end];
plot3(P(:,1),P(:,2),P(:,3));