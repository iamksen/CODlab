#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <stack>
#include <set>
using namespace std;

struct node{
	set<int> firstpos, lastpos;
	bool nullable;
};

map< pair<char, int>, struct node > mp; // map < pair<alphabet,position in postfix regex>, struct node >
map< char, vector<int> > position; // map< char, positions in infix regex >
set<char> alphabets; // alphabets in regular expression

/* function to convert infix regex to postfix */
string infix_to_postfix(string regex)
{
	stack< char > st;
	string result = "";
	for(int i = 0; i < (int)regex.length(); i++){
		if( regex[i] >= 'a' && regex[i] <= 'z' ) //if character is a alphabet
			result += regex[i];
		else if( regex[i] == '(')
			st.push('(');
		else if( regex[i] == ')'){
			while(st.top() != '('){
				result += st.top();
				st.pop();
			}
			if( st.top() == '(') st.pop();
		} else { 
			if( regex[i] == '*')
				result += '*';
			else { //if regex[i] is '.' or '|' ; assuming both have same priority
				while( !st.empty() && st.top() != '('){
					result += st.top();
					st.pop();
				}
				st.push(regex[i]);
			}
		}
	}
	while( !st.empty()){
		result += st.top();
		st.pop();
	}
	return result;
}

/* to assign firstpos and lastpos to leaf nodes; equal to their position in regex  */
void assign( string exp )
{
	int count = 1;
	for(int i = 0 ; i < (int)exp.length(); i++){
		if( (exp[i] >= 'a' && exp[i] <= 'z') || exp[i] == '#'){ //if alphabet
			struct node sn;
			sn.firstpos.insert(count); //count is assign according to prefix regex
			sn.lastpos.insert(count);
			sn.nullable = false; // leaf node are not nullable
			mp[ make_pair(exp[i], i+1)] = sn; // map position assign according to postfix regex
			if( exp[i] != '#') //skip #
				alphabets.insert( exp[i] );
			position[ exp[i] ].push_back(count);
			count++;
		}
	}
}

struct node apply_rule(struct node left, struct node right, char c)
{
	struct node sn;
	sn.nullable = false;
	if( c == '|'){
		if( left.nullable || right.nullable)
			sn.nullable = true;
		sn.firstpos.insert(left.firstpos.begin(), left.firstpos.end());
		sn.firstpos.insert(right.firstpos.begin(), right.firstpos.end());
		sn.lastpos.insert(left.lastpos.begin(), left.lastpos.end());
		sn.lastpos.insert(right.lastpos.begin(), right.lastpos.end());
	} else if ( c == '.'){
		if( left.nullable && right.nullable)
			sn.nullable = true;
		if( left.nullable ){
			sn.firstpos.insert(left.firstpos.begin(), left.firstpos.end());
			sn.firstpos.insert(right.firstpos.begin(), right.firstpos.end());
		}
		else
			sn.firstpos = left.firstpos;

		if( right.nullable ){
			sn.lastpos.insert(left.lastpos.begin(), left.lastpos.end());
			sn.lastpos.insert(right.lastpos.begin(), right.lastpos.end());
		}
		else
			sn.lastpos = right.lastpos;
	} else if ( c == '*'){
		sn.nullable = true;
		sn.firstpos = right.firstpos;
		sn.lastpos = right.lastpos;
	}
	return sn;
}

/* calculate firstpos and lastpos */
struct node first_last_pos( string exp )
{
	stack < struct node > st;
	struct node left, right, temp;
	for(int i = 0; i < (int)exp.length(); i++){
		if( (exp[i] >= 'a' && exp[i] <= 'z') || exp[i] == '#') // we will apply rules at *, . and | nodes 
			st.push( mp[make_pair(exp[i], i+1)] );
		else {
			if( !st.empty() ){ // check for right child
				right = st.top();
				st.pop();
			}
			left.firstpos.clear();
			left.lastpos.clear();
			if(exp[i] != '*'){ // if node is * node we just need one child
				if( !st.empty() ){ // check for left child
					left = st.top();
					st.pop();
				}
			}
			temp = apply_rule(left, right, exp[i]);
			mp[ make_pair( exp[i], i+1) ] = temp;
			st.push(temp);
		}
	}
	struct node f = st.top();
	st.pop();
	return f; // return top of stack (root of tree)
}

vector< set<int> > calculate_followpos( string exp )
{
	stack < struct node > st;
	vector< set<int> > followpos(exp.length() +1);
	set<int> ::iterator sit, jit;
	struct node left, right, temp;
	for(int i = 0; i < (int)exp.length(); i++){
		if( (exp[i] >= 'a' && exp[i] <= 'z') || exp[i] == '#')
			st.push( mp[make_pair(exp[i], i+1)]);
		else if( exp[i] == '|'){
			if( !st.empty()) st.pop(); //remove right child
			if( !st.empty()) st.pop(); //remove left child
			st.push( mp[make_pair(exp[i], i+1)]);
		} else {
			if( !st.empty()){
				right = st.top();
				st.pop();
			}
			if( exp[i] == '.'){ //concatenation
				if( !st.empty()){
					left = st.top();
					st.pop();
				}
				for( sit = left.lastpos.begin(); sit != left.lastpos.end(); sit++){
					for( jit = right.firstpos.begin() ; jit != right.firstpos.end(); jit++){
						followpos[*sit].insert(*jit);
					}
				}
			} else if( exp[i] == '*'){
				for( sit = right.lastpos.begin(); sit != right.lastpos.end(); sit++){
					for( jit = right.firstpos.begin() ; jit != right.firstpos.end(); jit++){
						followpos[*sit].insert(*jit);
					}
				}
			}	
			st.push( mp[ make_pair(exp[i], i+1)]);
		}
	}
	return followpos;
}

void print_firstpos()
{
	map< pair<char, int>, struct node > ::iterator it;
	set<int> ::iterator sit;
	cout<<"node position(in postfix exp)\n";
	for(it = mp.begin(); it != mp.end(); it++){
		cout<<it->first.first<<" "<<it->first.second;
		cout<<"\nfirstpost --> { ";
		for( sit = it->second.firstpos.begin() ; sit != it->second.firstpos.end(); sit++)
			cout<<*sit<<" ";
		cout<<"}\nlastpost --> { ";
		for( sit = it->second.lastpos.begin() ; sit != it->second.lastpos.end(); sit++)
			cout<<*sit<<" ";
		cout<<"}"<<endl<<endl;
	}
}

void print_followpos(vector< set<int > > followpos)
{
	set<int> ::iterator sit;
	cout<<"\nFollowpos\n";
	cout<<"node(infix) followpos\n";
	for(int i = 0 ; i < (int)followpos.size(); i++){
		if( (int)followpos[i].size() <= 0) continue;
		cout<<i<<" --> { ";
		for( sit = followpos[i].begin() ; sit != followpos[i].end(); sit++){
			cout<<*sit<<" ";
		}
		cout<<"}"<<endl;
	}
}

void print_dfa( set<int> firstpos, vector< set<int> > followpos)
{
	set<int> temp;
	set<int> ::iterator sit;
	stack< set<int> > state;
	map< pair< set<int> , char> , set<int> > Dtrans; //map< pair<state, character>, next_state>
	map< pair< set<int> , char> , set<int> > ::iterator mit;
	map< set<int>, bool> visited; 
	state.push(firstpos);
	set<char> :: iterator alpha;
	int count = 0;
	while( !state.empty() ){
		temp = state.top();
		state.pop();
		count++;
		//if(count > 1000) break;
		if( !visited[temp] ){
			visited[temp] = true;
			for( alpha = alphabets.begin(); alpha != alphabets.end(); alpha++){
				//cout<<" alpha "<<*alpha<<" pos "<<position[*alpha]<<endl;
				set<int> next_state;
				for(int i = 0 ; i < (int)position[*alpha].size(); i++){
					for(sit = temp.begin() ; sit != temp.end(); sit++){
						if( *sit == position[*alpha][i] )
							next_state.insert(followpos[*sit].begin(), followpos[*sit].end());
					}
				}
				Dtrans[make_pair(temp, *alpha)] = next_state;
				if( next_state.size() >0 && !visited[next_state] ) //if next_state is not visited till now
					state.push(next_state);
			}
		}
	}
	bool finalState;
	cout<<"\n\n# denotes start state, * denote final states";
	cout<<"\n( Current State, Input Symbol ) --> Next State\n";
	for( mit = Dtrans.begin(); mit != Dtrans.end(); mit++){
		finalState = false;
		cout<<"(";
		for(sit = mit->first.first.begin(); sit != mit->first.first.end(); sit++){
			cout<<*sit<<" ";
			if( *sit == position['#'][0] ) finalState = true;
		}
		cout<<", "<<mit->first.second<<")";
		if( mit->first.first == firstpos) cout<<"#";
		if( finalState) cout<<"*";
		cout<<" --> ( ";
		if( mit->second.size() == 0 ) cout<<"trap ";
		for(sit = mit->second.begin(); sit != mit->second.end(); sit++)
			cout<<*sit<<" ";
		cout<<")"<<endl;
	}
}

int main()
{
	string regex, result;
	cout<<"Enter a regular expression (please use . for concatenation eg abc => a.b.c) ";
	cin>>regex;

	cout<<"Your regular expression is "<<regex<<endl;
	result = infix_to_postfix(regex);
	cout<<"Infix to postfix of "<<regex<<" is "<<result<<endl;

	result += "#."; // add # at the end of postfix regex

	assign( result );
	struct node f = first_last_pos( result );
	print_firstpos();

	vector< set<int> > followpos( result.length() + 1);
	followpos = calculate_followpos( result );
	print_followpos(followpos);

	print_dfa(f.firstpos, followpos);
	return 0;
}