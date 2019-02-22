clear
clc
[dat(:,1),dat(:,2),dat(:,3)]=textread('../output/bounding_box_check.txt','%f%f%f');
tl=dat(end-1,:);
br=dat(end,:);
dat(end-1:end,:)=[];

dat=[dat;dat(1,:)];
plot3(dat(:,1),dat(:,2),dat(:,3));
DrawBox(tl,br,'g');



