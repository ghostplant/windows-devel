#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <gtk/gtk.h>

#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace cv;

#define SERVER_IP "127.0.0.1"//"202.119.81.70"
#define SERVER_PORT 8000
#define JPEG_BUF "/tmp/v4l-buf/buffer.jpg"

CvCapture* pCapture;
pthread_t ntid;
IplImage* currentImage;

void *showWindow(void *arg) {
    while (1) {
        currentImage = cvQueryFrame(pCapture);
        if (!currentImage)
            break;
        cvShowImage("摄像头采集窗口", currentImage);
        if (cvWaitKey(33) == 27)
            break;
        usleep(100);
    }
    cvDestroyWindow("摄像头采集窗口");
    return 0;
}

void destroy(GtkWidget *widget, gpointer data) {
    puts("Gtk-OpenCV 客户端主动退出！");
    exit(0);
}

int main(int argc, char **argv) {
    freopen(".config", "w", stderr);
    system("mkdir /tmp/v4l-buf");
    if (system("mount tmpfs /tmp/v4l-buf -t tmpfs -o size=8m")) {
        puts("请使用超级管理员权限执行该控制程序！");
        return 1;
    }

    sockaddr_in client_addr = {0}, server_addr = {0};
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);
    client_addr.sin_port = htons(SERVER_PORT);
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    bind(client_socket, (sockaddr*) & client_addr, sizeof (client_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr((argc > 1) ? argv[1] : SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);
    socklen_t server_addr_length = sizeof (server_addr);
    if (connect(client_socket, (struct sockaddr*) &server_addr, server_addr_length) < 0) {
        puts("不能链接到服务器！");
        return 1;
    }

    puts("成功运行，等待服务器的控制……");
    pCapture = cvCreateCameraCapture(-1);
    currentImage = cvQueryFrame(pCapture);

    cvNamedWindow("摄像头采集窗口", CV_WINDOW_AUTOSIZE);
    cvShowImage("摄像头采集窗口", currentImage);
    cvMoveWindow("摄像头采集窗口", 10,10);

    gtk_init(&argc, &argv);
    g_signal_connect(G_OBJECT(cvGetWindowHandle("摄像头采集窗口")), "destroy", G_CALLBACK(destroy), NULL);

    pthread_create(&ntid, NULL, showWindow, NULL);

    static char cmd, buffer[1 << 25];
    while (recv(client_socket, &cmd, 1, 0) == 1 && cmd != 'x') {
        if (cmd == 'c')
            cvSaveImage(JPEG_BUF, currentImage);
        else if (cmd == 'd')
            system("scrot " JPEG_BUF);
        FILE *fp = fopen(JPEG_BUF, "rb");
        if (fp == NULL)
            continue;
        fseek(fp, 0, SEEK_END);
        int length = ftell(fp);
        send(client_socket, &length, 4, 0);
        rewind(fp);
        fread(buffer, length, 1, fp);
        fclose(fp);
        send(client_socket, buffer, length, 0);
    }
    cvReleaseCapture(&pCapture);
    puts("已被服务器逐出！");
    return 0;
}