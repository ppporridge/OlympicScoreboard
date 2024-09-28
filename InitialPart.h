#ifndef INITIALPART_H
#define INITIALPART_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QPair>

class InitialPart : public QObject
{
    Q_OBJECT

public:
    InitialPart();

    // 计算国家的分数
    void calculateScores();

    // 获取排序后的得分
    QVector<QPair<int, int>> sortByCountryID();        // 按国家编号排序
    QVector<QPair<int, int>> sortByTotalScores();      // 按团体总分排序
    QVector<QPair<int, int>> sortByMaleScores();       // 按男子项目总分排序
    QVector<QPair<int, int>> sortByFemaleScores();     // 按女子项目总分排序

    QMap<int, QVector<QPair<int, int>>> findCountryProjectAwards() const;

private:
    // 国家编号 -> 分数映射
    QMap<int, int> maleScores;     // 男子项目得分
    QMap<int, int> femaleScores;   // 女子项目得分
    QMap<int, int> totalScores;    // 总分

    // 将 QMap 按值排序并返回 QVector<QPair<int, int>>
    QVector<QPair<int, int>> sortMapByValue(const QMap<int, int>& map) const;
};

#endif // INITIALPART_H
