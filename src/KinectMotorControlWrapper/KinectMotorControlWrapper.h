#ifndef KINECTMOTORCONTROLWRAPPER_H
#define KINECTMOTORCONTROLWRAPPER_H

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

#include "libfreenect.h"
#include "ui_motor_control.h"

using namespace std;

namespace Ui
{
class motor_control;
}

//!
//! \class
//! \brief The MotorControl class.
//! Finds and connects to a Kinect camera.
//! I case of multiple connected cameras, only the first
//! will be considered.
//! \details The class is a singleton. Which means that only a single object across
//! the application life span, can exist.
//!
class MotorControl : public QDialog
{
    Q_OBJECT

public:
    //! Return a static reference to the singleton object.
    static MotorControl &getInstance( );

    MotorControl(MotorControl &) = delete;

    void operator = (MotorControl &) = delete;

    inline Ui::motor_control * get_ui() const
    {
        return m_ui_ptr;
    }

private:

    explicit MotorControl(QDialog *parent = 0);
    ~MotorControl();

    Ui::motor_control *m_ui_ptr;

    //! Holds information about the usb context.
    freenect_context *m_fctx_ptr;

    //! Holds device information.
    freenect_device *m_fdev_ptr;

    //! Current tilt state.
    freenect_raw_tilt_state *m_current_tilt_state_ptr;
    //! Controls the frequency of checking the output of the camera
    QTimer *m_update_ptr;
    //! Step of camera tilt.
    double m_increment;
    //! Number of devices connected to the PC.
    int m_num_devices;

    static void depth_callback(freenect_device *, void *, unsigned int);
    static void video_callback(freenect_device *, void *, unsigned int);

    //! Create connections
    void create_connections();

    int update_tilt_state();
    double get_camera_tilt();
    int set_camera_tilt(double);
    int update_camera_tilt(double);

signals:

private slots:
    void update();

    void on__psh_connect_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
};

#endif // KINECTDEPTHIMAGEOUTPUTWRAPPER_H
