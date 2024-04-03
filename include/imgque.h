#ifndef IMGQUE_H
#define IMGQUE_H

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
#include <semaphore.h>


#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <condition_variable>


class imgQueue{
    std::deque<cv::Mat> que;
    std::condition_variable empty;
    std::mutex mtx; // declared for unique_lock;
    sem_t *img_sem;
public:
    imgQueue(){
        img_sem = sem_open("/img_sem", O_CREAT, 0644, 0);
        if (img_sem == SEM_FAILED) {
            perror("sem_open");
            exit(EXIT_FAILURE);
        }
        int ret = sem_init(img_sem, 0, 0);
    }
    cv::Mat take(){
        sem_wait(img_sem);  // wait for the sem, which is the frame
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
        sem_post(img_sem);
        
    }
    //int size();
};



#endif