close all
clear
clc
%[v,x,y,z]=textread('./room.txt','%s%f%f%f');
[ox,oy,oz,dx,dy,dz]=textread('./ray.txt','%f%f%f%f%f%f');

%plot3(x,y,z,'*')
 
for i=1:length(ox)
    hold on
    plot_ray([ox(i) oy(i) oz(i)],[dx(i) dy(i) dz(i)]);
end
xlabel('x')
ylabel('y')
zlabel('z')