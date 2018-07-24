#ifndef LOGGER_H
#define LOGGER_H

#include <QWidget>

namespace Ui
{
class Logger;
}

class Logger : public QWidget
{
    Q_OBJECT

public:
    explicit Logger(QWidget *parent = 0);
    ~Logger();

    //! Copy and move constructos and assignment opperators,
    Logger(Logger &);
    Logger & operator = (Logger &);
    Logger(Logger &&);
    Logger & operator = (Logger &&);

    inline Ui::Logger * get_ui_ptr()
    {
        return m_ui_ptr;
    }

    inline int set_ui_ptr(Ui::Logger * ui_ptr)
    {
        m_ui_ptr = ui_ptr;

        return 1;
    }

    int logger_main();

    int logger_kill(bool);

    int print(const QString &_s);

private:
    Ui::Logger *m_ui_ptr;

    //! Called by destructor
    //! and any other methods aiming to destruct the class
    int destructor(bool);
};

#endif // LOGGER_H
