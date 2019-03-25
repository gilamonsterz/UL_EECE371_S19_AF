clc,clear;

origPic = imread("great-hornbill1.jpg");
greyPic = rgb2gray(origPic);

%%various filters
edgeHorz = [-1 -1 -1; 2 2 2; -1 -1 -1];
edgeVert = [-1 2 -1; -1 2 -1; -1 2 -1];
edge45 =  [-1 -1 2; -1 2 -1; 2 -1 -1];
edge135 = [2 -1 -1; -1 2 -1; -1 -1 2];
edge8 = [-1 -1 -1; -1 8 -1; -1 -1 -1];
edge4 = [0 1 0; 1 -4 1; 0 1 0];
edge1 = [1 0 -1; 0 0 0; -1 0 1];
gauss = [0.0625 0.125 0.0625;0.125 0.25 0.125;0.0625 0.125 0.0625];

C = zeros(size(greyPic));

x = 0;


%%Pass 1 blurr
for j = 4:1:(size(greyPic,1)-4)
    for k = 4:1:(size(greyPic,2)-4)
        for p = 1:1:3
            for q = 1:1:3
                y(p,q) = double(greyPic(j+p-2,k+q-2));
                u(p,q) = gauss(p,q);
                g(p,q) = y(p,q).*u(p,q);
                x = x + y(p,q)*u(p,q);
            end
        end
        C(j,k) = x;
        x = 0;        
    end
end

%%Pass 2 blurr
for j = 4:1:(size(C,1)-4)
    for k = 4:1:(size(C,2)-4)
        for p = 1:1:3
            for q = 1:1:3
                y(p,q) = double(C(j+p-2,k+q-2));
                u(p,q) = gauss(p,q);
                g(p,q) = y(p,q).*u(p,q);
                x = x + y(p,q)*u(p,q);
            end
        end
        C2(j,k) = x;
        x = 0;        
    end
end

%%Pass 3 edge
for j = 4:1:(size(C2,1)-4)
    for k = 4:1:(size(C2,2)-4)
        for p = 1:1:3
            for q = 1:1:3
                y(p,q) = double(C2(j+p-2,k+q-2));
                u(p,q) = edge8(p,q);
                g(p,q) = y(p,q).*u(p,q);
                x = x + y(p,q)*u(p,q);
            end
        end
        C3(j,k) = x;
        x = 0;        
    end
end

C3=uint8(C3);

imwrite(greyPic,"greyPic.jpg");
imwrite(C3,"2gauss1edge1.jpg");
