#ifndef KINECTMOTORCONTROLWRAPPER_H
#define KINECTMOTORCONTROLWRAPPER_H

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

#include "ui_motor_control.h"
#include "kinectdepthimageoutputlibrary.h"

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
//!
class KinectFrontend : public QDialog
{
    Q_OBJECT

public:
    //! Return a static reference to the singleton object.
    explicit KinectFrontend(QDialog *parent = 0);
    ~KinectFrontend();

    KinectFrontend(KinectFrontend &);
    void operator = (KinectFrontend &);
    KinectFrontend(KinectFrontend &&);
    void operator = (KinectFrontend &&);

private:

    Ui::motor_control *m_ui_ptr;

signals:

private slots:
    void update();

    void on__psh_connect_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
};

#endif // KINECTDEPTHIMAGEOUTPUTWRAPPER_H
