#ifndef KONNECTOR_SETTINGS_H
#define KONNECTOR_SETTINGS_H

#include <QDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QFileDialog>

#include "ui_konnector_settings.h"

namespace Ui {
class Konnector_Settings;
}

class Konnector_Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Konnector_Settings(QWidget *parent = 0);
    ~Konnector_Settings();

    inline Ui::Konnector_Settings * get_ui()
    {
        return m_ui_ptr;
    }

private:
    Ui::Konnector_Settings *m_ui_ptr;

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();
};

#endif // KONNECTOR_SETTINGS_H
