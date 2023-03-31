#include <pthread.h>
#include <opencv2/opencv.hpp>

using namespace cv;

struct ThreadData {
    Mat image;
    Mat result;
    int startRow;
    int endRow;
};

void* sobelFilter(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    Mat image = data->image;
    Mat result = data->result;
    int startRow = data->startRow;
    int endRow = data->endRow;
    
    for (int i = startRow; i < endRow; i++) {
        for (int j = 1; j < image.cols - 1; j++) {
            int gx = (image.at<uchar>(i - 1, j - 1) + 2 * image.at<uchar>(i, j - 1) + image.at<uchar>(i + 1, j - 1)) -
                     (image.at<uchar>(i - 1, j + 1) + 2 * image.at<uchar>(i, j + 1) + image.at<uchar>(i + 1, j + 1));
            int gy = (image.at<uchar>(i - 1, j - 1) + 2 * image.at<uchar>(i - 1, j) + image.at<uchar>(i - 1, j + 1)) -
                     (image.at<uchar>(i + 1, j - 1) + 2 * image.at<uchar>(i + 1, j) + image.at<uchar>(i + 1, j + 1));
            int sum = abs(gx) + abs(gy);
            result.at<uchar>(i, j) = sum > 255 ? 255 : sum;
        }
    }
    pthread_exit(NULL);
}

int main() {
    Mat image = imread("kat.jpg", IMREAD_GRAYSCALE);
    Mat result = Mat::zeros(image.size(), CV_8UC1);
    
    int numThreads = 2;
    int rowsPerThread = image.rows / numThreads;
    
    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];

    auto start = std::chrono::system_clock::now();
    
    for (int i = 0; i < numThreads; i++) {
        threadData[i].image = image;
        threadData[i].result = result;
        threadData[i].startRow = i * rowsPerThread;
        threadData[i].endRow = (i + 1) * rowsPerThread;
        pthread_create(&threads[i], NULL, sobelFilter, (void*) &threadData[i]);
    }
    
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    auto end = std::chrono::system_clock::now();
    std::cout << "Время выполнения программы с " << numThreads << " потоками равняется " << (end - start).count() << " ms" << std::endl;    
    imwrite("result.jpg", result);
    
    return 0;
}