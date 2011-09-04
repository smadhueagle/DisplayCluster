#ifndef TOUCH_LISTENER_H
#define TOUCH_LISTENER_H

#include "TuioListener.h"
#include "TuioClient.h"
#include <QtGui>

#define DOUBLE_CLICK_DISTANCE 0.1 // recall this is the (0,0,1,1) coordinate system
#define DOUBLE_CLICK_TIME 500 // ms

class TouchListener : public TUIO::TuioListener
{
    public:

        TouchListener();

        void addTuioObject(TUIO::TuioObject *tobj);
        void updateTuioObject(TUIO::TuioObject *tobj);
        void removeTuioObject(TUIO::TuioObject *tobj);

        void addTuioCursor(TUIO::TuioCursor *tcur);
        void updateTuioCursor(TUIO::TuioCursor *tcur);
        void removeTuioCursor(TUIO::TuioCursor *tcur);

        void refresh(TUIO::TuioTime frameTime);

    private:

        TUIO::TuioClient client_;
        QPointF lastPoint_;

        // detect double-clicks
        QTime lastClickTime_;
        QPointF lastClickPoint_;
};

#endif