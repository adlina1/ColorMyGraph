# ColorMyGraph

## Description
This project contains various steps: 

1. The aim of this program is to construct an interconnected processes graph / network. Two processes **u** and **v** are said interconnected if they are neighbors in the graph, meaning they share a common edge *(u,v)*. The processes represent the nodes / vertices in the graph. By interconnected, we mean that there must be a communication channel between them. To do so, we are given a description of a graph in the DIMACS[^1] format. It contains any graph with 500 $\leq$ N $\leq$ 4000, with N the number of nodes. These graphs can be found [there](http://cedric.cnam.fr/~porumbed/graphs/) or directly downloaded in our Graphs repository. <br> <br>
2. To produce a communication channel, an interconnection, between two given nodes in the graph, we have used sockets, this way, they will be able to receive or send messages to each other, thanks to the P2P model, these two nodes will be client and server at the same time. <br> <br>
3. This ultimate step consists of solving the problem of Graph coloring[^2] with a distributed algorithm integrated in our program, and find the best chromatic number $\chi(G)$[^3] on a given instance/graph.


## Choices of implementation
The best way for us to implement this problem has been to choose a Peer to Peer model, where each node is both client and server. This way, at any time, if two processes are neighbors then they will be able to communicate to each other. Another solution, this time thought with the client server model, would have been to designate a certain number of servers between all processes of the graph and make the other ones that are their neigbors, clients, and connect to them. The main inconvenient in this solution is that the communication channel would have been unidirectional, i.e at any time, both neighbors sites can't initiate the communication but only the client. In other words, the processes designated as servers would not be able to communicate without the client first sending a message/request. That's why the P2P model have been more suitable for this kind of problem.

## Technologies used
* Sockets
* Threads
* Multiplexing

## How to use it
1. Compile it with: <br>
``` make ```
2. Execute giving the graph as the unique argument of the program. Example : <br>
``` ./eserver_central "Graphs/dsjc.250.5.col" ``` <br> 
3. Execute the nodes giving    as the unique argument of the program. Example : <br>
``` ./enode "your_ip" ``` <br> 


## Contributors
* Adrien Linares <br>
* Amirhossein Nasri

[^1]: http://lcs.ios.ac.cn/~caisw/Resource/about_DIMACS_graph_format.txt
[^2]: https://en.wikipedia.org/wiki/Graph_coloring
[^3]: https://www.sciencedirect.com/topics/mathematics/chromatic-number
