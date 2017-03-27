#include <bits/stdc++.h>
#define EPSILON "epsilon"
using namespace std;

typedef map< string, vector<string> > mp;
mp production; // production rules; production[left,right], left -> right

string remove_spaces(string str)
{
	string result="";
	for(int i = 0 ; i <(int)str.length(); i++){
		if(str[i] == ' ' || str[i] == '\t') 
			continue;
		result += str[i];
	}
	return result;
}

void add_to_production(string left, string right)
{
	vector<string> prod_right;
	int index = right.find('|');
	string rule;
	while( index != string::npos){ // search for all the rules on the right side
		rule = right.substr(0, index);
		prod_right.push_back(rule);
		right = right.substr(index+1);
		index = right.find('|');
	}	
	if( right.length() > 0 )
		prod_right.push_back(right);
	production[left] = prod_right; // left -> right
}

void print_all_production( mp prod)
{
	mp::iterator it;
	for( it = prod.begin(); it != prod.end(); it++){
		cout<<it->first<<" -> ";
		cout<<it->second[0];
		for(int i = 1 ; i < (int)(it->second).size(); i++) // if there are more than 1 rule on right side
			cout<<" | "<<(it->second)[i];
		cout<<endl;
	}
}

void read_input(const char* filename)
{
	string line, right, left;
	ifstream fp(filename);
	while( getline(fp, line)){
		int index = line.find("->"); // find index of -> in the rule(line)
		left = line.substr(0, index); // left side of the rule
		right = line.substr(index + 2); // right side of the rule
		left = remove_spaces(left);
		right = remove_spaces(right);
		add_to_production(left, right);
	}
	fp.close();
}

bool begins_with(string right, string left) //checks if right begins with left
{
	if(left.length() > right.length() ) return false;
	for(int i = 0 ; i < (int)left.size(); i++){
		if( right[i] != left[i]) return false;
	}
	return true;
}

void remove_direct_left_recursion(string left, vector<string> right)
{
	vector<string> alpha, beta, right1, right2;
	for(int i = 0 ; i < (int)right.size(); i++){
		if( begins_with(right[i], left)){
			alpha.push_back(right[i].substr(left.length()));
		}
		else
			beta.push_back(right[i]);
	}
	if( alpha.size() == 0 ) return;
	for(int i = 0 ; i < (int)beta.size(); i++)  //for A -> beta1A' | beta2A' | beta3A' ....
		right1.push_back(beta[i] + left + "'"); //if grammar already have A' then replace "'" with some other symbol or number

	for(int i = 0 ; i < (int)alpha.size(); i++) // for A' -> alpha1A' | alpha2A' | .... | EPSILON
		right2.push_back(alpha[i] + left + "'");
	right2.push_back(EPSILON);

	production[left] = right1; // A -> beta1A' |  beta2A' | ....
	production[left + "'"] = right2;  // A' -> alpha1A' | alpha2A' | ... | EPSILON
}

void remove_left_recursion(mp &prod)
{
	mp ::iterator i, j;
	vector<string> right, temp, j_right;
	string left, remaining;
	for(i = prod.begin(); i != prod.end(); i++){
		/* remove indirect left recursion */
		right = i->second;
		for( j = prod.begin(); j != i ; j++){
			left = j->first;
			temp.clear();
			for(int r = 0; r < (int)right.size(); r++){
				if(begins_with(right[r], left)){
					j_right = prod[left];
					remaining = right[r].substr(left.length());
					for(int jr = 0; jr <(int)j_right.size(); jr++)
						temp.push_back(j_right[jr] + remaining);
				} else
					temp.push_back(right[r]);
			}
			right = temp;
		}
		prod[i->first] = right;
		/* indirect left recursion removed for ith production (prod[i->first]) */
		remove_direct_left_recursion(i->first, right);  //prod[i->first] => right
	}
}

int main(int argc, char const *argv[])
{
	read_input("input.txt"); //argv[1] is the name of the input file which contains grammar
	cout<<"Productions before removing left recursion\n";
	print_all_production(production);
	remove_left_recursion(production);
	cout<<"\nProductions after removing left recursion\n";
	print_all_production(production);
	return 0;
}