#include "konnector.h"

#include <QFileDialog>
#include <QSettings>

Konnector::Konnector(QDialog *parent):
    QDialog(parent),
    m_ui_ptr(new Ui::konnector),
    m_update_ptr(new QTimer(this)),
    m_kinect_interface_ptr(new KinectInterface()),
    m_is_connected(false),
    m_is_acquiring(false)
{

    _logger = new Logger(this);
    connect(this, &Konnector::connection_status_changed, this, &Konnector::updateGUI_state);

    QSettings settings;
    //! \todo Load the settings from a settings file : done. Then actually pass them
    //! in the Backend in a meaningfull way.
    // example:
    //    if(settings.contains("output/set_depth_image"))
    //      BackEnd::setOption ( settings.value("output/set_depth_image").toBool()  );
    //    else
    //      BackEnd::setOption ( default );
    // More details can be found in konnector_settings.

}

Konnector::~Konnector()
{
    destructor(true);
}

Konnector::Konnector(Konnector &kinect_frontend_ref):
    m_ui_ptr(kinect_frontend_ref.get_ui_ptr()),
    m_update_ptr(kinect_frontend_ref.get_update_ptr()),
    m_kinect_interface_ptr(kinect_frontend_ref.get_kinect_interface_ptr()),
    m_is_connected(kinect_frontend_ref.get_is_connected())
{

}

Konnector & Konnector::operator = (Konnector &kinect_frontend_ref)
{
    m_ui_ptr = kinect_frontend_ref.get_ui_ptr();
    m_update_ptr = kinect_frontend_ref.get_update_ptr();
    m_kinect_interface_ptr = kinect_frontend_ref.get_kinect_interface_ptr();
    m_is_connected = kinect_frontend_ref.get_is_connected();

    return *this;
}

Konnector::Konnector(Konnector &&kinect_frontend_ref_ref):
    m_ui_ptr(kinect_frontend_ref_ref.get_ui_ptr()),
    m_update_ptr(kinect_frontend_ref_ref.get_update_ptr()),
    m_kinect_interface_ptr(kinect_frontend_ref_ref.get_kinect_interface_ptr()),
    m_is_connected(kinect_frontend_ref_ref.get_is_connected())
{

}

Konnector & Konnector::operator = (Konnector &&kinect_frontend_ref_ref)
{
    m_ui_ptr = kinect_frontend_ref_ref.get_ui_ptr();
    m_update_ptr = kinect_frontend_ref_ref.get_update_ptr();
    m_kinect_interface_ptr = kinect_frontend_ref_ref.get_kinect_interface_ptr();
    m_is_connected = kinect_frontend_ref_ref.get_is_connected();

    return *this;
}

int Konnector::konnector_main()
{
    m_ui_ptr->setupUi(this);

    _logger->setWindowFlag(Qt::Window);

    updateGUI_state();

    show();

    return 1;
}

int Konnector::konnector_kill(bool hard)
{
    destructor(hard);

    return 1;
}

int Konnector::destructor(bool hard)
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

int Konnector::update_output()
{
    string msg;
    msg = m_kinect_interface_ptr->get_kinect_backend_ref().get_depth_message() +
            m_kinect_interface_ptr->get_kinect_backend_ref().get_video_message() +
            m_kinect_interface_ptr->get_kinect_input_output_ptr()->get_depth_message() +
            m_kinect_interface_ptr->get_kinect_input_output_ptr()->get_point_cloud_message() +
            m_kinect_interface_ptr->get_kinect_input_output_ptr()->get_video_message() +
            m_kinect_interface_ptr->get_kinect_backend_ref().get_message() +
            " \n" + std::to_string(m_kinect_interface_ptr->get_kinect_backend_ref().get_stored_camera_tilt()) + " \n";

    QString q_msg(msg.c_str());

    _logger->print(q_msg);

    return 1;
}

void Konnector::updateGUI_state()
{
    m_ui_ptr->_psh_connect->setEnabled(!m_is_connected);
    m_ui_ptr->_psh_disconnect->setEnabled(m_is_connected && !m_is_acquiring);

    m_ui_ptr->_psh_acquire_start->setEnabled(m_is_connected && !m_is_acquiring);
    m_ui_ptr->_psh_acquire_stop->setEnabled(m_is_connected && m_is_acquiring);

    m_ui_ptr->_psh_output_path->setEnabled(!m_is_acquiring);
    m_ui_ptr->_psh_settings->setEnabled(!m_is_acquiring);

}

void Konnector::update()
{
    if(m_is_connected)
    {
        m_kinect_interface_ptr->update();

        if(_logger->isVisible())
            update_output();
    }
}

void Konnector::on__psh_connect_clicked()
{
    if(!m_is_connected)
    {
        if(m_kinect_interface_ptr->get_kinect_backend_ref().kinect_backend_main() > 0)
        {
            connect(m_update_ptr, SIGNAL(timeout()), this, SLOT(update()));
            m_update_ptr->start(33);

            m_is_connected = true;

            emit connection_status_changed();
        }
    }

    update_output();
}

void Konnector::on__psh_disconnect_clicked()
{
    if(m_is_connected)
    {
        m_kinect_interface_ptr->get_kinect_backend_ref().kinect_backend_kill(false);

        m_update_ptr->stop();

        m_is_connected = false;

        emit connection_status_changed();
    }

    update_output();
}

//void Konnector::on_pushButton_clicked()
//{
//    if(m_is_connected)
//    {
//        m_kinect_interface_ptr->get_kinect_backend_ref().set_stored_camera_tilt(1.0);

//        update_output();
//    }
//}

//void Konnector::on_pushButton_2_clicked()
//{
//    if(m_is_connected)
//    {
//        m_kinect_interface_ptr->get_kinect_backend_ref().set_stored_camera_tilt(-1.0);

//        update_output();
//    }
//}

void Konnector::on__psh_show_log_clicked()
{
    _logger->show();
}

void Konnector::on__psh_output_path_clicked()
{
    _output_path =  QFileDialog::getExistingDirectory (this, tr("Select the output path.") , _output_path,
                                                       QFileDialog::ShowDirsOnly
                                                       | QFileDialog::DontResolveSymlinks);
}

void Konnector::on__psh_settings_clicked()
{
    _setts = new Konnector_Settings(this);
    _setts->exec();
}
