// Stock Market Query Engine
// Code for priority changing treaps :)
#include<bits/stdc++.h>

#include<iostream>

#include<fstream>

#define ll long long

using namespace std;

typedef struct node {
  ll val;
  ll priority;
  string name;
  struct node * left;
  struct node * right;
}
node;
int size = 0;

node * create(ll value, ll priority, string name) {
  node * temp = new node;
  temp -> val = value;
  temp -> name = name;
  temp -> priority = priority;
  temp -> right = temp -> left = NULL;
  return temp;
}

node * left_rotate(node * curr) {
  node * temp1 = curr -> right;
  node * temp2 = temp1 -> left;
  temp1 -> left = curr;
  curr -> right = temp2;
  return temp1; //this becomes the new root of subtree
}

node * right_rotate(node * curr) {
  node * temp1 = curr -> left;
  node * temp2 = temp1 -> right;
  temp1 -> right = curr;
  curr -> left = temp2;
  return temp1;
}

node * Insert(node * root, ll key, ll priority, string name) {
  if (!root) return create(key, priority, name); // directly create the node and return
  if (key <= root -> val) {
    //move to the left child
    root -> left = Insert(root -> left, key, priority, name);
    if ((root -> left) -> priority > root -> priority) root = right_rotate(root); // we perform a rotate to shift as per priority
  } else {
    root -> right = Insert(root -> right, key, priority, name);
    if (root -> right -> priority > root -> priority) root = left_rotate(root); // we perform a rotate to shift as per priority
  }
  return root;
}

node * Delete(node * root, ll target) {
  if (root == NULL) return root; //The node with such key was not found
  if (target < root -> val) root -> left = Delete(root -> left, target); //go to left child
  else if (target > root -> val) root -> right = Delete(root -> right, target); //go to right child
  else if (root -> left == NULL) {
    // the node to be deleted has no left child
    node * temp = root -> right;
    delete(root);
    root = temp;
  } 
  else if (root -> right == NULL) {
    // the node to be deleted has no right child
    node * temp = root -> left;
    delete(root);
    root = temp;
  } 
  else if ((root -> left) -> priority >= (root -> right) -> priority) {
    // left child is to become the new root
    root = right_rotate(root);
    root -> right = Delete(root -> right, target);
  } 
  else {
    root = left_rotate(root);
    // right child becomes the new root
    root -> left = Delete(root -> left, target);
  }
  return root;
}

node *get_rel(node * root, ll k) {
  vector < ll > store;
  node * temp;
  temp = root;
  while (temp != NULL) {
    store.push_back(temp -> val);
    temp = temp -> left;
  }
  ll it = 0; // iterator to traverse the array / vector
  while (it < (ll) store.size() && store[it] > k) it++;
  if (it == (ll) store.size()) return NULL;
  else {
    temp = root;
    for (ll i = 0; i < it; i++) temp = temp -> left;
    return temp;
  }
}

node * priority_change(node * root, ll target, ll new_priority, string name) {
  root = Delete(root, target);
  root = Insert(root, target, new_priority, name);
}

// we can break the recusrive calls as soon as we get the answer

node * get_val(node * root, ll check) { //check is the least value of k that is to be examined
  //if(root->priority<check)return NULL;
  if (root -> right != NULL && root -> right -> priority >= check) return get_val(root -> right, check);
  else return root;
}

// get_val may seem like O(n) but due to the randomess in priority of different stocks its actually O(log(n))

node * search(node * root, ll target) {
  if (root == NULL || root -> val == target) return root;
  if (root -> val > target) return search(root -> left, target);
  return search(root -> right, target);
}

int main() {
  node * root = NULL;
  cout << "You can perform the following operations using treaps optimally :\n";
  cout << "-->Get the stock with kth largest reliability\n";
  cout << "-->Addition of new stock providing certain profit and having some reliability\n";
  cout << "-->Buying the stock and removing it off the list\n";
  cout << "-->Change in priority of stock over a duration(approximate, but actual change is almost undeterminate)\n";
  cout << "The randomness of reliability actually reduces the complexity\n";
  cout << "There is already some data taken input from the net in input file to give a little actual simulation :)\n";
  // all randomisations for change in reliability after k operations is balanced
  fstream file;
  file.open("input.txt", ios::in);
  if (!file) {
    cout << "Error\n";
    return -1; //exit code -1
  }
  // Reading data from file that is generated but values are taken from net to give a realistic simulation of the data structure
  map < string, ll > name_keep;
  for (ll i = 0; i < 100000; i++) {
    string name;
    file >> name;
    ll reliability, value;
    file >> reliability >> value;
    root = Insert(root, value, reliability, name);
    size++;
    name_keep[name] = value;
    if (file.eof()) break;
  }
  file.close();
  while (1) {
    cout << "1) Insert new stock\n2) Buy a stock\n3) Change priority of stock\n4) Get stock with certain reliability and most security\n5) Get Maximum reliability for given value of stock that you are willing to invest in :\n6) Exit\n";
    cout << "Enter your choice : ";
    int t;
    cin >> t;
    if (t == 1) {
      cout << "Stock Name(Must be unique and of utmost length 100) : ";
      string s;
      cin >> s;
      if ((ll) s.size() >= 100 || s.size() == 0 || name_keep.find(s) != name_keep.end()) { //map would be accessed and searches in logn
        cout << "Invalid entry\n\n";
        continue;
      }
      cout << "Enter the value of stock : ";
      ll val;
      cin >> val;
	  name_keep[s]=val;
      cout << "Enter the reliability(Enter value from 1 to 1000000) : ";
      ll reliability;
      cin >> reliability;
      if (reliability < 1 || reliability > 1000000) {
        cout << "Incorrect reliability\n";
        continue;
      }
      root = Insert(root, val, reliability, s);
      cout << "Successfully inserted\n\n";
      size++;
    } else if (t == 2) {
      if (root==NULL) {
        cout << "Market is empty\n";
        continue;
      }
      string s;
      cout << "Enter Stock Name you want to buy : ";
      cin >> s;
      if (name_keep.find(s) == name_keep.end()) {
        cout << "No such stock found\n\n";
        continue;
      }
	  //cout<<name_keep[s]<<" ";
      root=Delete(root, name_keep[s]);
      name_keep[s]=0;
      size--;
    } else if (t == 3) {
      string s;
      cout << "Enter Stock Name whose priority you wish to change : ";
      cin >> s;
      if (name_keep.find(s) == name_keep.end()) {
        cout << "No such stock found\n\n";
        continue;
      }
      ll change;
      cout << "Enter new priority for custom change : ";
      cin >> change;
      priority_change(root, name_keep[s], change, s);
    } else if (t == 4) {
      if (size == 0) {
        cout << "Market is empty :( \n)";
        continue;
      }
      cout << "Enter the minimum reliability index : ";
      ll k;
      cin >> k;
      if (root -> priority < k) {
        cout << "None available in the market\n";
        continue;
      }
      node * temp = get_val(root, k);
      if (temp == NULL) cout << "The expected reliability is too high\n";
      else cout << "The best value you can get is : " << temp -> val << " from the stock " << temp->name << endl;
    } else if (t == 5) {
      if (size == 0) {
        cout << "Market is empty :( \n)";
        continue;
      }
      cout << "Enter the value : ";
      ll k;
      cin >> k;
      node *temp = get_rel(root, k);
      if (temp==NULL) {
        cout << "None available in the market\n";
        continue;
      }
      cout << "The best reliability you can get for given value is : " << temp->priority << " from the stock " << temp->name << endl;
    } else break;
  }
  return 0;
}