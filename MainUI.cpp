#include "MainUI.h"
#include "ui_MainUI.h"
#include "Global.h"
#include "InitialPart.h"

MainUI::MainUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainUI)
{
    ui->setupUi(this);

    /******************第一页设置***********************/
    //左侧总体性介绍文字
    QString infoText = QString("当前共有 %1 个国家，%2 个男子项目，%3 个女子项目")
                           .arg(countryCount)
                           .arg(maleProjects)
                           .arg(femaleProjects);
    ui->infoLabel->setText(infoText);


    // 默认选择前三名
    ui->topThreeRadioButton->setChecked(true);

    // 连接提交按钮信号槽
    connect(ui->submitButton, &QPushButton::clicked, this, [=]() {
        // 获取输入的数据
        QString projectId = ui->projectLineEdit->text();
        QString countryIds = ui->countryLineEdit->text();
        bool isTopThree = ui->topThreeRadioButton->isChecked();
        bool isTopFive = ui->topFiveRadioButton->isChecked();

        // 验证项目编号和国家编号输入
        if (projectId.isEmpty() || countryIds.isEmpty())
        {
            QMessageBox::warning(this, "输入错误", "请填写项目编号和国家编号");
            ui->projectLineEdit->clear();
            ui->countryLineEdit->clear();
            initialCorrect=false;
            return;
        }
        else
        {
            //验证项目编号是否正确
            if(projectId.toInt()<=0 || projectId.toInt() > maleProjects + femaleProjects)
            {
                QMessageBox::warning(this, "输入错误", "请重新填写项目编号");
                ui->projectLineEdit->clear();
                ui->countryLineEdit->clear();
                initialCorrect=false;
                return;
            }
            else
            {
                // 解析国家编号并验证输入是否足够
                QStringList countryList = countryIds.split(" ", Qt::SkipEmptyParts);

                if ((isTopThree && countryList.size() != 3) || (isTopFive && countryList.size() != 5))
                {
                    QMessageBox::warning(this, "输入错误", isTopThree ? "请输入前三名国家编号" : "请输入前五名国家编号");
                    initialCorrect=false;
                    return;
                }
                else
                {
                    // 验证每个国家编号是否在有效范围内
                    for (const QString &countryIdStr : countryList) {
                        bool ok;
                        int countryId = countryIdStr.toInt(&ok);

                        // 验证输入是否为有效的整数，并且在 1 到 countryCount 范围内
                        if (!ok || countryId < 1 || countryId > countryCount) {
                            QMessageBox::warning(this, "输入错误", QString("国家编号 %1 无效，编号应在 1 到 %2 之间").arg(countryIdStr).arg(countryCount));
                            initialCorrect=false;
                            return;
                        }
                    }

                    // 保存数据的逻辑
                    initialCorrect=true;
                    int project = projectId.toInt();
                    QVector<int> countryVector;
                    for (const QString &countryId : countryList) {
                        countryVector.append(countryId.toInt());
                    }
                    projectResults[project] = countryVector;
                    // 显示成功消息
                    QMessageBox::information(this, "提交成功", "成绩已成功保存！");
                }
            }
        }
    });


    // 连接查看按钮信号槽
    connect(ui->viewResultsButton, &QPushButton::clicked, this, [=]() {
        if(initialCorrect == true )
        {
            if( projectResults.size() < maleProjects + femaleProjects)
            {
                QMessageBox::warning(this, "输入有误", "提交的数据不足，请继续提交");
                return;
            }
            else
            {
                QString resultText;
                for (auto it = projectResults.constBegin(); it != projectResults.constEnd(); ++it) {
                    resultText += QString("项目编号: %1\n").arg(it.key());
                    resultText += "国家编号: ";
                    for (int countryId : it.value()) {
                        resultText += QString("%1 ").arg(countryId);
                    }
                    resultText += "\n\n";
                }
                QMessageBox::information(this, "存储的数据", resultText);
            }
        }
        else
        {
            QMessageBox::warning(this, "输入有误", "提交过程中出现错误，请重新提交");
            return;
        }
    });



    /******************第二页设置***********************/
    ui->ScoreBoard->setColumnWidth(0, 100);
    ui->ScoreBoard->setColumnWidth(1, 300);

    connect(ui->SelectBy, SIGNAL(currentIndexChanged(int)), this, SLOT(onSortCriteriaChanged(int)));

    /******************第三页设置***********************/
    // 初始化国家 QComboBox
    for (int i = 1; i <= countryCount; ++i) {
        ui->CountryBox->addItem(QString::number(i));  // 使用国家编号作为选项
    }

    // 初始化项目 QComboBox
    for (int i = 1; i <= maleProjects + femaleProjects; ++i) {
        ui->ProjectBox->addItem(QString::number(i));  // 使用项目编号作为选项
    }

    connect(ui->CountryBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCountryChanged(int)));
    connect(ui->ProjectBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onProjectChanged(int)));




}

void MainUI::onSortCriteriaChanged(int index)
{
    qDebug() << "ComboBox current index:" << index;
    qDebug() << "ComboBox current index:" << index;
    ui->ScoreBoard->clearContents();  // 清除现有内容

    QVector<QPair<int, int>> sortedData;
    InitialPart initialPart;
    initialPart.calculateScores();

    /*ui->ScoreBoard->setRowCount(3);  // 示例行数
    ui->ScoreBoard->setColumnCount(2);  // 示例列数

    QTableWidgetItem *item1 = new QTableWidgetItem("1");
    QTableWidgetItem *item2 = new QTableWidgetItem("Example Country 1");
    ui->ScoreBoard->setItem(0, 0, item1);
    ui->ScoreBoard->setItem(0, 1, item2);*/

    switch (index) {
    case 0: // 国家编号排序
        sortedData = initialPart.sortByCountryID(); // 这个函数返回的是国家编号和总分的排序
        break;
    case 1: // 国家团体总分排序
        sortedData = initialPart.sortByTotalScores(); // 假设你已经实现了排序逻辑
        break;
    case 2: // 国家男子项目总分排序
        sortedData = initialPart.sortByMaleScores();
        break;
    case 3: // 国家女子项目总分排序
        sortedData = initialPart.sortByFemaleScores();
        break;
    default:
        return;
    }
    for (const auto &pair : sortedData) {
        qDebug() << "Country ID:" << pair.first << "Score:" << pair.second;
    }
    ui->ScoreBoard->setRowCount(sortedData.size());
    ui->ScoreBoard->show();


    // 更新 TableWidget 中的内容
    for (int i = 0; i < sortedData.size(); ++i) {
        QTableWidgetItem *rankItem = new QTableWidgetItem(QString::number(i + 1));  // 排序名次
        QTableWidgetItem *countryScoreItem = new QTableWidgetItem(QString("国家编号: %1，总分: %2")
                                                                      .arg(sortedData[i].first)
                                                                      .arg(sortedData[i].second));

        ui->ScoreBoard->setItem(i, 0, rankItem);
        ui->ScoreBoard->setItem(i, 1, countryScoreItem);
    }
}

void MainUI::onCountryChanged(int index)
{
    // 获取当前选择的国家编号
    int selectedCountry = index + 1;
    ui->CountryContent->clear();

    // 设置初始的文本
    QString initialText = QString("编号为 %1 的国家参与的项目及名次为:\n").arg(selectedCountry);
    ui->CountryContent->setPlainText(initialText);

    // 查询获得结果
    InitialPart initialPart;
    QMap<int, QVector<QPair<int, int>>> countryAwardsMap = initialPart.findCountryProjectAwards();

    // 获取该国家的获奖记录
    if (countryAwardsMap.contains(selectedCountry)) {
        QVector<QPair<int, int>> awards = countryAwardsMap[selectedCountry];

        QString resultText;
        for (const auto &award : awards) {
            resultText += QString("项目 %1 名次为 %2;\n").arg(award.first).arg(award.second);
        }
        ui->CountryContent->setPlainText(ui->CountryContent->toPlainText() + resultText);
    } else {
        ui->CountryContent->setPlainText(ui->CountryContent->toPlainText() + "该国家没有参与任何项目。\n");
    }
}



void MainUI::onProjectChanged(int index)
{
    // 获取当前选择的项目编号
    int selectedProject = index + 1;
    ui->ProjectContent->clear();  // 清除之前的内容

    // 设置初始的文本
    QString initialText = QString("编号为 %1 的项目涉及的国家及名次为:\n").arg(selectedProject);
    ui->ProjectContent->setPlainText(initialText);

    // 查询获得结果
    if (projectResults.contains(selectedProject)) {
        QList<int> countryList = projectResults[selectedProject];  // 获取对应国家列表

        QString resultText = "按名次顺序的国家编号: ";
        for (int countryId : countryList) {
            resultText += QString("%1 ").arg(countryId);
        }
        resultText += "\n";

        // 将结果显示在 ProjectContent 中
        ui->ProjectContent->setPlainText(ui->ProjectContent->toPlainText() + resultText);
    } else {
        ui->ProjectContent->setPlainText(ui->ProjectContent->toPlainText() + "该项目没有任何国家参与。\n");
    }
}


MainUI::~MainUI()
{
    delete ui;
}
