#include <iostream>
#include "opencv2/opencv.hpp"        //使用第三方库opencv 
#include <cmath>

using namespace std;
using namespace cv;

void Bluring(Mat im, int n);        //模糊化 
void Sobel(Mat image);              //检测边缘 

int main()
{
	Mat picture1 = imread("star.jpg");  //读入图片，保存为Mat类的一个对象 
	Mat picture2 = imread("star.jpg");
	Bluring(picture1, 7);            
	Sobel(picture2);
	return 0;
}

void Bluring(Mat im, int n)   //im为要处理的图片，n为方阵的边长         
{
	int t = (n - 1) / 2;
	for (int i = 0; i < im.rows; i++)                              
		for (int j = 0; j < im.cols; j++)
		{
			int blue = 0, green = 0, red = 0;
			int row_begin = i - t, row_end = i + t, col_begin = j - t, col_end = j + t;
			if (row_begin < 0)            //边缘处理                
				row_begin = 0;
			if (row_end > im.rows - 1)
				row_end = im.rows - 1;
			if (col_begin < 0)
				col_begin = 0;
			if (col_end > im.cols - 1)
				col_end = im.cols - 1;
			int num = (row_end - row_begin + 1) * (col_end - col_begin + 1);  
			for (int a = row_begin; a <= row_end; a++)
				for (int b = col_begin; b <= col_end; b++)
				{
					blue += im.at<Vec3b>(a, b)[0];          //计算方阵内的所有颜色值得和 
					green += im.at<Vec3b>(a, b)[1];
					red += im.at<Vec3b>(a, b)[2];
				}
			im.at<Vec3b>(i, j)[0] = blue / num;             //将计算结果取平均 
			im.at<Vec3b>(i, j)[1] = green / num;
			im.at<Vec3b>(i, j)[2] = red / num;
		}
	imwrite("star1.jpg", im);                               //保存为图片 
}

void Sobel(Mat image)   //image为要处理的图片 
{
	Mat im;
	cvtColor(image, im, COLOR_BGR2GRAY, 1);     //将彩色的三通道图灰度化保存为单通道的灰度图 
	imwrite("star2.jpg", im);                   //将灰度图保存下来方便查看 
	Mat IM = im.clone();                        //克隆一个im,方便保存sobel计算后的结果 
	for (int i = 0; i < im.rows; i++)
		for (int j = 0; j < im.cols; j++)
		{
			if (i == 0 || i == im.rows - 1 || j == 0 || j == im.cols - 1)   //对于边缘无需处理，直接赋值 
				IM.at<uchar>(i, j) = im.at<uchar>(i, j);
			else
			{
				int Gx = 0, Gy = 0;                 //sobel算子来计算某个像素的灰度值 
				double G = 0;
				Gx = (im.at<uchar>(i - 1, j + 1) + 2 * im.at<uchar>(i, j + 1) + im.at<uchar>(i + 1, j + 1)) - (im.at<uchar>(i - 1, j - 1) + 2 * im.at<uchar>(i, j - 1) + im.at<uchar>(i + 1, j - 1));
				Gx = (im.at<uchar>(i - 1, j - 1) + 2 * im.at<uchar>(i - 1, j) + im.at<uchar>(i - 1, j + 1)) - (im.at<uchar>(i + 1, j - 1) + 2 * im.at<uchar>(i + 1, j) + im.at<uchar>(i + 1, j + 1));
				G = sqrt(Gx * Gx + Gy * Gy);
				IM.at<uchar>(i, j) = G;
			}
		}

	double sum = 0;
	double scale = 12;
	double average;
	for (int i = 0; i < im.rows; i++)
		for (int j = 0; j < im.cols; j++)
		{
			sum += im.at<uchar>(i, j);            //灰度值的和
		}
	average = sum / (im.rows * im.cols);          //取平均值 
//	cout << average << endl;
	double cutoff = scale * average;             
	double thresh = sqrt(cutoff);

	Mat ret;
	threshold(IM, ret, thresh, 255, THRESH_BINARY);   //图像的二值化 

	imwrite("star3.jpg", ret);                   //保存最终二值化后的图片 
}
