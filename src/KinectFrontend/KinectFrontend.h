#ifndef KINECTFRONTEND_H
#define KINECTFRONTEND_H

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>

#include <pthread.h>

#include "ui_motor_control.h"
#include "KinectBackend.h"

using namespace std;

namespace Ui
{
class motor_control;
}

//!
//! \class
//! \brief The Kinect Frontend class.
//! Finds and connects to a Kinect camera.
//! I case of multiple connected cameras, only the first
//! will be considered.
//! \details The class is a singleton. Which means that only a single object across
//! the application life span, can exist.
//!
class KinectFrontend : public QDialog
{
    Q_OBJECT

public:

    //! Return a static reference to the singleton object.
    static KinectFrontend &getInstance( );

    KinectFrontend(KinectFrontend &) = delete;
    void operator = (KinectFrontend &) = delete;
    KinectFrontend(KinectFrontend &&) = delete;
    void operator = (KinectFrontend &&) = delete;

private:

    //! Return a static reference to the singleton object.
    explicit KinectFrontend(QDialog *parent = 0);
    ~KinectFrontend();

    Ui::motor_control *m_ui_ptr;

    pthread_t m_update;

    static void * update(void *);

signals:

private slots:

    void on__psh_connect_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
};

#endif // KINECTFRONTEND_H
