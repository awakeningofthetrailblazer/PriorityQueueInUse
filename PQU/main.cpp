#include<iostream>
#include<queue>
#include<set>
#include<unordered_map>
#include<ctime>
#include<fstream>

#include"SQL_FUN.h"

using namespace std;

struct node {
	friend bool operator< (node n1, node n2) {
		return n1.dis > n2.dis;
	}
	int v;
	double dis;
};

void MatrixDijkstra(set<int>& vertices, 
	unordered_map<int, unordered_map<int, double>> edges);

void NeighborDijkstra(set<int>& vertices,
	unordered_map<int, unordered_map<int, double>> edges);

void PriorityQueueDijkstra(set<int>& vertices,
	unordered_map<int, unordered_map<int, double>> edges);

int main() {
	cout << "priority queue in use" << endl;

	vector<string> nettable = {
		"Siouxfalls",
		"Anaheim",
		"Berlin_center",
		"Birmingham",
		"Chicago",
		"GoldCoast",
		"Philadelphia",
		"Sydney"
	};

	ofstream fout("result.csv", ios::app);
	fout << "intances,nodes,edges,time in matrix,time in neighbors,time in priority queues\n";

	double consume_time;
	int a_num, v_num;

	for (auto str : nettable) {
		//SQL_FUN sql("siouxfalls");
		SQL_FUN sql(str);

		unordered_map<int, double> xcoord, ycoord;

		set<int> vv = sql.loadNodes(xcoord, ycoord);
		unordered_map<int, unordered_map<int,
			double>> ee = sql.loadEdges();

		sql.getSize(a_num, v_num);

		fout << str << ',' << a_num << ',' << v_num << ',';

		clock_t start_time = clock();

		if (str != "Sydney") MatrixDijkstra(vv, ee);

		consume_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;

		fout << consume_time << ',';

		cout << "Time:\t" << consume_time << "s" << endl << endl;
		start_time = clock();

		NeighborDijkstra(vv, ee);

		consume_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;

		fout << consume_time << ',';

		cout << "Time:\t" << consume_time << "s" << endl << endl;
		start_time = clock();

		PriorityQueueDijkstra(vv, ee);

		consume_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;

		fout << consume_time << endl;

		cout << "Time:\t" << consume_time << "s" << endl << endl;
		start_time = clock();

		//break;
	}

	fout.close();

	return 0;
}

void MatrixDijkstra(set<int>& vertices,
	unordered_map<int, unordered_map<int, double>> edges) {

	cout << "use matrix:\t";

	unordered_map<int, unordered_map<int, bool>> edge_visited;
	for (auto pr : edges) {
		for (auto sn : pr.second)
			if (sn.second > 0) edge_visited[pr.first][sn.first] = true;
	}
	
	for (auto source : vertices) {
		//cout << '\t' << source;
		unordered_map<int, bool> vis;
		unordered_map<int, int> pre;
		unordered_map<int, double> dis;
		for (auto v : vertices) {
			pre[v] = -1;
			dis[v] = INFINITY;
		}

		int cnt = vertices.size();
		dis[source] = 0;

		while (cnt--) {
			double mindis = INFINITY;
			int u = -1;
			for (auto v : vertices) {
				if (!vis[v] && dis[v] < mindis) { mindis = dis[v], u = v; }
			}

			if (u == -1) break;
			else {
				vis[u] = true;
				//cout << '\t' << u << ":" << dis[u];
			}

			for (auto v : vertices) {
				if (!vis[v] && edge_visited[u][v] && dis[v] > dis[u] + edges[u][v]) {
					dis[v] = dis[u] + edges[u][v];
					pre[v] = u;
				}
			}
		}

		break;
	}
}

void NeighborDijkstra(set<int>& vertices,
	unordered_map<int, unordered_map<int, double>> edges) {

	cout << "use neighbors:\t";

	

	unordered_map<int, set<int>> children;
	for (auto pr : edges) {
		for (auto sn : pr.second) 
			if(sn.second>0) children[pr.first].insert(sn.first);
	}

	for (auto source : vertices) {
		//cout << '\t' << source;
		unordered_map<int, bool> vis;
		unordered_map<int, int> pre;
		unordered_map<int, double> dis;

		for (auto v : vertices) {
			pre[v] = -1;
			dis[v] = INFINITY;
		}

		int cnt = vertices.size();
		dis[source] = 0;

		while (cnt--) {
			double mindis = INFINITY;
			int u = -1;
			for (auto v : vertices) {
				if (!vis[v] && dis[v] < mindis) { mindis = dis[v], u = v; }
			}

			if (u == -1) break;
			else {
				vis[u] = true;
				//cout << '\t' << u << ":" << dis[u];
			}

			for (auto v : children[u]) {
				if (!vis[v] && dis[v] > dis[u] + edges[u][v]) {
					dis[v] = dis[u] + edges[u][v];
					pre[v] = u;
				}
			}
		}

		break;
	}
}

void PriorityQueueDijkstra(set<int>& vertices,
	unordered_map<int, unordered_map<int, double>> edges) {

	cout << "use priority queue:\t";

	unordered_map<int, set<int>> children;
	for (auto pr : edges) {
		for (auto sn : pr.second)
			if (sn.second > 0) children[pr.first].insert(sn.first);
	}

	for (auto source : vertices) {
		//cout << '\t' << source;

		unordered_map<int, bool> vis;
		unordered_map<int, int> pre;
		unordered_map<int, double> dis;

		for (auto v : vertices) {
			pre[v] = -1;
			dis[v] = INFINITY;
		}

		dis[source] = 0;

		priority_queue<node> current_node;

		current_node.push({ source,0 });

		while (current_node.size()) {

			int u = current_node.top().v;
			current_node.pop();

			if (vis[u]) continue;

			vis[u] = true;
			//cout << '\t' << u << ":" << dis[u];

			for (auto v : children[u]) {
				if (!vis[v] && dis[v] > dis[u] + edges[u][v]) {
					dis[v] = dis[u] + edges[u][v];
					pre[v] = u;

					current_node.push({ v,dis[v] });
				}
			}
		}

		break;
	}
}