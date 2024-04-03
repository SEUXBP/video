#ifndef IMGQUEUE_H
#define IMGQUEUE_H

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
#include <deque>


#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <condition_variable>


class imgQueue{
    std::deque<cv::Mat> que;
    std::condition_variable empty;
    std::mutex mtx; // declared for unique_lock;
public:
    imgQueue(){}
    cv::Mat take(){
        std::unique_lock<std::mutex> lock{mtx};
        //printf("Take! que size = %d\n", que.size());
        //empty.wait(lock, [this]{return !que.empty();});
        //empty.wait(lock, [&](){return !que.empty();});
        while(que.empty()){
            empty.wait(lock);
        }
        auto res = que.back();
        //printf("Take success!\n");
        que.pop_front();
        return res;
    }

    void put(cv::Mat elem){
        //std::unique_lock<std::mutex> lock{mtx};
        if (que.size() > 0){
            que.pop_front();
        }
        que.push_back(elem);
        empty.notify_all(); //����
        //if (!que.empty()){
        //    printf("que is not empty!");
        //}
        //printf("Push! que size = %d\n", que.size());
    }
    //int size();
};



#endif