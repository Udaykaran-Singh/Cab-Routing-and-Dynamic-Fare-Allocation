// Optimized cab_system.cpp with hash fix
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

struct Edge {
    double base_cost;
    double congestion_cost;
    int cars;
    int seats;

    Edge(double base = 0, double congestion = 0, int cabs = 0)
        : base_cost(base), congestion_cost(congestion), cars(cabs), seats(cabs * 4) {}
};

// Custom hash for pair<string, string>
struct pair_hash {
    size_t operator()(const pair<string, string>& p) const {
        return hash<string>()(p.first) ^ (hash<string>()(p.second) << 1);
    }
};

pair<string, string> make_key(const string& a, const string& b) {
    return a < b ? make_pair(a, b) : make_pair(b, a);
}

class CabSystem {
private:
    unordered_map<string, unordered_map<string, Edge>> graph;
    unordered_map<pair<string, string>, int, pair_hash> inactive_routes; // fixed hash type

public:
    void add_route(const string& a, const string& b, double base, double congestion, int cars) {
        Edge e(base, congestion, cars);
        graph[a][b] = e;
        graph[b][a] = e;
    }

    void set_inactive_route(const string& a, const string& b, int spare_seats) {
        inactive_routes[make_key(a, b)] = spare_seats;
    }

    pair<double, vector<string>> find_shortest_path(const string& src, const string& dest) {
        unordered_map<string, double> dist;
        unordered_map<string, string> parent;
        for (const auto& [node, _] : graph) dist[node] = numeric_limits<double>::max();
        dist[src] = 0;

        using PQ = pair<double, string>;
        priority_queue<PQ, vector<PQ>, greater<PQ>> pq;
        pq.emplace(0, src);

        while (!pq.empty()) {
            auto [cur_cost, u] = pq.top(); pq.pop();
            if (u == dest) break;

            for (const auto& [v, e] : graph[u]) {
                double cost = e.base_cost + e.congestion_cost;
                if (dist[v] > dist[u] + cost) {
                    dist[v] = dist[u] + cost;
                    parent[v] = u;
                    pq.emplace(dist[v], v);
                }
            }
        }

        vector<string> path;
        for (string curr = dest; curr != src; curr = parent[curr]) {
            if (parent.find(curr) == parent.end()) return {numeric_limits<double>::max(), {}};
            path.push_back(curr);
        }
        path.push_back(src);
        reverse(path.begin(), path.end());
        return {dist[dest], path};
    }

    int assign_spare_seats(int required) {
        int assigned = 0;
        for (auto& [route, spare] : inactive_routes) {
            int now = min(required - assigned, spare);
            spare -= now;
            assigned += now;
            if (assigned >= required) break;
        }
        return assigned;
    }

    double calculate_fare(const string& src, const string& dest, int P, vector<string>& path_used) {
        auto [base_cost, path] = find_shortest_path(src, dest);
        path_used = path;
        if (path.empty()) return -1;

        int seat_cap = INT_MAX;
        for (size_t i = 0; i + 1 < path.size(); ++i) {
            seat_cap = min(seat_cap, graph[path[i]][path[i + 1]].seats);
        }

        if (P <= seat_cap) return base_cost;

        int unfilled = P - seat_cap;
        int spare_total = 0;
        for (const auto& [_, spare] : inactive_routes) spare_total += spare;

        if (spare_total >= unfilled) {
            assign_spare_seats(unfilled);
            return 1.25 * base_cost;
        } else {
            assign_spare_seats(spare_total);
            return 1.5 * base_cost;
        }
    }
};

void process_input(CabSystem& cs, const string& infile_path, const string& outfile_path) {
    ifstream infile(infile_path);
    ofstream outfile(outfile_path);

    string line, section;
    while (getline(infile, line)) {
        if (line.empty()) continue;
        if (line.find("NODES:") == 0) { section = "nodes"; continue; }
        if (line.find("ROUTES:") == 0) { section = "routes"; continue; }
        if (line.find("INACTIVE:") == 0) { section = "inactive"; continue; }
        if (line.find("REQUESTS:") == 0) { section = "requests"; continue; }

        istringstream iss(line);
        string a, b; double base, cong; int val;

        if (section == "routes") {
            if (iss >> a >> b >> base >> cong >> val)
                cs.add_route(a, b, base, cong, val);
        } else if (section == "inactive") {
            if (iss >> a >> b >> val)
                cs.set_inactive_route(a, b, val);
        } else if (section == "requests") {
            int passengers;
            if (iss >> a >> b >> passengers) {
                vector<string> path;
                double fare = cs.calculate_fare(a, b, passengers, path);
                if (fare != -1) {
                    outfile << "Request: " << a << " -> " << b << " (" << passengers << " passengers)\n";
                    outfile << "Path used: ";
                    for (size_t i = 0; i < path.size(); ++i) {
                        outfile << path[i];
                        if (i + 1 < path.size()) outfile << " -> ";
                    }
                    outfile << "\nFare: " << fare << "\n\n";
                } else {
                    outfile << "Request from " << a << " to " << b << " failed. No valid path.\n\n";
                }
            }
        }
    }
}

int main() {
    CabSystem system;
    process_input(system, "input.txt", "output.txt");
    cout << "Simulation complete. Check output.txt for results." << endl;
    return 0;
}