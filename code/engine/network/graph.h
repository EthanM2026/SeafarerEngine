#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
struct _Graph_Connection
{
    unsigned short Node_IDs[2];
    double Strength_And_Direction;
};

struct _Graph_Node
{
    unsigned long Category;
    unsigned long Subcategory;
    unsigned long ID;
    double Node_Activation_Strength;
};

struct _Graph_Spark
{
    unsigned short Parent_Graph_ID;
    unsigned short Current_Node;
    unsigned short Current_Connection;
    unsigned short Maximum_Travel_Steps;
    unsigned short Current_Travel_Step;
    bool Destination_Reached;
};

struct _Graph
{
    unsigned short Number_Of_Nodes;
    unsigned short Number_Of_Connections;
    struct _Graph_Node* Nodes[65536];
    struct _Graph_Connection* Connections[65536];
};

struct _Graph_Connection* Create_Graph_Connection();
struct _Graph_Node* Create_Graph_Node();
struct _Graph_Spark* Create_Graph_Spark();
struct _Graph* Create_Graph();

void Initialize_Graph_Connection(struct _Engine* Engine,unsigned short Node_IDs[2], double Strength_And_Direction);
void Initialize_Graph_Node(struct _Engine* Engine,unsigned int Category,unsigned int Subcategory,unsigned long ID,double Node_Activation_Strength);
void Initialize_Graph_Spark(struct _Engine* Engine,unsigned short Parent_Graph_ID,unsigned short Current_Node,unsigned short Current_Connection,unsigned short Maximum_Travel_Steps,unsigned short Current_Travel_Step,bool Destination_Reached);
void Initialize_Graph(struct _Engine* Engine, unsigned short Number_Of_Nodes, unsigned short Number_Of_Connections);

void Process_Graph_Connection(struct _Engine* Engine, unsigned short Parent_ID);
void Process_Graph_Node(struct _Engine* Engine, unsigned short Parent_ID);
void Process_Graph_Spark(struct _Engine* Engine, unsigned short Parent_ID);
void Process_Graph(struct _Engine* Engine, unsigned short Parent_ID);

#endif // GRAPH_H_INCLUDED
