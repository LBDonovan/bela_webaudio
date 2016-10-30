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

std::vector<AudioNode*> nodes;

// parse messages received by OSC Server
void parseMessage(oscpkt::Message msg){
    
    printf("received message to: %s\n", msg.addressPattern().c_str());
    
    int intArgs[4];
    float floatArgs[4];
    
    if (msg.match("/connect-nodes")
    	.popInt32(intArgs[0])
    	.popInt32(intArgs[1])
    	.popInt32(intArgs[2])
    	.popInt32(intArgs[3])
    	.isOkNoMoreArgs())
	{
		
        printf("connecting nodes %i %i (%i)\n", intArgs[0], intArgs[1], nodes[intArgs[1]]->getID());
        nodes[intArgs[0]]->connectTo(nodes[intArgs[1]], intArgs[2], intArgs[3]);
        
    } else if (msg.partialMatch("/create-node/")){
		
    	if (msg.match("/create-node/GainNode")
	    	.popInt32(intArgs[0])
	    	.popInt32(intArgs[1])
	    	.isOkNoMoreArgs())
		{
			
			printf("creating gain node %i\n", intArgs[0]);
			if (intArgs[0] != nodes.size()){
				fprintf(stderr, "can't create GainNode, wrong ID\n");
				return;
			}
			nodes.push_back(new GainNode(intArgs[0]));

		} else if (msg.match("/create-node/ChannelSplitterNode")
			.popInt32(intArgs[0])
			.popInt32(intArgs[1])
			.isOkNoMoreArgs())
		{
			
			printf("creating channel splitter node %i %i\n", intArgs[0], intArgs[1]);
			if (intArgs[0] != nodes.size()){
				fprintf(stderr, "can't create ChannelSplitterNode, wrong ID\n");
				return;
			}
			nodes.push_back(new ChannelSplitterNode(intArgs[0], intArgs[1]));
	        
	    } else if (msg.match("/create-node/ChannelMergerNode")
	    	.popInt32(intArgs[0])
	    	.popInt32(intArgs[1])
	    	.isOkNoMoreArgs())
		{
			
	        printf("creating channel merger node %i %i\n", intArgs[0], intArgs[1]);
	        if (intArgs[0] != nodes.size()){
	        	fprintf(stderr, "can't create ChannelMergerNode, wrong ID\n");
	        	return;
	        }
	        nodes.push_back(new ChannelMergerNode(intArgs[0], intArgs[1]));
	        
	    } 
	    
    } else if (msg.match("/create-param")
    	.popInt32(intArgs[0])
    	.popInt32(intArgs[1])
    	.popFloat(floatArgs[0])
    	.isOkNoMoreArgs())
    {
    	printf("creating audio param %i of node %i with value %f\n", intArgs[0], intArgs[1], floatArgs[0]);
        if (intArgs[0] != nodes.size()){
        	fprintf(stderr, "can't create param, wrong ID\n");
        	return;
        }
        nodes.push_back(new AudioParam(intArgs[0], nodes[intArgs[1]], floatArgs[0]));
        
    } else if (msg.match("/set-param")
    	.popInt32(intArgs[0])
    	.popFloat(floatArgs[0])
    	.isOkNoMoreArgs())
    {
    	printf("setting param %i to value %f\n", intArgs[0], floatArgs[0]);
    	nodes[intArgs[0]]->setValue(floatArgs[0]);
    }

}

bool setup(BelaContext *context, void *userData)
{
    oscServer.setup(OSC_RECIEVE);
    oscClient.setup(OSC_SEND);
    
    nodes.reserve(4096);
    
    nodes.push_back(&source);
    nodes.push_back(&destination);
    
    // the following code sends an OSC message to address /osc-setup
    // then waits 1 second for a reply on /osc-setup-reply
    bool handshakeReceived = false;
    oscClient.sendMessageNow(oscClient.newMessage.to("/osc-setup").end());
    oscServer.receiveMessageNow(1000);
    while (oscServer.messageWaiting()){
    	oscpkt::Message msg = oscServer.popMessage();
        if (msg.match("/osc-setup-reply")){
            handshakeReceived = true;
        } else {
        	parseMessage(msg);
        }
    }
    
    if (handshakeReceived){
        printf("setup complete!\n");
    } else {
        printf("timeout!\n");
        return false;
    }
    
	return true;
}

bool done = false;
int count = 0;
void render(BelaContext *context, void *userData)
{
    // receive OSC messages, parse them, and send back an acknowledgment
    while (oscServer.messageWaiting()){
        parseMessage(oscServer.popMessage());
    }

	// send a timestamp once a second    
    for (int n=0; n<context->audioFrames; n++){
	    if (!(count++%44100)){
	    	oscClient.queueMessage(oscClient.newMessage.to("/osc-timestamp").add(count).end());
	    }
    }
    
    if (!done){
    	//done = true;
    	source.receiveInterleavedInput(context);
	    destination.setInterleavedOutput(context);
    }
    
    for (auto node : nodes){
    	node->resetInputs();
    }
}

void cleanup(BelaContext *context, void *userData)
{

}
