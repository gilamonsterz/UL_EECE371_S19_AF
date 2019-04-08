clc, clear;

new = imread("great-hornbill128x128.jpg");
greyPic = rgb2gray(new);

imwrite(greyPic,"newGrey.jpg");

csvwrite("greyPic128x128.csv",greyPic);