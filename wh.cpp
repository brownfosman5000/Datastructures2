#include <iostream>
#include <fstream>
using namespace std;

ifstream fin;

const int NY = 0;
const int LA = 1;
const int MIA = 2;
const int HOU = 3;
const int CHI = 4; 
const int NUMCITIES = 5;
const int NUMITEMS = 3;


void readPrices(double[]);
void processCards(int [][3],double []);
void shipmentProcessing(int [][3]);
void orderProcessing(int [][3]);
void readInfo(string&,string[],int&);
void initializeWarehouses(int [][3]);
bool  searchAndShip(int [][3],int,int,int);
void printInfo(string,string,string[]);
void update(int [][NUMITEMS],bool ,string[],int,double&,double[]);
void printUpdatedInfo(int[][NUMITEMS],int,string,double&);

int main(){
	int warehouses[NUMCITIES][NUMITEMS];
	
	double prices[3];
	fin.open("data.txt");

	initializeWarehouses(warehouses);
	readPrices(prices);		
	
	processCards(warehouses,prices);
	return 0;
}

//Initialize warehouse items with a 0
void initializeWarehouses(int warehouses[][3]){
	for(int i = 0;i<=NUMCITIES-1;i++)
		for(int j = 0;j<=NUMITEMS-1;j++)
			warehouses[i][j] = 0;
		
}

//Read Prices from first line and store in array
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
//Go through each car
void processCards(int warehouse[][3],double prices[]){
	string shipmenttype,city,amount[NUMITEMS];
	int cityindex;
	bool shipment;
	double priceoforder = 0;
	//Get shipment type and change to char
	while(fin){
		fin >> shipmenttype;
		
		readInfo(city,amount,cityindex);
		printInfo(shipmenttype,city,amount);

		if(shipmenttype == "s") shipment = true;

		else if(shipmenttype == "o") shipment = false;

		else
			cerr<<"Error Shipment Type not 's' or 'o': "<<shipmenttype<<endl;
		
		update(warehouse,shipment,amount,cityindex,priceoforder,prices);
		printUpdatedInfo(warehouse,cityindex,city,priceoforder);
		priceoforder = 0;
		cout<<endl<<endl;
	}

}

void readInfo(string &city,string amount[],int &cityindex){

	fin >> city;

	//Which city to process
	if (city == "New_York")
		cityindex = NY;
	else if (city == "Miami")
		 cityindex = MIA ;
	else if (city == "Los_Angeles")
		cityindex = LA;
	else if (city == "Houston")
		cityindex = HOU;
	else if (city == "Chicago")
		cityindex = CHI;
	else
		cerr<<"Error we do not have warehouses in: "<<city<<endl;

	
	//Read in amounts 
	for(int i = 0;i<3;i++)
		fin >> amount[i];
}


void printInfo(string type,string city,string amount[]){
	cout<<type<<"\t"<<city<<"\t\t";
	for(int i = 0;i<NUMITEMS;i++){
		cout<<amount[i]<<'\t';	
	}
	cout<<endl;


}

//Processes shipment or orders 
void update(int warehouse[][NUMITEMS],bool shipment,string amount[],int cityindex,double &priceoforder,double prices[]){
	int amountneeded;
	double extra;
	//Add amount to inventory if shipment
	for(int i = 0;i<NUMITEMS;i++){
		if(shipment)
			warehouse[cityindex][i] += stoi(amount[i]);
		
		//Subtract amount if order
		else if(!shipment){
			//Get priceoforder 
			priceoforder += prices[i] * stoi(amount[i]);
			//cout<<"PriceofOrder: "<<priceoforder<<"    "<<prices[i]<<"   "<<amount[i]<<endl;
			
			//If warehouse doesnt have enough of a certain item
			if(stoi(amount[i]) > warehouse[cityindex][i]){
				//Get amount needed
				amountneeded = stoi(amount[i]) - warehouse[cityindex][i];
				
					
				//If some place could fufill the order
				if(searchAndShip(warehouse,i,amountneeded,cityindex)){
					warehouse[cityindex][i] -= stoi(amount[i]);
					extra = prices[i]*amountneeded * 0.10;
					//cout<<"Extra"<<extra<<endl;
					priceoforder+=extra;
				}
			}

			else{ 
				warehouse[cityindex][i] -= stoi(amount[i]);
			}
		}

	}

}

void printUpdatedInfo(int warehouse[][NUMITEMS],int cityindex,string city,double& priceoforder){
	cout<<"Updated: "<<city<<"\t\t";;
	for(int i = 0;i<3;i++){
		cout<<warehouse[cityindex][i]<<'\t';
	}
	cout<<endl;
	if(priceoforder)
		cout<<"Price of Order: $"<<priceoforder;
	cout<<endl;


}

bool searchAndShip(int warehouse[][3],int item,int amountneeded,int citylowinv){
	int city,highest = warehouse[0][item];
	string cities[5] = {"New_York","Los_Angeles","Miami","Houston","Chicago"};
	for(int i = 1;i<NUMCITIES;i++){	
		if (highest < warehouse[i][item]){				
			highest = warehouse[i][item];
			city = i;
		}
	}	


	//If we could find an inv higher than the amount needed
	if(highest>amountneeded){
		warehouse[city][item]-=amountneeded;
		warehouse[citylowinv][item]+= amountneeded;
		cout<<amountneeded<<" of item "<<item<<" shipped from "<<cities[city]<<" to "<< cities[citylowinv]<<endl; 	
		
		//Print sender info
		cout<<cities[city]<<"\t";
		for(int i = 0;i<NUMITEMS;i++)
			cout<<warehouse[city][i]<<"\t";
		cout<<endl;
		return true;
	}
	else{
		cout<<"Order Unfilled"<<endl;
		return false;
	}


}
