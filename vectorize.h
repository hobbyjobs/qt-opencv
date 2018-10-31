#ifndef VECTORIZE_H
#define VECTORIZE_H

#include <vector>
#include <opencv2/opencv.hpp>

class CVectorize
{
public:
    CVectorize();
    ~CVectorize();

    cv::Mat skel(cv::Mat& srcImg);
    std::vector<cv::Point2i> trace(cv::Mat& skelImg);
    cv::Mat write(std::vector<cv::Point2i> contourPoints,int rows,int cols);

private:
    void thinningIteration(cv::Mat& img, int iter);
};

#endif // VECTORIZE_H
