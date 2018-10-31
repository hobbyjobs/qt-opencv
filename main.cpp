#include <iostream>
#include <opencv2/opencv.hpp>
#include "vectorize.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{

    Mat srcImg = imread(argv[1],IMREAD_UNCHANGED);
    srcImg = 255 - srcImg;

    CVectorize map;
    Mat skelImg = map.skel(srcImg);    
    vector<Point2i> contourPoints;
    contourPoints = map.trace(skelImg);
    Mat outputImg = map.write(contourPoints,srcImg.rows,srcImg.cols);

    imshow("src",srcImg);
    imshow("out",outputImg);
    waitKey(0);
    return 0;
}
