#include <iostream>
#include <cstdio>
#include <algorithm>
#include <queue>
#include <fstream>
#include <cstring>
using namespace std;

#define MAX_FILE_SIZE 0x3f3f3f
#define MAX_NODE_SIZE 300

char *fbuf = new char[MAX_FILE_SIZE + 1];
char *ext = new char[MAX_FILE_SIZE + 1];
char filename[30];

struct HufNode
{
    int weight, parents, lchild, rchild;
    char elem;
    HufNode(){};
}HUFTREE[MAX_NODE_SIZE];

struct NodeGreat{
    bool operator()(int a, int b){
        return HUFTREE[a].weight > HUFTREE[b].weight;
    }
};



//creat the tree with code list, 
void Restore_Tree(char node, char* code, int &nodePos){
            //the tree root;
    int NODE = 0;
    for (int i = 0; code[i] != '\0' ; ++i)
    {
        if(code[i] == '1'){
            if(HUFTREE[NODE].rchild == 0){
                HUFTREE[NODE].rchild = nodePos++;
            }
            NODE = HUFTREE[NODE].rchild;
        }
        else{
            if(HUFTREE[NODE].lchild == 0){
                HUFTREE[NODE].lchild = nodePos++;
            }
            NODE = HUFTREE[NODE].lchild;
        }
    //  cout << NODE;
    }
    //cout << "ss" <<NODE << node << endl;
    HUFTREE[NODE].elem = node;
}


void Extract(int zero, long flen, char *fbuf){
            //set file pos, NODE, end 0 count
    long long filePos = 0;
    int NODE = 0;
    int end = 0;
    for(int i = 0; i < flen; i++)
    {   
                //temp for temp file element
        char temp = fbuf[i];
                //in case 0
        if(i == flen - 1)
            end = zero;
                //analyse file element 
        for(int j = 7; j >= end; j--)
        {   
                    //& operator, return 1 or 0
            if((1 << j) & temp)
                        //if 1
                NODE = HUFTREE[NODE].rchild;
            else
                        // if 0
                NODE = HUFTREE[NODE].lchild;
                    // when it is the leaf
            if(HUFTREE[NODE].rchild == 0 && HUFTREE[NODE].lchild == 0)
            {   

                ext[filePos++] = HUFTREE[NODE].elem;
                cout <<NODE << endl;
                NODE = 0;
            }
        }
    }

    cout<< "len::" << filePos <<endl;
    ofstream out(filename, ios::out);
    out.write(ext, sizeof(char)*(filePos));
    out.close();
}




int main(int argc, char *argv[]){
    argv[1] = "haha.txt.temp.txt";
            //copy filename
/*********************%s.temp************************************/
    sprintf(filename, "%s.out.txt", argv[1]);
/*********************%s.temp************************************/
            //open compress file
    ifstream fin(argv[1], ios::in);
    if(!fin.good())
        throw "open file error";
            //get node count, node and code;
    int count;
    char *code = new char[count + 1];
    char *node = new char[5];
            //get count
    fin >> count;
    fin.getline(node, count);
            //node pos
    int nodePos = 1;
            //each loop get a node and a code
    for (int i = 1; i < count; ++i)
    {
        fin.getline(node, count);
                //if node is blank, get code again
        if(node[0] == '\0'){
            fin.getline(node, count);
            node[0] = '\n';
        }
        fin.getline(code, count);
                //creat a tree with list code
        Restore_Tree(node[0], code, nodePos);
                //reset code ????
        node[0] = 0;
    }

            //the zero count, temp file length;
    int zero;
    long long flen;
    char* tbuf = new char[flen + 1];
            //get zero count, get temp file length
    fin>>zero; 
    fin>>flen;
    cout << flen << endl;
            //get temp file content
    fin.getline(tbuf, count);
            //copy temp file content to file buffer 
    fin.read(fbuf,sizeof(char)*flen);
            //translate the temp file
    Extract(zero, flen, fbuf);
    return 0;
}
