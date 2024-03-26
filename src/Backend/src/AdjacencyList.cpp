/**********************************************
 * @file AdjacencyList.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.26
 * @license GNU General Public License (GPL)
 **********************************************/

#include "AdjacencyList.h"

#include <Definition.h>
#include <cassert>

using vector_i = std::vector<int>;
using pair_ii = std::pair<int, int>;

pair_ii makeOrderedPair(int a, int b) {
    std::pair result(a, b);
    if (a > b) {
        result.first = b;
        result.second = a;
    }
    return result;
}

AdjacencyList::AdjacencyList(const int row, const int column) :
    mazeRow(row), mazeColumn(column)
  , indexToNeighbor(sc_int(mazeRow * mazeColumn))
  , indexToSurround(sc_int(mazeRow * mazeColumn))
  , actionType(BreakWall) {
    fillSurround();
}

void AdjacencyList::connectAllSurround() {
    assert(valid());

    actionType = BuildWall;
    indexToNeighbor = indexToSurround;
}

void AdjacencyList::fillSurround() {
    const int size = nodeCount();
    for (int i = 0; i < size; i++) {
        if (!(isLeftTop(i) || isRightTop(i) || isTopEdge(i))) {
            indexToSurround[sc_int(i)].push_back(i - mazeColumn);
        }
        if (!(isLeftBottom(i) || isRightBottom(i) || isBottomEdge(i))) {
            indexToSurround[sc_int(i)].push_back(i + mazeColumn);
        }
        if (!(isLeftTop(i) || isLeftBottom(i) || isLeftEdge(i))) {
            indexToSurround[sc_int(i)].push_back(i - 1);
        }
        if (!(isRightTop(i) || isRightBottom(i) || isRightEdge(i))) {
            indexToSurround[sc_int(i)].push_back(i + 1);
        }
    }
}

void AdjacencyList::connect(const int i, const int j) {
    assert(valid() && validIndex(i) && validIndex(j));
    indexToNeighbor[sc_int(i)].push_back(j);
    indexToNeighbor[sc_int(j)].push_back(i);
    actions.push_back(makeOrderedPair(i, j));
}

void AdjacencyList::disconnect(const int i, const int j) {
    assert(valid() && validIndex(i) && validIndex(j));
    indexToNeighbor[sc_int(i)].erase(std::ranges::find(neighbor(i), j));
    indexToNeighbor[sc_int(j)].erase(std::ranges::find(neighbor(j), i));
    actions.push_back(makeOrderedPair(i, j));
}

int AdjacencyList::row() const { return mazeRow; }

bool AdjacencyList::isLeftTop(const int index) const {
    assert(validIndex(index));
    return index == 0;
}

bool AdjacencyList::isRightTop(const int index) const {
    assert(validIndex(index));
    return index == (mazeColumn - 1);
}

bool AdjacencyList::isLeftBottom(const int index) const {
    assert(validIndex(index));
    return index == (mazeRow - 1) * mazeColumn;
}

bool AdjacencyList::isRightBottom(const int index) const {
    assert(validIndex(index));
    return index == (mazeRow * mazeColumn - 1);
}

bool AdjacencyList::isLeftEdge(const int index) const {
    assert(validIndex(index));
    if (isLeftTop(index) || isLeftBottom(index)) {
        return false;
    }
    return index % mazeColumn == 0;
}

bool AdjacencyList::isRightEdge(const int index) const {
    assert(validIndex(index));
    if (isRightTop(index) || isRightBottom(index)) {
        return false;
    }
    return index % mazeColumn == (mazeColumn - 1);
}

bool AdjacencyList::isTopEdge(const int index) const {
    assert(validIndex(index));
    if (isLeftTop(index) || isRightTop(index)) {
        return false;
    }
    return index > 0 && index < (mazeColumn - 1);
}

bool AdjacencyList::isBottomEdge(const int index) const {
    assert(validIndex(index));
    if (isLeftBottom(index) || isRightBottom(index)) {
        return false;
    }
    return index > ((mazeRow - 1) * mazeColumn) && index < (mazeRow * mazeColumn - 1);
}

vector_i AdjacencyList::neighborStat() const {
    assert(valid());
    vector_i result(4, 0);
    for (int i = 0; i < nodeCount(); i++) {
        assert(!neighbor(i).empty() && neighbor(i).size() <= 4);
        result[neighbor(i).size() - 1]++;
    }
    return result;
}

AdjacencyList::GenerationActionType AdjacencyList::generationActionType() const {
    return actionType;
}

int AdjacencyList::generationActionCount() const {
    return static_cast<int>(actions.size());
}

std::vector<pair_ii> AdjacencyList::generationActions() const {
    return actions;
}

int AdjacencyList::column() const {
    return mazeColumn;
}

int AdjacencyList::valid() const {
    return mazeRow > 1 && mazeColumn > 1;
}

int AdjacencyList::nodeCount() const {
    return mazeRow * mazeColumn;
}

bool AdjacencyList::validIndex(const int i) const {
    return i >= 0 && i < nodeCount();
}

const vector_i &AdjacencyList::neighbor(const int i) const {
    assert(validIndex(i));
    return indexToNeighbor[sc_int(i)];
}

vector_i &AdjacencyList::neighbor(const int i) {
    assert(validIndex(i));
    return indexToNeighbor[sc_int(i)];
}

const vector_i &AdjacencyList::surround(const int i) const {
    assert(validIndex(i));
    return indexToSurround[sc_int(i)];
}

vector_i &AdjacencyList::surround(const int i) {
    assert(validIndex(i));
    return indexToSurround[sc_int(i)];
}
