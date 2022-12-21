#include<bits/stdc++.h>
#include<windows.h>
#include<vector>
#include<string.h>
#include <string>

using namespace std;

//max num of characters a node can store
#define len 10

vector<string> undo;
vector<string> redo;
struct ropeNode
{
    ropeNode* lc;
    ropeNode* rc;
    ropeNode* par;
    char *str;
    int ct;
};

ropeNode* root = NULL;
ropeNode* root1=NULL;
ropeNode* root2=NULL;

ropeNode* create(ropeNode *&node,ropeNode* p,char s[],int Lindex,int Rindex)
{
    ropeNode* temp=new ropeNode();
    temp->lc=temp->rc=NULL;
    temp->par=p;

    //if the string size is more than the max size ...split it
    //split into halves and store them in seperate nodes
    if(Rindex-Lindex > len)
    {
        temp->str = NULL;
        temp->ct = (Rindex - Lindex)/2;
        node = temp;
        int m = Lindex+(Rindex - Lindex)/2;
        create(node->lc, node, s, Lindex, m-1);
        create(node->rc, node, s, m, Rindex);
    }

    //if the string size is less than max size
    else
    {
        node = temp;
        temp->ct = (Rindex-Lindex);
        int size = 0;
        temp->str = new char[len]();

        for (int i=Lindex; i<=Rindex; i++)
            {
                temp->str[size++] = s[i];
            }
    }
    return node;
}

//get node function
ropeNode* getNode(ropeNode* node)
{
    node = NULL;
    node = new ropeNode();
    return node;
}

void display(ropeNode *node)
{
    if (node==NULL)
        return;
    if (node->lc==NULL && node->rc==NULL && node->str!=NULL)
    {
            cout << node->str;
    }
    display(node->lc);
    display(node->rc);
}

//search for an element at a particular index
char searchNode(ropeNode* node,int index)
{
    //if index>node->ct ... search in the right subtree
    if(index > node->ct && node->rc != NULL)
    {
        return searchNode(node->rc,index-node->ct);
    }

    //if index < node->ct ... search in the left subtree
    if(node->lc != NULL && index <= node->ct)
    {
        return searchNode(node->lc,index);
    }

    int size=strlen(node->str);
    string temp=node->str;
    char tempStr[size];

    //if the node is found ... search for the elt in that index
    for(int i=0;i<size;i++)
    {
        tempStr[i]=temp[i];
    }
    return tempStr[index];

}


//returns the count of the letters in left subtree
int weight(ropeNode* node)
{
    if(node != NULL)
    {
        return 0;
    }
    return node->ct + weight(node->rc);
}


//concatenate two individual rope trees
void concatenate(ropeNode *&rootnew,ropeNode *root1,ropeNode *root2,int n1)
{

    ropeNode *flag=new ropeNode();
    flag->par=NULL;
    flag->lc=root1;
    flag->rc=root2;
    root1->par=root2->par=flag;
    flag->ct=n1;
    flag->str=NULL;
    rootnew=flag;
}

//store the strings in rope trees node to a string
void storeString(ropeNode *node,string &st)
{

    if (node==NULL)
        return;
    if (node->lc==NULL && node->rc==NULL && node->str!=NULL)
    {
        string temp=node->str;
        st.append(temp);
    }
    storeString(node->lc,st);
    storeString(node->rc,st);

}

//split the rope tree at a particular index
ropeNode* split(ropeNode* node,int index)
{
    int i=index;
    ropeNode* cur=node;
    ropeNode* prev=NULL;

    //find the index where split operation should be performed
    while(cur!=NULL)
    {
        prev=cur;
        if(index<cur->ct)
        {
            cur=cur->lc;
        }
        else
        {
            index = index - node->ct;
            cur=cur->rc;
        }
    }

    //get the string present in the rope data structure
    string dummy="";
    storeString(node,dummy);
    int length=sizeof(dummy);

    //now split the string into two based on the index
    //copy the left side of the string to one char array
    char temp1[index];
    for(int i=0;i<index;i++)
    {
        temp1[i]=dummy[i];
    }

    //copy the right side of the string to one char array
    char temp2[length-index];
    int k=0;
    for(int i=index;i<length;i++)
    {
        temp2[k]=dummy[i];
        k++;
    }
    int size2 = sizeof(temp2)/sizeof(temp2[0]);
    int size1 = sizeof(temp1)/sizeof(temp1[0]);
    ropeNode* r1;
    ropeNode* r2;

    //create 2 individual rope trees with the splitted strings and concatenate them
    create(r1, NULL, temp1, 0, size1);
    create(r2, NULL, temp2, 0, size2);
    concatenate(node,r1,r2,size1);
    return node;
}

//insert a string at a particular index
ropeNode* insertStr(ropeNode* node,char st[],int index)
{
    int i=index;
    ropeNode* cur=node;
    ropeNode* prev=NULL;

     //find the index where insert operation should be performed
    while(cur!=NULL)
    {
        prev=cur;
        if(index<cur->ct)
        {
            cur=cur->lc;
        }
        else
        {
            index = index - node->ct;
            cur=cur->rc;
        }
    }

    //get the string present in the rope data structure
    string dummy="";
    storeString(node,dummy);
    int length=sizeof(dummy);

    //now split the string into two based on the index
    //copy the left side of the string to one char array
    char temp1[index];
    for(int i=0;i<index;i++)
    {
        temp1[i]=dummy[i];
    }

    //copy the right side of the string to one char array
    char temp2[length-index];
    int k=0;
    for(int i=index;i<length;i++)
    {
        temp2[k]=dummy[i];
        k++;
    }

    //create 3 individual rope trees ...2 for splitted strings and 1 for the string to be inserted
    int size2 = sizeof(temp2)/sizeof(temp2[0]);
    int size1 = sizeof(temp1)/sizeof(temp1[0]);
    int size3 = sizeof(st);
    ropeNode* r1;//left tree
    ropeNode* r2;//ryt tree
    ropeNode* r3;//to be inserted

    create(r1, NULL, temp1, 0, size1-1);
    create(r2, NULL, temp2, 0, size2);
    create(r3, NULL, st, 0, size3);
    ropeNode* newRoot=NULL;

    //first concatenate the string to be inserted with left tree and thn the merged tree with right tree
    concatenate(node,r1,r3,size1);
    concatenate(root,node,r2,size1+size3);
    return root;

}

ropeNode* deletion(ropeNode* node,int index,int num)
{
    int i=index;
    ropeNode* cur=node;
    ropeNode* prev=NULL;

    //find the index where insert operation should be performed
    while(cur!=NULL)
    {
        prev=cur;
        if(index<cur->ct)
        {
            cur=cur->lc;
        }
        else
        {
            index = index - node->ct;
            cur=cur->rc;
        }
    }

    //get the string present in the rope data structure
    string dummy="";
    storeString(node,dummy);
    int length=sizeof(dummy);

    //now split the string into two based on the index
    //copy the left side of the string to one char array
    char temp1[index];
    for(int i=0;i<index;i++)
    {
        temp1[i]=dummy[i];
    }

    //copy the right side of the string leaving the num characters after index to one char array
    char temp2[length-(index+num)];
    int k=0;
    for(int i=index+num;i<length;i++)
    {
        temp2[k]=dummy[i];
        k++;
    }
    int size2 = sizeof(temp2)/sizeof(temp2[0]);
    int size1 = sizeof(temp1)/sizeof(temp1[0]);
    ropeNode* r1;//left tree
    ropeNode* r2;//ryt tree

    //create 2 individual rope trees with the splitted strings and concatenate them
    create(r1, NULL, temp1, 0, size1-1);
    create(r2, NULL, temp2, 0, size2);
    concatenate(node,r1,r2,size1);
    root=node;
    return root;

}

ropeNode* cut(ropeNode* node,int index,int num,int nump)
{
     int i=index;
    ropeNode* cur=node;
    ropeNode* prev=NULL;

    //find the index where insert operation should be performed
    while(cur!=NULL)
    {
        prev=cur;
        if(index<cur->ct)
        {
            cur=cur->lc;
        }
        else
        {
            index = index - node->ct;
            cur=cur->rc;
        }
    }

    //get the string present in the rope data structure
    string dummy="";
    storeString(node,dummy);
    int length=sizeof(dummy);

    //now split the string into two based on the index
    //copy the left side of the string to one char array
    char temp1[index];
    for(int i=0;i<index;i++)
    {
        temp1[i]=dummy[i];
    }

    //copy the right side of the string leaving the num characters after index to one char array
    char temp2[length-(index+num)];
    int k=0;
    for(int i=index+num;i<length;i++)
    {
        temp2[k]=dummy[i];
        k++;
    }

    //copy the string to be cut into one array
    char temp3[num];
    k=0;
    for(int i=index;i<index+num;i++)
    {
        temp3[k]=dummy[i];
        k++;
    }
    int size1 = sizeof(temp1)/sizeof(temp1[0]);
    int size2 = sizeof(temp2)/sizeof(temp2[0]);
    int size3 = sizeof(temp3)/sizeof(temp3[0]);

    ropeNode* r1;//left
    ropeNode* r2;//ryt
    ropeNode* r3;//cut string

    //create 3 individual rope trees ...2 for splitted strings and 1 for the string which is to cut and concatenate them
    create(r1, NULL, temp1, 0, size1-1);
    create(r2, NULL, temp2, 0, size2);
    create(r3, NULL, temp3, 0, num);
    concatenate(node,r1,r2,size1);

    //paste procedure
    i=nump;
    cur=node;
    prev=NULL;

    //find the index where paste operation should be performed
    while(cur!=NULL)
    {
        prev=cur;
        if(nump<cur->ct)
        {
            cur=cur->lc;
        }
        else
        {
            nump = nump - node->ct;
            cur=cur->rc;
        }
    }

    //get the string present in the rope data structure
    dummy="";
    storeString(node,dummy);
    length=sizeof(dummy)/sizeof(dummy[0]);

    //now split the string into two based on the index at which it should be pasted
    //copy the left side of the string to one char array
    char temp4[i];
    for(int x=0;x<i;x++)
    {
        temp4[x]=dummy[x];
    }

    //copy the right side of the string to one char array
    temp2[length-(i)];
    k=0;
    for(int y=i;y<length+1;y++)
    {
        temp2[k]=dummy[y];
        k++;
    }

    ropeNode* r4;
    int size4 = sizeof(temp4)/sizeof(temp4[0]);
    size2 = sizeof(temp2)/sizeof(temp2[0]);

    //create 2 individual rope trees with the splitted strings
    create(r4, NULL, temp4, 0, size4-1);
    create(r2, NULL, temp2, 0, size2+1);
    //concatenate left rope and the tree with copied string
    concatenate(node,r4,r3,size4);
    //concatenate right tree withe merged tree in prev step
    concatenate(root,node,r2,size4+size3+1);
    return root;
}

ropeNode* copyy(ropeNode* node,int index,int num,int numc)
{
     int in=index;
    ropeNode* cur=node;
    ropeNode* prev=NULL;

    //find the index where insert operation should be performed
    while(cur!=NULL)
    {
        prev=cur;
        if(index<cur->ct)
        {
            cur=cur->lc;
        }
        else
        {
            index = index - node->ct;
            cur=cur->rc;
        }
    }

    //get the string present in the rope data structure
    string dummy="";
    storeString(node,dummy);
    int length=sizeof(dummy);

   //copy the string to be pasted
    char temp3[num];
    int k=0;
    for(int i=in;i<in+num;i++)
    {
        temp3[k]=dummy[i];
        k++;
    }


    int size3 = sizeof(temp3)/sizeof(temp3[0]);
    ropeNode* r3;//tree with copied string

    //create a tree with copied string
    create(r3, NULL, temp3, 0, num-1);

    //paste procedure
    int i=numc;
    cur=node;
    prev=NULL;

    //find the index where insert operation should be performed
    while(cur!=NULL)
    {
        prev=cur;
        if(numc<cur->ct)
        {
            cur=cur->lc;
        }
        else
        {
            numc = numc - node->ct;
            cur=cur->rc;
        }
    }

    //get the string present in the rope data structure
    dummy="";
    storeString(node,dummy);
    length=dummy.length();

    char temp4[i];
    for(int y=0;y<i;y++)
    {
        temp4[y]=dummy[y];
    }

    char temp2[length-(i)];
    k=0;
    for(int x=i;x<length+1;x++)
    {
        temp2[k]=dummy[x];
        k++;
    }

    ropeNode* r4;
    ropeNode* r2;
    int size4 = sizeof(temp4)/sizeof(temp4[0]);
    int size2 = sizeof(temp2)/sizeof(temp2[0]);
    create(r4, NULL, temp4, 0, size4-1);
    create(r2, NULL, temp2, 0, size2+1);
    //concatenate the rope with copied string with left tree
    concatenate(node,r4,r3,size4);
    //concatenate the right tree with the merged tree
    concatenate(root,node,r2,size4+size3+1);
    return root;

}

void colour(ropeNode* node,int index,int num)
{
     int i=index;
    ropeNode* cur=node;
    ropeNode* prev=NULL;

    while(cur!=NULL)
    {
        prev=cur;
        if(index<cur->ct)
        {
            cur=cur->lc;
        }
        else
        {
            index = index - node->ct;
            cur=cur->rc;
        }
    }
    string dummy="";
    storeString(node,dummy);
    int length=dummy.length();
    char temp1[index];
    for(int i=0;i<index;i++)
    {
        temp1[i]=dummy[i];
    }
    char temp2[length-(index+num)];
    int k=0;
    for(int i=index+num;i<length;i++)
    {
        temp2[k]=dummy[i];
        k++;
    }
    char temp3[num];
    k=0;
    for(int i=index;i<index+num;i++)
    {
        temp3[k]=dummy[i];
        k++;
    }
    int size1 = sizeof(temp1)/sizeof(temp1[0]);
    int size2 = sizeof(temp2)/sizeof(temp2[0]);
    int size3 = sizeof(temp3)/sizeof(temp3[0]);
    ropeNode* r1;//left
    ropeNode* r2;//ryt
    ropeNode* r3;//cut string
    create(r1, NULL, temp1, 0, size1-1);
    //cout<<"r1"<<endl;

    create(r2, NULL, temp2, 0, size2);
    create(r3, NULL, temp3, 0, num);
    display(r1);
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    display(r3);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    display(r2);

}

ropeNode* undoOperation(ropeNode* node)
{
    int length=undo.size();
    string temp=undo[length-2];
    redo.push_back(undo[length-1]);
    undo.pop_back();
    length=undo.size();
    int size1=temp.size();
    char a[size1];
    for(int i=0;i<size1;i++)
    {
        a[i]=temp[i];
    }

    create(node, NULL, a, 0, size1-1);

    root=node;
    return root;
}

ropeNode* redoOperation(ropeNode* node)
{
    int sizeRedo=redo.size();
    string str=redo[sizeRedo-1];
    int n=str.length();
    char a[n];
    strcpy(a,str.c_str());
    create(node,NULL,a,0,n);
    redo.pop_back();
    root=node;
    return root;
}

int main()
{
    cout<<"Enter 1 to create a string.\n";
    cout<<"Enter 2 to concatenate.\n";
    cout<<"Enter 3 to search for an element at a particular index.\n";
    cout<<"Enter 4 to split the string.\n";
    cout<<"Enter 5 to insert a string at a position.\n";
    cout<<"Enter 6 to delete a string.\n";
    cout<<"Enter 7 to cut and paste a string.\n";
    cout<<"Enter 8 to copy paste a string.\n";
    cout<<"Enter 9 to display.\n";
    cout<<"Enter 10 to undo.\n";
    cout<<"Enter 11 to redo.\n";

    int choice;
    do
    {
        cout<<"\nEnter your choice : ";
        cin>>choice;
        switch(choice)
        {

        case 1:
           {
              cout<<"\nEnter the string : ";
              string temp1,temp2;
              cin.ignore();
            getline(cin,temp1);
            int n = temp1.length();
            char a[n + 1];
            strcpy(a, temp1.c_str());
            root=create(root, NULL, a, 0, n-1);
            storeString(root,temp2);
            undo.push_back(temp2);
            break;

           }
        case 2:
           {
                 cout<<"\nEnter the string : ";
                string temp1;
                cin.ignore();
                getline(cin,temp1);
                int n1 = temp1.length();
                char a1[n1 + 1];
                strcpy(a1, temp1.c_str());
                root1=create(root1, NULL, a1, 0, n1-1);

                string rootstr;
                storeString(root,rootstr);
                n1=rootstr.length();
                concatenate(root,root,root1,n1);
                string temp;
                storeString(root,temp);
                undo.push_back(temp);
                break;
           }

        case 3:

            {
                int index;
                 cout<<"\nEnter the index : ";
                 cin>>index;
                 char temp;
                 temp=searchNode(root,index);
                 cout<<"\nElement at the index "<<index<<" is : "<<temp;
                 break;
            }


        case 4:
             {
                 int index;
                 cout<<"\nEnter the index at which we have to split : ";
                 cin>>index;
                 root=split(root,index);
                 string temp;
                 storeString(root,temp);
                 undo.push_back(temp);
                 break;
             }

        case 5:

            {
                 cout<<"\nEnter the string to be inserted : ";
                 string temp;
                 cin.ignore();
                 getline(cin,temp);
                 int n = temp.length();
                 char a[n + 1];
                 strcpy(a, temp.c_str());
                 cout<<"\nEnter the index at which you have to insert : ";
                 int index;
                 cin>>index;
                 root=insertStr(root,a,index);
                 storeString(root,temp);
                 undo.push_back(temp);
                 break;
            }


        case 6:
             {
                 cout<<"\nEnter the index at which you have to delete : ";
                 int index;
                 cin>>index;
                 cout<<"\nEnter the number of characters to be deleted : ";
                 int num;
                 cin>>num;
                 root=deletion(root,index,num);
                 string temp;
                 storeString(root,temp);
                 undo.push_back(temp);
                 break;

             }
        case 7:

            {
                cout<<"\nEnter the index at which you have to cut : ";
                int index;
                cin>>index;
                cout<<"\nEnter the number of characters to be cut : ";
                int num;
                cin>>num;
                cout<<"\nEnter the index at which you have to paste : ";
                int nump;
                cin>>nump;
                root=cut(root,index,num,nump);
                string temp;
                storeString(root,temp);
                undo.push_back(temp);
                break;
            }


        case 8:
           {
                cout<<"\nEnter the index at which you have to copy : ";
                int index;
                cin>>index;
                cout<<"\nEnter the number of characters to be copied : ";
                int num;
                cin>>num;
                cout<<"\nEnter the index at which you have to paste : ";
                int nump;
                cin>>nump;
                root=copyy(root,index,num,nump);
                string temp;
                storeString(root,temp);
                undo.push_back(temp);
                break;
           }

        case 9:
            {

                cout<<"\n\t\t\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\n";
        cout<<"\t\t\xb0                                               \xb0\n";
        cout<<"\n\t\t\xb0      \xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4       \xb0\n";
        cout<<"\t\t\xb0                 TEXT EDITOR                   \xb0\n";
        cout<<"\t\t\xb0      \xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4       \xb0\n";
        cout<<"\t\t\xb0                                               \xb0\n";
        cout<<"\t\t\t";
        display(root);
        cout<<endl;

        cout<<"\t\t\xb0                                               \xb0\n";
        cout<<"\t\t\xb0                                               \xb0\n";
        cout<<"\n\t\t\xb0      \xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4       \xb0\n";
        cout<<"\t\t\xb0                                               \xb0\n";
        cout<<"\n\t\t\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\xb0\n";
                break;

            }

        case 10:
            {
                root=undoOperation(root);
                break;
            }

        case 11:
            {
                root=redoOperation(root);
                break;
            }


        }
    }while(choice>0 && choice<12);

    return 0;
}
