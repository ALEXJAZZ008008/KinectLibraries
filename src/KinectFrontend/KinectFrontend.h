#ifndef KINECTFRONTEND_H
#define KINECTFRONTEND_H

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

#include "ui_motor_control.h"
#include "src/include/KinectInterface.h"

using namespace std;

//! Used by Qt GUI
namespace Ui
{
class motor_control;
}

//!
//! \class
//! \brief The KinectFrontend class.
//! This is a Qt frontend for the KinectBackend class
//! This class calls the KinectBackend class,
//! collects its outputs and displays them to the user
//! This class is capable of moving the Kinect's motor
//!
class KinectFrontend : public QDialog
{

    Q_OBJECT

public:
    //! Constructor
    explicit KinectFrontend(QDialog *parent = 0);

    //! Destructor
    ~KinectFrontend();

    //! Copy and move constructos and assignment opperators,
    //! not currently implemented,
    //! only here to provide delete function in case of singleton
    KinectFrontend(KinectFrontend &);
    KinectFrontend & operator = (KinectFrontend &);
    KinectFrontend(KinectFrontend &&);
    KinectFrontend & operator = (KinectFrontend &&);

    inline Ui::motor_control * get_ui_ptr()
    {
        return m_ui_ptr;
    }

    inline int set_ui_ptr(Ui::motor_control *ui_ptr)
    {
        m_ui_ptr = ui_ptr;

        return 1;
    }

    inline QTimer * get_update_ptr()
    {
        return m_update_ptr;
    }

    inline int set_update_ptr(QTimer * update_ptr)
    {
        m_update_ptr = update_ptr;

        return 1;
    }

    inline KinectInterface * get_kinect_interface_ptr()
    {
        return m_kinect_interface_ptr;
    }

    inline int ser_kinect_interface_ptr(KinectInterface * kinect_interface_ptr)
    {
        m_kinect_interface_ptr = kinect_interface_ptr;

        return 1;
    }

    inline bool get_is_connected()
    {
        return m_is_connected;
    }

    inline int set_is_connected(bool is_connected)
    {
        m_is_connected = is_connected;

        return 1;
    }

    int kinect_frontend_main();

    int kinect_frontend_kill(bool);

private:

    //! Pointer to the UI namespace
    Ui::motor_control *m_ui_ptr;

    //! Pointer to the update timer
    QTimer *m_update_ptr;

    KinectInterface *m_kinect_interface_ptr;

    //! Tracks if a camera is connected
    bool m_is_connected;

    //! Called by destructor
    //! and any other methods aimign to destruct the class
    int destructor(bool);

    //! Updates the text in the UI
    int update_output();

signals:

private slots:

    //! Called by timer on timeout
    void update();

    //! Event handler for Connect button
    void on__psh_connect_clicked();

    //! Event handler for Disconnect button
    void on__psh_disconnect_clicked();

    //! Event handler for Up button
    void on_pushButton_clicked();

    //! Event handler for Down button
    void on_pushButton_2_clicked();
};

#endif // KINECTFRONTEND_H
