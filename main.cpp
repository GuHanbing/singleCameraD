#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

#define IMAGEWIDTH 640
#define IMAGEHEIGHT 480

#define TRUE 1
#define FALSE 0

#define FILE_VIDEO1 "/dev/video0"

static int fd;
struct v4l2_streamparm setfps;
struct v4l2_capability cap;
struct v4l2_fmtdesc fmtdesc;
struct v4l2_format fmt,fmtack;
struct v4l2_requestbuffers req;
struct v4l2_buffer buf;
enum   v4l2_buf_type type;

int init_v4l2(void);

int main()
{

    printf("first~~\n");
    if(init_v4l2() == FALSE){
        printf("Init fail~~\n");
        exit(1);
    }


    CvCapture *capture;
    capture = cvCaptureFromCAM(0);

    std::cout << cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,720) << std::endl;
    std::cout << cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,1280) << std::endl;
    std::cout << cvSetCaptureProperty(capture,CV_CAP_PROP_BRIGHTNESS,124) << std::endl;
    std::cout << cvSetCaptureProperty(capture,CV_CAP_PROP_HUE,90) << std::endl;
    std::cout << cvSetCaptureProperty(capture,CV_CAP_PROP_SATURATION,128) << std::endl;
    std::cout << cvSetCaptureProperty(capture,CV_CAP_PROP_FPS,10) << std::endl;

    if(capture == NULL)
        std::cout << "Open Camera Error" << std::endl;
    IplImage *get;
    get = cvQueryFrame(capture);

    if(get == NULL)
        std::cout << "capture Error" << std::endl;

    std::cout << "color : " << get->channelSeq << std::endl;

    cv::Mat change;
    change = cv::Mat(get);
//    cv::cvtColor(change,change,cv::COLOR_BGR2RGB);

    std::cout << "channels : " << change.channels() << std::endl;
    cv::cvtColor(change,change,CV_BGR2RGB);
//    std::cout << "color : " << change << std::endl;

    cvNamedWindow("0");
    cvShowImage("0",get);

    cv::imshow("1",change);
    cvWaitKey(0);

    cvReleaseImage(&get);
    cvReleaseCapture(&capture);
    return 0;
}

int init_v4l2(void){
    if ((fd = open(FILE_VIDEO1, O_RDWR)) == -1){
        printf("Opening video device error\n");
        return FALSE;
    }
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1){
        printf("unable Querying Capabilities\n");
        return FALSE;
    }
    else{
        printf( "Driver Caps:\n"
                "  Driver: \"%s\"\n"
                "  Card: \"%s\"\n"
                "  Bus: \"%s\"\n"
                "  Version: %d\n"
                "  Capabilities: %x\n",
                cap.driver,
                cap.card,
                cap.bus_info,
                cap.version,
                cap.capabilities);

    }

    if((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE){
        printf("Camera device %s: support capture\n",FILE_VIDEO1);
    }
    if((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING){
        printf("Camera device %s: support streaming.\n",FILE_VIDEO1);
    }

    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    printf("Support format: \n");
    while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc) != -1){
        printf("\t%d. %s\n",fmtdesc.index+1,fmtdesc.description);
        fmtdesc.index++;
    }

    struct v4l2_control control_exp;
    control_exp.id = V4L2_CID_EXPOSURE_AUTO;
    control_exp.value = V4L2_EXPOSURE_MANUAL;
    ioctl(fd,VIDIOC_S_CTRL,&control_exp);

    control_exp.id = V4L2_CID_EXPOSURE_ABSOLUTE;
    control_exp.value = 100;
    ioctl(fd,VIDIOC_S_CTRL,&control_exp);

//    //set fmt
//    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//    fmt.fmt.pix.width = IMAGEWIDTH;
//    fmt.fmt.pix.height = IMAGEHEIGHT;
//    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG; //*************************V4L2_PIX_FMT_YUYV****************
//    fmt.fmt.pix.field = V4L2_FIELD_ANY;

//    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1){
//        printf("Setting Pixel Format error\n");
//        return FALSE;
//    }
//    if(ioctl(fd,VIDIOC_G_FMT,&fmt) == -1){
//        printf("Unable to get format\n");
//        return FALSE;
//    }
    close(fd);
    return TRUE;
}
