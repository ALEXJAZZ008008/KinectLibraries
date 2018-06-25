#include "KinectFrontend.h"

KinectFrontend::KinectFrontend(QDialog *parent): QDialog(parent), m_ui_ptr(new Ui::motor_control)
{
    m_ui_ptr->setupUi(this);
}

KinectFrontend::~KinectFrontend()
{
    if(m_ui_ptr != nullptr)
    {
        delete m_ui_ptr;

        m_ui_ptr = nullptr;
    }
}

KinectFrontend & KinectFrontend::getInstance( )
{
    static KinectFrontend instance;

    return instance;
}

void * KinectFrontend::update(void *args)
{
    KinectBackend::getInstance().update();

    KinectFrontend::getInstance().m_ui_ptr->_lbl_output_msg->append(KinectBackend::getInstance().get_output().c_str());
    KinectFrontend::getInstance().m_ui_ptr->_lbl_output_msg->append(KinectBackend::getInstance().get_depth_output().c_str());
    KinectFrontend::getInstance().m_ui_ptr->_lbl_output_msg->append(KinectBackend::getInstance().get_video_output().c_str());
}

void KinectFrontend::on__psh_connect_clicked()
{
    KinectBackend::getInstance().kinect_backend_main();

    m_ui_ptr->_lbl_output_msg->append(KinectBackend::getInstance().get_output().c_str());

    pthread_create(&m_update, NULL, update, NULL);
}

void KinectFrontend::on_pushButton_clicked()
{
    KinectBackend::getInstance().set_camera_tilt(1.0);

    m_ui_ptr->_lbl_output_msg->append(KinectBackend::getInstance().get_output().c_str());

    m_ui_ptr->_lne_current_tilt->setText(QString::number(KinectBackend::getInstance().get_camera_tilt()));
}

void KinectFrontend::on_pushButton_2_clicked()
{
    KinectBackend::getInstance().set_camera_tilt(-1.0);

    m_ui_ptr->_lbl_output_msg->append(KinectBackend::getInstance().get_output().c_str());

    m_ui_ptr->_lne_current_tilt->setText(QString::number(KinectBackend::getInstance().get_camera_tilt()));
}
