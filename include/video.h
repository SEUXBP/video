#ifndef VIDEO_H
#define VIDEO_H

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

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include "imgQueue.hpp"
#include "imgque.h"

using namespace std;

int init(string vid_path);
int init(int vid_id);
void *VideoRead(void *arg);
void changeDetecting(bool Detecting);
void *VideoReadStart();
cv::Mat get_frame();


#endif