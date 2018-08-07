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
class Konnector;
}

//!
//! \class Konnector
//! \brief The Konnector class.
//! This is a Qt frontend for the KinectBackend class
//! This class calls the KinectBackend class,
//! collects its outputs and displays them to the user
//! This class is capable of moving the Kinect's motor
//!
class Konnector : public QDialog
{
public:
    //! Constructor
    explicit Konnector(QDialog *parent = nullptr);

    //! Destructor
    ~Konnector();

    //! Copy and move constructos and assignment opperators,
    Konnector(Konnector &);
    Konnector & operator = (Konnector &);
    Konnector(Konnector &&);
    Konnector & operator = (Konnector &&);

    //! Gets ui ptr
    inline Ui::Konnector * get_ui_ptr()
    {
        return m_ui_ptr;
    }

    //! Sets ui ptr
    inline int set_ui_ptr(Ui::Konnector *ui_ptr)
    {
        m_ui_ptr = ui_ptr;

        return 1;
    }

    //! Gets logger ptr
    inline KLLogger * get_logger_ptr()
    {
        return m_logger_ptr;
    }

    //! Sets logger ptr
    inline int set_logger_ptr(KLLogger *logger_ptr)
    {
        m_logger_ptr = logger_ptr;

        return 1;
    }

    //! Gets update ptr
    inline QTimer * get_update_ptr()
    {
        return m_update_ptr;
    }

    //! Sets update ptr
    inline int set_update_ptr(QTimer *update_ptr)
    {
        m_update_ptr = update_ptr;

        return 1;
    }

    //! Gets kinect interface ptr
    inline shared_ptr<KinectInterface> & get_kinect_interface_ptr()
    {
        return m_kinect_interface_ptr;
    }

    //! Sets kinect interface ptr
    inline int set_kinect_interface_ptr(shared_ptr<KinectInterface> &kinect_interface_ptr)
    {
        m_kinect_interface_ptr = kinect_interface_ptr;

        return 1;
    }

    //! Gets acquisition start time object
    inline high_resolution_clock::time_point & get_acquisition_start_time()
    {
        return m_acquisition_start_time;
    }

    //! Sets acquisition start time object
    inline int set_acquitions_start_time(high_resolution_clock::time_point &acquisition_start_time)
    {
        m_acquisition_start_time = acquisition_start_time;

        return 1;
    }

    //! Gets acquisition frequency value
    inline float get_acquisition_frequency()
    {
        return m_acquisition_frequency;
    }

    //! Sets acquisition frequency value
    inline int set_acquitions_frequency(float acquisition_frequency)
    {
        m_acquisition_frequency = acquisition_frequency;

        return 1;
    }

    //! Gets acquisition speed value
    inline float get_acquisition_speed()
    {
        return m_acquisition_speed;
    }

    //! Sets acquisition speed value
    inline int set_acquitions_speed(float acquisition_speed)
    {
        m_acquisition_speed = acquisition_speed;

        return 1;
    }

    //! Gets write offset value
    inline unsigned char get_write_offset()
    {
        return m_write_offset;
    }

    //! Sets write offset value
    inline int set_write_offset(unsigned char write_offset)
    {
        m_write_offset = write_offset;

        return 1;
    }

    //! Gets is connected bool
    inline bool get_is_connected()
    {
        return m_is_connected;
    }

    //! Sets is connected bool
    inline int set_is_connected(bool is_connected)
    {
        m_is_connected = is_connected;

        return 1;
    }

    //! Gets is acquiring bool
    inline bool get_is_acquiring()
    {
        return m_is_acquiring;
    }

    //! Sets is acquiring bool
    inline int set_is_acquiring(bool is_acquiring)
    {
        m_is_acquiring = is_acquiring;

        return 1;
    }

    //! Main
    int konnector_main();

    //! Destruct remotely
    int konnector_kill(bool);

private:

    //! Macro to indicate this is a QT object
    Q_OBJECT

    //! Pointer to the UI namespace
    Ui::Konnector *m_ui_ptr;

    //! A window to display the log
    KLLogger *m_logger_ptr;

    //! Pointer to the update timer
    QTimer *m_update_ptr;

    //! Holds pointer to the kinect interface
    shared_ptr<KinectInterface> m_kinect_interface_ptr;

    //! Holds the time point that the current acquisition started at
    high_resolution_clock::time_point m_acquisition_start_time;

    //! Holds the how often the acquisition occurs in a second
    float m_acquisition_frequency;

    //! Holds how many ms need to pass before a new acquisition happens
    float m_acquisition_speed;

    //! Holds how often a write should happen
    unsigned char m_write_offset;

    //! Tracks if a camera is connected
    bool m_is_connected;

    //! True when data are acquired. Actually when saved.
    bool m_is_acquiring;

    //! Called by destructor
    //! and any other methods aiming to destruct the class
    int destructor(bool);

    //! Updates the values which store default values
    int update_settings();

    //! Updates the text in the UI
    int update_output();

signals:
    //! Emitted when connection status changes
    void connection_status_changed();

    //! Emitted when cammera angle changes
    void camera_angle_changed();

    //! Emitted when acquisition has started
    void acquisition_status_changed();

private slots:

    //! Updates the state of the GUI
    //! Three states considered:
    //! - Disconnected
    //! - Connected - idle
    //! - Acquiring
    void updateGUI_state();

    //! Called by timer on timeout
    void update();

    //! Event handler for Connect button
    void on__psh_connect_clicked();

    //! Event handler for Disconnect button
    void on__psh_disconnect_clicked();

    //! Event handler for Disconnect button
    void on_psh_tilt_up_clicked();

    //! Event handler for Tilt Down button
    void on_psh_tilt_down_clicked();

    //! Event handler for Current Tilt box
    void on_le_cur_tilt_returnPressed();

    //! Event handler for Acquire Start button
    void on__psh_acquire_start_clicked();

    //! Event handler for Acquire Stop button
    void on__psh_acquire_stop_clicked();

    //! Event handler for Show Log button
    void on__psh_show_log_clicked();

    //! Event handler for Output Path button
    void on__psh_output_path_clicked();

    //! Event handler for Settings button
    void on__psh_settings_clicked();
};

#endif // KINECTFRONTEND_H
