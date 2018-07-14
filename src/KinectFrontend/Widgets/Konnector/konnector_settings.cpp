#include "konnector_settings.h"
#include "ui_konnector_settings.h"

#include <QSettings>
#include <QStandardPaths>
#include <QFileDialog>

Konnector_Settings::Konnector_Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Konnector_Settings)
{
    ui->setupUi(this);
    QSettings settings;

    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());

    if(settings.contains("output/set_depth_image"))
        ui->_chk_ouput_depth->setChecked(settings.value("output/set_depth_image").toBool());
    else
        ui->_chk_ouput_depth->setChecked(false);

    if(settings.contains("output/set_rgb_image"))
        ui->_chk_output_rgb->setChecked(settings.value("output/set_rgb_image").toBool());
    else
        ui->_chk_output_rgb->setChecked(false);

    if(settings.contains("output/set_pc_txt"))
        ui->_chk_output_pc_txt->setChecked(settings.value("output/set_pc_txt").toBool());
    else
        ui->_chk_output_pc_txt->setChecked(false);

    if(settings.contains("output/set_pc_bin"))
        ui->_chk_output_pc_bin->setChecked(settings.value("output/set_pc_bin").toBool());
    else
        ui->_chk_output_pc_bin->setChecked(false);

    if(settings.contains("input/resolution_small"))
        ui->rd_small->setChecked(settings.value("input/resolution_small").toBool());
    else
        ui->rd_small->setChecked(false);

    if(settings.contains("input/resolution_med"))
        ui->rd_med->setChecked(settings.value("input/resolution_med").toBool());
    else
        ui->rd_med->setChecked(true);

    if(settings.contains("input/resolution_high"))
        ui->rd_high->setChecked(settings.value("input/resolution_high").toBool());
    else
        ui->rd_high->setChecked(false);

    if(settings.contains("defaults/output_path"))
        ui->_le_default_output->setText(settings.value("defaults/output_path").toString());
    else
    {
        QString tmp = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        ui->_le_default_output->setText(tmp);
    }
}

Konnector_Settings::~Konnector_Settings()
{
    delete ui;
}

void Konnector_Settings::on_buttonBox_accepted()
{
    QSettings settings;
    settings.setValue("output/set_depth_image", ui->_chk_ouput_depth->isChecked());
    settings.setValue("output/set_rgb_image", ui->_chk_output_rgb->isChecked());
    settings.setValue("output/set_pc_txt", ui->_chk_output_pc_txt->isChecked());
    settings.setValue("output/set_pc_bin", ui->_chk_output_pc_bin->isChecked());

    settings.setValue("input/resolution_small", ui->rd_small->isChecked());
    settings.setValue("input/resolution_med", ui->rd_med->isChecked());
    settings.setValue("input/resolution_high", ui->rd_high->isChecked());

     settings.setValue("defaults/output_path", ui->_le_default_output->text());

}

void Konnector_Settings::on_pushButton_clicked()
{
    QString _output_path =  QFileDialog::getExistingDirectory (this, tr("Select the output path.") , _output_path,
                                                       QFileDialog::ShowDirsOnly
                                                       | QFileDialog::DontResolveSymlinks);

    ui->_le_default_output->setText(_output_path);
}
