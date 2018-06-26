#include "KinectFrontend.h"

KinectFrontend::KinectFrontend(QDialog *parent): QDialog(parent), m_ui_ptr(new Ui::motor_control), m_update(nullptr)
{
    m_ui_ptr->setupUi(this);
}

KinectFrontend::~KinectFrontend()
{
    destructor();
}

int KinectFrontend::destructor()
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
}

int KinectFrontend::update_output()
{
    //m_ui_ptr->_lbl_output_msg->insertPlainText(KinectBackend::getInstance().get_depth_output().c_str());
    //m_ui_ptr->_lbl_output_msg->insertPlainText(KinectBackend::getInstance().get_video_output().c_str());

    m_ui_ptr->_lbl_output_msg->insertPlainText(KinectBackend::getInstance().get_output().c_str());

    m_ui_ptr->_lbl_output_msg->moveCursor(QTextCursor::End);

    m_ui_ptr->_lne_current_tilt->setText(QString::number(KinectBackend::getInstance().get_camera_tilt()));

    return 1;
}

void KinectFrontend::update()
{
    KinectBackend::getInstance().update();

    update_output();
}

void KinectFrontend::on__psh_connect_clicked()
{
    if(KinectBackend::getInstance().kinect_backend_main() > 0)
    {
        if(m_update == nullptr)
        {
            m_update = new QTimer(this);
        }

        connect(m_update, SIGNAL(timeout()), this, SLOT(update()));
        m_update->start(33);
    }

    update_output();
}

void KinectFrontend::on__psh_disconnect_clicked()
{
    KinectBackend::getInstance().kinect_backend_kill();

    m_update->stop();

    update_output();
}

void KinectFrontend::on_pushButton_clicked()
{
    KinectBackend::getInstance().set_camera_tilt(1.0);

    update_output();
}

void KinectFrontend::on_pushButton_2_clicked()
{
    KinectBackend::getInstance().set_camera_tilt(-1.0);

    update_output();
}
