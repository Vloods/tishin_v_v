/**
    lab-01: Gamma correction research
    @file main.cpp
    @author Vlad Tishin
    @version 1.0 15/02/21
*/

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <ctime>
using namespace cv;

/**
 * Applies a gamma correction with the specified coefficient to the image using the pow function.
 * @param image - image for gamma correction.
 * @param gamma_ - gamma correction coefficient.
 * @return image with gamma correction
 */
Mat gam_corr_pow(const Mat& image, double gamma_);

/**
 * Applies a gamma correction with the specified coefficient to the image using direct access to pixels.
 * @param image - image for gamma correction.
 * @param gamma_ - gamma correction coefficient.
 * @return image with gamma correction
 */
Mat gam_corr(const Mat& image, double gamma_);

/**
 * Applies a gamma correction with the specified coefficient to the image using direct access to pixels with TUP.
 * @param image - image for gamma correction.
 * @param gamma_ - gamma correction coefficient.
 * @return image with gamma correction
 */
Mat gam_corr_tup(const Mat& image, double gamma_);

// Controls operation of the program.
int main() {
    // Generates a gradient black-white
    Mat lines(60, 768, CV_8U, Scalar(0));
    for (int r = 0; r < lines.cols; r++)
    {
        lines.col(r).setTo((int) (r/3));
    }

    unsigned int start, end;
    // Checking time
    start = clock();
    Mat image_gcp = gam_corr_pow(lines, 2.4);
    end = clock();
    std::cout << "Время выполнения гамма-коррекции (функция pow): " << end - start << " ms." << std::endl;

    start = clock();
    Mat image_gc = gam_corr(lines, 2.4);
    end = clock();
    std::cout << "Время выполнения гамма-коррекции (прямое обращение): " << end - start << " ms." << std::endl;

    start = clock();
    Mat image_tup = gam_corr_tup(lines, 2.4);
    end = clock();
    std::cout << "Время выполнения гамма-коррекции (прямое обращение с TUP): " << end - start << " ms." << std::endl;

    // Concatenate images
    Mat result;
    vconcat(lines, image_gcp, result);
    vconcat(result, image_gc, result);

    // Save result
    imwrite("test.jpg", image_tup);
    imwrite("result.jpg", result);
}

// Applies a gamma correction with the specified coefficient to the image using the pow function.
Mat gam_corr_pow(const Mat& image, double gamma_){
    Mat image_float;
    image.convertTo(image_float, CV_64F);
    image_float /= 255.0;
    cv::pow(image_float, gamma_, image_float);
    image_float *= 255.0;
    image_float.convertTo(image_float, CV_8U);

    return image_float;
}

// Applies a gamma correction with the specified coefficient to the image using direct access to pixels.
Mat gam_corr(const Mat& image, double gamma_){
    Mat image_float;
    image.convertTo(image_float, CV_64F);
    for(int i=0; i<image_float.rows; i++)
        for(int j=0; j<image_float.cols; j++)
            image_float.at<double>(i, j) = (pow(image_float.at<double>(i, j) / 255.0, gamma_) * 255.0);
    image_float.convertTo(image_float, CV_8U);

    return image_float;
}

// Applies a gamma correction with the specified coefficient to the image using direct access to pixels with TUP.
Mat gam_corr_tup(const Mat& image, double gamma_){
    Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for( int i = 0; i < 256; ++i)
        p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma_) * 255.0);
    Mat res = image.clone();
    LUT(image, lookUpTable, res);

    return res;
}
