#include <stdio.h>//ע�ⲻҪ����Ƶʱ��̫����Ҫ���������߶Ⱥ�char_imgs[]
#include <Windows.h>
#include <iostream>
using namespace std;
//����������ͷ�ļ�
#include <mmsyscom.h>
#pragma comment(lib, "winmm.lib")
#include <opencv2/opencv.hpp>
using namespace cv; //����opencv�������ռ�
int main(void) {
	//1������Ƶ�ļ�
	//����һ��mp4��Ƶ����Դ�ļ�ͬĿ¼��
	//��mp4תΪmp3����Դ�ļ�ͬĿ¼����Ϊ��ƵתΪ�ַ���û�������ˣ�
	VideoCapture video;
	bool ret = video.open("��Ƶ�ļ���.mp4");//ret = return
	if (ret == false) {
		printf("��Ƶ�ļ���ʧ�ܣ��ѩҩn�ѩ�\n");
	}
	else {
		printf("��Ƶ�ļ��򿪳ɹ���(*^��^*)\n");
	}
	//2����ȡ��Ƶ�ĺ�����Ϣ���ж���֡����Ƶ�Ŀ�ȣ��߶ȣ����ŵ��ٶȣ�
	video.get(CAP_PROP_FRAME_COUNT);
	//֡��
	int frame_count = video.get(CAP_PROP_FRAME_COUNT);
	int fps = video.get(CAP_PROP_FPS);
	//���
	int cols = video.get(CAP_PROP_FRAME_WIDTH);
	int rows = video.get(CAP_PROP_FRAME_HEIGHT);
	//��������ĳ߶�
	int w_size = 9;
	int h_size = 20;
	//׼�� ÿ10�У�ÿ5�г�ȡһ���㣬ת����һ���ַ�
	//ԭ���ܰ� -> �ո�
	char char_imgs[] = "`.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@@@@@@@@@@@@@@@";//0 - 255,�ֳ�69��
	int len = sizeof(char_imgs) / sizeof(char_imgs[0]);
	//ÿ�����ص�ĻҶ�ֵ ����20 ��Ϊ�ַ������

	//����̨�Ĵ���
	int width = cols / w_size;
	int height = rows / h_size;
	// mode con cols = 100 lines = 80
	char cmd[128];
	sprintf_s(cmd, sizeof(cmd), "mode con cols = %d lines = %d", width, height);
	system(cmd);

	//�����ڴ� 
	int frame_size = height * (width + 1) + 1;
	//frame_size * frame_count
	//���Ż���ÿһ֡ͼƬ������һ���ڴ�
	char* data = (char*)malloc(frame_size * frame_count);
	if (data == NULL) {
		printf("�ڴ治��");
	}

	//3������Ƶ���ݣ�ת�����ַ�����
	Mat frame_img;
	Mat gray_img;
	for (int n = 0; n < frame_count; n++) {
		char* p = data + n * frame_size;
		int k = 0;
		//��ȡһ֡����
		video.read(frame_img);

		//�Ѷ�����ͼ�����ݣ�ת�ɡ��Ҷȡ��ļ�
		cvtColor(frame_img, gray_img, COLOR_BGR2GRAY);

		for (int row = 0; row < rows - h_size; row += h_size) {
			for (int col = 0; col < cols - w_size; col += w_size) {
				//��ȡָ���У��ƶ��еĻҶ�ֵ
				int value = gray_img.at<uchar>(row, col);
				p[k++] = char_imgs[value / (255/len + 1)];
			}
			p[k++] = '\n';
		}
		p[k++] = 0;

		//system("cls");//�ɼӿɲ���

		printf("���ڶ�ȡ�� %d / %d", n + 1, frame_count);
		cout << endl;
	}
	//4�����ţ�ѭ����ӡ��
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { 0, 0 };
	while (1) {
		cout << "----------------------------------------------" << endl;
		for (int i = 0; i < frame_count; i++) {
			char* p = data + i * frame_size;//ÿһ֡�ַ�������ʼλ��

			//�Ȱѿ���̨�Ĺ���ƶ����ʼ��λ��
			SetConsoleCursorPosition(h, pos);
			printf("%s", p);
			Sleep(750 / fps);//֡�ȴ�ʱ��

		}
	}

	return 0;
}