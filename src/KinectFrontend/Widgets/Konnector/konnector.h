#ifndef KONNECTOR_H
#define KONNECTOR_H

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

#include "ui_konnector.h"
#include "src/include/KinectInterface.h"
#include "src/KinectFrontend/Widgets/Logger/logger.h"
#include "konnector_settings.h"

using namespace std;

//! Used by Qt GUI
namespace Ui
{
class konnector;
}

//!
//! \class
//! \brief The Konnector class.
//! This is a Qt frontend for the KinectBackend class
//! This class calls the KinectBackend class,
//! collects its outputs and displays them to the user
//! This class is capable of moving the Kinect's motor
//!
//! \todo Start / Stop writting frames on start acquisition
//! \todo The output path should be taken from output_path QString
//! \todo The outputs should be the selected by the options widget.
//!
class Konnector : public QDialog
{

    Q_OBJECT

public:
    //! Constructor
    explicit Konnector(QDialog *parent = 0);

    //! Destructor
    ~Konnector();

    //! Copy and move constructos and assignment opperators,
    //! not currently implemented,
    //! only here to provide delete function in case of singleton
    Konnector(Konnector &);
    Konnector & operator = (Konnector &);
    Konnector(Konnector &&);
    Konnector & operator = (Konnector &&);

    inline Ui::konnector * get_ui_ptr()
    {
        return m_ui_ptr;
    }

    inline int set_ui_ptr(Ui::konnector *ui_ptr)
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

    int konnector_main();

    int konnector_kill(bool);

private:

    //! Pointer to the UI namespace
    Ui::konnector *m_ui_ptr;

    //! Pointer to the update timer
    QTimer *m_update_ptr;

    KinectInterface *m_kinect_interface_ptr;

    //! Tracks if a camera is connected
    bool m_is_connected;

    //! True when data are acquired. Actually when saved.
    bool m_is_acquiring;

    //! Called by destructor
    //! and any other methods aimign to destruct the class
    int destructor(bool);

    //! Updates the text in the UI
    //! \todo Write the number of frames on ui->lbl_frames_recd
    //! \todo Write the number of time lapsed on ui->lbl_time_lapsed
    int update_output();

    //! A window to display the log
    Logger * _logger;
    //! Settings related with the acquisition
    Konnector_Settings* _setts;

    //! The path in which the output files will be written
    QString _output_path;

signals:
    //! Emitted when connection status changes
    void connection_status_changed();

    //! Emitted when acquisition has started
    void acquisition_status_changed();

private slots:

    //! Called by timer on timeout
    void update();

    //! Updates the state of the GUI
    //! Three states considered:
    //! - Disconnected
    //! - Connected - idle
    //! - Acquiring
    void updateGUI_state();

    //! Event handler for Connect button
    void on__psh_connect_clicked();

    //! Event handler for Disconnect button
    void on__psh_disconnect_clicked();

    //    //! Event handler for Up button
    //    void on_pushButton_clicked();

    //    //! Event handler for Down button
    //    void on_pushButton_2_clicked();
    //! Show the log window
    void on__psh_show_log_clicked();

    //! Select the path of the output
    //! \todo let the backend know this path
    void on__psh_output_path_clicked();
    void on__psh_settings_clicked();
};

#endif // KINECTFRONTEND_H
