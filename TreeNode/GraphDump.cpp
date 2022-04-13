#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <kms/GraphDump.h>

#include "../Tree.h"

using namespace Tree;

void Node::graphDump (Graph* graph, void (*dataDump) (FILE* file, const void* data), Error err,  size_t counter)
{
    assert (graph);
    assert (dataDump);

    static GraphNode node;
    static GraphCluster cluster;
    static GraphEdge edge;
    
    if (counter == 0)
    {
        err = notOK();

        node = {};
        node.rounded = true;
        node.shape = RECORD_SHAPE;
        node.color = 0xffffff;

        cluster = {};
        cluster.rounded = true;
        cluster.id = &err;
        cluster.color = (err.getCode() != 0)? 0xff9090 : 0x90ff90;

        edge = {};
        edge.minLen = 3;
    }


    node.id = this;
    cluster.id = this;

    GraphBeginCluster (graph, &cluster,
            "Tree node<br align=\"left\"/>"
            "%p",
            this);

    GraphAddNode (graph, &node, "{");

    long pos = ftell (graph -> file);

    dataDump (graph -> file, data_);

    pos = ftell (graph -> file) - pos;

    if (pos == 0)
        fprintf (graph -> file, "<data>data");

    fprintf (graph -> file,
        "|{<left>left: %p|"
        "<right>right: %p}}",
        left_,
        right_);

    if (pos == 0 && data_ != nullptr)
    {
        GraphAddImage (graph, data_);
        GraphAddEdge (graph, &edge, node.id, ":data", data_, ":n", 0, "");
    }

    GraphEndCluster (graph);

    if (counter < root_ -> getSize())
    {

        if (left_ != nullptr)
        {
            GraphAddEdge (graph, &edge, this, ":left", left_, ":ne", 1, "");
            left_ -> graphDump (graph, dataDump, err, counter + 1);
        }

        if (right_ != nullptr)
        {
            GraphAddEdge (graph, &edge, this, ":right", right_, ":nw", 1, "");
            right_ -> graphDump (graph, dataDump, err, counter + 1);
        }
    }
}

Error Node::graphDump (const char* fileName, void (*dataDump) (FILE* file, const void* data))
{
    assert (dataDump);


    Error err = notOK();

    Graph* graph = GraphOpen ();

    GraphNode node = {};
    node.rounded = 1;
    node.shape = RECORD_SHAPE;
    node.color = 0xffffff;

    GraphCluster cluster = {};

    cluster.rounded = true;
    cluster.id = root_;

    cluster.color = (err.getCode() != 0)? 0xff9090 : 0x90ff90;


    GraphEdge edge = {};

   
    GraphBeginCluster (graph, &cluster,
            "<table align=\"left\" bgcolor=\"invis\" color=\"invis\">"
                "<tr><td>Tree root</td></tr>"
                "<tr><td>%p</td></tr>"
                "<tr><td style=\"rounded\" bgcolor=\"white\"><font color=\"%s\"><b>",
            root_,
            (err.getCode() == 0) ? "green" : "red");
    err.print (graph -> file);
    fprintf (graph -> file, "</b></font></td></tr>"
            "</table>");

    if (root_ != nullptr)
    {
        
        node.id = root_;
        GraphAddNode (graph, &node,
                    "{<RootNode>RootNode: %p|"
                    "Size: %lu|"
                    "DataSize: %lu|}",
                    root_ -> getRootNode(),
                    root_ -> getSize(),
                    root_ -> getDataSize());


                    GraphEndCluster (graph);

                    if (root_ -> getRootNode() == this)
                        GraphAddEdge (graph, &edge, node.id, ":RootNode", root_ -> getRootNode(), "", 1, "");
    }
    else
    {
        GraphAddNode (graph, &node, "%p", nullptr);

        GraphEndCluster (graph);
    }

    graphDump (graph, dataDump, err, 0);


    if (fileName == nullptr)
    {
        char* pointerName = (char*) calloc (7 + sizeof (void*) * 2, sizeof (char));
        sprintf (pointerName, "%p.svg", this);
        
        GraphDraw (graph, pointerName, "svg");

        free (pointerName);
    }
    else
        GraphDraw (graph, fileName, "svg");

    return err;
}
