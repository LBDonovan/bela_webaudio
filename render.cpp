#include <Bela.h>
#include <OSCServer.h>
#include <OSCClient.h>
#include <AudioNode.h>
#include <AudioNodes.h>

#define OSC_RECIEVE 3427
#define OSC_SEND 3426

OSCServer oscServer;
OSCClient oscClient;

AudioSourceNode source(0);
AudioDestinationNode destination(1);
GainNode gain(2, 10.0);

std::vector<AudioNode*> nodes;

// parse messages received by OSC Server
void parseMessage(oscpkt::Message msg){
    
    rt_printf("received message to: %s\n", msg.addressPattern().c_str());
    
    int intArg;
    float floatArg;
    if (msg.match("/osc-test").popInt32(intArg).popFloat(floatArg).isOkNoMoreArgs()){
        rt_printf("received int %i and float %f\n", intArg, floatArg);
    }
    
}

bool setup(BelaContext *context, void *userData)
{
    oscServer.setup(OSC_RECIEVE);
    oscClient.setup(OSC_SEND);
    
    nodes.reserve(4096);
    
    nodes.push_back(&destination);
    nodes.push_back(&gain);
    
    rt_printf("%i\n", nodes.size());
    
    source.connectTo(0, &gain, 0);
    gain.connectTo(0, &destination, 0);
    // source.connectTo(0, &destination, 0);
    
    // the following code sends an OSC message to address /osc-setup
    // then waits 1 second for a reply on /osc-setup-reply
    /*bool handshakeReceived = false;
    oscClient.sendMessageNow(oscClient.newMessage.to("/osc-setup").end());
    oscServer.receiveMessageNow(1000);
    while (oscServer.messageWaiting()){
        if (oscServer.popMessage().match("/osc-setup-reply")){
            handshakeReceived = true;
        }
    }
    
    if (handshakeReceived){
        rt_printf("handshake received!\n");
    } else {
        rt_printf("timeout!\n");
    }*/
    
	return true;
}

bool done = false;
void render(BelaContext *context, void *userData)
{
    // receive OSC messages, parse them, and send back an acknowledgment
    /*while (oscServer.messageWaiting()){
        parseMessage(oscServer.popMessage());
    }*/
    
    if (!done){
    	//done = true;
    	source.receiveInterleavedInput(context);
	    destination.setInterleavedOutput(context);
	    // gain.resetInputs();
	    // destination.resetInputs();
    }
    
    for (auto node : nodes){
    	node->resetInputs();
    }
}

void cleanup(BelaContext *context, void *userData)
{

}
