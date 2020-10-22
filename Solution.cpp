#include<iostream>
#include <vector>
#include <cstring>
#include <sstream>
#include <fstream>
#include <regex>
using namespace std;

const int IGNORE_CASE = 1 << 0;
const int INVERT_MATCH = 1 << 1;
const int MATCH_WHOLE_WORD = 1 << 2;
const int PRINT_LINE_NO = 1 << 3;

void prinHelp() {
	cout<<"valid command"<<endl;
	cout<<"  [options] [patern] [file name]"<<endl;
	cout<<"  eg. line input.txt"<<endl;
	cout<<"  eg. -w line input.txt"<<endl;
	cout<<"  eg. -i -n line input.txt"<<endl<<endl;
	cout<<"valid option"<<endl;
	cout<<"  -v --invert-match : invert sense of matching"<<endl;
	cout<<"  -i --ignore-case : ignore case in patern and file"<<endl;
	cout<<"  -w --word-regexp : match whole word"<<endl;
	cout<<"  -n --line-number : prefix line number in each output"<<endl;
	return;
}

bool isalphaNum(char c) {
	if((c >= 'a' &&  c<= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') {
	    return true;
	}
	return false;
}

void printMatch(int& flag, string patternToSearch, string fileName) {
	ifstream file;
	file.open(fileName.c_str());
    if (!file) {
       cout<<"File " + fileName + " not present at current directory."<<endl;
       return;
    }
    regex expr = flag & IGNORE_CASE ? regex(patternToSearch, regex_constants::icase)
			: regex(patternToSearch); 
	string lineData="";
	int lineNo=0;
	while (getline(file, lineData)) {
		lineNo++;
		bool match = false;
		smatch matchingWord;
		if ((flag&MATCH_WHOLE_WORD)==0) {
			match = regex_search (lineData, matchingWord, expr);
		}
		else {
			string lineToMatch = lineData;
			while (regex_search (lineToMatch, matchingWord, expr)) {
		        string temp = matchingWord[0];
		        int start = matchingWord.position();
		        if ((start == 0 || !isalphaNum(lineToMatch[start - 1]))
			            && (start + temp.size() == lineToMatch.size() || !isalphaNum(lineToMatch[start + temp.size()]))) {
		        	match = true;
		        	break;
				}
		        lineToMatch = matchingWord.suffix();
		    }
		}
		if (flag & INVERT_MATCH) {
			if (match == false) {
				if (flag&PRINT_LINE_NO) {
					cout<<lineNo<<": ";
				}
				cout<<lineData<<endl;
			}
		}
		else {
			if (match) {
				if (flag&PRINT_LINE_NO) {
					cout<<lineNo<<": ";
				}
				cout<<lineData<<endl;
			}
		}
	}
}

bool isValidOption(string option) {
	if(option == "-v" || option == "--invert-match" || option == "-i" || option == "--ignore-case"
	        || option == "-w" || option == "--word-regexp" || option == "-n" || option == "--line-number") {
		return true;	
	}
	return false;
}

void updateFlag(int& flag, string option) {
	if (option == "-i" || option == "--ignore-case") {
		flag |= IGNORE_CASE;
	}
	if (option == "-v" || option == "--invert-match") {
		flag |= INVERT_MATCH;
	}
	if (option == "-w" || option == "--word-regexp") {
		flag |= MATCH_WHOLE_WORD;
	}
	if (option == "-n" || option == "--line-number") {
		flag |= PRINT_LINE_NO;
	}
}

int main(int argc, char *argv[]) {
	while(1) {
		cout<<endl;
		cout<< "Type help for valid command. Type exit to terminate programme. Type [options] [patern]"
		        << "[file_name] to search pattern in <file_name> file."<<endl;
		string input;
		getline(cin, input);
		if (input == "exit") {
			return 0;
		}
		if (input == "help") {
			prinHelp();
			continue;
		}
		vector<string> args;
		stringstream ss(input);
		string temp;
		while (ss>>temp) {
			args.push_back(temp);
		}
		if (args.size() < 2) {
			cout<<"Invalid Command."<<endl;
			continue;
		}
		int flag=0;
		bool valid=true;
		for (int i=0;i<=(int)args.size()-3;i++) {
			if(!isValidOption(args[i])){
				cout<<args[i]<<" is not a valid option."<<endl;
				valid=false;
				break;
			}
			updateFlag(flag, args[i]);
		}
		if (valid == false) {
			continue;
		}
		printMatch(flag, args[args.size()-2], args[args.size()-1]);
	}
}
