/**********************************************
 * @file AdjacencyList.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.26
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H
#include <vector>

class AdjacencyList {
    using vector_i = std::vector<int>;
    using pair_ii = std::pair<int, int>;

public:
    enum GenerationActionType {
        BuildWall,
        BreakWall
    };

    explicit AdjacencyList(int row = 0, int column = 0);

    void connectAllSurround();

    void connect(int i, int j);

    void disconnect(int i, int j);

    int row() const;

    int column() const;

    int valid() const;

    int nodeCount() const;

    bool validIndex(int i) const;

    const vector_i &neighbor(int i) const;

    vector_i &neighbor(int i);

    const vector_i &surround(int i) const;

    vector_i &surround(int i);

    vector_i neighborStat() const;

    GenerationActionType generationActionType() const;

    int generationActionCount() const;

    std::vector<pair_ii> generationActions() const;

private:
    void fillSurround();

    bool isLeftTop(int index) const;

    bool isRightTop(int index) const;

    bool isLeftBottom(int index) const;

    bool isRightBottom(int index) const;

    bool isLeftEdge(int index) const;

    bool isRightEdge(int index) const;

    bool isTopEdge(int index) const;

    bool isBottomEdge(int index) const;

    int mazeRow;
    int mazeColumn;

    std::vector<vector_i> indexToNeighbor;
    std::vector<vector_i> indexToSurround;

    GenerationActionType actionType;
    std::vector<pair_ii> actions;
};

#endif
