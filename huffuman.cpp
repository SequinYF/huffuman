//http://blog.csdn.net/rushkid02/article/details/7687125
//http://blog.csdn.net/tsdl2009/article/details/6514057
//http://blog.csdn.net/ns_code/article/details/20227303
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <queue>
#include <fstream>
#include <cstring>
using namespace std;

#define MAX_FILE_SIZE 0x3f3f3f
#define MAX_NODE_SIZE 300

char *node = new char[MAX_NODE_SIZE + 1];
int *nodeNum = new int[MAX_NODE_SIZE + 1];
int *tbuf = new int[MAX_NODE_SIZE + 1];
char *code[MAX_NODE_SIZE];
char filename[30];

struct HufNode
{
	int weight, parents, lchild, rchild;
	HufNode(){};
}HUFTREE[MAX_NODE_SIZE];

struct NodeGreat{
	bool operator()(int a, int b){
		return HUFTREE[a].weight > HUFTREE[b].weight;
	}
};

//compress
bool Compress(int count){

}

//get the Node code with 0 and 1
void Code_Node(int count){
			//temp path
	char *path = new char[count];
	path[count-1] = '\0';
			//get leaves path in Code
	for (int i = 1; i < count; ++i)
	{
		int child = i;
		int parent = HUFTREE[child].parents;
		int num = count-1;
				//from leaf to root
		while(parent){
					//path wont long as leaves count;
			num--;
			if(HUFTREE[parent].rchild == child){
				path[num] = '0';
			}
			else if(HUFTREE[parent].lchild == child){
				path[num] = '1';
			}
			else{
				throw "code tree error";
			}
			
			child = parent;
			parent = HUFTREE[parent].parents;
		}

				//copy to Code
		code[i] = new char[count - num];
		strcpy(code[i], &path[num]);
	}
}



//build tree
void Creat_Tree(int count){
			
			//priority queues save the index
	priority_queue<int, vector<int>, NodeGreat> N;
	for (int i = 0; i < count; ++i)
	{
		N.push(i);
	}

	int len = count;
	for (int i = 0; i < count - 2; i++)
	{	
				//get the minimum two;
		int lc, rc;
		lc = N.top();
		N.pop();
		rc = N.top();
		N.pop();
				//make node
		HUFTREE[len].weight = HUFTREE[lc].weight + HUFTREE[rc].weight;
		HUFTREE[len].lchild = lc;
		HUFTREE[len].rchild = rc;
		HUFTREE[lc].parents = HUFTREE[rc].parents = len;
		
		N.push(len++);
	}

			//set the root parents;
	int root;
	root = N.top();
	HUFTREE[root].parents = 0;
}


//get tree leaves, return leaves count;
int Creat_Node(char *fbuf, long flen){

			//get tree node value;
	for (int i = 0; i < flen; ++i)
	{
		tbuf[fbuf[i]]++;
	}
			//tree leaves
	int count = 1;
	for (int i = 0; i < flen; ++i)
	{
		if(tbuf[fbuf[i]]){

			HUFTREE[count].weight = tbuf[fbuf[i]];
			HUFTREE[count].lchild = 0;
			HUFTREE[count].rchild = 0;
			HUFTREE[count].parents = 0;
			tbuf[fbuf[i]] = 0;

					//note the node number
			if(!nodeNum[fbuf[i]]){
				nodeNum[fbuf[i]] = count; 
			}
					// note the node
			node[count] = fbuf[i];
			count++;
		}
	}
	return count;
}


int main(int argc, char* argv[]){

			//copy filename
	sprintf(filename, "%s.temp", argv[1]);
	
	ifstream fin;

			// file check, put pointer on the file end
	fin.open(argv[1], ios::ate | ios::in);
	if(!fin.good())
		throw "open file filed";

			//get file length, put pointer on the file begin
	long flen = fin.tellg();
	if(flen > MAX_FILE_SIZE)
		throw "file too big";
	fin.seekg(0, ios::beg);

			//copy file to file buffer
	char *fbuf = new char[flen + 1];
	fin.read(fbuf, flen);

			//file operater done;
	cout << "FILE: " << argv[1] << "  LENHTH: " << flen << "Btye" << endl;


			//creaat huffuman Node, get node count
	int count = 0;
	count = Creat_Node(fbuf, flen);
			// build tree
	Creat_Tree(count);
			// get the node code
	Code_Node(count);
			//compress the file
	Compress(count);
}