#include <bits/stdc++.h>
using namespace std;

struct keyword_tree {
	bool isleaf; 				   //isleaf will be set if current state is end of a keyword
	struct keyword_tree *next[26]; //assuming keyword contain on lowercase letters
};

//Allocate memory for a state (node)
struct keyword_tree *create_state() 
{
	struct keyword_tree *temp;
	temp = (struct keyword_tree*) malloc(sizeof(keyword_tree));
	if( !temp ) return temp;

	temp->isleaf = false;
	for(int i = 0 ; i < 26; i++)
		temp->next[i] = NULL;

	return temp;
}

string keyword[] = {
	"if",
	"else",
	"do",
	"while",
	"for",
	"switch",
	"int",
	"float",
	"char",
	"double",
};

//To calculate the total no of elements in string array keyword
int count_no_of_keyword() 
{
	int count = 0;
	for(int i = 0; i < sizeof(keyword)/sizeof(*keyword); i++)
		if( keyword[i] != "")
			count++;
	return count;
}

//Make a nodes(states) for a keyword ( if necessary)
void make_node(string word, struct keyword_tree * root)
{
	struct keyword_tree *temp;
	temp = root;

	for(int i = 0 ; i < word.length(); i++) {
		if( temp->next[ word[i]-97 ] == NULL ) //if state(node) is not present; make one
			temp->next[ word[i]-97 ] = create_state();
		temp = temp->next[ word[i]-97 ];
	}
	temp->isleaf = true;	//for final state; isleaf is true
}

//Loop through all keywords	
void create_keyword_tree( struct keyword_tree *root)
{
	for(int i = 0 ; i < count_no_of_keyword(); i++)
		make_node(keyword[i], root);
}

bool is_keyword( string word, struct keyword_tree *root )
{
	struct keyword_tree *temp;
	temp = root;
	for(int i = 0 ; i < word.length(); i++){
		if( temp->next[ word[i] -97 ] == NULL )	//if state is not present
			return false;
		temp = temp->next[ word[i]-97 ];
	}
	if( temp != NULL && temp->isleaf == true) // if temp is a final state 
		return true;
	else 
		return false;
}

int main()
{
	struct keyword_tree *root;
	root = create_state();
	create_keyword_tree(root);

	string word;
	while(1) {
		cout<<"Please enter a word ( quit to exit) > ";
		cin>>word;
		if( word == "quit")
			break;

		if( is_keyword(word, root))
			cout<<word<<" is a keyword\n";
		else
			cout<<word<<" is not a keyword\n";
	}
}