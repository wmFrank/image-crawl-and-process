#include <iostream>
#include "opencv2/opencv.hpp"        //ʹ�õ�������opencv 
#include <cmath>

using namespace std;
using namespace cv;

void Bluring(Mat im, int n);        //ģ���� 
void Sobel(Mat image);              //����Ե 

int main()
{
	Mat picture1 = imread("star.jpg");  //����ͼƬ������ΪMat���һ������ 
	Mat picture2 = imread("star.jpg");
	Bluring(picture1, 7);            
	Sobel(picture2);
	return 0;
}

void Bluring(Mat im, int n)   //imΪҪ�����ͼƬ��nΪ����ı߳�         
{
	int t = (n - 1) / 2;
	for (int i = 0; i < im.rows; i++)                              
		for (int j = 0; j < im.cols; j++)
		{
			int blue = 0, green = 0, red = 0;
			int row_begin = i - t, row_end = i + t, col_begin = j - t, col_end = j + t;
			if (row_begin < 0)            //��Ե����                
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
					blue += im.at<Vec3b>(a, b)[0];          //���㷽���ڵ�������ɫֵ�ú� 
					green += im.at<Vec3b>(a, b)[1];
					red += im.at<Vec3b>(a, b)[2];
				}
			im.at<Vec3b>(i, j)[0] = blue / num;             //��������ȡƽ�� 
			im.at<Vec3b>(i, j)[1] = green / num;
			im.at<Vec3b>(i, j)[2] = red / num;
		}
	imwrite("star1.jpg", im);                               //����ΪͼƬ 
}

void Sobel(Mat image)   //imageΪҪ�����ͼƬ 
{
	Mat im;
	cvtColor(image, im, COLOR_BGR2GRAY, 1);     //����ɫ����ͨ��ͼ�ҶȻ�����Ϊ��ͨ���ĻҶ�ͼ 
	imwrite("star2.jpg", im);                   //���Ҷ�ͼ������������鿴 
	Mat IM = im.clone();                        //��¡һ��im,���㱣��sobel�����Ľ�� 
	for (int i = 0; i < im.rows; i++)
		for (int j = 0; j < im.cols; j++)
		{
			if (i == 0 || i == im.rows - 1 || j == 0 || j == im.cols - 1)   //���ڱ�Ե���账��ֱ�Ӹ�ֵ 
				IM.at<uchar>(i, j) = im.at<uchar>(i, j);
			else
			{
				int Gx = 0, Gy = 0;                 //sobel����������ĳ�����صĻҶ�ֵ 
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
			sum += im.at<uchar>(i, j);            //�Ҷ�ֵ�ĺ�
		}
	average = sum / (im.rows * im.cols);          //ȡƽ��ֵ 
//	cout << average << endl;
	double cutoff = scale * average;             
	double thresh = sqrt(cutoff);

	Mat ret;
	threshold(IM, ret, thresh, 255, THRESH_BINARY);   //ͼ��Ķ�ֵ�� 

	imwrite("star3.jpg", ret);                   //�������ն�ֵ�����ͼƬ 
}
