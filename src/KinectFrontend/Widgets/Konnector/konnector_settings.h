#ifndef KONNECTOR_SETTINGS_H
#define KONNECTOR_SETTINGS_H

#include <QDialog>

namespace Ui {
class Konnector_Settings;
}

class Konnector_Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Konnector_Settings(QWidget *parent = 0);
    ~Konnector_Settings();

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

private:
    Ui::Konnector_Settings *ui;
};

#endif // KONNECTOR_SETTINGS_H
