#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>

// при написании кода опиралась на https://habrahabr.ru/post/209610/

int N;
int curr_path_cost(const std::vector<int>& path,
                   const std::vector<std::vector<int>>& graph) {
    int sum = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        sum = sum + graph[path[i]][path[i + 1]];
    }
    return sum;
}

std::vector<int> Metropolis(const std::vector<std::vector<int>>& graph) {
    std::vector<int> path;
    for (int i = 0; i < N; i++) {
        path.push_back(i);
    }
    auto from = 0, to = 0;
    int count = 1;
    int contrast;
    double probability;
    double temperature = 25;
    std::vector<int> tmp_path;
    while (count < 100000) {
        count++;
        tmp_path = path;
        from = rand() % N;
        to = from + (rand() % (N - from));
        int tmp;
        tmp = tmp_path[from];
        tmp_path[from] = tmp_path[to];
        tmp_path[to] = tmp;
        int a = curr_path_cost(tmp_path, graph);
        int b = curr_path_cost(path, graph);
        contrast = a - b;
        if (contrast <= 0) {
            path = tmp_path;
        } else {
            probability = exp((-(double)1 * contrast) / (temperature * 1.38));
            if ((double(rand()) / 32767) <= probability) {
                path = tmp_path;
            }
        }
    }
    return path;
}

std::vector<int> Simulated_annealing(const std::vector<std::vector<int>>& graph) {
    std::vector<int> path;
    for (int i = 0; i < N; i++) {
        path.push_back(i);
    }
    auto from = 0, to = 0;
    int count = 1;
    int contrast;
    double probability;
    double init_temp = 100000;
    double min_temp = 1;
    double temperature = init_temp;
    std::vector<int> tmp_path;
    while (temperature > min_temp) {
        count++;
        from = rand() % N;
        to = from + (rand() % (N - from));
        tmp_path = path;
        int tmp;
        tmp = tmp_path[from];
        tmp_path[from] = tmp_path[to];
        tmp_path[to] = tmp;
        int a = curr_path_cost(tmp_path, graph);
        int b = curr_path_cost(path, graph);
        contrast = a - b;
        if (contrast <= 0) {
            path = tmp_path;
        } else {
            probability = exp((-(double)1 * contrast) / (temperature * 1.38));
            if ((double(rand()) / 32767) <= probability) {
                path = tmp_path;
            }
        }
        temperature = init_temp / count;
    }
    return path;
}

std::vector<int> Gradient_descent(const std::vector<std::vector<int>>& graph) {
    std::vector<int> path;
    for (int i = 0; i < N; i++) {
        path.push_back(i);
    }
    auto from = 0, to = 0;
    int count = 1;
    int contrast;
    std::vector<int> tmp_path;
    while (count < 100000) {
        count++;
        from = rand() % N;
        to = from + (rand() % (N - from));
        tmp_path = path;
        int tmp;
        tmp = tmp_path[from];
        tmp_path[from] = tmp_path[to];
        tmp_path[to] = tmp;
        int a = curr_path_cost(tmp_path, graph);
        int b = curr_path_cost(path, graph);
        contrast = a - b;
        if (contrast <= 0) {
            path = tmp_path;
        }
    }
    return path;
}

std::vector<int> find_answer(std::vector<int>& path, std::vector<std::vector<int>>& graph) {
    std::vector<int> answer, tmp_answer;
    for (int i = 0; i < path.size() - 1; i++ ) {
        if (i != path.size() - 2){
            if (graph[path[i]][path[i + 1]] == 0) {
                tmp_answer.push_back(path[i]);
            } else {
                tmp_answer.push_back(path[i]);
                if (tmp_answer.size() > answer.size()) {
                    answer = tmp_answer;
                }
                tmp_answer.clear();
            }
        } else if (i == path.size() - 2) {
            if (graph[path[i]][path[i + 1]] == 0) {
                tmp_answer.push_back(path[i]);
                tmp_answer.push_back(path[i + 1]);
            } else {
                tmp_answer.push_back(path[i]);
            }
            if ( answer.size() < tmp_answer.size() ) {
                answer = tmp_answer;
            }
        }
    }
    return answer;
}


int main() {
    int M;
    std::cin >> N;
    std::cin >> M;
    std::vector<std::vector<int>> graph(N, std::vector<int>(N, 10));
    for (auto i = 0; i < M; i++) {
        int from, to;
        std::cin >> from;
        std::cin >> to;
        graph[from][to] = 0;
        graph[to][from] = 0;
    }

    std::vector<int> path;
    path = Simulated_annealing(graph);
    //path = Metropolis(graph);
    //path = Gradient_descent(graph);

    std::vector<int> answer;
    answer = find_answer(path, graph);
    for (auto x : answer) {
        std::cout << x << " ";
    }
}
