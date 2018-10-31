#include "vectorize.h"

using namespace std;
using namespace cv;

CVectorize::CVectorize()
{

}


CVectorize::~CVectorize()
{

}

void CVectorize::thinningIteration(Mat& img, int iter)
{
    Mat marker = Mat::zeros(img.size(), CV_8UC1);

    for (int i = 1; i < img.rows-1; i++)
    {
        for (int j = 1; j < img.cols-1; j++)
        {
            uchar p2 = img.at<uchar>(i-1, j);
            uchar p3 = img.at<uchar>(i-1, j+1);
            uchar p4 = img.at<uchar>(i, j+1);
            uchar p5 = img.at<uchar>(i+1, j+1);
            uchar p6 = img.at<uchar>(i+1, j);
            uchar p7 = img.at<uchar>(i+1, j-1);
            uchar p8 = img.at<uchar>(i, j-1);
            uchar p9 = img.at<uchar>(i-1, j-1);

            int A  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                    (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                    (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                    (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
            int B  = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
            int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
            int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

            if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
                marker.at<uchar>(i,j) = 1;
        }
    }

    img &= ~marker;
}

Mat CVectorize::skel(Mat& srcImg)
{
    Mat grayImg, binaryImg;
//    cvtColor(srcImg, grayImg, CV_BGR2GRAY);
    threshold(srcImg, binaryImg, 180, 255, THRESH_BINARY);

    binaryImg /= 255;
    Mat prev = Mat::zeros(binaryImg.size(), CV_8UC1);
    Mat diff;

    do {
        thinningIteration(binaryImg, 0);
        thinningIteration(binaryImg, 1);
        absdiff(binaryImg, prev, diff);
        binaryImg.copyTo(prev);
    }
    while (countNonZero(diff) > 0);

    binaryImg *= 255;
    binaryImg = 255 - binaryImg;

    return binaryImg;

}

vector<cv::Point2i> CVectorize::trace(Mat& skelImg)
{
    vector<Point2i> contourPoints;
    int rows = skelImg.rows;
    int cols = skelImg.cols;
    Mat tempImg = skelImg.clone();
    Mat isVisited = skelImg.clone();

    int offSet[8][2] = {
        {-1,0},
        {-1,1},
        {0,1},
        {1,1},
        {1,0},
        {1,-1},
        {0,-1},
        {-1,-1},
    };

    int directionLookUpTable1[8] = {8,8,2,2,4,4,6,6};
    int directionLookUpTable2[8] = {2,3,4,5,6,7,8,1};

    for (int rr = 0; rr < rows; rr++)
    {
        uchar* data = tempImg.ptr<uchar>(rr);
        uchar* visited = isVisited.ptr<uchar>(rr);
        for (int cc = 0; cc < cols; cc++)
        {
            if ( 0 == data[cc] && 0 == visited[cc])
            {
                bool bDone = false;
                int initDirection, direction;
                initDirection = -1;
                direction = 2;
                Point2i startPoint, currentPoint;
                startPoint.x = cc;
                startPoint.y = rr;
                currentPoint = startPoint;
                contourPoints.push_back(startPoint);


                while (!bDone)
                {
                    bool bfoundNext = false;
                    Point2i nextPoint;

                    for (int nn = 0; nn < 8; nn++)
                    {
                        nextPoint.x = currentPoint.x + offSet[direction-1][1];
                        nextPoint.y = currentPoint.y + offSet[direction-1][0];
                        uchar* data = tempImg.ptr<uchar>(nextPoint.y);
                        uchar* visited = isVisited.ptr<uchar>(nextPoint.y);

                        int kk = int(nextPoint.x);

                        if (0 == data[kk])
                        {
                            if ( initDirection == -1 )
                            {
                                initDirection = direction;
                            }
                            else if (currentPoint.x == startPoint.x &&
                                     currentPoint.y == startPoint.y &&
                                     initDirection == direction)
                            {
                                bDone = true;
                                bfoundNext = true;
                                break;
                            }

                            bfoundNext = true;
                            direction = directionLookUpTable1[direction-1];
                            currentPoint.x = nextPoint.x;
                            currentPoint.y = nextPoint.y;
                            contourPoints.push_back(currentPoint);
                            visited[kk] = 255;
                            break;
                        }
                        else
                        {
                            direction = directionLookUpTable2[direction-1];
                        }
                    }

                    if (!bfoundNext)
                    {
                        bDone = true;
                    }
                }
            }

        }

    }

    return contourPoints;

}

Mat CVectorize::write(vector<Point2i> contourPoints, int rows,int cols)
{
    Mat outputImg = Mat::ones(rows,cols,CV_8UC1);
    outputImg *= 255;
    vector<Point2i>::iterator iter;

    for (iter = contourPoints.begin(); iter < contourPoints.end(); iter++)
    {
        int rr = iter->y;
        int cc = iter->x;

        uchar* data = outputImg.ptr<uchar>(rr);
        data[cc] = 0;
    }

    return outputImg;

}




