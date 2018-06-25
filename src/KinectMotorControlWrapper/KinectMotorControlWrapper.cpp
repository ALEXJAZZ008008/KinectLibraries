#include "KinectMotorControlWrapper.h"

KinectFrontend::KinectFrontend(QDialog *parent): QDialog(parent), m_ui_ptr(new Ui::motor_control)
{
    m_ui_ptr->setupUi(this);
    //m_update_ptr =new QTimer(this);
}

KinectFrontend::~KinectFrontend()
{
    delete m_ui_ptr;
}

void KinectFrontend::update()
{

}

void KinectFrontend::on__psh_connect_clicked()
{
    KinectBackend::getInstance().create_connections();

    m_ui_ptr->_lbl_output_msg->setText(KinectBackend::getInstance().get_output().c_str());

    //connect(m_update_ptr, SIGNAL(timeout()), this, SLOT(update()));
    //m_update_ptr->start(100);
}

void KinectFrontend::on_pushButton_clicked()
{
    KinectBackend::getInstance().set_camera_tilt(1.0);

    m_ui_ptr->_lbl_output_msg->setText(KinectBackend::getInstance().get_output().c_str());

    m_ui_ptr->_lne_current_tilt->setText(QString::number(KinectBackend::getInstance().get_camera_tilt()));
}

void KinectFrontend::on_pushButton_2_clicked()
{
    KinectBackend::getInstance().set_camera_tilt(-1.0);

    m_ui_ptr->_lbl_output_msg->setText(KinectBackend::getInstance().get_output().c_str());

    m_ui_ptr->_lne_current_tilt->setText(QString::number(KinectBackend::getInstance().get_camera_tilt()));
}
