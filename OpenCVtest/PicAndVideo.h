#pragma once
#include<opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

/*���ļ�ʵ�֣�ͼƬת��Ƶ����ƵתͼƬ��ͼƬ����*/

//������Ƶ
void Get_Video(vector<Mat>& img);

//��ƵתͼƬ ���浽res
void VideoToPic(vector<Mat>& picFromVideo, string decodeVideoName);

//ͼƬ����
void handlePicFromVideo(vector<Mat>& resPic, vector<Mat>& picFromVideo);
void Handle_Img(Mat& srcImg, Mat& midImg);
bool Get_ROI(Mat& midImg, Mat& srcImg, Mat& dstImg);
//��λ����
Point2f* Sort_Corner(vector<Point2f>& mc);
vector<Point2f> Get_Center(vector<vector<Point>>& corner);
vector<vector<Point>> Find_Corner(Mat& midImg);
