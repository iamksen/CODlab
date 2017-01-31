#include <iostream>
#include <stack>
using namespace std;

stack< char > st;

string infix_to_postfix(string regex)
{
	string result = "";
	for(int i = 0; i < (int)regex.length(); i++){
		if( regex[i] >= 'a' && regex[i] <= 'z') //if character is a alphbat
			result += regex[i];
		else if( regex[i] == '(')
			st.push('(');
		else if( regex[i] == ')'){
			while(st.top() != '('){
				result += st.top();
				st.pop();
			}
			if( st.top() == '(') st.pop();
		} else { //
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

int main()
{
	string regex, result;
	cout<<"Enter a regular expression (please use . for concatenation eg abc => a.b.c) ";
	cin>>regex;

	cout<<"Your regular expression is "<<regex<<endl;
	result = infix_to_postfix(regex);
	
	cout<<"Infix to postfix of "<<regex<<" is "<<result<<endl;
	return 0;
}
