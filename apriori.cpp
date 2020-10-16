#include <bits/stdc++.h>
#include <unordered_map>

using namespace std;

int numTransactions;
float minSup;

void printFrquentSets(map<vector<int>, int> F, ofstream& outFile){
	for(pair<vector<int>,int> itemSet: F){
		sort(itemSet.first.begin(), itemSet.first.end());
		for(int item: itemSet.first){
			outFile << item << " ";
			cout << item << " ";
		}
		outFile << endl;
		cout << endl;
	}
}

bool isEqual(vector<int>& v1, vector<int> v2){
	if(v1.size() != v2.size()) return false;
	int N = v1.size();
	for(int i=0; i<N; i++){
		if(v1[i]!=v2[i]) return false;
	}
	return true;
}

map<vector<int>,int> candidateGen(map<vector<int>,int> F){
	map<vector<int>,int> C;
	for(auto it1=F.begin(); it1!=F.end(); it1++){
		for(auto it2=F.begin(); it2!=F.end(); it2++){
			vector<int> set1 = it1->first;
			vector<int> set2 = it2->first;
			bool flag = true;
			int L = set1.size();
			for(int l=0; l<L-1; l++){
				if(set1[l]!= set2[l]){
					flag = false;
					break;
				}
			}
			vector<int> c;
			if(flag==true && set1[L-1]<set2[L-1]){
				c = set1;
				c.push_back(set2[L-1]);
				C[c] = 0;
				for(int l=0; l<=L;l++){
					vector<int> subset = c;
					subset.erase(subset.begin()+l);
					if(F.find(subset)==F.end()){ //Not Found
						C.erase(c);
						break;
					}
				}
			}
		}
	}
	return C;
}

map<vector<int>,int> filter(map<vector<int>,int> C){
	map<vector<int>,int> result;
	for(pair<vector<int>,int> item: C){
		vector<int> itemSet = item.first;
		float sup = (1.0*item.second)/numTransactions;
		if(sup>=minSup){
			result[itemSet] = 1;
		}
	}
	return result;
}

int main(int argc, char** argv){
	int total = 0;
	//Input, Output and Threshold
	minSup = (stoi(argv[1])*1.0)/100;
	string inputFile = argv[2];
	string outputFile = argv[3];
	//minSup = 2.0/10;
	//string inputFile = "transact.txt";
	//string outputFile = "result.txt";
	map<vector<int>,int> C; //CandidateItem Set(This should be a hashmap)
	map<vector<int>,int> F; //FrequentItem Set(This can be a set)
	
	numTransactions =0;
	
	ifstream inFile;
	inFile.open(inputFile);
	
	ofstream outFile;
	outFile.open(outputFile);

	string line;
	while(getline(inFile, line)){
		stringstream ss;
		ss << line;
		cout << line << endl;
		int item;
		while(ss >> item){
			vector<int> itemSet(1);
			itemSet[0] = item;
			C[itemSet] = C[itemSet]+1;
		}
		numTransactions = numTransactions+1;
	}
	inFile.close();
	F = filter(C);
	total = total+F.size();
	printFrquentSets(F, outFile);
	for(int i=2; F.empty()==false; i++){
		C = candidateGen(F);
		inFile.open(inputFile);
		while(getline(inFile, line)){ //For each transaction loop
			stringstream ss;
			ss << line;
			unordered_map<int,int> transaction;
			int item;
			while(ss >> item){
				transaction[item] = 1;
			}
			for(pair<vector<int>,int> itemSet: C){ //For each itemSet loop
				bool flag = true;
				for(int k=0; k<itemSet.first.size(); k++){
					if(transaction.find(itemSet.first[k])==transaction.end()){
						flag = false;
						break;
					}
				}
				if(flag==true) C[itemSet.first] = C[itemSet.first]+1;
			}
		}
		inFile.close();
		F = filter(C);
		printFrquentSets(F, outFile);
		total = total+F.size();
	}
	cout << "Total frequent sets: "<<total << endl;

}


/* TestCase
1 2 3 4
6 5 3
2 4 5
1 2 3 4
6 5 3
2 4 5
*/
