#include "konnector_settings.h"

Konnector_Settings::Konnector_Settings(QWidget *parent) :
    QDialog(parent),
    m_ui_ptr(new Ui::Konnector_Settings)
{
    m_ui_ptr->setupUi(this);
    QSettings settings;

    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());

    if(settings.contains("output/set_depth_image"))
    {
        m_ui_ptr->_chk_ouput_depth->setChecked(settings.value("output/set_depth_image").toBool());
    }
    else
    {
        m_ui_ptr->_chk_ouput_depth->setChecked(false);
    }

    if(settings.contains("output/set_rgb_image"))
    {
        m_ui_ptr->_chk_output_rgb->setChecked(settings.value("output/set_rgb_image").toBool());
    }
    else
    {
        m_ui_ptr->_chk_output_rgb->setChecked(false);
    }

    if(settings.contains("output/set_pc_txt"))
    {
        m_ui_ptr->_chk_output_pc_txt->setChecked(settings.value("output/set_pc_txt").toBool());
    }
    else
    {
        m_ui_ptr->_chk_output_pc_txt->setChecked(false);
    }

    if(settings.contains("output/set_pc_bin"))
    {
        m_ui_ptr->_chk_output_pc_bin->setChecked(settings.value("output/set_pc_bin").toBool());
    }
    else
    {
        m_ui_ptr->_chk_output_pc_bin->setChecked(false);
    }

    if(settings.contains("input/resolution_small"))
    {
        m_ui_ptr->rd_small->setChecked(settings.value("input/resolution_small").toBool());
    }
    else
    {
        m_ui_ptr->rd_small->setChecked(false);
    }

    if(settings.contains("input/resolution_med"))
    {
        m_ui_ptr->rd_med->setChecked(settings.value("input/resolution_med").toBool());
    }
    else
    {
        m_ui_ptr->rd_med->setChecked(true);
    }

    if(settings.contains("input/resolution_high"))
    {
        m_ui_ptr->rd_high->setChecked(settings.value("input/resolution_high").toBool());
    }
    else
    {
        m_ui_ptr->rd_high->setChecked(false);
    }

    if(settings.contains("defaults/output_path"))
    {
        m_ui_ptr->_le_default_output->setText(settings.value("defaults/output_path").toString());
    }
    else
    {
        m_ui_ptr->_le_default_output->setText(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    }
}

Konnector_Settings::~Konnector_Settings()
{
    if(m_ui_ptr != nullptr)
    {
        delete m_ui_ptr;

        m_ui_ptr = nullptr;
    }
}

void Konnector_Settings::on_buttonBox_accepted()
{
    QSettings settings;

    settings.setValue("output/set_depth_image", m_ui_ptr->_chk_ouput_depth->isChecked());
    settings.setValue("output/set_rgb_image", m_ui_ptr->_chk_output_rgb->isChecked());
    settings.setValue("output/set_pc_txt", m_ui_ptr->_chk_output_pc_txt->isChecked());
    settings.setValue("output/set_pc_bin", m_ui_ptr->_chk_output_pc_bin->isChecked());

    settings.setValue("input/resolution_small", m_ui_ptr->rd_small->isChecked());
    settings.setValue("input/resolution_med", m_ui_ptr->rd_med->isChecked());
    settings.setValue("input/resolution_high", m_ui_ptr->rd_high->isChecked());

    settings.setValue("defaults/output_path", m_ui_ptr->_le_default_output->text());
}

void Konnector_Settings::on_pushButton_clicked()
{
    QString _output_path =  QFileDialog::getExistingDirectory (this, tr("Select the output path."),
                                                               _output_path,
                                                               QFileDialog::ShowDirsOnly
                                                               | QFileDialog::DontResolveSymlinks);

    m_ui_ptr->_le_default_output->setText(_output_path);
}
