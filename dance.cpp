#include <stdio.h>
#include <Windows.h>
#include <iostream>
using namespace std;
//播放声音的头文件
#include <mmsyscom.h>
#pragma comment(lib, "winmm.lib")
//视频转换//1、安装OpenCV（扫描二维码，加西西老师QQ）
#include <opencv2/opencv.hpp>
using namespace cv; //声明opencv的命名空间
int main(void) {
	//1、打开视频文件
	//下载一个mp4视频，与源文件同目录；
	//将mp4转为mp3，与源文件同目录，因为视频转为字符就没有声音了；
	VideoCapture video;
	bool ret = video.open("楼梯科目三.mp4");//ret = return
	if (ret == false) {
		printf("视频文件打开失败！┭┮﹏┭┮\n");
	}
	else {
		printf("视频文件打开成功！(*^▽^*)\n");
	}
	//2、读取视频的核心信息（有多少帧，视频的宽度，高度，播放的速度）
	video.get(CAP_PROP_FRAME_COUNT);
	//帧数
	int frame_count = video.get(CAP_PROP_FRAME_COUNT);
	int fps = video.get(CAP_PROP_FPS);
	//宽高
	int cols = video.get(CAP_PROP_FRAME_WIDTH);
	int rows = video.get(CAP_PROP_FRAME_HEIGHT);
	//定义抽样的尺度
	int w_size = 9;
	int h_size = 20;
	//准备 每10行，每5列抽取一个点，转换成一个字符
	//原理：很白 -> 空格
	char char_imgs[] = "`.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@@@@@@@@@@@@@@@";//0 - 255,分成69级
	int len = sizeof(char_imgs) / sizeof(char_imgs[0]);
	//每个像素点的灰度值 除以20 作为字符的序号

	//控制台的窗口
	int width = cols / w_size;
	int height = rows / h_size;
	// mode con cols = 100 lines = 80
	char cmd[128];
	sprintf_s(cmd, sizeof(cmd), "mode con cols = %d lines = %d", width, height);
	system(cmd);

	//申请内存 
	int frame_size = height * (width + 1) + 1;
	//frame_size * frame_count
	//待优化：每一帧图片，申请一块内存
	char* data = (char*)malloc(frame_size * frame_count);
	if (data == NULL) {
		printf("内存不足");
	}

	//3、把视频数据，转换成字符数组
	Mat frame_img;
	Mat gray_img;
	for (int n = 0; n < frame_count; n++) {
		char* p = data + n * frame_size;
		int k = 0;
		//读取一帧数据
		video.read(frame_img);

		//把读到的图像数据，转成“灰度”文件
		cvtColor(frame_img, gray_img, COLOR_BGR2GRAY);

		for (int row = 0; row < rows - h_size; row += h_size) {
			for (int col = 0; col < cols - w_size; col += w_size) {
				//抽取指定行，制定列的灰度值
				int value = gray_img.at<uchar>(row, col);
				p[k++] = char_imgs[value / (255/len + 1)];
			}
			p[k++] = '\n';
		}
		p[k++] = 0;

		//system("cls");//可加可不加

		printf("正在读取： %d / %d", n + 1, frame_count);
		cout <<  endl;
	}
	//4、播放（循环打印）
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { 0, 0 };
	while (1) {
		cout << "----------------------------------------------" << endl;
		for (int i = 0; i < frame_count; i++) {
			char* p = data + i * frame_size;//每一帧字符串的起始位置

			//先把控制台的光标移动到最开始的位置
			SetConsoleCursorPosition(h, pos);
			printf("%s", p);
			Sleep(750 / fps);//帧等待时间

		}
	}

	return 0;
}
