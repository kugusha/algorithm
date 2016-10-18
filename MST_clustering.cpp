#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;


struct Point2D {
    double x, y;
};

//расстояние между двумя точками
double EuclidianDistance(const Point2D& first, const Point2D& second) {
    return std::sqrt((first.x - second.x) * (first.x - second.x) +
                     (first.y - second.y) * (first.y - second.y));
}

//класс непересекающихся систем множеств 
class DisjointSetUnion {
private:
    vector<int> parent_;
    vector<int> ranks_;

public:
    explicit DisjointSetUnion(size_t size)
            : parent_()
            , ranks_(size, 0)
    {
        parent_.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            parent_.push_back(i);
        }
    }

    int find(int node) {
        if (parent_[node] != node) {
            parent_[node] = find(parent_[node]);
        }
        return parent_[node];
    }

    void union_sets(int first, int second) {
        int first_root = find(first);
        int second_root = find(second);
        if (first_root == second_root) {
            return;
        }

        if (ranks_[first_root] < ranks_[second_root]) {
            parent_[first_root] = second_root;
        } else if (ranks_[first_root] > ranks_[second_root]) {
            parent_[second_root] = first_root;
        } else {
            parent_[second_root] = first_root;
            ++ranks_[first_root];
        }
    }
};


struct Edge {
    size_t from;
    size_t to;
    double weight;
};


// Map arbitrary labels to 0, ..., (n-1) labels
//
vector<size_t> RenumerateLabels(const vector<size_t>& rawLabels) {
    vector<int> rawToNew(rawLabels.size(), -1);
    size_t indexesUsed = 0;
    vector<size_t> newLabels(rawLabels.size());
    for (size_t i = 0; i < rawLabels.size(); ++i) {
        size_t oldLabel = rawLabels[i];
        if (rawToNew[oldLabel] == -1) {
            rawToNew[oldLabel] = indexesUsed;
            ++indexesUsed;
        }
        newLabels[i] = rawToNew[oldLabel];
    }
    return newLabels;
}

//сортирует по возрастанию(рот самых близких и тд)
vector<size_t> ClusterGraph(vector<Edge> edges,  
                            size_t vertexCount,
                            size_t clusterCount)  {
    std::sort(edges.begin(),
              edges.end(),
              [](const Edge& first, const Edge& second) {
                  return first.weight < second.weight;
              });
    DisjointSetUnion components(vertexCount);
    size_t currentClusters = vertexCount;
    for (const Edge& edge : edges) {
        if (components.find(edge.from) != components.find(edge.to)) {
            components.union_sets(edge.from, edge.to);
            --currentClusters;
            if (currentClusters == clusterCount) {
                break;
            }
        }
    }

    vector<size_t> rawLabels(vertexCount);
    for (size_t vert = 0; vert < vertexCount; ++vert) {
        rawLabels[vert] = components.find(vert);
    }
    return RenumerateLabels(rawLabels);
}

//использует EuclidianDistance для подсчета расстояния от каждой точки до каждой и записывает в вектор
template <typename T, typename Dist>
vector<Edge> PairwiseDistances(vector<T> objects, Dist distance) {
    vector<Edge> edges;
    for (size_t i = 0; i < objects.size(); ++i) {
        for (size_t j = i + 1; j < objects.size(); ++j) {
            edges.push_back({i, j, distance(objects[i], objects[j])});
        }
    }
    return edges;
}

//вызывает PairwiseDistances а потом ClusterGraph
template <typename T, typename Dist>
vector<size_t> ClusterMST(const vector<T>& objects, Dist distance, size_t clusterCount) {
    vector<Edge> edges = PairwiseDistances(objects, distance);
    return ClusterGraph(edges, objects.size(), clusterCount);
}



vector<Point2D> Random2DClusters(size_t clusterCount, size_t pointsCount) {
    auto generator = std::default_random_engine();
    auto variance = std::uniform_real_distribution<double>(0.2, 2);
    auto mean = std::uniform_real_distribution<double>(-30, 30);
    auto deviation = std::normal_distribution<double>();
    auto clusterNumber = std::uniform_int_distribution<size_t>(0, clusterCount-1);


    vector<double> xVariances(clusterCount, 0);
    vector<double> yVariances(clusterCount, 0);
    vector<double> xMeans(clusterCount, 0);
    vector<double> yMeans(clusterCount, 0);

    for (size_t i = 0; i < clusterCount; ++i) {
        xVariances[i] = variance(generator);
        yVariances[i] = variance(generator);
        xMeans[i] = mean(generator);
        yMeans[i] = mean(generator);
    }

    vector<Point2D> results;

    for (size_t i = 0; i < pointsCount; ++i) {
        size_t c = clusterNumber(generator);
        double x = xMeans[c] + deviation(generator) * xVariances[c];
        double y = yMeans[c] + deviation(generator) * yVariances[c];
        results.push_back({x, y});
    }

    return results;
}


void GNUPlotClusters2D(const vector<Point2D>& points,
    const vector<size_t>& labels,
    size_t clustersCount,
    const string& outFolder)
{
    std::ofstream scriptOut(outFolder + "/script.txt");
    scriptOut << "plot ";
 
    for (size_t cluster = 0; cluster < clustersCount; ++cluster) {
        string filename = std::to_string(cluster) + ".dat";
        std::ofstream fileOut(outFolder + "/" + filename);
        scriptOut << "\"" << filename << "\"" << " with points, ";
 
        for (size_t i = 0; i < points.size(); ++i) {
            if (labels[i] == cluster) {
                fileOut << points[i].x << "\t" << points[i].y << "\n";
            }
        }
    }
}


int main() {
    auto points = Random2DClusters(5, 5000);

    vector<size_t> labels(points.size(), 0);

    size_t clustersCount = 5;

    labels = ClusterMST(points, EuclidianDistance, clustersCount);
    GNUPlotClusters2D(points, labels, clustersCount, "for_data");

    return 0;
}

