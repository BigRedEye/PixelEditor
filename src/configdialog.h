#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    
    int getTheme() const;
    
private slots:
    void on_LightThemeButton_clicked();
    
    void on_DarkThemeButton_clicked();
    
private:
    Ui::ConfigDialog *ui;
    int theme;
};

#endif // CONFIGDIALOG_H
