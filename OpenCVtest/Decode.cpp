#include "Decode.h"

void Help_Decode(Scalar& color, string& code) {
	if (color[0] > 180 && color[1] > 180 && color[2] > 180) {
		code += "1";//白
	}
	else {
		code += "0";
	}
}

//解码的主要函数
string Decode(Mat& img) {

	string code;

	//中间的部分
	for (int col = 0; col < 72; col++) {
		for (int row = 0; row < 48; row++) {
			Point pos(row * 10, col * 10), center(5, 5), tr(120, 0);
			Scalar color = img.at<Vec3b>(tr + pos + center);//at取出在(i,j)位置的三通道像素值
			Help_Decode(color, code);
		}
	}
	//最下面部分
	for (int col = 0; col < 48; col++) {
		for (int row = 48; row < 60; row++) {
			Point pos(row * 10, col * 10), center(5, 5), tr(120, 120);
			Scalar color = img.at<Vec3b>(tr + pos + center);//at取出在(i,j)位置的三通道像素值
			Help_Decode(color, code);
		}
	}
	//最上面部分
	for (int col = 0; col < 48; col++) {
		for (int row = 0; row < 12; row++) {
			Point pos(row * 10, col * 10), center(5, 5), tr(0, 120);
			Scalar color = img.at<Vec3b>(tr + pos + center);//at取出在(i,j)位置的三通道像素值
			Help_Decode(color, code);
		}
	}

	return code;
}

//二进制转字符
void BinToText(string& text, string& resCode) {
	size_t cnt = resCode.length() / 8;
	for (size_t i = 0; i < cnt; i++) {
		int sum = 0;
		for (int j = 0; j < 8; j++) {
			sum += (resCode[i * 8 + j] - '0') * pow(2, 7 - j);
		}
		text += (unsigned char)sum;
	}
}