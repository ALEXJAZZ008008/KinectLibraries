#include "logger.h"
#include "ui_logger.h"

Logger::Logger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Logger)
{
    ui->setupUi(this);
}

Logger::~Logger()
{
    delete ui;
}

void Logger::print(const QString& _s)
{
    ui->_lbl_output_msg->insertPlainText(_s);
}
