#ifndef ALL4ALGORITHM_GRAPHICAL_H
#define ALL4ALGORITHM_GRAPHICAL_H

#include "geometry.h"
#include <math.h>

typedef struct {
    int width, height;
    char *name;
    void *data;
} Window;

enum MouseEventTypes {
    EVENT_MOUSEMOVE      = 0, //!< indicates that the mouse pointer has moved over the window.
    EVENT_LBUTTONDOWN    = 1, //!< indicates that the left mouse button is pressed.
    EVENT_RBUTTONDOWN    = 2, //!< indicates that the right mouse button is pressed.
    EVENT_MBUTTONDOWN    = 3, //!< indicates that the middle mouse button is pressed.
    EVENT_LBUTTONUP      = 4, //!< indicates that left mouse button is released.
    EVENT_RBUTTONUP      = 5, //!< indicates that right mouse button is released.
    EVENT_MBUTTONUP      = 6, //!< indicates that middle mouse button is released.
    EVENT_LBUTTONDBLCLK  = 7, //!< indicates that left mouse button is double clicked.
    EVENT_RBUTTONDBLCLK  = 8, //!< indicates that right mouse button is double clicked.
    EVENT_MBUTTONDBLCLK  = 9, //!< indicates that middle mouse button is double clicked.
    EVENT_MOUSEWHEEL     = 10,//!< positive and negative values mean forward and backward scrolling, respectively.
    EVENT_MOUSEHWHEEL    = 11 //!< positive and negative values mean right and left scrolling, respectively.
  };

//! Mouse Event Flags see cv::MouseCallback
enum MouseEventFlags {
    EVENT_FLAG_LBUTTON   = 1, //!< indicates that the left mouse button is down.
    EVENT_FLAG_RBUTTON   = 2, //!< indicates that the right mouse button is down.
    EVENT_FLAG_MBUTTON   = 4, //!< indicates that the middle mouse button is down.
    EVENT_FLAG_CTRLKEY   = 8, //!< indicates that CTRL Key is pressed.
    EVENT_FLAG_SHIFTKEY  = 16,//!< indicates that SHIFT Key is pressed.
    EVENT_FLAG_ALTKEY    = 32 //!< indicates that ALT Key is pressed.
  };

#ifdef __cplusplus
extern "C" {
#endif

void graphicalInit();

Window *getNewWindow(const char *name, int width, int height);

Window *getSubWindow(const Window *window, int x, int y, int width, int height);

void windowFill(const Window *window, unsigned char r, unsigned char g, unsigned char b);

void drawPoint(const Window *window, Point2i p, int rgb);

void drawPointSet(const Window *window, const Point2i *points, int npoints, int rgb);

void drawLine(const Window *window, Point2i p1, Point2i p2, int rgb, int thickness);

void drawRect(const Window *window, Point2i lefttop, int width, int height, int rgb, int thickness);

void drawPoly(const Window *window, const Point2i *points, int nponts, int rgb, int thickness, int connect);

void drawCircle(const Window *window, Point2i center, int radius, int rgb, int thickness);

void drawText(const Window *window, const char *text, Point2i leftbottom, int rgb, int fontsize);

char waitKey(int ms);

void setMouseCallback(const Window *window, void (*callback)(int event, int x, int y, int flags, void *userdata), void *userdata);

void showWindow(const Window *window);

void destroyWindow(const Window *window);

#ifdef __cplusplus
}
#endif

#endif //ALL4ALGORITHM_GRAPHICAL_H
