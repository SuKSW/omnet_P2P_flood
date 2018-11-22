#include <omnetpp.h>
#include <string.h>
#include <list>

using namespace omnetpp;
using std::string;
using std::list;

class Node : public cSimpleModule
{
    private:
        double address;
        list<string> oldMessages = {};
        bool isNewMessage = TRUE;
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Node);

void Node::initialize()
{
    address = par("address");
    if (address == 0.0) {
        for(int i = 0; i<gateSize("g$o"); i++) {
            cMessage *msg = new cMessage("everything");
            msg->addPar("id");
            msg->par("id").setStringValue("end5359req");
            msg->addPar("receiverPublicKey");
            msg->par("receiverPublicKey") = 5;
            sendDelayed(msg, 2, "g$o", i);
        }
        EV << "I'm Public key aaaaaa. Sending an endorsement request to Public Key bbbbbb\n";

        if (hasGUI())
            bubble("Generating endorsement request..");
    }

}

void Node::handleMessage(cMessage *msg)
{
    isNewMessage = TRUE;
    simtime_t arrivalTime = msg->getArrivalTime();
    int inGateIndex = msg->getArrivalGate()->getIndex();
    for (string oldMessage : oldMessages) {
         if(oldMessage == msg->par("id").stringValue())
             isNewMessage = FALSE;
    }
    if(isNewMessage) {
        oldMessages.push_back(msg->par("id").stringValue());
        double receiverPublicKey = msg->par("receiverPublicKey");
        if(receiverPublicKey != address){           //not for me, so forward
            for(int i = 0; i<gateSize("g$o"); i++) {  //broadcasting
                if(i != inGateIndex) {                  //not to the node that sent me
                    send(msg->dup(), "g$o", i);
                }
            }
            delete msg;
        } else {           //this message is for me
            if (hasGUI())
                bubble("Received message");
            if(receiverPublicKey == 5 && strcmp(msg->par("id").stringValue(), "end5359req")==0) {
                for(int i = 0; i<gateSize("g$o"); i++) {
                    cMessage *msg1 = new cMessage("everything");
                    msg1->addPar("id");
                    msg1->par("id").setStringValue("end5y33res");
                    msg1->addPar("receiverPublicKey");
                    msg1->par("receiverPublicKey") = 0;
                    send(msg1, "g$o", i);
                }
                EV << "I'm Public Key bbbbbb. Replying to Public key aaaaaa.. Sent endorsement.\n";
            }
            if(receiverPublicKey == 0 && strcmp(msg->par("id").stringValue(), "end5y33res")==0)
                EV << "I'm Public Key aaaaaa. Received endorsement from aaaaaa.\n";
            delete msg;
        }
    } else {
        delete msg;
    }
}

