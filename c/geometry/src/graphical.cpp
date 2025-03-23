#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utils/logger.hpp"
#include "geometry.h"
#include "graphical.h"

static inline cv::Scalar toScalar(const int &rgb) {
    return {(double) (rgb & 0xff), (double) ((rgb >> 8) & 0xff), (double) (rgb >> 16)};
}

extern "C" {
void graphicalInit() {
#ifdef WIN32
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
#endif
}

Window *getNewWindow(const char *name, const int width, const int height) {
    const auto w = new Window;
    w->width = width;
    w->height = height;
    w->name = new char[32];
    strncpy(w->name, name, 31);
    w->name[31] = '\0';
    w->data = new cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

    return w;
}

Window *getSubWindow(const Window *window, const int x, const int y, const int width, const int height) {
    const auto sub = new Window;
    sub->width = width;
    sub->height = height;
    sub->name = new char[1];
    *sub->name = '\0';
    sub->data = new cv::Mat;
    *(cv::Mat *) sub->data = (*(cv::Mat *) window->data)(cv::Rect(x, y, width, height));
    return sub;
}

void windowFill(const Window *window, const uchar r, const uchar g, const uchar b) {
    *(cv::Mat *) window->data = cv::Scalar(b, g, r);
}

void drawPoint(const Window *window, const Point2i point, const int rgb) {
    cv::circle(*(cv::Mat *) window->data, reinterpret_cast<const cv::Point2i &>(point), 3, toScalar(rgb), -1);
}

void drawPointSet(const Window *window, const Point2i *points, const int npoints, const int rgb) {
    cv::Mat img = *(cv::Mat *) window->data;
    const cv::Scalar color = toScalar(rgb);

    for (int i = 0; i < npoints; ++i)
        cv::circle(img, *(cv::Point2i *) (points + i), 3, color, -1);
}

void drawLine(const Window *window, const Point2i p1, const Point2i p2, const int rgb, const int thickness) {
    cv::line(*(cv::Mat *) window->data, reinterpret_cast<const cv::Point2i &>(p1),
             reinterpret_cast<const cv::Point2i &>(p2),
             toScalar(rgb), thickness);
}

void drawRect(const Window *window, const Point2i lefttop, const int width, const int height, const int rgb,
              const int thickness) {
    cv::rectangle(*(cv::Mat *) window->data, cv::Rect(lefttop.x, lefttop.y, width, height), toScalar(rgb), thickness);
}

void drawPoly(const Window *window, const Point2i *points, const int nponts, const int rgb, const int thickness,
              const int connect) {
    cv::Mat img = *(cv::Mat *) window->data;
    const cv::Scalar color = toScalar(rgb);

    if (thickness < 0) {
        auto *pts = new cv::Point[nponts];
        for (int i = 0; i < nponts; ++i)
            pts[i] = *(cv::Point2i *) (points + i);

        cv::fillConvexPoly(img, pts, nponts, color);

        delete[] pts;
        return;
    }


    for (int i = 0, j = 1; j < nponts; ++i, ++j) {
        cv::line(img, *(cv::Point2i *) (points + i), *(cv::Point2i *) (points + j),
                 color, thickness);
    }

    if (connect)
        cv::line(img, *(cv::Point2i *) (points + nponts - 1), *(cv::Point2i *) points,
                 color, thickness);
}

void drawCircle(const Window *window, const Point2i center, const int radius, const int rgb, const int thickness) {
    const cv::Scalar color = toScalar(rgb);

    cv::circle(*(cv::Mat *) window->data, reinterpret_cast<const cv::Point2i &>(center), radius,
               color, thickness);
}

void drawText(const Window *window, const char *text, const Point2i leftbottom, const int rgb, const int fontsize) {
    cv::putText(*(cv::Mat *) window->data, text, reinterpret_cast<const cv::Point2i &>(leftbottom),
                cv::FONT_HERSHEY_SIMPLEX, fontsize / 20.0, toScalar(rgb));
}

char waitKey(const int ms) {
    return (char) cv::waitKey(ms);
}

void setMouseCallback(const Window *window, void (*callback)(int, int, int, int, void *), void *userdata) {
    cv::setMouseCallback(window->name, callback, userdata);
}

void showWindow(const Window *window) {
    cv::imshow(window->name, *(cv::Mat *) window->data);
}

void destroyWindow(const Window *window) {
    cv::destroyWindow(window->name);
    delete[] window->name;
    delete (cv::Mat *) window->data;
    delete window;
}
}
