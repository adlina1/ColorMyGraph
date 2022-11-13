# ColorMyGraph

## Description
The aim of this program is to construct an interconnected processes graph / network. Two processes **u** and **v** are said interconnected if they are neighbors in the graph, meaning they share a common edge *(u,v)*. The processes represent the nodes / vertices in the graph. By interconnected, we mean that there must be a communication channel between them. To do so, we are given a description of a graph in the DIMACS[^1] format. It contains any graph with 500 $\leq$ N $\leq$ 4000, with N the number of nodes. These graphs can be found [there](http://cedric.cnam.fr/~porumbed/graphs/) or directly downloaded in our Graphs repository.

## Choices of implementation
The best way for us to implement this problem has been to choose a Peer to Peer model, where each node is both client and server.

## Technologies used
* Sockets
* Threads
* Multiplexing

## How to use it


## Contributors
* Adrien Linares <br>
* Amirhossein Nasri

[^1]: http://lcs.ios.ac.cn/~caisw/Resource/about_DIMACS_graph_format.txt
