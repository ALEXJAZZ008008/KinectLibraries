#include "KinectFrontend.h"

KinectFrontend::KinectFrontend(QDialog *parent):
    QDialog(parent),
    m_ui_ptr(new Ui::motor_control),
    m_update(new QTimer(this)),
    m_kinect_input_output_ptr(new KinectInputOutput(KinectBackend::getInstance().get_kinect_object_ptr())),
    is_connected(false)
{

}

KinectFrontend::~KinectFrontend()
{
    destructor(true);
}

int KinectFrontend::kinect_frontend_main()
{
    m_ui_ptr->setupUi(this);

    show();

    return 1;
}

int KinectFrontend::kinect_frontend_kill(bool hard)
{
    destructor(hard);

    return 1;
}

int KinectFrontend::destructor(bool hard)
{
    if(m_ui_ptr != nullptr)
    {
        delete m_ui_ptr;

        m_ui_ptr = nullptr;
    }

    if(m_update != nullptr)
    {
        delete m_update;

        m_update = nullptr;
    }

    if(m_kinect_input_output_ptr != nullptr)
    {
        delete m_kinect_input_output_ptr;

        m_kinect_input_output_ptr = nullptr;
    }
}

int KinectFrontend::update_output()
{
    m_ui_ptr->_lbl_output_msg->insertPlainText(KinectBackend::getInstance().get_depth_output().c_str());
    m_ui_ptr->_lbl_output_msg->insertPlainText(KinectBackend::getInstance().get_video_output().c_str());

    m_ui_ptr->_lbl_output_msg->insertPlainText(m_kinect_input_output_ptr->get_depth_output().c_str());
    m_ui_ptr->_lbl_output_msg->insertPlainText(m_kinect_input_output_ptr->get_point_cloud_output().c_str());
    m_ui_ptr->_lbl_output_msg->insertPlainText(m_kinect_input_output_ptr->get_video_output().c_str());

    m_ui_ptr->_lbl_output_msg->insertPlainText(KinectBackend::getInstance().get_output().c_str());

    m_ui_ptr->_lbl_output_msg->moveCursor(QTextCursor::End);

    m_ui_ptr->_lne_current_tilt->setText(QString::number(KinectBackend::getInstance().get_stored_camera_tilt()));

    return 1;
}

void KinectFrontend::update()
{
    if(is_connected)
    {
        KinectBackend::getInstance().update();

        m_kinect_input_output_ptr->kinect_input_output_main();

        update_output();
    }
}

void KinectFrontend::on__psh_connect_clicked()
{
    if(KinectBackend::getInstance().kinect_backend_main() > 0)
    {
        connect(m_update, SIGNAL(timeout()), this, SLOT(update()));
        m_update->start(33);

        is_connected = true;
    }

    update_output();
}

void KinectFrontend::on__psh_disconnect_clicked()
{
    KinectBackend::getInstance().kinect_backend_kill(false);

    m_update->stop();

    is_connected = false;

    update_output();
}

void KinectFrontend::on_pushButton_clicked()
{
    if(is_connected)
    {
        KinectBackend::getInstance().set_stored_camera_tilt(1.0);

        update_output();
    }
}

void KinectFrontend::on_pushButton_2_clicked()
{
    if(is_connected)
    {
        KinectBackend::getInstance().set_stored_camera_tilt(-1.0);

        update_output();
    }
}
