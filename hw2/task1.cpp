#include <iostream>
#include <vector>

struct edges {
    int from;
    int to;
};

int main() {
    int N, M;
    std::cin >> N;
    std::cin >> M;
    std::vector<edges> edge(M);
    std::vector<bool> used(N, false);
    std::vector<edges> answer;
    for (int i = 0; i < M; i++ ) {
        std::cin >> edge[i].from;
        std::cin >> edge[i].to;
        edge[i].from--;
        edge[i].to--;
        if (used[edge[i].from] == false && used[edge[i].to] == false) {
            answer.push_back(edge[i]);
            used[edge[i].from] = true;
            used[edge[i].to] = true;
        }
    }
    for (auto edge: answer){
        std::cout << edge.from + 1 << " " << edge.to + 1 << std::endl;
    }
    return 0;
}
