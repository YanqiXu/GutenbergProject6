
#ifndef BOOKDIREC_H
#define BOOKDIREC_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <utility>
// Include stemming stuff
#include "english_stem.h"
#include "BookDirec.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <algorithm>
#include <cstdlib>
#include <map>
using namespace std;


struct Pair{
	int bookIndex;
	int position;
};


int fileCount = 0;
int matchCount = 0;
int fileMatchCount = 0;
string delimiters = " ,.;:?'\"()[]";
  map<string,vector <Pair> > refs;
  map<string,bool> stopwords;
  vector<string> bookDirec;
string path = "/home/skon/books";




void ProcessDirectory(string directory, vector<string>& bookDirec);
void ProcessEntity(struct dirent* entity, vector<string>& bookDirec);
void StopWordMap(ifstream& infile, map<string, bool>& stopwords);


bool hasEnding (string const &fullString, string const &ending);
string getNext(string & line);


bool hasEnding (string const &fullString, string const &ending) {
  if (fullString.length() >= ending.length()) {
    return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
  } else {
    return false;
  }
}


void ProcessDirectory(string directory, vector<string>& bookDirec)
{
  string dirToOpen = path + directory;
  DIR *dir;
  dir = opendir(dirToOpen.c_str());

  //set the new path for the content of the directory
  path = dirToOpen + "/";

  //cout << "Process directory: " << dirToOpen.c_str() << endl;

  if(NULL == dir)
    {
      cout << "could not open directory: " << dirToOpen.c_str() << endl;
      return;
    }
  struct dirent *entity;
  entity = readdir(dir);

  while(entity != NULL)
    {
      ProcessEntity(entity,bookDirec);
      entity = readdir(dir);
    }

  //we finished with the directory so remove it from the path
  path.resize(path.length() - 1 - directory.length());
  closedir(dir);
}

void ProcessEntity(struct dirent* entity, vector<string>& bookDirec)
{
  //find entity type
  if(entity->d_type == DT_DIR)
    {//it's an direcotry
      //don't process the  '..' and the '.' directories
      if(entity->d_name[0] == '.')
		{
	  		return;
		}
      //it's an directory so process it
      ProcessDirectory(string(entity->d_name),bookDirec);
      return;
    }

  if(entity->d_type == DT_REG)
    {//regular file
      string fileWithPath = path+entity->d_name;
      bookDirec.push_back(fileWithPath.c_str());
      
      return;
    }

  //there are some other types
  //read here http://linux.die.net/man/3/readdir
  cout << "Not a file or directory: " << entity->d_name << endl;
}

void StopWordMap(ifstream& infile, map<string, bool> &stopwords)
{
    infile.open("/home/students/xuy/project6/stopwords.txt");
    //if(!infile.fail())
    //{  cout<< "open SUCCESS!"<<endl;}
    string word="";
    while(!infile.eof())
    {
       infile>>word;
       stopwords[word]=0;
       //cout<< word<<stopwords.at(word)<<endl;
    }

}

/*void ProcessFile(string file, string word)
{
  string fileType = ".txt";
  if (hasEnding(file,fileType)) {
      fileCount++;
      if (word.length()>0) {
	int matches = stringMatchCount(fionle,word);
	if (matches > 0) {
	  fileMatchCount++;
	  matchCount += matches;
	  cout << "   " << path << file;
	  cout << " " << matches << endl;
	}
      }
  }
  //if you want to do something with the file add your code here
}*/

string getNext(string & line) {
  string next;
  //cout << "$" << line.length();
  size_t start = line.find_first_not_of(delimiters);
  if (start != string::npos) {
    //cout << ":" << start;
    size_t end = line.find_first_of(delimiters,start);
    if (end != string::npos) {
      //cout << "#" << end;
      // word with delimiters on both sides
      next = line.substr(start,end-start);
      line.erase(0,end+1);
    } else {
      // word with delimiter only at start
      next = line.substr(start);
      line = "";
    }
  } else {
    // not delimiters found at all
    next = line;
    line = "";
  }
  //cout << "%" << next << "&" << line << endl;
  return next;;
}

#endif