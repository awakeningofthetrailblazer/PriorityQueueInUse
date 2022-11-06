#include "SQL_FUN.h"

SQL_FUN::SQL_FUN(string location) {
	char user[] = "root";
	char pswd[] = "gs2006527mdMySql";
	char host[] = "localhost";
	char dbname[] = "traffic_networks";

	unsigned int port = 3306;

	position = location;

	mysql_init(&cont);
	if (mysql_real_connect(&cont, host, user, pswd, dbname, port, nullptr, 0)) {
		cout << "connect !" << endl;
		mysql_query(&cont, "SET NAMES UTF8");

		connect_success = true;
	}
	else { 
		cout << "connect failed !" << endl; 
	}
}

SQL_FUN::~SQL_FUN() {
	mysql_close(&cont);
	cout << "disconnected !" << endl;
}

MYSQL_RES* SQL_FUN::queryTables(char* qry) {
	MYSQL_RES* result = nullptr;
	int res = mysql_query(&cont, qry);

	if (!res) {
		result = mysql_store_result(&cont);
	}
	else cout << "query sql failed !" << endl;

	return result;
}

set<int> SQL_FUN::loadNodes(unordered_map<int, double>& xcoord, 
	unordered_map<int, double>& ycoord) {
	set<int> res;

	cout << "loadNodes..";

	string qry = "select * from ";
	qry += position.data();
	qry += "_nodes";

	cout << qry << endl;

	MYSQL_RES* result = queryTables(qry.data());
	MYSQL_ROW sql_row;

	int id;
	double xc, yc;
	while (sql_row = mysql_fetch_row(result)) {
		id = stoi(sql_row[0]);
		xc = stod(sql_row[1]);
		yc = stod(sql_row[2]);

		res.insert(id);
		xcoord[id] = xc;
		ycoord[id] = yc;
	}

	v_num = res.size();

	cout << "size of nodes: " << res.size() << endl;

	mysql_free_result(result);

	return res;
}

unordered_map<int, unordered_map<int, double>> SQL_FUN::loadEdges() {

	cout << "loadLanes..";
	string qry = "select * from ";
	qry += position.data();
	qry += "_edges";

	cout << qry << endl;

	MYSQL_RES* result = queryTables(qry.data());

	MYSQL_ROW sql_row;

	unordered_map<int, unordered_map<int, double>> mp;

	int cnt = 0;

	while (sql_row = mysql_fetch_row(result)) {
		cnt++;
		mp[stoi(sql_row[0])][stoi(sql_row[1])] = stod(sql_row[3]);
	}

	a_num = cnt;

	cout << "size of edges: " <<cnt<< endl;

	mysql_free_result(result);

	return mp;
}

unordered_map<int, unordered_map<int, double>> SQL_FUN::loadDemand() {

	cout << "loadDemand..";
	string qry = "select * from ";
	qry += position.data();
	qry += "_demand";

	cout << qry << endl;

	MYSQL_RES* result = queryTables(qry.data());

	MYSQL_ROW sql_row;

	unordered_map<int, unordered_map<int, double>> mp;

	while (sql_row = mysql_fetch_row(result)) {
		mp[stoi(sql_row[0])][stoi(sql_row[1])] = stod(sql_row[2]);
	}

	mysql_free_result(result);

	return mp;
}

void SQL_FUN::getSize(int& node_num, int& edge_num) {
	node_num = v_num;
	edge_num = a_num;
}