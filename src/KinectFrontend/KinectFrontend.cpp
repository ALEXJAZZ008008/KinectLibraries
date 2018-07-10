#include "KinectFrontend.h"

KinectFrontend::KinectFrontend(QDialog *parent):
    QDialog(parent),
    m_ui_ptr(new Ui::motor_control()),
    m_update_ptr(new QTimer(this)),
    m_kinect_interface_ptr(new KinectInterface()),
    m_is_connected(false)
{

}

KinectFrontend::~KinectFrontend()
{
    destructor(true);
}

KinectFrontend::KinectFrontend(KinectFrontend &kinect_frontend_ref):
    m_ui_ptr(kinect_frontend_ref.get_ui_ptr()),
    m_update_ptr(kinect_frontend_ref.get_update_ptr()),
    m_kinect_interface_ptr(kinect_frontend_ref.get_kinect_interface_ptr()),
    m_is_connected(kinect_frontend_ref.get_is_connected())
{

}

KinectFrontend & KinectFrontend::operator = (KinectFrontend &kinect_frontend_ref)
{
    m_ui_ptr = kinect_frontend_ref.get_ui_ptr();
    m_update_ptr = kinect_frontend_ref.get_update_ptr();
    m_kinect_interface_ptr = kinect_frontend_ref.get_kinect_interface_ptr();
    m_is_connected = kinect_frontend_ref.get_is_connected();

    return *this;
}

KinectFrontend::KinectFrontend(KinectFrontend &&kinect_frontend_ref_ref):
    m_ui_ptr(kinect_frontend_ref_ref.get_ui_ptr()),
    m_update_ptr(kinect_frontend_ref_ref.get_update_ptr()),
    m_kinect_interface_ptr(kinect_frontend_ref_ref.get_kinect_interface_ptr()),
    m_is_connected(kinect_frontend_ref_ref.get_is_connected())
{

}

KinectFrontend & KinectFrontend::operator = (KinectFrontend &&kinect_frontend_ref_ref)
{
    m_ui_ptr = kinect_frontend_ref_ref.get_ui_ptr();
    m_update_ptr = kinect_frontend_ref_ref.get_update_ptr();
    m_kinect_interface_ptr = kinect_frontend_ref_ref.get_kinect_interface_ptr();
    m_is_connected = kinect_frontend_ref_ref.get_is_connected();

    return *this;
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

    if(m_update_ptr != nullptr)
    {
        delete m_update_ptr;

        m_update_ptr = nullptr;
    }

    if(m_kinect_interface_ptr != nullptr)
    {
        delete m_kinect_interface_ptr;

        m_kinect_interface_ptr = nullptr;
    }
}

int KinectFrontend::update_output()
{
    m_ui_ptr->_lbl_output_msg->insertPlainText(m_kinect_interface_ptr->get_kinect_backend_ref().get_depth_output().c_str());
    m_ui_ptr->_lbl_output_msg->insertPlainText(m_kinect_interface_ptr->get_kinect_backend_ref().get_video_output().c_str());

    m_ui_ptr->_lbl_output_msg->insertPlainText(m_kinect_interface_ptr->get_kinect_input_output_ptr()->get_depth_output().c_str());
    m_ui_ptr->_lbl_output_msg->insertPlainText(m_kinect_interface_ptr->get_kinect_input_output_ptr()->get_point_cloud_output().c_str());
    m_ui_ptr->_lbl_output_msg->insertPlainText(m_kinect_interface_ptr->get_kinect_input_output_ptr()->get_video_output().c_str());

    m_ui_ptr->_lbl_output_msg->insertPlainText(m_kinect_interface_ptr->get_kinect_backend_ref().get_output().c_str());

    m_ui_ptr->_lbl_output_msg->moveCursor(QTextCursor::End);

    m_ui_ptr->_lne_current_tilt->setText(QString::number(m_kinect_interface_ptr->get_kinect_backend_ref().get_stored_camera_tilt()));

    return 1;
}

void KinectFrontend::update()
{
    if(m_is_connected)
    {
        m_kinect_interface_ptr->update();

        update_output();
    }
}

void KinectFrontend::on__psh_connect_clicked()
{
    if(!m_is_connected)
    {
        if(m_kinect_interface_ptr->get_kinect_backend_ref().kinect_backend_main() > 0)
        {
            connect(m_update_ptr, SIGNAL(timeout()), this, SLOT(update()));
            m_update_ptr->start(33);

            m_is_connected = true;
        }
    }

    update_output();
}

void KinectFrontend::on__psh_disconnect_clicked()
{
    if(m_is_connected)
    {
        m_kinect_interface_ptr->get_kinect_backend_ref().kinect_backend_kill(false);

        m_update_ptr->stop();

        m_is_connected = false;
    }

    update_output();
}

void KinectFrontend::on_pushButton_clicked()
{
    if(m_is_connected)
    {
        m_kinect_interface_ptr->get_kinect_backend_ref().set_stored_camera_tilt(1.0);

        update_output();
    }
}

void KinectFrontend::on_pushButton_2_clicked()
{
    if(m_is_connected)
    {
        m_kinect_interface_ptr->get_kinect_backend_ref().set_stored_camera_tilt(-1.0);

        update_output();
    }
}
