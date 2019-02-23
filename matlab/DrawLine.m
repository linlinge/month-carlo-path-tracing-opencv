function DrawLine(v1,v2,color)
if size(v1,1) > size(v1,2)
    v=[v1';v2'];
else
    v=[v1;v2];
end
plot3(v(:,1),v(:,2),v(:,3),color);
hold on
plot3(v1(1),v1(2),v1(3),'ro');
plot3(v2(1),v2(2),v2(3),'go');