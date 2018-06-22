#include "KinectMotorControlWrapper.h"

MotorControl::MotorControl(QDialog *parent): QDialog(parent), m_ui_ptr(new Ui::motor_control), m_increment(2.0), m_num_devices(0)
{
    m_ui_ptr->setupUi(this);
    m_update_ptr =new QTimer(this);
    create_connections();
}

MotorControl::~MotorControl()
{
    freenect_stop_depth(m_fdev_ptr);
    freenect_stop_video(m_fdev_ptr);
    freenect_close_device(m_fdev_ptr);
    freenect_shutdown(m_fctx_ptr);

    delete m_ui_ptr;
}

MotorControl & MotorControl::getInstance( )
{
    static MotorControl instance;

    return instance;
}

void MotorControl::depth_callback(freenect_device *fdev_ptr, void *data, unsigned int timestamp)
{
     getInstance().get_ui()->_lbl_output_msg->append("Received depth frame at " + QString::number(timestamp));
}

void MotorControl::video_callback(freenect_device *fdev_ptr, void *data, unsigned int timestamp)
{
     getInstance().get_ui()->_lbl_output_msg->append("Received video frame at "  + QString::number(timestamp));
}

void MotorControl::create_connections()
{
    //Create some connections
}

int MotorControl::update_tilt_state()
{
    int status = freenect_update_tilt_state(m_fdev_ptr);

    if (status < 0)
    {
        m_ui_ptr->_lbl_output_msg->append("Unable to retrieve tilt status!!\n");

        return 0;
    }

    m_current_tilt_state_ptr = freenect_get_tilt_state(m_fdev_ptr);

    return 0;
}

double MotorControl::get_camera_tilt()
{
    update_tilt_state();

    return freenect_get_tilt_degs(m_current_tilt_state_ptr);
}

int MotorControl::set_camera_tilt(double camera_tilt)
{
    freenect_tilt_status_code tilt_status;

    while(!(tilt_status == freenect_tilt_status_code::TILT_STATUS_STOPPED))
    {
        if (freenect_set_tilt_degs(m_fdev_ptr, camera_tilt) < 0)
        {
            m_ui_ptr->_lbl_output_msg->append("Unable to tilt the camera!!\n");
        }

        tilt_status = freenect_get_tilt_status(m_current_tilt_state_ptr);
    }

    return 0;
}

int MotorControl::update_camera_tilt(double increment)
{
    set_camera_tilt(get_camera_tilt() + increment);
    update_tilt_state();

    m_ui_ptr->_lne_current_tilt->setText(QString::number(get_camera_tilt()));
}

void MotorControl::update()
{
    freenect_process_events(m_fctx_ptr);
}

void MotorControl::on__psh_connect_clicked()
{
    if (freenect_init(&m_fctx_ptr, NULL) < 0)
    {
        QMessageBox msgBox;
        QPushButton *abortButton = msgBox.addButton(QMessageBox::Abort);

        msgBox.setText("Initialisation error!");
        msgBox.exec();

        if (msgBox.clickedButton() == abortButton)
        {
            //abort nicely.

            getInstance().~MotorControl();

            return;
        }
    }

    freenect_set_log_level(m_fctx_ptr, FREENECT_LOG_DEBUG);
    freenect_select_subdevices(m_fctx_ptr, (freenect_device_flags)(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));

    m_num_devices = freenect_num_devices(m_fctx_ptr);

    m_ui_ptr->_lbl_output_msg->clear();

    if (m_num_devices == 0)
    {
        m_ui_ptr->_lbl_output_msg->append("No devices found!!\n");

        return;
    }
    else
    {
        m_ui_ptr->_lbl_output_msg->setText("Num devices found: " + QString::number(m_num_devices) + "\n");
    }

    //Open the device.
    if (freenect_open_device(m_fctx_ptr, &m_fdev_ptr, 0) < 0)
    {
        m_ui_ptr->_lbl_output_msg->append("Unable to connect with camera!!\n");

        return;
    }

    //Set depth and video modes
    if(freenect_set_depth_mode(m_fdev_ptr, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_MM)))
    {
        m_ui_ptr->_lbl_output_msg->append("Unable to set depth mode!!\n");

        return;
    }

    if (freenect_set_video_mode(m_fdev_ptr, freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB)))
    {
        m_ui_ptr->_lbl_output_msg->append("Unable to set video mode!!\n");

        return;
    }

    //Set frame callback
    freenect_set_depth_callback(m_fdev_ptr, MotorControl::depth_callback);
    freenect_set_video_callback(m_fdev_ptr, MotorControl::video_callback);

    if (freenect_start_depth(m_fdev_ptr))
    {
        m_ui_ptr->_lbl_output_msg->append("Unable to start depth!!\n");

        return;
    }

    if (freenect_start_video(m_fdev_ptr))
    {
        m_ui_ptr->_lbl_output_msg->append("Unable to start video!!\n");

        return;
    }

    connect(m_update_ptr, SIGNAL(timeout()), this, SLOT(update()));
    //m_update_ptr->start(100);
}

void MotorControl::on_pushButton_clicked()
{
    update_camera_tilt(m_increment);
}

void MotorControl::on_pushButton_2_clicked()
{
    update_camera_tilt(-m_increment);
}
