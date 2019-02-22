[x,y,z]=textread('../output/kd-tree-test.txt','%f%f%f');
dat=[x,y,z];
DrawBox(dat(1,:),dat(2,:),'r');
DrawBox(dat(3,:),dat(4,:),'g');
DrawBox(dat(5,:),dat(6,:),'b');