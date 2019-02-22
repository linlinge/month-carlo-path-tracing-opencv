function DrawBox(tl,br,color)
x1=tl(1);y1=tl(2);z1=tl(3);
x2=br(1);y2=br(2);z2=br(3);
line=[x1 y1 z1;x1 y2 z1];
hold on
plot3(line(:,1),line(:,2),line(:,3),color);
line=[x2 y1 z1;x2 y2 z1];
hold on
plot3(line(:,1),line(:,2),line(:,3),color);
line=[x2 y1 z2;x2 y2 z2];
hold on
plot3(line(:,1),line(:,2),line(:,3),color);
line=[x1 y1 z2;x1 y2 z2];
hold on
plot3(line(:,1),line(:,2),line(:,3),color);

line=[x1 y1 z1;x2 y1 z1];
hold on
plot3(line(:,1),line(:,2),line(:,3),color);
line=[x2 y1 z1;x2 y1 z2];
hold on
plot3(line(:,1),line(:,2),line(:,3),color);
line=[x2 y1 z2; x1 y1 z2];
hold on
plot3(line(:,1),line(:,2),line(:,3),color);
line=[x1 y1 z1;x1 y1 z2];
hold on
plot3(line(:,1),line(:,2),line(:,3),color);

line=[x1 y2 z1;x2 y2 z1];
hold on
plot3(line(:,1),line(:,2),line(:,3),color);
line=[x2 y2 z1;x2 y2 z2];
hold on
plot3(line(:,1),line(:,2),line(:,3),color);
line=[x2 y2 z2;x1 y2 z2];
hold on
plot3(line(:,1),line(:,2),line(:,3),color);
line=[x1 y2 z1;x1 y2 z2];
hold on
plot3(line(:,1),line(:,2),line(:,3),color);

xlabel('x')
ylabel('y');
zlabel('z');
%grid on