#include "konnector.h"

Konnector::Konnector(QDialog *parent):
    QDialog(parent),
    m_ui_ptr(new Ui::konnector),
    m_logger_ptr(new Logger(this)),
    m_kinect_interface_ptr(new KinectInterface()),
    m_update_ptr(new QTimer(this)),
    m_acquisition_start_time(),
    m_output_path(""),
    m_write_offset(0),
    m_is_connected(false),
    m_is_acquiring(false)
{

}

Konnector::~Konnector()
{
    destructor(true);
}

Konnector::Konnector(Konnector &kinect_frontend_ref):
    m_ui_ptr(kinect_frontend_ref.get_ui_ptr()),
    m_logger_ptr(kinect_frontend_ref.get_logger_ptr()),
    m_kinect_interface_ptr(kinect_frontend_ref.get_kinect_interface_ptr()),
    m_update_ptr(kinect_frontend_ref.get_update_ptr()),
    m_acquisition_start_time(kinect_frontend_ref.get_acquisition_start_time()),
    m_output_path(kinect_frontend_ref.get_output_path()),
    m_is_connected(kinect_frontend_ref.get_is_connected()),
    m_is_acquiring(kinect_frontend_ref.get_is_acquiring())
{

}

Konnector & Konnector::operator = (Konnector &kinect_frontend_ref)
{
    m_ui_ptr = kinect_frontend_ref.get_ui_ptr();
    m_logger_ptr = kinect_frontend_ref.get_logger_ptr();
    m_kinect_interface_ptr = kinect_frontend_ref.get_kinect_interface_ptr();
    m_update_ptr = kinect_frontend_ref.get_update_ptr();
    m_acquisition_start_time = kinect_frontend_ref.get_acquisition_start_time();
    m_output_path = kinect_frontend_ref.get_output_path();
    m_is_connected = kinect_frontend_ref.get_is_connected();
    m_is_acquiring = kinect_frontend_ref.get_is_acquiring();

    return *this;
}

Konnector::Konnector(Konnector &&kinect_frontend_ref_ref):
    m_ui_ptr(kinect_frontend_ref_ref.get_ui_ptr()),
    m_logger_ptr(kinect_frontend_ref_ref.get_logger_ptr()),
    m_kinect_interface_ptr(kinect_frontend_ref_ref.get_kinect_interface_ptr()),
    m_update_ptr(kinect_frontend_ref_ref.get_update_ptr()),
    m_acquisition_start_time(kinect_frontend_ref_ref.get_acquisition_start_time()),
    m_output_path(kinect_frontend_ref_ref.get_output_path()),
    m_is_connected(kinect_frontend_ref_ref.get_is_connected()),
    m_is_acquiring(kinect_frontend_ref_ref.get_is_acquiring())
{

}

Konnector & Konnector::operator = (Konnector &&kinect_frontend_ref_ref)
{
    m_ui_ptr = kinect_frontend_ref_ref.get_ui_ptr();
    m_logger_ptr = kinect_frontend_ref_ref.get_logger_ptr();
    m_kinect_interface_ptr = kinect_frontend_ref_ref.get_kinect_interface_ptr();
    m_update_ptr = kinect_frontend_ref_ref.get_update_ptr();
    m_acquisition_start_time = kinect_frontend_ref_ref.get_acquisition_start_time();
    m_output_path = kinect_frontend_ref_ref.get_output_path();
    m_is_connected = kinect_frontend_ref_ref.get_is_connected();
    m_is_acquiring = kinect_frontend_ref_ref.get_is_acquiring();

    return *this;
}

int Konnector::konnector_main()
{
    m_ui_ptr->setupUi(this);

    m_logger_ptr->setWindowFlag(Qt::Window);

    connect(this, &Konnector::connection_status_changed, this, &Konnector::updateGUI_state);

    //QSettings settings;

    //! \todo Load the settings from a settings file : done. Then actually pass them
    //! in the Backend in a meaningfull way.
    // example:
    //    if(settings.contains("output/set_depth_image"))
    //      BackEnd::setOption ( settings.value("output/set_depth_image").toBool()  );
    //    else
    //      BackEnd::setOption ( default );
    // More details can be found in konnector_settings.

    updateGUI_state();

    show();

    m_logger_ptr->show();

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

    if(m_logger_ptr != nullptr)
    {
        delete m_logger_ptr;

        m_logger_ptr = nullptr;
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
    m_logger_ptr->print(m_kinect_interface_ptr->get_kinect_object_ptr()->get_log().c_str());

    m_kinect_interface_ptr->get_kinect_object_ptr()->get_log() = "";

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

        if(m_logger_ptr->isVisible())
        {
            update_output();
        }

        if(m_write_offset >= 30)
        {
            m_ui_ptr->lbl_frames_recd->setText(to_string(m_kinect_interface_ptr->get_kinect_input_output_ptr()->get_frames_recorded()).c_str());
            m_ui_ptr->lbl_time_lapsed->setText(to_string(duration_cast<duration<int>>(high_resolution_clock::now() - m_acquisition_start_time).count()).c_str());

            m_write_offset = 0;
        }
        else
        {
            ++m_write_offset;
        }
    }
}

void Konnector::on__psh_connect_clicked()
{
    if(!m_is_connected)
    {
        if(m_kinect_interface_ptr->get_kinect_backend_ref().kinect_backend_main() > 0)
        {
            m_is_connected = true;

            updateGUI_state();

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

        if(m_is_acquiring)
        {
            m_update_ptr->stop();

            m_is_acquiring = false;
        }

        m_is_connected = false;

        updateGUI_state();

        emit connection_status_changed();
    }

    update_output();
}

void Konnector::on__psh_acquire_start_clicked()
{
    if(m_is_connected)
    {
        connect(m_update_ptr, SIGNAL(timeout()), this, SLOT(update()));
        m_update_ptr->start(33);

        m_kinect_interface_ptr->get_kinect_input_output_ptr()->set_frames_recorded(0);
        m_acquisition_start_time = high_resolution_clock::now();

        m_is_acquiring = true;

        updateGUI_state();

        emit acquisition_status_changed();
    }

    update_output();
}

void Konnector::on__psh_acquire_stop_clicked()
{
    if(m_is_connected)
    {
        m_update_ptr->stop();

        m_is_acquiring = false;

        updateGUI_state();

        emit acquisition_status_changed();
    }

    update_output();
}

void Konnector::on__psh_show_log_clicked()
{
    if (m_logger_ptr->isVisible())
    {
        m_logger_ptr->show();
    }
    else
    {
        m_logger_ptr->hide();
    }
}

void Konnector::on__psh_output_path_clicked()
{
    m_output_path =  QFileDialog::getExistingDirectory(this,
                                                       tr("Select the output path."),
                                                       m_output_path,
                                                       QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}

void Konnector::on__psh_settings_clicked()
{
    Konnector_Settings *konnector_settings_ptr = new Konnector_Settings(this);

    konnector_settings_ptr->exec();

    if(konnector_settings_ptr != nullptr)
    {
        delete konnector_settings_ptr;

        konnector_settings_ptr = nullptr;
    }
}
