#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include "Global.h"
#include "MainUI.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 弹出输入框，获取国家数量、男子项目和女子项目数量
    bool ok;
    countryCount = QInputDialog::getInt(nullptr, "输入国家数量",
                                            "请输入国家数量:", 0, 1, 100, 1, &ok);
    if (!ok) {
        QMessageBox::warning(nullptr, "警告", "您必须输入国家数量");
        return -1;  // 用户取消输入，终止程序
    }

    maleProjects = QInputDialog::getInt(nullptr, "输入男子项目数量",
                                            "请输入男子项目数量:", 0, 1, 100, 1, &ok);
    if (!ok) {
        QMessageBox::warning(nullptr, "警告", "您必须输入男子项目数量");
        return -1;
    }

    femaleProjects = QInputDialog::getInt(nullptr, "输入女子项目数量",
                                              "请输入女子项目数量:", 0, 1, 100, 1, &ok);
    if (!ok) {
        QMessageBox::warning(nullptr, "警告", "您必须输入女子项目数量");
        return -1;
    }

    //

    // 创建并显示主窗口
    MainUI w;
    w.show();

    return a.exec();
}
