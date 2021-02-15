# Работа 1. Исследование гамма-коррекции
автор: Тишин В.В.  
дата: 15.02.2021

Задание
-------
1. Сгенерировать серое тестовое изображение $I_1$ в виде прямоугольника размером 768х60 пикселя с плавным изменение пикселей от черного к белому, одна градация серого занимает 3 пикселя по горизонтали.
2. Применить  к изображению $I_1$ гамма-коррекцию с коэффициентом из интервала 2.2-2.4 и получить изображение $G_1$ при помощи функци pow.
3. Применить  к изображению $I_1$ гамма-коррекцию с коэффициентом из интервала 2.2-2.4 и получить изображение $G_2$ при помощи прямого обращения к пикселям.
4. Показать визуализацию результатов в виде одного изображения (сверху вниз $I_1$, $G_1$, $G_2$).
5. Сделать замер времени обработки изображений в п.2 и п.3, результаты отфиксировать в отчете.

Результаты
----------
![](lab01.jpg)

Рис. 1. Результаты работы программы (сверху вниз $I_1$, $G_1$, $G_2$)

### Сравнение времени выполнения:
Время выполнения гамма-коррекции (функция pow): 407 ms.  
Время выполнения гамма-коррекции (прямое обращение): 1694 ms.

При использовании встроенной в opencv функции pow время применения гамма-коррекции уменьшается в 4 раза, что говорит о высокой степени оптимизации данной библиотеки.

Текст программы
---------------
```cpp
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

    // Concatenate images
    Mat result;
    vconcat(lines, image_gcp, result);
    vconcat(result, image_gc, result);

    // Save result
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
@cpp_source@
```
