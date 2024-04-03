#include <stdio.h>
#include <memory>
#include <sys/time.h>
#include <string.h>
#include "pthread.h"
#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <queue>
#include "video.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace std;


string vid_path = "./truck_392.avi";
int vid_id = 0;
cv::Mat img_out;;
cv::VideoCapture capture;
pthread_mutex_t Mutex;   //大写M防止c++歧义
imgQueue que;
int img_ready = 0;
//std::queue<cv::Mat> imgs;
bool isDetecting = false;
bool usingCamera;
//const char buf[] = "hello";
const int32_t bufSize = 1920*1080*3;




int init(string vid_path){
    usingCamera = false;
    pthread_mutex_init(&Mutex, NULL); //初始化线程锁
    //capture.open(
    //    "v4l2src device=/dev/video11 io-mode=4 ! queue ! video/x-raw,format=NV12,width=1920,height=1080,framerate=60/1 ! appsink", 
    //    cv::CAP_GSTREAMER);
    capture.open(vid_path);
    if (capture.isOpened()){
        printf("video open success!\n");  
    }
    else{
        printf("video open failed!\n");
        return -1;
    }
    capture.set(cv::CAP_PROP_FPS, 30);
    capture.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);
    capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    int fps = capture.get(cv::CAP_PROP_FPS);
    printf("FPS = %d\n", fps);

    return 0;
}

int init(int vid_id){
    usingCamera = true;
    pthread_mutex_init(&Mutex, NULL); //初始化线程锁
    //string vid = "v4l2src device=/dev/video" + vid_id + " io-mode=4 ! queue ! video/x-raw,format=NV12,width=1920,height=1080,framerate=" + fps + "/1 ! appsink";
    //capture.open(vid, cv::CAP_GSTREAMER);
    capture.open(vid_id, cv::CAP_V4L2);
    if (capture.isOpened()){
        printf("video open success!\n");  
    }
    else{
        return  -1;
        printf("video open failed!\n");
    }
    capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    capture.set(cv::CAP_PROP_FPS, 30);
    capture.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);
    
    int fps = capture.get(cv::CAP_PROP_FPS);
    printf("FPS = %d\n", fps);

    return 0;
}


void *VideoRead(void *arg){
/*
    循环读取摄像头，根据当前检测状态决定读取的帧是否进入检测算法，将处理之后的图像帧插入队列等待读取
*/
    struct timeval time;
    gettimeofday(&time, nullptr);
    auto startTime = time.tv_sec * 1000 + time.tv_usec / 1000;
    auto beforeTime = startTime;
    int frame_num = 0;
    int detFrameNum = 0;
    while(capture.isOpened())
    {
        cv::Mat frame;
        gettimeofday(&time, nullptr);
        auto readStartTime = time.tv_sec * 1000 + time.tv_usec / 1000;
        if (capture.read(frame) == false){
            printf("read image fail!\n");
            break;
        }
        gettimeofday(&time, nullptr);
        auto readEndTime = time.tv_sec * 1000 + time.tv_usec / 1000;
        //printf("读取时间:\t %fms \n",  float(readEndTime - readStartTime));
        
        que.put(frame);     //图片帧写入队列
        img_ready = 1;
        
        frame_num++;
        if (frame_num % 30 == 0){
            gettimeofday(&time, nullptr);
            auto currentTime = time.tv_sec * 1000 + time.tv_usec / 1000;
            //printf("30帧内平均帧率:\t %f fps/s\n", 30.0 / float(currentTime - beforeTime) * 1000.0);
            beforeTime = currentTime;
            frame_num = 0;
        }
    }
    img_ready = 0;
    return NULL;
}

void changeDetecting(bool Detecting)
{
    isDetecting = Detecting;
}

void *VideoReadStart(){
    pthread_t t1;
    pthread_create(&t1, NULL, VideoRead, NULL);
    printf("Video read start!\n");
    return NULL;
}


cv::Mat get_frame(){
    if (img_ready == 0){
        printf("Infer images not ready!\n");
    }
    cv::Mat det_frame;
    det_frame = que.take();  //获取队列最后一个元素
    return det_frame; 
}

int is_imgReady(){
    return img_ready;
}

int main() {
    int num = 0;
    init(vid_id);
    VideoReadStart();
    //capture.open(vid_id);
    cv::namedWindow("video test");
    //int fps = capture.get(cv::CAP_PROP_FPS);

    // capture.set(cv::CAP_PROP_FPS, 30);
    // capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    // capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    // capture.set(cv::CAP_PROP_FPS, 30);
    // printf("FPS = %d\n", fps);
    while(1) {
        num++;
        cv::Mat frame;
        frame = get_frame();
        //capture.read(frame);
        cv::imshow("video test", frame);
        if (cv::waitKey(1) == 'q')
            break;
        printf("frame %d size = %d, %d \n", num, frame.size[0], frame.size[1]);
    }
}



