#include "configdialog.h"
#include "ui_configdialog.h"
#include <QApplication>

#include <QSettings>

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    
    ui->DarkThemeButton->setChecked(true);
    ui->LightThemeButton->setChecked(true);
    QSettings appSettings;
    theme = appSettings.value("APP_THEME").toInt();
    if (theme)
        ui->LightThemeButton->setChecked(true);
    else
        ui->DarkThemeButton->setChecked(true);
    ui->buttonBox->setAutoFillBackground(true);
    for (QAbstractButton *button : ui->buttonBox->buttons())
    {
        QPalette pal = button->palette();
        pal.setColor(QPalette::Button, this->palette().color(QPalette::Button));
        button->setAutoFillBackground(true);
        button->setPalette(QApplication::palette());
        button->update();
    }
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

int ConfigDialog::getTheme() const
{
    return theme;
}

void ConfigDialog::on_LightThemeButton_clicked()
{
    theme = 1;
}

void ConfigDialog::on_DarkThemeButton_clicked()
{
    theme = 0;
}
