/* ###########################################################################################################
#   This project has been given as a task for hackathon for the domain spider algos induction 2016...
#	
#	This project basically is an attempt for making a lexical analyser using string comparisons,string 
#	pattern matching and manipulations. It helps identify to a certain extent 
#	what language a code is written.   
#
#
#  ###########################################################################################################
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <algorithm>
#include <regex>

using namespace std;

char* datatypeset[]={"int","unsignedshort","unsignedlong","unsignedlonglong","bool","char","signedchar","unsignedchar","long","double","void","float","wchar_t"};

int c=0,cpp=0,java=0,python=0;


// Regexes for checking the occurences of common and popular statements in a language - just the tip of the iceberg 

char r_cpp[][200] = {"\\s*(public|private|protected)\\s*:\\s*", "\\s*(std::)?(cout|cin|cerr)\\s*(<<|>>)[^;]*([\)]|;)\\s*" , "\\s*typedef [[:print:]]* [[:print:]]*;\\s*", 
   "\\s*#include\\s*(<|\")\\w*(.h)?(>|\")\\s*" , "\\s*#define\\s+[[:print:]]*" , 
  "\\s*(int|void)?\\s*main\\s*[\(][[:print:]]*[\)][[:print:]]*" , "[[:print:]]*for\\s*[\(][[:print:]]*;[[:print:]]*;[[:print:]]*[\)][[:print:]]*" ,
 
 };
 
char r_java[][200] = { "\\s*import java.[[:print:]]*\\s*" , "\\s*public [\\w* ]*main\\s*[\(][[:print:]]*" };

char r_python[][200] = { "\\s*(def|class|if|elif|else)\\s+[^:?]*:\\s*" , "\\s*@[[:print:]]*\\s*" ,"\\s*(try|except|finally)[^:]*:\\s*" ,  
	"\\s*global [[:print:]]*" , "\\s*(for|while)\\s+[^\(][[:print:]]*:\\s*" , 
};

// Regexes end


int check_python(string text) { 
	regex a;
	
	int i=0;
	for(i=0;i<5;i++) {
		a.assign(r_python[i]);
		if(regex_match(text,a)) 
			return 1;
	}
	return 0;
};

int check_cpp(string text) {
	regex a;
	int i=0;
	
	for(i=0;i<2;i++) {
		a.assign(r_cpp[i]);
		if(regex_match(text,a)) {
			return 1;
		}
	}
	return 0;
}

int check_java(string text) {
	regex a;
	int i=0;
	
	for(i=0;i<2;i++) {
		a.assign(r_java[i]);
		if(regex_match(text,a)) {
			return 1;
		}
	}
	return 0;
}


int checkdatatype(char* text) {
	for(int i=0;i<13;i++) {
		char* temp = datatypeset[i];
		char * p =strstr(text, temp
		);
		if(p!=NULL) {
			return 1;
		}
	}
	return 0;
}

string removedoublequotes(string text) {
	string text2;
	int i=0;
	for(int i=0;text[i] != '\0';i++) {
		if(text[i] == '\"') {
			i++;
			while(text[i] != '\"') {
				i++;
			}
			continue;
		}
		if(text[i] == '\'') {
			i++;
			while(text[i] != '\'') {
				i++;
			}
			continue;
		}
		text2 += text[i];
	}
	text2 += '\0';
	return text2;
}

int maxfour(int first,int second, int third, int fourth)
{
    if((first>second) && (first>third) && (first>fourth))
        return 0;
    else if((second>first) && (second>third) && (second>fourth))
        return 1;
    else if((third>second) && (third>first) && (third>fourth))
        return 2;
    else if((fourth>second) && (fourth>third) && (fourth>first))
        return 3;

return -1;
}

void checklang(char* text1, char* text2) {
    
    //lang[0]=c,lang[1]=cpp,lang[2]=java,lang[3]=python
	//the absence of semicolon and paratheses certify the language to be python 
	if(python!=-1 && (!strstr(text1,";") || !strstr(text1,"{") || !strstr(text1,"}"))) 
	{
			printf("python");
			return;
	}
	//the absence of header files and some keywords help to a certain extent to certify  python
	if(python!=-1 && !strstr(text1,"#include<") && !strstr(text1,"import") && !strstr(text1,"#define") && !strstr(text1,"class"))
	{
			printf("python");
			return;
	}
	//keywords and statements for java
	if(java!=-1 && strstr(text1,"importjava.") || strstr(text1,"System.out") || strstr(text1,"System.in") || strstr(text1,"Jframe") 
		|| strstr(text1,"java.") || strstr(text1,"PrintStream")|| strstr(text1,"Instream") || strstr(text1,"Outstream")
		|| strstr(text1,"publicclass"))
		{
			printf("Java");
			return;
		
		}
	//the absence of any of datatypes for python language
	if(python!=-1 && !checkdatatype(text1))
	{
			printf("python");
			return;
	}		
	//presence of cout<<,cin>>,"iostream, etc for cpp
	if(strstr(text1,"cout=")) 
	{
		if(python!=-1){++python;}
		if(java!=-1){++java;}
		if(c!=-1){++c;}
		if(cpp!=-1){cpp=-1;}
   }
	else if(strstr(text1, "cout<<")){
		cout<<"C++";
		return;
	}
	if(strstr(text1,"cin=")) {
		if(python!=-1){++python;}
		if(java!=-1){++java;}
		
		c=cpp=-1;
	}
	else if(strstr(text1, "cin<<")){
		cout<<"C++";
		return;
	}
	
	//# define for c and cppp
	if(strstr(text1,"#define"))
	{
		if(c!=-1){c++;}
		if(cpp!=-1){cpp++;}
		java=-1;
		python=-1;
	}
	//print keyword for identifying language
	if(strstr(text1,"print"))
	{
		
		
		if(c!=-1 && cpp!=-1 && (strstr(text1,"printf(") || strstr(text1,"scanf(") ))
		{
			printf("Its either C or C++...");
			return;
		}
		else if(java!=-1&& strstr(text1,"println") || strstr(text1,"print("))
		{
	    printf("java");
	    return;
		}
		else if(python!=-1)
		{
	    printf("python");
	    return;
		}
		
	}
	//differentiating header files in c and cpp

	if((c!=-1 && cpp!=-1) && strstr(text1,"#include<"))
	{
		c++;
		cpp++;
		python=java=-1;
		if(strstr(text1,"<iostream")||strstr(text1,"<bits/stdc++"))
		{
			printf("c++");
			return;			
		}
	
		if (strstr(text1,"map>") || strstr(text1,"set>") || strstr(text1,"list>") ||
	   	strstr(text1,"cmath>") || strstr(text1,"ctime>") || strstr(text1,"deque>") ||
		strstr(text1,"queue>") || strstr(text1,"stack>") || strstr(text1,"string>") ||
		strstr(text1,"bitset>") || strstr(text1,"cstudio>") || strstr(text1,"limits>") ||
		strstr(text1,"vector>") || strstr(text1,"cstring>") || strstr(text1,"cstdlab>") ||
		strstr(text1,"fstream>") || strstr(text1,"numeric>") || strstr(text1,"sstream>") ||
		strstr(text1,"algorithm>") || strstr(text1,"unordered_map>")
	    )
		{
			printf("c++");
			return;
		}
	}
	
	
	//keyword import for python and java
	if(java!=-1 && python!=-1 && strstr(text1,"import"))
	{
		python++;
		java++;
	}
	//keyword for,while,if,else,elif for python 
	if(python!=-1 && (strstr(text1,"for") && !strstr(text1,"for("))||(strstr(text1,"if") && !strstr(text1,"if("))||(strstr(text1,"elif"))
		||(strstr(text1,"while") && strstr(text1,"while()")))
	{
		printf("python");
		return;
	}
	
	
	//maximum points for a language help certify what language a program is written
 	int result;
 	result=maxfour(c,cpp,java,python);
	if(result==0)
		{
			printf("c");
			return;
		}
	if(result==1)
		{
			printf("cpp");
			return;	
		}	
	if(result==2)
		{
			printf("java");
			return;
		}
	if(result==3)
		{
			printf("python");
			return;	
		}	
	else if(result==-1)
	{
	int k=0;
	
	//if the count of c and cpp are the same
	if(c==cpp && c!=-1 && cpp!=-1 && c!=0 && cpp!=0)
	{k=1;
		if(strstr(text1,"class") || strstr(text1,"new") || strstr(text1,"delete") || strstr(text1,"::"))
		{
		printf("c++");
		return;	
		}
		if(strstr(text1,"class")) /*&& datatypeprev("class"))*/
		{
		printf("c++");
		return;
		}
	}
	if(k==0)
	 { 
		printf("The code is either c or cpp");
		return;
	 }
	
	}
	cout<<"JAVA";
	return;
}

//removes spaces, the contents inside doublequotes and single quotes
string removeunwanted(string text1) {
	string text2;
	int i=0,j=0;
	for(int i=0;text1[i] != '\0';i++) {
		if(text1[i] == '\"') {
			i++;
			while(text1[i] != '\"') {
				i++;
			}
			continue;
		}
		if(text1[i] == '\'') {
			i++;
			while(text1[i] != '\'') {
				i++;
			}
			continue;
		}
		if(text1[i] != ' ' && text1[i] != '\t') {
			text2[j++] = text1[i];
		}
	}
	text2[j]= '\0';
	return text2;
}

//removes the double and single quotes inside the string i.e., (\',\")
string removebackquotes(string text) {
	string text2;
	for(int i=0;text[i] != '\0';i++) {
		if((text[i] == '\\' && text[i+1] == '\"') || (text[i] == '\\' && text[i+1] == '\'')) {
			i++;
			continue;
		}
		text2+=text[i];
	}
	text2 += '\0';
	return text2;
}

// function to remove single line comments i.e., succeding '//'
string removesinglelinecomments(char* q) {
	char *pos = strstr(q,"//");
	if(strstr(q,"//")) {
		int p = (pos-q)/sizeof(q[0]);
		char m[200];
		int i;
		for(i=0;i<p;i++) {
			m[i]=q[i];
		}
		m[i] = '\0';
		return m;
	}
	else {
		return q;
	}
}

// this function adds \n after encountering '(', ')', '{', '}', ';' so as to avoid multiple instructions in a single line
void addnewline() {
	
	ifstream fin;
	fin.open("inputcode.txt");
	
	static int count = 0;
	char* text1;
	string text;
	char p[200];
	
	while(!fin.eof()) {
		fin.getline(p, 200,'\n');
		string q(p);
		for(int i=0;q[i] !='\0';i++) {
			if(q[i] == '(') {
				count++;
				text += q[i];
				continue;
			}
			else if(q[i] == ')') {
				count--;
				text += q[i];
				continue;
			}
			else if((q[i] == ';') && count == 0) {
				text+=q[i];
				text+='\n';
				continue;
			}
			else if(q[i] == '{') {
				text+='\n';
				text+=q[i];
				text+='\n';
			}
			else if(q[i] == '}') {
				text+='\n';
				text+=q[i];
			}
			else {
				text += q[i];
				continue;
			}
		}
		text+='\n';
	}
	text+='\0';
	text = removebackquotes(text);
	text = removedoublequotes(text);
	fin.close();
	ofstream fout;
	fout.open("newfile.txt");
	fout<<text;
	fout.close();
}

int check_regex(string text) {
	    
	    // Driver funcion for running the regex checks on each line of the input file
	    
	    if(check_python(text)==1) {
	     cout << "\n\tPython\n\n";
		 return 1;	
	    }
	    
	    if(check_cpp(text)==1) {
	     cout << "\n\tC++\n\n";
		 return 1;	
	    }
	    if(check_java(text)==1) {
	     cout << "\n\tJava\n\n";
		 return 1;
	    }
	
	// If all regex fails on the line, return 0
	return 0;
}

int main() {
	
	int flag = 0;
	
	// this function adds \n after encountering '(', ')', '{', '}', ';' so as to avoid multiple instructions in a single line
	addnewline();
	ifstream fin;
	//extracting the file containing the input code
	fin.open("newfile.txt");
	
	//variables to store the extracted code
	char* text1;
	string text;
	char p[200];
	string tmp;
	int i=0;
	
	//loop to store the contents in the file delimited by '\n'
	while(!fin.eof()) {
	    fin.getline(p, 200,'\n');
	    string q(p);
	    q = removesinglelinecomments(p);
	    text += q;
	    text += " ";
	    if(check_regex(q)) {
	    	flag = 1;
	    	break;
		}
	}
	
	fin.close(); //close file
	
	//to add null character to the end of the text and store it in character pointer
	text+='\0';
	text1 = &text[0];
	if(flag == 0) {
		//to remove the \" 's to avoid confusion when removing double quotes
		text = removebackquotes(text);
		
		//to remove all the unwanted spaces and double qoutes
		string texttmp = removeunwanted(text);
	
		char* text2 = &texttmp[0];
		
		//to check for the language
		checklang(text2, text1);
	}
	
	return 0;
}
