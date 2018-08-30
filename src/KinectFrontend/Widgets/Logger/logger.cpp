#include "logger.h"
#include "ui_logger.h"

KLLogger::KLLogger(QWidget *parent) :
    QWidget(parent),
    m_ui_ptr(new Ui::Logger())
{
    m_ui_ptr->setupUi(this);
}

KLLogger::~KLLogger()
{

}

KLLogger::KLLogger(KLLogger &logger_ref):
    m_ui_ptr(logger_ref.get_ui_ptr())
{

}

KLLogger & KLLogger::operator = (KLLogger &logger_ref)
{
    m_ui_ptr = logger_ref.get_ui_ptr();

    return *this;
}

KLLogger::KLLogger(KLLogger &&logger_ref_ref):
    m_ui_ptr(logger_ref_ref.get_ui_ptr())
{

}

KLLogger & KLLogger::operator = (KLLogger &&logger_ref_ref)
{
    m_ui_ptr = logger_ref_ref.get_ui_ptr();

    return *this;
}

int KLLogger::logger_main()
{
    return 1;
}

int KLLogger::logger_kill(bool hard)
{
    destructor(hard);

    return 1;
}

int KLLogger::print(string &string)
{
    m_ui_ptr->_lbl_output_msg->insertPlainText(string.c_str());

    m_ui_ptr->_lbl_output_msg->moveCursor(QTextCursor::End);

    return 1;
}

int KLLogger::destructor(bool hard)
{
    if(hard)
    {

    }

    if(m_ui_ptr != nullptr)
    {
        delete m_ui_ptr;

        m_ui_ptr = nullptr;
    }

    return 1;
}
