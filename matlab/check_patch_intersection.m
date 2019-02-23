% ray origin
% ray direction
% vertex 1
% vertex 2
% vertex 3
% vertex 4

[x,y,z]=textread('../output/patch.txt','%f%f%f');

vertex=[x(3:end) y(3:end) z(3:end)];
vertex=[vertex; vertex(1,:)];
plot3(vertex(:,1),vertex(:,2),vertex(:,3),'g');
hold on
v1=[x(1) y(1) z(1)];
v2=[x(2) y(2) z(2)];
DrawLine(v1,v2,'b');
