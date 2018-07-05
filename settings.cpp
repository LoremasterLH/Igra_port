#include "settings.h"
#include "ui_settings.h"
#include <QDebug>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    QCoreApplication::setOrganizationName("Martin");
    QCoreApplication::setApplicationName("The Game");

    QSettings settings;

    int num = settings.value("player/number").toInt();
    switch(num)
    {
    case 2:
        ui->radioButton2->setChecked(true);
        break;
    case 3:
        ui->radioButton3->setChecked(true);
        break;
    case 4:
        ui->radioButton4->setChecked(true);
        break;
    default:
        qDebug() << "Radio button not found. Valid values 2-4. Value: " << num;
    }

    setCheckBox(ui->checkBoxBlack, settings.value("player/black").toBool());
    setCheckBox(ui->checkBoxGreen, settings.value("player/green").toBool());
    setCheckBox(ui->checkBoxRed, settings.value("player/red").toBool());
    setCheckBox(ui->checkBoxWhite, settings.value("player/white").toBool());
    //qDebug() << settings.value("player/black").toBool() << settings.value("player/green").toBool() <<  settings.value("player/red").toBool() <<  settings.value("player/white").toBool();
    /*if(settings.value("player/black").toBool())
        ui->checkBoxBlack->setChecked(true);
    else
        ui->checkBoxBlack->setChecked(false);*/
}

Settings::~Settings()
{
    delete ui;
}

void Settings::accept()
{
    int num_players=0;
    const QString tmp = "checkBox";
    QList<QCheckBox*> list = ui->groupBoxPlayers->findChildren<QCheckBox*>();
    //qDebug() << list.count();
    foreach(QCheckBox *box,list)
        if(box->isChecked())
            num_players++;
    //qDebug() << num_players;
    //qDebug() << settings.value("player/green");

    int radio_button = 0;
    if(ui->radioButton2->isChecked())
        radio_button = 2;
    else if(ui->radioButton3->isChecked())
        radio_button = 3;
    else if(ui->radioButton4->isChecked())
        radio_button = 4;
    else
        qDebug() << "Error, no radiobutton checked";

    if(num_players != radio_button)
    {
        if(num_players < radio_button)
            foreach(QCheckBox *box,list)
            {
                if(!box->isChecked())
                {
                    box->setChecked(true);
                    num_players++;
                }
                if(num_players == radio_button)
                    break;
            }
        else
            for(int i=list.count()-1;i>=0;i--)
            {
                if(list[i]->isChecked())
                {
                    list[i]->setChecked(false);
                    num_players--;
                }
                if(num_players == radio_button)
                    break;
            }
    }
    //qDebug() << num_players << radio_button;

    saveSettings(radio_button);

    //settings.setValue("value",1);
    //qDebug() << settings.value("value").toInt();
    //qDebug() << ui->checkBoxGreen->isChecked();
    //qDebug() << settings.value("player/black") << settings.value("player/green") <<  settings.value("player/red") <<  settings.value("player/white");
    //delete list;
    this->close();
}

void Settings::saveSettings(int radioButton)
{
    QSettings settings;
    settings.setValue("player/number",radioButton);
    settings.setValue("player/black",ui->checkBoxBlack->isChecked());
    settings.setValue("player/green",ui->checkBoxGreen->isChecked());
    settings.setValue("player/red",ui->checkBoxRed->isChecked());
    settings.setValue("player/white",ui->checkBoxWhite->isChecked());
    settings.sync();
}

void Settings::setCheckBox(QCheckBox *box, bool value)
{
    if(value)
        box->setChecked(true);
    else
        box->setChecked(false);
}
