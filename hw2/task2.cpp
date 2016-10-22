#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>

int N, M;
std::set<std::vector<short int>> lines;
std::map<std::vector<short int>, bool> used_l;

std::vector<short int> generator(const std::vector<short int> &line, int &first, int &second) {
    std::vector<short int> new_line = line;
    new_line[first] = !new_line[first];
    if (first != second) {
        new_line[second] = !new_line[second];
    }
    return new_line;
}

void BFS(const std::vector<short int>& line) {
    used_l[line] = true;
    for (int i = 0; i < M; i++) {
        for (int j = i; j < M; j++) {
            auto iter = lines.find(generator(line, i, j));
            if (iter != lines.end()) {
                if (used_l[*iter] == false) {
                    BFS(*iter);
                }
            }
        }
    }
}

int main() {
    std::cin >> N;
    std::cin >> M;
    for (int i = 0; i < N; i++) {
        std::string l;
        std::cin >> l;
        std::vector<short int> buff;
        for (auto x : l) {
            buff.push_back(x - '0');
        }
        lines.insert(buff);
        used_l[buff] = false;
    }

    int clust = 0;
    for (auto line : lines) {
        if (used_l[line] == false) {
            BFS(line);
            clust++;

        }
    }
    std::cout << clust;

    return 0;
}
