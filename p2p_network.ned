// taken from neddemo sample
simple GeneratedNetworkNode extends Node
{
    parameters:
        string device;
    gates:
        inout g[];
}

network GeneratedNetwork
{
    parameters:
        int n @prompt("Number of nodes");
        double connectedness = default(0.15); // 0.0<x<1.0
        double change = default(0.8);
    submodules:
        node[n]: GeneratedNetworkNode{
            @display("i=device/$device");
        	device = uniform(0,1)<change ? "laptop" : "pocketpc";
        }
    connections:
        for i=0..n-1, for j=0..n-1 {
            node[i].g++ <--> node[j].g++ if i!=j && uniform(0,1)<connectedness;
        }
}
