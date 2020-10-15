#include <bits/stdc++.h>

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

bool compareItems(int item1, int item2){
	return (order[item1] < order[item2]);
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

	void incrementCount(){
		this->count = this->count+1;
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
	FpTree(){
	    table.resize(order.size());
		root = new Node(-1,NULL);
		this->size=1;
		string line;
		while(getline(cin, line)){
		    //cout << "Transact: "<<line << endl;
			stringstream ss;
			ss << line;
			vector<int> transaction;
			int item;
			while(ss >> item){
			    if(order.find(item)!=order.end())
				transaction.push_back(item);
			}
			sort(transaction.begin(), transaction.end(), compareItems);
			addTransaction(root, transaction, 0);
		}
	}

	void addTransaction(Node* curr, vector<int> transaction, int index){
		if(transaction.size()==index) return;
		else {
			int item = transaction[index];
			int itemId = order[item];
			//cout << "AddTransactions "<<item << " " << itemId << endl;
			Node* node;
			if(curr->children[itemId]==NULL){
			    this->size = this->size+1;
				node = new Node(item, curr);
				node->next = NULL;
				curr->children[itemId] = node;
				//if(item == 3 && table[itemId]!=NULL) //cout<< table[itemId] << "********"<<table[itemId]->next<<endl;
				if(this->table[itemId]==NULL) table[itemId]=node;
				else{
					Node* prev = table[itemId];
					table[itemId] = node;
					node->next = prev;
				}
			}
			else{
				node = curr->children[itemId];
				node->incrementCount();
			}
			addTransaction(node, transaction, index+1);
		}
	}
    
    vector<vector<int>> getFrequentItemSets(){
        vector<vector<int>> result;
        //cout << "Frequent" <<endl;
		//Base Case
		if(this->size==1){
		    vector<vector<int>> result(0);
		    //cout << "IIIIIIIIIIIIIIIIIII "<< result.size() << endl;
		    return result;
		}
		//Recursive step
		vector<int> old_order_vec = order_vec;
		unordered_map<int,int> old_order = order;
		for(int i=0; i<old_order_vec.size(); i++){
		    unordered_map<int, int> itemCount;
		    int condItem = old_order_vec[i];
		    int condItemId = old_order[condItem]; //Changed to old order
		    Node* lastNode = table[condItemId];
		    int condItemCount = 0;
		    cout << "Frequent loop: "<<i << " " << condItem << endl;
		    for(int x: old_order_vec) cout << x << " --";
		    cout << old_order_vec.size()<< endl;
		    //for(pair<int,int> p: order) //cout << "============"<< p.first << " " << p.second << endl;
		    while(lastNode!=NULL){
		        //cout << "Passed this--" <<endl;
		        //cout << "Passed this--"<<(lastNode==NULL) <<endl;
		        condItemCount = condItemCount+lastNode->count;
		        //cout << "Hey1" << endl;
		        Node* curr = lastNode->parent;
		        //cout << "Hey2" << endl;
		        while(curr->item!=-1){
		            //cout << curr->item << endl;
		            int item = curr->item;
		            //itemCount[item] = itemCount[item]+curr->count;  this is a really hard bug
		            itemCount[item] = itemCount[item]+lastNode->count;
		            curr = curr->parent;
		        }
		        cout << "Passed this->" <<endl;
		        lastNode = lastNode->next;
		    }
		    cout << "Passed this" <<endl;
		    //Checking if this satisfies min
		    float sup = condItemCount*1.0/numTransactions;
		    //if(this->size==2) //cout << "Support "<< sup << " condItem " << condItem << endl;
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
		            vector<int> condTransaction;
		            cout << lastNode->item << " hahaha "<< lastNode->parent<< endl;
		            Node* curr = lastNode->parent;
		            while(curr->item!=-1){
		                int item = curr->item;
		                if(order.find(item)!=order.end()){
		                    condTransaction.push_back(item);
		                }
		                sort(condTransaction.begin(),condTransaction.end(),compareItems);
		                cout <<"Transact " << endl;
		                for(int t: condTransaction) cout<<t<<" ";
		                cout << endl;
		                for(int i=0; i<curr->count; i++){
		                    condTree.addTransaction(condTree.root, condTransaction, 0);
		                }
	                    curr = curr->parent;
		            }
		            lastNode = lastNode->next;
		        }
		        cout << condTree.size << "**" << endl;
		        condTree.print();
		        vector<vector<int>> condResult = condTree.getFrequentItemSets();
		        cout << condResult.size()<<"-----------------------------"<< this->size<<endl;
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
		cout << "EndItems" << endl;
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


int main(int argc, char** argv){
	//Input, Output and Threshold
	minSup = 0.8;
	unordered_map<int,int> counter;
	numTransactions =0;
	string line;
	int c = 3;
	while(c--){
	    getline(cin, line);
		stringstream ss;
		ss << line;
		int item;
		while(ss >> item){
			counter[item] = counter[item]+1;
		}
		numTransactions = numTransactions+1;
	}
	//Discard all the items that have support less than minSup
	vector<pair<int,int>> itemList(counter.begin(), counter.end());
	itemList = filter(itemList);
	sort(itemList.begin(), itemList.end(), compareCounts);
	int index = 0;
	for(pair<int,int> item: itemList){
		order[item.first] = index;
		index = index+1;
		//cout <<"Order: "<<item.first <<" " <<order[item.first] << endl;
		order_vec.push_back(item.first);
	}
	reverse(order_vec.begin(), order_vec.end());
	//for(int x: order_vec) //cout <<"O "<< x << " ";
    //cout << "het----"<< endl;
	FpTree tree;
	tree.print();
	//cout << "het----";
	//Node* n = tree.table[3];
	//cout << n->item << endl;
	//cout << (n->next==NULL) << n->next->item<< endl;
	vector<vector<int>> result = tree.getFrequentItemSets();
	//cout << "het----";
	//cout << "Result size: " << result.size() << endl;
	for(vector<int>& itemSet: result){
	    for(int& item: itemSet) cout << item << " --";
	    cout << endl;
	}
	cout << result.size() << endl;

}


/* TestCase
1 2 3 4
6 5 3
2 4 5
1 2 3 4
6 5 3
2 4 5
*/
