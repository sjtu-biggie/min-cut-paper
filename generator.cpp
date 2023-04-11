#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <tuple>
#include <algorithm>

std::vector<std::tuple<int, int, double>> generate_edges(int N, double m1, double m2) {
    std::vector<std::tuple<int, int, double>> edges;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> weight_distr(m1, m2);

    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            double weight = weight_distr(gen);
            edges.emplace_back(i, j, weight);
        }
    }
    return edges;
}

std::vector<std::tuple<int, int, double>> generate_connected_graph(int N, double m1, double m2) {
    std::vector<std::tuple<int, int, double>> edges = generate_edges(N, m1, m2);
    std::vector<int> nodes(N);
    for (int i = 0; i < N; ++i) nodes[i] = i;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(nodes.begin(), nodes.end(), gen);
    std::set<int> connected;
    connected.insert(nodes[0]);

    std::vector<std::tuple<int, int, double>> connected_edges;

    for (int i = 1; i < N; ++i) {
        int v1 = nodes[i];
        std::uniform_int_distribution<> index_distr(0, i - 1);
        int v2 = nodes[index_distr(gen)];
        double weight = std::get<2>(edges[v1 * (v1 - 1) / 2 + v2]);
        connected_edges.emplace_back(v1, v2, weight);
        connected.insert(v1);
    }

    // Adding random extra edges
    std::uniform_int_distribution<> extra_edges_distr(1, N * (N - 1) / 2 - N);
    int extra_edges_count = extra_edges_distr(gen);

    for (int i = 0; i < extra_edges_count; ++i) {
        std::uniform_int_distribution<> node_distr(0, N - 1);
        int v1 = node_distr(gen);
        int v2 = node_distr(gen);

        while (v1 == v2 || std::find(connected_edges.begin(), connected_edges.end(), std::make_tuple(v1, v2, std::get<2>(edges[v1 * (v1 - 1) / 2 + v2]))) != connected_edges.end() || std::find(connected_edges.begin(), connected_edges.end(), std::make_tuple(v2, v1, std::get<2>(edges[v1 * (v1 - 1) / 2 + v2]))) != connected_edges.end()) {
            v1 = node_distr(gen);
            v2 = node_distr(gen);
        }

        double weight = std::get<2>(edges[v1 * (v1 - 1) / 2 + v2]);
        connected_edges.emplace_back(v1, v2, weight);
    }

    return connected_edges;
}

int main(int argc, char** argv) {
    if(argc < 3){
        std::cout << "Usage: " << argv[0] << " N m1 m2" << std::endl;
        exit(1);
    }
    int N = std::stoi(argv[1]);
    double m1 = std::stod(argv[2]);
    double m2 = std::stod(argv[3]);
    std::vector<std::tuple<int, int, double>> connected_edges = generate_connected_graph(N, m1, m2);
    std::cout << N << " " << connected_edges.size() << std::endl;
    for (const auto& edge : connected_edges) {
        std::cout << std::get<0>(edge) << " " << std::get<1>(edge) << " " << std::get<2>(edge) << std::endl;
    }
    return 0;
}