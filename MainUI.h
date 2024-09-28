#ifndef MAINUI_H
#define MAINUI_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QLineEdit>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QMap>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainUI;
}
QT_END_NAMESPACE

class MainUI : public QWidget
{
    Q_OBJECT

public:
    MainUI(QWidget *parent = nullptr);
    ~MainUI();

private slots:
    void onSortCriteriaChanged(int index);
    void onCountryChanged(int index);
    void onProjectChanged(int index);


private:
    Ui::MainUI *ui;
};
#endif // MAINUI_H
