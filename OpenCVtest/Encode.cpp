#include "Encode.h"

bool readBinFile(string& str, const string& encodeBinFileNme) {
    ifstream fpr(encodeBinFileNme, ios::in | ios::binary);
    if (!fpr.is_open())
    {
        cout << "Open bin_file error!" << endl;
        return false;
    }

    stringstream ss;

    ss << fpr.rdbuf();
    str = ss.str();

    return true;
}

//将读取的字符串转化为01二进制数字字符串
void string01(string& data, string& str, int& img_count) {
    int length_char, length_bit;
    length_char = str.size();
    length_bit = length_char * 8;

    img_count = length_bit / 9184;
    if (length_bit % 9184 != 0) {
        img_count++;//判断几张图
    }

    int binary_int[8] = { 0 };//暂时存放字符的二进制编码
    str += "\0";

    //传输时 字节大端，位小端
    for (int i = 0; i < length_char; i++) {
        for (int j = 7; j >= 0; j--) {
            binary_int[j] = str[i] % 2;
            str[i] /= 2;
        }
        for (int j = 0; j < 8; j++) {
            if (binary_int[j] == 1) {
                data.append(1, '1');
            }
            else if (binary_int[j] == 0) {
                data.append(1, '0');
            }
        }
    }
    data += "\0";//结束符
    cout << "编码后的二进制文件字符个数为：" << data.length() << endl;
    cout << "二进制的位数为：" << data.length() * 8 << endl;

}

//定位矩形绘制，用黑白黑3个矩形框叠加
void DrawBlock(Mat img, Point tr) {
    rectangle(img, Point(0, 0) + tr, Point(105, 105) + tr, Scalar(0, 0, 0), -1);//最外面的矩形框，RGB（黑色）
    rectangle(img, Point(15, 15) + tr, Point(90, 90) + tr, Scalar(255, 255, 255), -1);//中间的矩形框,RGB（白色）
    rectangle(img, Point(30, 30) + tr, Point(75, 75) + tr, Scalar(0, 0, 0), -1);//最里面的矩形框,RGB（黑色）
}

//确定像素颜色
void DrawColor(string code, int& R, int& G, int& B, int& key) {
    if (code[key] == '0') { R = 0; G = 0; B = 0; }//黑
    if (code[key] == '1') { R = 255; G = 255; B = 255; }//白
    key = key + 1;
}

//二维码图绘制
void drawCode(string code, vector<Mat>& img) {
    //cout << code.size() << endl;
    int key = 0;//code数组的编码序号
    int R = 255, G = 255, B = 255;//存放三通道的值
    Mat src(780, 780, CV_8UC3, Scalar(255, 255, 255));//像素点矩阵src（长宽，类型，颜色）
    Point recSize(10, 10);//像素大小

    DrawBlock(src, Point(0 + 30, 0 + 30));//左上
    DrawBlock(src, Point(615 + 30, 0 + 30));//右上
    DrawBlock(src, Point(0 + 30, 615 + 30));//左下
    DrawBlock(src, Point(615 + 30, 615 + 30));//右下

    //中间部分，一列一列涂
    for (int col = 0; col < 72; col++) {//中间一共72列
        for (int row = 0; row < 48; row++) {//中间一共48行
            Point pos(row * 10 + 120 + 30, col * 10 + 30);//确定像素位置,120是因为定位矩形，30是二维码外面部分
            DrawColor(code, R, G, B, key);//计算像素颜色
            rectangle(src, pos, pos + recSize, Scalar(B, G, R), -1);//绘制像素
            //当图像是最后一张时，将src存入img中
            if (key >= code.size()) {
                img.push_back(src);
                return;
            }
        }
    }

    //最下面的部分，一列一列涂
    for (int col = 0; col < 48; col++) {//除了定位矩形一共48列
        for (int row = 48; row < 60; row++) {//一共12行
            Point pos(row * 10 + 120 + 30, col * 10 + 120 + 30);//只能从120开始画，前面是定位的方格
            DrawColor(code, R, G, B, key);//计算像素颜色
            rectangle(src, pos, pos + recSize, Scalar(B, G, R), -1);//绘制像素
            //当图像是最后一张时，将src存入img中
            if (key >= code.size()) {
                img.push_back(src);
                return;
            }
        }
    }

    //最上面的部分，一列一列涂
    for (int col = 0; col < 48; col++) {
        for (int row = 0; row < 12; row++) {
            Point pos(row * 10 + 30, col * 10 + 120 + 30);//只能从120开始画，前面是定位的方格
            DrawColor(code, R, G, B, key);//计算像素颜色
            rectangle(src, pos, pos + recSize, Scalar(B, G, R), -1);//绘制像素
            //当图像是最后一张时，将src存入img中
            if (key >= code.size()) {
                img.push_back(src);
                return;
            }
        }
    }
    img.push_back(src);
}