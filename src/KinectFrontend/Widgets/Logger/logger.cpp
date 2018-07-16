#include "logger.h"
#include "ui_logger.h"

Logger::Logger(QWidget *parent) :
    QWidget(parent),
    m_ui_ptr(new Ui::Logger())
{
    m_ui_ptr->setupUi(this);
}

Logger::~Logger()
{
    delete m_ui_ptr;
}

Logger::Logger(Logger &logger_ref):
    m_ui_ptr(logger_ref.get_ui_ptr())
{

}

Logger & Logger::operator = (Logger &logger_ref)
{
    m_ui_ptr = logger_ref.get_ui_ptr();

    return *this;
}

Logger::Logger(Logger &&logger_ref_ref):
    m_ui_ptr(logger_ref_ref.get_ui_ptr())
{

}

Logger & Logger::operator = (Logger &&logger_ref_ref)
{
    m_ui_ptr = logger_ref_ref.get_ui_ptr();

    return *this;
}

void Logger::print(const QString& string)
{
    m_ui_ptr->_lbl_output_msg->insertPlainText(string);
}
