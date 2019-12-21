#include<opencv2\opencv.hpp>
#include <cmath>
#include <string>
#include<io.h>
#include "windows.h"

using namespace cv;
using namespace std;

void getFiles(string path, vector<string>& files)
{
	//文件句柄
	long long   hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

int main()
{
	char * filePath = "C:\\Users\\zzh\\Pictures\\gai";
	vector<string> files;
	getFiles(filePath, files);
	char str[100];
	int size = files.size();
	for (int i = 0; i < size; i++)
	{
		cout << files[i].c_str() << endl;
	}

	int num;
	cout << "输入要测试的文件名：" << endl;
	cin >> num;
	string imgpath = files[num];
	cout << files[num] << endl;
	Mat src = imread(imgpath, 1);

	Mat edge, grayImage, img;
	resize(src, img, Size(512, 512));
	cvtColor(img, grayImage, COLOR_BGR2GRAY);

	Mat grad_y, abs_grad_y;
	Sobel(grayImage, grad_y, CV_16S, 0, 1, 1, 2, 2, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	Mat img1 = img;
	int s = 0;
	int n = 0;
	for (int col = 0; col < abs_grad_y.cols; col++)
	{
		int i = 0;
		for (int row = 0; row < abs_grad_y.rows; row++)
		{
			if (i == 0)
				if (abs_grad_y.at<uchar>(row, col) > 40)
				{
					abs_grad_y.at<uchar>(row, col) = uchar(255);
					i += 1;
					s += row;
					n += 1;
				}
				else
					abs_grad_y.at<uchar>(row, col) = uchar(0);
			else
				abs_grad_y.at<uchar>(row, col) = uchar(0);
		}
	}

	int ave = s / n;
	int d = 0;
	int nn = 0;
	int n1 = 0;
	int n2 = 0;
	for (int col = 0; col < abs_grad_y.cols; col++)
	{
		for (int row = 0; row < abs_grad_y.rows; row++)
		{
			if (abs_grad_y.at<uchar>(row, col) == 255)
			{
				d += abs(row - ave);
				nn += 1;
				if (nn == 1)
					n1 = col;
				else if (nn == n)
					n2 = col;
			}
		}
	}

	d /= n;
	cout << d << endl;
	if (d > 3)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "不合格。" << endl;
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "合格。" << endl;
	}

	for (int col = 0; col < abs_grad_y.cols; col++)
	{
		for (int row = 0; row < abs_grad_y.rows; row++)
		{
			if (abs_grad_y.at<uchar>(row, col) == 255)
				if (d > 3)
				{
					img1.at<Vec3b>(row, col)[0] = uchar(0);
					img1.at<Vec3b>(row, col)[1] = uchar(0);
					img1.at<Vec3b>(row, col)[2] = uchar(255);
				}
				else
				{
					img1.at<Vec3b>(row, col)[0] = uchar(0);
					img1.at<Vec3b>(row, col)[1] = uchar(255);
					img1.at<Vec3b>(row, col)[2] = uchar(0);
				}
		}
	}
	imshow("Image", img1);

	waitKey(0);
	return 0;
}
