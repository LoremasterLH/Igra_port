#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QSettings>
#include <QCheckBox>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private:
    Ui::Settings *ui;

    void accept();
    void setCheckBox(QCheckBox *box, bool value);
    void saveSettings(int radioButton);
};

#endif // SETTINGS_H
