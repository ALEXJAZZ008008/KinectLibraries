#ifndef KONNECTOR_H
#define KONNECTOR_H

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QSettings>
#include <QTimer>

#include <chrono>

#include "ui_konnector.h"
#include "src/include/KinectInterface.h"
#include "src/KinectFrontend/Widgets/Logger/logger.h"
#include "konnector_settings.h"

using namespace std;
using namespace std::chrono;

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

    inline Logger * get_logger_ptr()
    {
        return m_logger_ptr;
    }

    inline int set_logger_ptr(Logger *logger_ptr)
    {
        m_logger_ptr = logger_ptr;

        return 1;
    }

    inline KinectInterface * get_kinect_interface_ptr()
    {
        return m_kinect_interface_ptr;
    }

    inline int set_kinect_interface_ptr(KinectInterface *kinect_interface_ptr)
    {
        m_kinect_interface_ptr = kinect_interface_ptr;

        return 1;
    }

    inline QTimer * get_update_ptr()
    {
        return m_update_ptr;
    }

    inline int set_update_ptr(QTimer *update_ptr)
    {
        m_update_ptr = update_ptr;

        return 1;
    }

    inline high_resolution_clock::time_point & get_acquisition_start_time()
    {
        return m_acquisition_start_time;
    }

    inline int set_acquitions_start_time(high_resolution_clock::time_point &acquisition_start_time)
    {
        m_acquisition_start_time = acquisition_start_time;

        return 1;
    }

    inline QString & get_output_path()
    {
        return m_output_path;
    }

    inline int set_output_path(QString &output_path)
    {
        m_output_path = output_path;

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

    inline bool get_is_acquiring()
    {
        return m_is_acquiring;
    }

    inline int set_is_acquiring(bool is_acquiring)
    {
        m_is_acquiring = is_acquiring;

        return 1;
    }

    int konnector_main();

    int konnector_kill(bool);

private:

    //! Pointer to the UI namespace
    Ui::konnector *m_ui_ptr;

    //! A window to display the log
    Logger *m_logger_ptr;

    KinectInterface *m_kinect_interface_ptr;

    //! Pointer to the update timer
    QTimer *m_update_ptr;

    high_resolution_clock::time_point m_acquisition_start_time;

    //! The path in which the output files will be written
    QString m_output_path;

    quint8 m_write_offset;

    //! Tracks if a camera is connected
    bool m_is_connected;

    //! True when data are acquired. Actually when saved.
    bool m_is_acquiring;

    //! Called by destructor
    //! and any other methods aiming to destruct the class
    int destructor(bool);

    //! Updates the text in the UI
    //! \todo Write the number of frames on ui->lbl_frames_recd
    //! \todo Write the number of time lapsed on ui->lbl_time_lapsed
    int update_output();

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

    void on__psh_acquire_start_clicked();

    void on__psh_acquire_stop_clicked();

    //! Show the log window
    void on__psh_show_log_clicked();

    //! Select the path of the output
    //! \todo let the backend know this path
    void on__psh_output_path_clicked();

    void on__psh_settings_clicked();
};

#endif // KINECTFRONTEND_H
