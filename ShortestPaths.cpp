/*
Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то
вершинами. Найдите количество различных кратчайших путей между заданными вершинами. Требования: сложность
O(V+E), граф реализован в виде класса. Формат ввода. v: кол-во вершин (макс. 50000), n: кол-во ребер
(макс. 200000), n пар реберных вершин, пара вершин u, w для запроса. Формат вывода. Количество кратчайших
путей от u к w.

in    out
4     2
5
0 1
0 2
1 2
1 3
2 3
0 3
*/

#include <iostream>
#include <queue>
#include <stdexcept>
#include <vector>

//const int INT_MAX = 2147483647;

class ListGraph {
    public:
        ListGraph(int vertices) : adj(vertices) {};

        ~ListGraph() = default;

        void AddEdge(int from, int to) {
            if (from < 0 || from >= (int)adj.size() || to < 0 || to >= (int)adj.size())
                throw std::out_of_range("vertex index out of range");

            adj[from].push_back(to);
        }

        std::vector<int> GetNextVertices(int vertex) const {
            if (vertex < 0 || vertex >= (int)adj.size())
                throw std::out_of_range("vertex index out of range");

            return adj[vertex];
        }

        std::vector<int> GetPrevVertices(int vertex) const {
            if (vertex < 0 || vertex >= (int)adj.size())
                throw std::out_of_range("vertex index out of range");

            std::vector<int> prev;
            for (int i = 0; i < (int)adj.size(); ++i) {
                for (int to : adj[i]) {
                    if (to == vertex) {
                        prev.push_back(i);
                        break;
                    }
                }
            }
            return prev;
        }

        int VerticesCount() const { return static_cast<int>(adj.size()); }

    private:
        std::vector<std::vector<int>> adj;
};

void BFS(ListGraph& graph, int u, int n, std::vector<int>& d, std::vector<int>& k) {
    std::queue<int> qu;
    qu.push(u);

    while (!qu.empty()) {
        int v = qu.front();
        qu.pop();

        for (int w : graph.GetNextVertices(v)) {
            if (d[v] + 1 < d[w]) {
                d[w] = d[v] + 1;
                k[w] = k[v];
                qu.push(w);
            } else if (d[v] + 1 == d[w]) {
                k[w] += k[v];
            }
        }
    }
}

int shortestPathsCount(ListGraph& graph, int u, int w) {
    int n = graph.VerticesCount();
    if (!(u < n && w < n)) {
        throw std::range_error("u or w is bigger then n");
    }

    std::vector<int> d;
    std::vector<int> k;
    d.assign(n, INT_MAX);
    k.assign(n, 0);
    d[u] = 0;
    k[u] = 1;

    BFS(graph, u, n, d, k);
    return k[w];
}

int main() {
    int v, n = 0;
    std::cin >> v;
    std::cin >> n;

    ListGraph graph(v);

    for (int i = 0; i < n; ++i) {
        int v1, v2 = 0;
        std::cin >> v1 >> v2;
        graph.AddEdge(v1, v2);
        graph.AddEdge(v2, v1);
    }

    int u, w;
    std::cin >> u >> w;

    std::cout << shortestPathsCount(graph, u, w) << std::endl;

    return 0;
}
