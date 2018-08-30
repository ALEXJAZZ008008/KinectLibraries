#ifndef KLLOGGER_H
#define KLLOGGER_H

#include <QWidget>

#include <string>

using namespace std;

//! Used by Qt GUI
namespace Ui
{
class Logger;
}

//!
//! \class Logget
//! \brief The Logget class.
//! This is a Qt widget for the KinectFrontend class.
//! This class outputs logs to the screen
//!
class KLLogger : public QWidget
{
public:

    //! Constructor
    explicit KLLogger(QWidget *parent = nullptr);

    //! Destructor
    ~KLLogger();

    //! Copy and move constructos and assignment opperators,
    KLLogger(KLLogger &);
    KLLogger & operator = (KLLogger &);
    KLLogger(KLLogger &&);
    KLLogger & operator = (KLLogger &&);

    //! Gets the ui ptr
    inline Ui::Logger * get_ui_ptr()
    {
        return m_ui_ptr;
    }

    //! Sets the ui ptr
    inline int set_ui_ptr(Ui::Logger * ui_ptr)
    {
        m_ui_ptr = ui_ptr;

        return 1;
    }

    //! Main
    int logger_main();

    //! Destruct remotely
    int logger_kill(bool);

    //! Prints the current log to the screen
    int print(string &);

private:

    //! Macro to indicate this is a QT object
    Q_OBJECT

    //! Holds the pointer to the ui
    Ui::Logger *m_ui_ptr;

    //! Called by destructor
    //! and any other methods aiming to destruct the class
    int destructor(bool);
};

#endif // KLLOGGER_H
