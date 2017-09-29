#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

ifstream fin;

const int NY = 0;
const int LA = 1;
const int MIA = 2;
const int HOU = 3;
const int CHI = 4; 

struct Warehouse{
	int items[3];
	char type;

};



void readPrices(double[]);

int main(){
	Warehouse warehouse[5];
	
	double prices[3];
	fin.open("data.txt");

	readPrices(prices);		
	
	readCards();
	return 0;
}


void readPrices(double prices[]){
	string line;
	string dollarsign = "$";
	string price;
	
	getline(fin,line);	
	//Find dollar sign and get next 4 chars after for price
	
	size_t found = line.find(dollarsign);
	for(int i = 0;i<3;i++){
		prices[i] = stod(line.substr(found+1,4));
		cout<<prices[i]<<endl;
		found = line.find(dollarsign,found+1);
	}	
	
}

void readCards();
	
	fin >> 
