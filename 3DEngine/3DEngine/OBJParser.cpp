#include <iostream>
#include <fstream>
using namespace std;


void parseOBJ(string fname) {
	ifstream objfile;
	char data[100];
	
	//open obj file
	objfile.open(fname);

	objfile >> data;
	cout << data;


	//close the obj file
	objfile.close();
}
