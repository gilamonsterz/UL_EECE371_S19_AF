import pandas as pd
import numpy

#pmdata is the file where the final output is written, to be later copied into CPP code
pmdata = open("progmemdata.txt","w");

# reading image file, at the moment it's done from a csv which can be easily replaced with any image read in python
image1 = pd.read_csv("greyPic128x128.csv", index_col=None, header=None);
#transforming it into numpy array
img = numpy.array(image1);

#nested loops to generate data in the form of 2D values of m x n size: {{,,,},{,,,},{,,,},{,,,]}
pmdata.write('{')
d1,d2 = img.shape
for i in range(d1):
    pmdata.write('{')
    for j in range(d2):
        pmdata.write(str(img[i][j]))
        if j!= d2-1:
            pmdata.write(',')
    pmdata.write('}')
    if i!=d1-1:
        pmdata.write(',')
pmdata.write('}')
