function plot_ray(origin,direction)
plot3(origin(1),origin(2),origin(3),'o');
destination=origin+direction*10;
temp=[origin;direction];
hold on
plot3(temp(:,1),temp(:,2),temp(:,3),'color',[rand rand rand]);

