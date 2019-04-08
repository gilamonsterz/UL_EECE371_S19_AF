clc, clear;

output = csvread("puTTYlog1.csv");

output0000 = output(1:32,1:32);
output0001 = output(33:64,1:32);
output0010 = output(65:96,1:32);
output0011 = output(97:128,1:32);
output0100 = output(129:160,1:32);
output0101 = output(161:192,1:32);
output0110 = output(193:224,1:32);
output0111 = output(225:256,1:32);
output1000 = output(257:288,1:32);
output1001 = output(289:320,1:32);
output1010 = output(321:352,1:32);
output1011 = output(353:384,1:32);
output1100 = output(385:416,1:32);
output1101 = output(417:448,1:32);
output1110 = output(449:480,1:32);
output1111 = output(481:512,1:32);

outputFinal = [output0000 output0001 output0010 output0011 ; output0100 output0101 output0110 output0111;output1000 output1001 output1010 output1011;output1100 output1101 output1110 output1111];

imwrite(uint8(outputFinal), "output.jpg");
