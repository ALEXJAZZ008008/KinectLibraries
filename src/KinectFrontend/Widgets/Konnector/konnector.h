#ifndef KONNECTOR_H
#define KONNECTOR_H

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QSettings>
#include <QTimer>

#include <memory>
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
//! \class Konnector
//! \brief The Konnector class.
//! This is a Qt frontend for the KinectBackend class
//! This class calls the KinectBackend class,
//! collects its outputs and displays them to the user
//! This class is capable of moving the Kinect's motor
//!
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

    inline shared_ptr<Ui::konnector> & get_ui_ptr()
    {
        return m_ui_ptr;
    }

    inline int set_ui_ptr(shared_ptr<Ui::konnector> &ui_ptr)
    {
        m_ui_ptr = ui_ptr;

        return 1;
    }

    inline shared_ptr<Logger> & get_logger_ptr()
    {
        return m_logger_ptr;
    }

    inline int set_logger_ptr(shared_ptr<Logger> &logger_ptr)
    {
        m_logger_ptr = logger_ptr;

        return 1;
    }

    inline shared_ptr<KinectInterface> & get_kinect_interface_ptr()
    {
        return m_kinect_interface_ptr;
    }

    inline int set_kinect_interface_ptr(shared_ptr<KinectInterface> &kinect_interface_ptr)
    {
        m_kinect_interface_ptr = kinect_interface_ptr;

        return 1;
    }

    inline shared_ptr<QTimer> & get_update_ptr()
    {
        return m_update_ptr;
    }

    inline int set_update_ptr(shared_ptr<QTimer> update_ptr)
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

    inline float get_acquisition_frequency()
    {
        return m_acquisition_frequency;
    }

    inline int set_acquitions_frequency(float acquisition_frequency)
    {
        m_acquisition_frequency = acquisition_frequency;

        return 1;
    }

    inline float get_acquisition_speed()
    {
        return m_acquisition_speed;
    }

    inline int set_acquitions_speed(float acquisition_speed)
    {
        m_acquisition_speed = acquisition_speed;

        return 1;
    }

    inline unsigned char get_write_offset()
    {
        return m_write_offset;
    }

    inline int set_write_offset(unsigned char write_offset)
    {
        m_write_offset = write_offset;

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
    shared_ptr<Ui::konnector> m_ui_ptr;

    //! A window to display the log
    shared_ptr<Logger> m_logger_ptr;

    shared_ptr<KinectInterface> m_kinect_interface_ptr;

    //! Pointer to the update timer
    shared_ptr<QTimer> m_update_ptr;

    high_resolution_clock::time_point m_acquisition_start_time;

    float m_acquisition_frequency;

    float m_acquisition_speed;

    unsigned char m_write_offset;

    //! Tracks if a camera is connected
    bool m_is_connected;

    //! True when data are acquired. Actually when saved.
    bool m_is_acquiring;

    //! Called by destructor
    //! and any other methods aiming to destruct the class
    int destructor(bool);

    int update_settings();

    //! Updates the text in the UI
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
