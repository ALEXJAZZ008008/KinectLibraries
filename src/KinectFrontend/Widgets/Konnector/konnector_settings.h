#ifndef KONNECTOR_SETTINGS_H
#define KONNECTOR_SETTINGS_H

#include <QDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QFileDialog>

#include "ui_konnector_settings.h"

using namespace std;

namespace Ui
{
class Konnector_Settings;
}

class Konnector_Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Konnector_Settings(QWidget *parent = 0);
    ~Konnector_Settings();

    Konnector_Settings(Konnector_Settings &);
    Konnector_Settings & operator = (Konnector_Settings &);
    Konnector_Settings(Konnector_Settings &&);
    Konnector_Settings & operator = (Konnector_Settings &&);

    inline Ui::Konnector_Settings * get_ui_ptr()
    {
        return m_ui_ptr;
    }

    inline int set_ui_ptr(Ui::Konnector_Settings *ui_ptr)
    {
        m_ui_ptr = ui_ptr;

        return 1;
    }

    int konnector_settings_main();

    int konnector_settings_kill(bool);

private:
    Ui::Konnector_Settings *m_ui_ptr;

    //! Called by destructor
    //! and any other methods aiming to destruct the class
    int destructor(bool);

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();
};

#endif // KONNECTOR_SETTINGS_H
