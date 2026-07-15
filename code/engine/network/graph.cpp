#include "graph.h"

struct _Graph_Connection* Create_Graph_Connection()
{
struct _Graph_Connection* I = (struct _Graph_Connection*) calloc(1,sizeof (struct _Graph_Connection));
    return I;
}

struct _Graph_Node* Create_Graph_Node()
{
struct _Graph_Node* I = (struct _Graph_Node*) calloc(1,sizeof (struct _Graph_Node));
    return I;
}

struct _Graph_Spark* Create_Graph_Spark()
{
struct _Graph_Spark* I = (struct _Graph_Spark*) calloc(1,sizeof (struct _Graph_Spark));
    return I;
}

struct _Graph* Create_Graph()
{
struct _Graph* I = (struct _Graph*) calloc(1,sizeof (struct _Graph));
    return I;
}


void Initialize_Graph_Connection(struct _Engine* Engine,unsigned short Node_IDs[2], double Strength_And_Direction)
{

}

void Initialize_Graph_Node(struct _Engine* Engine,unsigned int Category,unsigned int Subcategory,unsigned long ID,double Node_Activation_Strength)
{

}

void Initialize_Graph_Spark(struct _Engine* Engine,unsigned short Parent_Graph_ID,unsigned short Current_Node,unsigned short Current_Connection,unsigned short Maximum_Travel_Steps,unsigned short Current_Travel_Step,bool Destination_Reached)
{

}

void Initialize_Graph(struct _Engine* Engine, unsigned short Number_Of_Nodes, unsigned short Number_Of_Connections)
{

}


void Process_Graph_Connection(struct _Engine* Engine, unsigned short Parent_ID)
{

}

void Process_Graph_Node(struct _Engine* Engine, unsigned short Parent_ID)
{

}

void Process_Graph_Spark(struct _Engine* Engine, unsigned short Parent_ID)
{

}

void Process_Graph(struct _Engine* Engine, unsigned short Parent_ID)
{

}

