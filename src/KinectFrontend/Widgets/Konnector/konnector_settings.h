#ifndef KONNECTOR_SETTINGS_H
#define KONNECTOR_SETTINGS_H

#include <QDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QFileDialog>

#include "ui_konnector_settings.h"

using namespace std;

//! Used by Qt GUI
namespace Ui
{
class Konnector_Settings;
}

//!
//! \class Konnector_Settings
//! \brief The Konnector Settings class.
//! Holds the settings for the Konnector class
//!
class Konnector_Settings : public QDialog
{
public:
    //! Constructor
    explicit Konnector_Settings(QWidget *parent = nullptr);

    //! Destructor
    ~Konnector_Settings();

    //! Copy and move constructos and assignment opperators
    Konnector_Settings(Konnector_Settings &);
    Konnector_Settings & operator = (Konnector_Settings &);
    Konnector_Settings(Konnector_Settings &&);
    Konnector_Settings & operator = (Konnector_Settings &&);

    //! Gets the ui ptr
    inline Ui::Konnector_Settings * get_ui_ptr()
    {
        return m_ui_ptr;
    }

    //! Sets the ui ptr
    inline int set_ui_ptr(Ui::Konnector_Settings *ui_ptr)
    {
        m_ui_ptr = ui_ptr;

        return 1;
    }

    //! Main
    int konnector_settings_main();

    //! Destruct remotely
    int konnector_settings_kill(bool);

private:

    //! Macro to indicate this is a QT object
    Q_OBJECT

    //! Pointer to the UI namespace
    Ui::Konnector_Settings *m_ui_ptr;

    //! Called by destructor
    //! and any other methods aiming to destruct the class
    int destructor(bool);

private slots:

    //! Event handler for Buttons
    void on_buttonBox_accepted();

    //! Event handler for button
    void on_pushButton_clicked();
};

#endif // KONNECTOR_SETTINGS_H
