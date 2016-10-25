#include <stdlib.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include <unistd.h>
#include <highgui.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT 8000
#define LISTEN_AMOUNT 100
#define JPEG_BUF "/tmp/v4l-buf/receive.jpg"

#include <string>
#include <map>

using namespace std;
using namespace cv;

map<string, int> ip2sock;
char current;

void on_menuCamera_activate(GtkMenuItem* item, gpointer data) {
    int client_socket = ip2sock[(gchar*) data];
    current = 'c';
    send(client_socket, &current, 1, 0);
}

void on_menuDesktop_activate(GtkMenuItem* item, gpointer data) {
    int client_socket = ip2sock[(gchar*) data];
    current = 'd';
    send(client_socket, &current, 1, 0);
}

void on_menuEliminate_activate(GtkMenuItem* item, gpointer data) {
    int client_socket = ip2sock[(gchar*) data];
    current = 'x';
    send(client_socket, &current, 1, 0);
    shutdown(client_socket, SHUT_RDWR);
}

void button_press_event(GtkWidget * widget, GdkEventButton * event, gpointer data) {
    if (event->button == 1 && event->type == GDK_2BUTTON_PRESS)
        puts((gchar*) data);
    else if (event->button == 3 && event->type == GDK_BUTTON_PRESS) {
        GdkEventButton *bevent = (GdkEventButton *) event;

        GtkWidget *menu = gtk_menu_new();
        GtkWidget *menuCamera = gtk_menu_item_new_with_label("鐩戣鎽勫儚澶(&C)");
        g_signal_connect(G_OBJECT(menuCamera), "activate", G_CALLBACK(on_menuCamera_activate), data);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuCamera);
        GtkWidget *menuDesktop = gtk_menu_item_new_with_label("鐩戣妗岄潰(&D)");
        g_signal_connect(G_OBJECT(menuDesktop), "activate", G_CALLBACK(on_menuDesktop_activate), data);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuDesktop);
        GtkWidget *menuEliminate = gtk_menu_item_new_with_label("閫愬嚭鍒楄〃(&X)");
        g_signal_connect(G_OBJECT(menuEliminate), "activate", G_CALLBACK(on_menuEliminate_activate), data);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuEliminate);

        gtk_widget_show_all(GTK_WIDGET(menu));
        gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, bevent->button, bevent->time);
    }
}

#define P() gdk_threads_enter()
#define V() gdk_threads_leave()
#define I() gdk_threads_init()

GtkWidget *win, *vbox, *gtklist;

bool readToEnd(int socket, char *out, int size) {
    int offset = 0;
    while (offset != size) {
        int got = recv(socket, out + offset, size - offset, 0);
        if (got <= 0)
            return false;
        offset += got;
    }
    return true;
}

void *client_receive(void *arg) {
    char *ip = (char*) arg;
    int client_socket = ip2sock[ip];

    P();
    GtkWidget* list_item = gtk_list_item_new_with_label(ip);
    g_signal_connect(G_OBJECT(list_item), "button_press_event", G_CALLBACK(button_press_event), (gpointer) ip);
    gtk_container_add(GTK_CONTAINER(gtklist), list_item);
    gtk_widget_show_all(list_item);
    gtk_widget_show_all(gtklist);
    gtk_widget_show_all(win);
    V();

    int length;
    static char buffer[1 << 25];
    while (readToEnd(client_socket, (char*) &length, 4)) {
        if (readToEnd(client_socket, buffer, length)) {
            FILE *fp = fopen(JPEG_BUF, "wb");
            fwrite(buffer, length, 1, fp);
            fclose(fp);
            printf("鑾峰彇鍒板崟甯уぇ灏忥細%d\n", length);

            static GtkWidget* monitor = NULL;

            if (!monitor) {
                monitor = gtk_window_new(GTK_WINDOW_TOPLEVEL);
                gtk_window_set_title(GTK_WINDOW(monitor), "瑙嗛鐩戞帶");
                gtk_window_set_position(GTK_WINDOW(monitor), GTK_WIN_POS_CENTER);
                gtk_widget_set_usize(monitor, 640, 480);
                gtk_widget_show_all(monitor);
            }

            /*GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(JPEG_BUF, NULL);
            gdk_draw_pixbuf(monitor->window, monitor->style->white_gc, pixbuf,
                    0, 0, 0, 0, 320, 240, GDK_RGB_DITHER_NORMAL, 0, 0);
            */usleep(500);
            send(client_socket, &current, 1, 0);
        } else
            break;
    }

    P();
    gtk_container_remove(GTK_CONTAINER(gtklist), list_item);
    gtk_widget_show_all(win);
    V();
    ip2sock.erase(ip);
}

int server_socket;

void *server_listen(void *arg) {
    sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof (server_addr))) {
        puts("缁戝畾绔彛澶辫触锛岃绔彛宸茬粡琚崰鐢紒");
        exit(1);
        return 0;
    }
    listen(server_socket, LISTEN_AMOUNT);
    while (1) {
        sockaddr_in client_addr = {0};
        socklen_t length = sizeof (client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &length);
        if (client_socket == -1)
            break;
        char *ip = inet_ntoa(client_addr.sin_addr);
        if (ip2sock.find(ip) != ip2sock.end()) {
            char cmd = 'x';
            send(client_socket, &cmd, 1, 0);
            shutdown(client_socket, SHUT_RDWR);
            continue;
        }
        ip2sock[ip] = client_socket;
        pthread_create(new pthread_t(), NULL, client_receive, ip);
    }
    return 0;
}

void destroy() {
    int opt = TRUE;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (int));
    shutdown(server_socket, SHUT_RDWR);
    close(server_socket);
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    system("mkdir /tmp/v4l-buf");
    if (system("mount tmpfs /tmp/v4l-buf -t tmpfs -o size=8m")) {
        puts("璇蜂娇鐢ㄨ秴绾х鐞嗗憳鏉冮檺鎵ц璇ユ帶鍒剁▼搴忥紒");
        return 1;
    }

    if (!g_thread_supported())
        g_thread_init(NULL);
    I();

    gtk_init(&argc, &argv);
    win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(win), "瑙嗛鐩戞帶 - 鏈嶅姟鍣 for Linux");
    gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
    gtk_widget_set_usize(win, 640, 480);
    g_signal_connect(win, "destroy", GTK_SIGNAL_FUNC(destroy), NULL);
    vbox = gtk_vbox_new(FALSE, 6);
    gtk_container_add(GTK_CONTAINER(win), vbox);

    GtkWidget *label = gtk_label_new("瀹㈡埛绔?IP 鍦板潃鍒楄〃锛");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    gtk_widget_show(label);

    gtklist = gtk_list_new();
    gtk_box_pack_start(GTK_BOX(vbox), gtklist, FALSE, FALSE, 0);
    gtk_widget_show_all(win);
    g_thread_create(server_listen, NULL, FALSE, NULL);

    P();
    gtk_main();
    V();
    return 0;
}
