#pragma once

#include<string>
#include<cstring>
#include<iostream>
#include<mysql.h>
#include<fstream>

#include<unordered_map>
#include<unordered_set>
#include<set>
#include<vector>

using namespace std;

class SQL_FUN
{
private:

	string position;

	MYSQL cont;
	MYSQL_RES* queryTables(char* qry);

	bool connect_success = false;

	int v_num, a_num;

public:
	SQL_FUN(string location);
	~SQL_FUN();

	set<int> loadNodes(unordered_map<int,double> &xcoord, 
		unordered_map<int,double> &ycoord);

	unordered_map<int, unordered_map<int, double>> loadEdges();

	unordered_map<int, unordered_map<int, double>> loadDemand();

	void getSize(int& node_num, int& edge_num);
};

