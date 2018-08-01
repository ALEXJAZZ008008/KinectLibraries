#include "konnector.h"

Konnector::Konnector(QDialog *parent):
    QDialog(parent),
    m_ui_ptr(new Ui::Konnector),
    m_logger_ptr(new Logger(this)),
    m_update_ptr(new QTimer(this)),
    m_kinect_interface_ptr(new KinectInterface()),
    m_acquisition_start_time(),
    m_acquisition_frequency(30.0f),
    m_acquisition_speed(1000.0f / m_acquisition_frequency),
    m_write_offset(0),
    m_is_connected(false),
    m_is_acquiring(false)
{

}

Konnector::~Konnector()
{
    destructor(true);
}

Konnector::Konnector(Konnector &konnector_ref):
    m_ui_ptr(konnector_ref.get_ui_ptr()),
    m_logger_ptr(konnector_ref.get_logger_ptr()),
    m_update_ptr(konnector_ref.get_update_ptr()),
    m_kinect_interface_ptr(konnector_ref.get_kinect_interface_ptr()),
    m_acquisition_start_time(konnector_ref.get_acquisition_start_time()),
    m_acquisition_frequency(konnector_ref.get_acquisition_frequency()),
    m_acquisition_speed(konnector_ref.get_acquisition_speed()),
    m_write_offset(konnector_ref.get_write_offset()),
    m_is_connected(konnector_ref.get_is_connected()),
    m_is_acquiring(konnector_ref.get_is_acquiring())
{

}

Konnector & Konnector::operator = (Konnector &konnector_ref)
{
    m_ui_ptr = konnector_ref.get_ui_ptr();
    m_logger_ptr = konnector_ref.get_logger_ptr();
    m_update_ptr = konnector_ref.get_update_ptr();
    m_kinect_interface_ptr = konnector_ref.get_kinect_interface_ptr();
    m_acquisition_start_time = konnector_ref.get_acquisition_start_time();
    m_acquisition_frequency = konnector_ref.get_acquisition_frequency();
    m_acquisition_speed = konnector_ref.get_acquisition_speed();
    m_write_offset = konnector_ref.get_write_offset();
    m_is_connected = konnector_ref.get_is_connected();
    m_is_acquiring = konnector_ref.get_is_acquiring();

    return *this;
}

Konnector::Konnector(Konnector &&konnector_ref_ref):
    m_ui_ptr(konnector_ref_ref.get_ui_ptr()),
    m_logger_ptr(konnector_ref_ref.get_logger_ptr()),
    m_update_ptr(konnector_ref_ref.get_update_ptr()),
    m_kinect_interface_ptr(konnector_ref_ref.get_kinect_interface_ptr()),
    m_acquisition_start_time(konnector_ref_ref.get_acquisition_start_time()),
    m_acquisition_frequency(konnector_ref_ref.get_acquisition_frequency()),
    m_acquisition_speed(konnector_ref_ref.get_acquisition_speed()),
    m_write_offset(konnector_ref_ref.get_write_offset()),
    m_is_connected(konnector_ref_ref.get_is_connected()),
    m_is_acquiring(konnector_ref_ref.get_is_acquiring())
{

}

Konnector & Konnector::operator = (Konnector &&konnector_ref_ref)
{
    m_ui_ptr = konnector_ref_ref.get_ui_ptr();
    m_logger_ptr = konnector_ref_ref.get_logger_ptr();
    m_update_ptr = konnector_ref_ref.get_update_ptr();
    m_kinect_interface_ptr = konnector_ref_ref.get_kinect_interface_ptr();
    m_acquisition_start_time = konnector_ref_ref.get_acquisition_start_time();
    m_acquisition_frequency = konnector_ref_ref.get_acquisition_frequency();
    m_acquisition_speed = konnector_ref_ref.get_acquisition_speed();
    m_write_offset = konnector_ref_ref.get_write_offset();
    m_is_connected = konnector_ref_ref.get_is_connected();
    m_is_acquiring = konnector_ref_ref.get_is_acquiring();

    return *this;
}

int Konnector::konnector_main()
{
    m_ui_ptr->setupUi(this);

    m_logger_ptr->setWindowFlag(Qt::Window);

    connect(this, &Konnector::connection_status_changed, this, &Konnector::updateGUI_state);
    connect(this, &Konnector::camera_angle_changed, this, &Konnector::updateGUI_state);
    connect(this, &Konnector::acquisition_status_changed, this, &Konnector::updateGUI_state);

    update_settings();

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
    if(hard)
    {

    }

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
        m_kinect_interface_ptr = nullptr;
    }

    return 1;
}

int Konnector::update_settings()
{
    QSettings settings;

    if(settings.contains("defaults/output_path"))
    {
        m_kinect_interface_ptr->get_kinect_input_output_ptr()->set_output_path(settings.value("defaults/output_path").toString().toStdString());
    }
    else
    {
        m_kinect_interface_ptr->get_kinect_input_output_ptr()->set_output_path(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toStdString());
    }

    if(settings.contains("output/set_depth_image"))
    {
        m_kinect_interface_ptr->get_kinect_backend_ref().set_depth_image_bool(settings.value("output/set_depth_image").toBool());
        m_kinect_interface_ptr->get_kinect_input_output_ptr()->set_depth_image_bool(settings.value("output/set_depth_image").toBool());
    }
    else
    {
        m_kinect_interface_ptr->get_kinect_backend_ref().set_depth_image_bool(false);
        m_kinect_interface_ptr->get_kinect_input_output_ptr()->set_depth_image_bool(false);
    }

    if(settings.contains("output/set_rgb_image"))
    {
        m_kinect_interface_ptr->get_kinect_backend_ref().set_rgb_image_bool(settings.value("output/set_rgb_image").toBool());
        m_kinect_interface_ptr->get_kinect_input_output_ptr()->set_rgb_image_bool(settings.value("output/set_rgb_image").toBool());
    }
    else
    {
        m_kinect_interface_ptr->get_kinect_backend_ref().set_rgb_image_bool(false);
        m_kinect_interface_ptr->get_kinect_input_output_ptr()->set_rgb_image_bool(false);
    }

    if(settings.contains("input/resolution_small"))
    {
        m_kinect_interface_ptr->get_kinect_backend_ref().set_resolution_small_bool(settings.value("input/resolution_small").toBool());
    }
    else
    {
        m_kinect_interface_ptr->get_kinect_backend_ref().set_resolution_small_bool(false);
    }

    if(settings.contains("input/resolution_med"))
    {
        m_kinect_interface_ptr->get_kinect_backend_ref().set_resolution_med_bool(settings.value("input/resolution_med").toBool());
    }
    else
    {
        m_kinect_interface_ptr->get_kinect_backend_ref().set_resolution_med_bool(true);
    }

    if(settings.contains("input/resolution_high"))
    {
        m_kinect_interface_ptr->get_kinect_backend_ref().set_resolution_high_bool(settings.value("input/resolution_high").toBool());
    }
    else
    {
        m_kinect_interface_ptr->get_kinect_backend_ref().set_resolution_high_bool(true);
    }

    return 1;
}

int Konnector::update_output()
{
    m_logger_ptr->print(m_kinect_interface_ptr->get_kinect_object_ptr()->get_log().c_str());

    m_ui_ptr->le_cur_tilt->setText(to_string(m_kinect_interface_ptr->get_kinect_object_ptr()->get_current_camera_tilt()).c_str());

    m_kinect_interface_ptr->get_kinect_object_ptr()->get_log() = "";

    return 1;
}

void Konnector::updateGUI_state()
{
    m_ui_ptr->_psh_connect->setEnabled(!m_is_connected);
    m_ui_ptr->_psh_disconnect->setEnabled(m_is_connected && !m_is_acquiring);

    m_ui_ptr->psh_tilt_up->setEnabled(m_is_connected);
    m_ui_ptr->psh_tilt_down->setEnabled(m_is_connected);
    m_ui_ptr->le_cur_tilt->setEnabled(m_is_connected);

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

        if(m_write_offset >= m_acquisition_frequency)
        {
            if(m_logger_ptr->isVisible())
            {
                update_output();
            }

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

            m_logger_ptr->show();

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

        emit connection_status_changed();
    }

    update_output();
}

void Konnector::on_psh_tilt_up_clicked()
{
    if(m_is_connected)
    {
        KinectBackend::getInstance().set_current_camera_tilt_with_increment(1.0);

        emit camera_angle_changed();
    }

    update_output();
}

void Konnector::on_psh_tilt_down_clicked()
{
    if(m_is_connected)
    {
        KinectBackend::getInstance().set_current_camera_tilt_with_increment(-1.0);

        emit camera_angle_changed();
    }

    update_output();
}

void Konnector::on_le_cur_tilt_returnPressed()
{
    if(m_is_connected)
    {
        KinectBackend::getInstance().set_current_camera_tilt_with_angle(m_ui_ptr->le_cur_tilt->text().toDouble());

        emit camera_angle_changed();
    }

    update_output();
}

void Konnector::on__psh_acquire_start_clicked()
{
    if(m_is_connected)
    {
        connect(m_update_ptr, SIGNAL(timeout()), this, SLOT(update()));
        m_update_ptr->start(static_cast<int>(m_acquisition_frequency));

        m_kinect_interface_ptr->get_kinect_input_output_ptr()->set_frames_recorded(0);
        m_acquisition_start_time = high_resolution_clock::now();

        m_is_acquiring = true;

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

        emit acquisition_status_changed();
    }

    update_output();
}

void Konnector::on__psh_output_path_clicked()
{
    m_kinect_interface_ptr->get_kinect_input_output_ptr()->get_output_path() =
            QFileDialog::getExistingDirectory(this,
                                              tr("Select the output path."),
                                              QString::fromStdString(m_kinect_interface_ptr->get_kinect_input_output_ptr()->get_output_path()),
                                              QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks).toStdString();

    updateGUI_state();
}

void Konnector::on__psh_show_log_clicked()
{
    if (!m_logger_ptr->isVisible())
    {
        m_logger_ptr->show();
    }
    else
    {
        m_logger_ptr->hide();
    }

    updateGUI_state();
}

void Konnector::on__psh_settings_clicked()
{
    Konnector_Settings *konnector_settings_ptr = new Konnector_Settings(this);

    konnector_settings_ptr->exec();

    update_settings();

    if(konnector_settings_ptr != nullptr)
    {
        delete konnector_settings_ptr;

        konnector_settings_ptr = nullptr;
    }

    updateGUI_state();
}
