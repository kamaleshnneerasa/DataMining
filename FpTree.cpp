#include <bits/stdc++.h>
#include <unordered_map>

using namespace std;

int numTransactions;
float minSup;
unordered_map<int,int> order;
vector<int> order_vec;

vector<pair<int,int>> filter(vector<pair<int,int>> itemList){
	vector<pair<int,int>> result;
	for(pair<int,int> item: itemList){
		float sup = (1.0*item.second)/numTransactions;
		if(sup>=minSup){
			result.push_back(item);
		}
	}
	return result;
}

bool compareCounts(pair<int,int> p1, pair<int,int> p2){
    return ((p1.second > p2.second) || (p1.second==p2.second && p1.first < p2.first));
}

bool compareItems(pair<int,int> item1, pair<int,int> item2){
	return (order[item1.first] < order[item2.first]);
}

class Node{
public:
	int item;
	int count;
	int level;
	vector<Node*> children;
	Node* parent;
	Node* next;

public:
	Node(int item, Node* parent){
		this->item = item;
		this->parent = parent;
		this->count = 1;
		vector<Node*> children(order.size()); 
		this->children = children;
		if(parent==NULL) this->level = 0;
		else this->level = parent->level +1;
	}

	void incrementCount(int itemCount){
		this->count = this->count+itemCount;
	}

	int getItem(){
		return this->item;
	}

	int getCount(){
		return this->count;
	}
};

class FpTree{
public:
	Node* root;
	vector<Node*> table; //header table 
	int height;
	int size;
public:
    FpTree(int condElem){
        table.resize(order.size(), NULL);
		root = new Node(-1,NULL);
		this->size=1;
		this->height =0;
    }
	FpTree(string filename){
		ifstream inFile;
		inFile.open(filename);
	    table.resize(order.size());
		root = new Node(-1,NULL);
		this->size=1;
		string line;
		while(getline(inFile, line)){
			stringstream ss;
			ss << line;
			vector<pair<int,int>> transaction;
			int item;
			while(ss >> item){
			    if(order.find(item)!=order.end())
				transaction.push_back(make_pair(item,1));
			}
			sort(transaction.begin(), transaction.end(), compareItems);
			addTransaction(root, transaction, 0);
		}
	}

	void addTransaction(Node* curr, vector<pair<int,int>> transaction, int index){
		if(transaction.size()==index) return;
		else {
			int item = transaction[index].first;
			int itemCount = transaction[index].second;
			int itemId = order[item];
			Node* node;
			if(curr->children[itemId]==NULL){
			    this->size = this->size+1;
				node = new Node(item, curr);
				node->count = itemCount;
				node->next = NULL;
				curr->children[itemId] = node;
				if(this->table[itemId]==NULL) table[itemId]=node;
				else{
					Node* prev = table[itemId];
					table[itemId] = node;
					node->next = prev;
				}
			}
			else{
				node = curr->children[itemId];
				node->incrementCount(itemCount);
			}
			addTransaction(node, transaction, index+1);
		}
	}
    
    vector<vector<int>> getFrequentItemSets(){
        vector<vector<int>> result;
        //Base Case
		if(this->size==1){
		    return result;
		}
		//Recursive Step
		vector<int> old_order_vec = order_vec;
		unordered_map<int,int> old_order = order;
		for(int i=0; i<old_order_vec.size(); i++){
		    unordered_map<int, int> itemCount;
		    int condItem = old_order_vec[i];
		    int condItemId = old_order[condItem]; //Changed to old order
		    Node* lastNode = table[condItemId];
		    int condItemCount = 0;
		    while(lastNode!=NULL){
		        condItemCount = condItemCount+lastNode->count;
		        Node* curr = lastNode->parent;
		        while(curr->item!=-1){
		            int item = curr->item;
		            //itemCount[item] = itemCount[item]+curr->count;  this is a really hard bug
		            itemCount[item] = itemCount[item]+lastNode->count;
		            curr = curr->parent;
		        }
		        lastNode = lastNode->next;
		    }
		    //Checking if this satisfies min
		    float sup = condItemCount*1.0/numTransactions;
		    if(sup>=minSup){
		        vector<pair<int,int>> itemList(itemCount.begin(), itemCount.end());
		        itemList = filter(itemList);
		        order.clear();
		        order_vec.clear();
		    
		        int index = 0; 
	            for(pair<int,int> item: itemList){
		            order[item.first] = index;
		            order_vec.push_back(item.first);
		            index = index+1;
	            }
	            reverse(order_vec.begin(), order_vec.end());
	            
		        FpTree condTree(condItem);
		        lastNode = table[condItemId];
		        while(lastNode != NULL){
		            vector<pair<int,int>> condTransaction;
		            Node* curr = lastNode->parent;
		            while(curr->item!=-1){
		                int item = curr->item;
		                if(order.find(item)!=order.end()){
		                    condTransaction.push_back(make_pair(item, curr->count));
		                }
		                sort(condTransaction.begin(),condTransaction.end(),compareItems);
		                condTree.addTransaction(condTree.root, condTransaction, 0);
	                    curr = curr->parent;
		            }
		            lastNode = lastNode->next;
		        }
		        
		        vector<vector<int>> condResult = condTree.getFrequentItemSets();
		        vector<int> self(1,condItem);
		        result.push_back(self);
		        if(condResult.size()>0){
		            for(vector<int> itemSet: condResult){
		                itemSet.push_back(condItem);
		                result.push_back(itemSet);
		            }
		        }
		    }
		}
		return result;
	}
	
	void printTree(Node* curr){
		cout << curr->item << " "<< curr->level << " "<<curr->count<< endl;
		if(curr==NULL) return;
		else{
			for(int i=0; i<order.size(); i++){
				Node* node = curr->children[i];
				if(curr->children[i]!=NULL) printTree(node);
			}
		}
	}

	void print(){
	    cout << "Printing Tree: Item, Level" << endl;
		printTree(root);
	}
	
};

//suppport inputFile outputFile
int main(int argc, char** argv){
	//Input, Output and Threshold
	minSup = (stoi(argv[1])*1.0)/100;
	string inputFile = argv[2];
	string outputFile = argv[3];
	//minSup = 2.0/10.0;
	//string inputFile = "transact.txt";
	//string outputFile = "result.txt";
	unordered_map<int,int> counter;
	numTransactions =0;

	ifstream inFile;
	inFile.open(inputFile);

	string line;
	while(getline(inFile, line)){
		stringstream ss;
		ss << line;
		int item;
		while(ss >> item){
			counter[item] = counter[item]+1;
		}
		numTransactions = numTransactions+1;
	}
	inFile.close();
	//Discard all the items that have support less than minSup
	vector<pair<int,int>> itemList(counter.begin(), counter.end());
	itemList = filter(itemList);
	sort(itemList.begin(), itemList.end(), compareCounts);
	int index = 0;
	for(pair<int,int> item: itemList){
		order[item.first] = index;
		index = index+1;
		order_vec.push_back(item.first);
	}
	reverse(order_vec.begin(), order_vec.end());
	FpTree tree(inputFile);
	//tree.print();
	vector<vector<int>> result = tree.getFrequentItemSets();

	ofstream outFile;
	outFile.open(outputFile);
	for(vector<int>& itemSet: result){
		sort(itemSet.begin(), itemSet.end());
	    for(int& item: itemSet){
	    	outFile << item << " ";
	    	cout << item << " ";
	    }
	    outFile << endl;
	    cout << endl;
	}
	cout << result.size() << endl;
}


/* TestCase-1
1 2 3 4
6 5 3
2 4 5
1 2 3 4
6 5 3
2 4 5
*/

/*TestCse-2
1 2 5
2 4
2 3
1 2 4
1 3
2 3
1 3
1 2 3 5
1 2 3
6
*/
