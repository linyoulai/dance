
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
		cout << endl;
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