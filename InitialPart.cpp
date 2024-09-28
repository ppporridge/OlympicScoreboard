#include "InitialPart.h"
#include "Global.h"
#include <algorithm>

InitialPart::InitialPart()
{
    // 初始化如果需要，可以在这里进行
}

void InitialPart::calculateScores()
{
    for (int countryId = 1; countryId <= countryCount; ++countryId) {
        maleScores[countryId] = 0;
        femaleScores[countryId] = 0;
        totalScores[countryId] = 0;
    }

    for (auto it = projectResults.constBegin(); it != projectResults.constEnd(); ++it) {
        int projectId = it.key();
        QVector<int> countryIds = it.value();

        QVector<int> pointsof3 = {5, 3, 2};
        QVector<int> pointsof5 = {7, 5, 3, 2, 1};

        for (int i = 0; i < countryIds.size(); ++i) {
            int countryId = countryIds[i];
            int score;

            if(countryIds.size() == 3)
                score = pointsof3[i];
            else
                score = pointsof5[i];

            // 根据项目是男子或女子项目分别累加分数
            if (projectId <= maleProjects) {
                maleScores[countryId] += score;
            } else {
                femaleScores[countryId] += score;
            }

            // 累加到总分
            totalScores[countryId] += score;
        }
    }
}

QVector<QPair<int, int>> InitialPart::sortByCountryID()
{
    QVector<QPair<int, int>> sortedData;

    // 按国家编号排序
    for (auto it = totalScores.constBegin(); it != totalScores.constEnd(); ++it) {
        sortedData.append(qMakePair(it.key(), it.value()));
    }

    std::sort(sortedData.begin(), sortedData.end(), [](const QPair<int, int>& a, const QPair<int, int>& b) {
        return a.first < b.first;
    });

    return sortedData;
}

QVector<QPair<int, int>> InitialPart::sortByTotalScores()
{
    return sortMapByValue(totalScores);
}

QVector<QPair<int, int>> InitialPart::sortByMaleScores()
{
    return sortMapByValue(maleScores);
}

QVector<QPair<int, int>> InitialPart::sortByFemaleScores()
{
    return sortMapByValue(femaleScores);
}

QVector<QPair<int, int>> InitialPart::sortMapByValue(const QMap<int, int>& map) const
{
    QVector<QPair<int, int>> sortedData;

    // 将 QMap 转换为 QVector<QPair<int, int>> 并排序
    for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
        sortedData.append(qMakePair(it.key(), it.value()));
    }

    // 按分数排序，降序排列
    std::sort(sortedData.begin(), sortedData.end(), [](const QPair<int, int>& a, const QPair<int, int>& b) {
        return a.second > b.second;
    });

    return sortedData;
}

QMap<int, QVector<QPair<int, int>>> InitialPart::findCountryProjectAwards() const
{
    QMap<int, QVector<QPair<int, int>>> countryAwardsMap;

    for (auto it = projectResults.constBegin(); it != projectResults.constEnd(); ++it) {
        int projectId = it.key();
        const QList<int>& countryRankings = it.value(); // 修改类型为 QList<int>

        // 假设名次是按列表顺序排列的
        for (int i = 0; i < countryRankings.size(); ++i) {
            int countryId = countryRankings[i];
            int rank = i + 1;  // 名次为索引 + 1

            // 在国家编号对应的 QVector 中加入项目和对应的名次
            countryAwardsMap[countryId].append(qMakePair(projectId, rank));
        }
    }

    return countryAwardsMap;
}



