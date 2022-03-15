#pragma once
#include<opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <fstream>

using namespace cv;
using namespace std;

/*���ļ�ʵ�֣�
1. ��ȡ�������ļ�
2. �������ļ�����*/

//��ȡ�û�����Ķ������ļ�
bool readBinFile(string& str, const string& encodeBinFileNme);

//����ȡ���ַ���ת��Ϊ01�����������ַ���
void string01(string& data, string& str, int& img_count);

//��ά��ͼ����
void drawCode(string code, vector<Mat>& img);

//��λ���λ��ƣ��úڰ׺�3�����ο����
void DrawBlock(Mat img, Point tr);

//ȷ��������ɫ
void DrawColor(string code, int& R, int& G, int& B, int& key);
