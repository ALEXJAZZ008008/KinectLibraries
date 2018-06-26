#ifndef KINECTFRONTEND_H
#define KINECTFRONTEND_H

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

#include "ui_motor_control.h"
#include "KinectBackend.h"

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
    void operator = (KinectFrontend &);
    KinectFrontend(KinectFrontend &&);
    void operator = (KinectFrontend &&);

private:

    //! Pointer to the UI namespace
    Ui::motor_control *m_ui_ptr;

    //! Pointer to the update timer
    QTimer *m_update;

    //! Tracks if a camera is connected
    bool is_connected;

    //! Called by destructor
    //! and any other methods aimign to destruct the class
    int destructor();

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
