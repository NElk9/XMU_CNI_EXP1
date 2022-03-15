#include<opencv2/opencv.hpp>
#include <iostream>
#include <string>

#include "Encode.h"
#include "Decode.h"
#include "PicAndVideo.h"

using namespace cv;
using namespace std;

int main()
{

    cout << "请输入您想编码的二进制文件名（如test.bin）：" << endl;
    string encodeBinFileName;
    cin >> encodeBinFileName;

    //读取二进制文件
    string str;//记录读取的字符串
    bool flag = readBinFile(str, encodeBinFileName);
    if (flag == false) {
        exit(0);
    }

    //转化为01字符串存入data
    string data;//存储二进制编码
    int img_cnt;
    string01(data, str, img_cnt);

    //cout << img_cnt << endl;
    //cout << data.substr(0, 10) << endl;

    //编写二维码
    //每次截取9184个位编写为一张二维码图片，若最后不足9184位则只把剩下的传输过去
    vector<Mat>img;//储存图片
    for (int i = 0; i < img_cnt; i++) {
        int begin = i * 9184;
        int end = 9184;
        if (begin + end > data.size()) {
            end = data.size() - begin + 1;
        }
        drawCode(data.substr(begin, end), img);
    }

    //图片转视频
    Get_Video(img);

    //视频转图片
    cout << "请输入您想解码的视频名（如Video.mp4）：" << endl;
    string decodeVideoName;
    cin >> decodeVideoName;

    cout << "请输入您保存解码后的二进制文件名（如res.bin）：" << endl;
    string resBinFileName;
    cin >> resBinFileName;

    vector<Mat> picFromVideo;//保存从视频提取出的图片
    VideoToPic(picFromVideo, decodeVideoName);//得到图片

    //图片解码：
    //图片处理，提取的二维码存到resPic
    vector<Mat> resPic;
    handlePicFromVideo(resPic, picFromVideo);
    
    //对resPic的二维码解码到resCode字符串
    string resCode = "";
    for (vector<Mat>::iterator it = resPic.begin(); it != resPic.end(); it++) {
        string tem = Decode(*it);//得到每张图片的编码
        if (tem.empty()) {
            continue;
        }
        resCode += tem;
    }

    //二进制转字符
    string text = "";
    BinToText(text, resCode);

    //写入文件
    ofstream ofs(resBinFileName, ios::binary | ios::out);
    if (!ofs.is_open()) {
        cout << "fail to open resBinFile" << endl;
    }
    ofs << text;
    cout << "写入成功！" << endl;

    return 0;
}
