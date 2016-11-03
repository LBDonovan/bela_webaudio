/***** WebAudioOSC.cpp *****/
#include <WebAudioOSC.h>
#include <Bela.h>
#include <OSCServer.h>
#include <OSCClient.h>
#include <AudioNodes.h>

OSCServer oscServer;
OSCClient oscClient;

extern std::vector<AudioNode*> nodes;
extern std::vector<OscillatorNode*> oscillatorNodes;

int intArgs[4];
float floatArgs[4];

namespace osc {
	
	void setup(){
		oscServer.setup(OSC_RECIEVE);
		oscClient.setup(OSC_SEND);
	}

	// create nodes
	void createNode(oscpkt::Message msg){
		
		if (msg.match("/node/create/GainNode")
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
	
		} 
		else if (msg.match("/node/create/ChannelSplitterNode")
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
	        
	    } 
	    else if (msg.match("/node/create/ChannelMergerNode")
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
	    else if (msg.match("/node/create/OscillatorNode")
	    	.popInt32(intArgs[0])
	    	.popInt32(intArgs[1])
	    	.isOkNoMoreArgs())
		{
			
	        printf("creating oscillator node %i\n", intArgs[0]);
	        if (intArgs[0] != nodes.size()){
	        	fprintf(stderr, "can't create OscillatorNode, wrong ID\n");
	        	return;
	        }
	        OscillatorNode* node = new OscillatorNode(intArgs[0]);
	        oscillatorNodes.push_back(node);
	        nodes.push_back(node);
	        
	    }
	}
	
	// OSC messages about nodes
	void node(oscpkt::Message msg){
		
		if (msg.partialMatch("/node/create/")){
			createNode(msg);
		} 
		else if (msg.match("/node/connect")
	    	.popInt32(intArgs[0])
	    	.popInt32(intArgs[1])
	    	.popInt32(intArgs[2])
	    	.popInt32(intArgs[3])
	    	.isOkNoMoreArgs())
		{
			
	        printf("connecting nodes %i %i (%i)\n", intArgs[0], intArgs[1], nodes[intArgs[1]]->getID());
	        nodes[intArgs[0]]->connectTo(nodes[intArgs[1]], intArgs[2], intArgs[3]);
	        
	    }
	     else if (msg.match("/node/set-oscillator-type")
		    	.popInt32(intArgs[0])
		    	.popInt32(intArgs[1])
		    	.isOkNoMoreArgs())
		{
			printf("setting type of oscillator node %i to %i\n", intArgs[0], intArgs[1]);
	    	nodes[intArgs[0]]->setType(intArgs[1]);
	        
	    } 
	    else if (msg.match("/node/set-oscillator-state")
		    	.popInt32(intArgs[0])
		    	.popInt32(intArgs[1])
		    	.isOkNoMoreArgs())
		{
			printf("setting state of oscillator node %i to %i\n", intArgs[0], intArgs[1]);
	    	nodes[intArgs[0]]->setState(intArgs[1]);
	        
	    }
	}
	
	// OSC messages about AudioParams
	void param(oscpkt::Message msg){
		
		if (msg.match("/param/create")
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
	        
	    } 
	    else if (msg.match("/param/set")
	    	.popInt32(intArgs[0])
	    	.popFloat(floatArgs[0])
	    	.isOkNoMoreArgs())
	    {
	    	printf("setting param %i to value %f\n", intArgs[0], floatArgs[0]);
	    	nodes[intArgs[0]]->setValue(floatArgs[0]);
	    	
	    } 
	    else if (msg.match("/param/event")
		    	.popInt32(intArgs[0])
		    	.popFloat(floatArgs[0])
		    	.popFloat(floatArgs[1])
		    	.isOkNoMoreArgs())
		{
			unsigned long sample = floatArgs[1] * 44100.0;
			printf("setting param %i to value %f at time %f, sample %lu\n", intArgs[0], floatArgs[0], floatArgs[1], sample);
	    	nodes[intArgs[0]]->addEvent(SET_VALUE, floatArgs[0], sample);
	    }
	    
	}
	
	void parseMessage(oscpkt::Message msg){
	    
	    printf("received message to: %s\n", msg.addressPattern().c_str());
	    
	    if (msg.partialMatch("/node/")){
	    	node(msg);
	    } else if (msg.partialMatch("/param/")){
	    	param(msg);
	    } 
	    
	}
	
	bool handshake(){
		bool received = false;
	    oscClient.sendMessageNow(oscClient.newMessage.to("/osc-setup").end());
	    oscServer.receiveMessageNow(1000);
	    while (oscServer.messageWaiting()){
	    	oscpkt::Message msg = oscServer.popMessage();
	        if (msg.match("/osc-setup-reply")){
	            received = true;
	        } else {
	        	parseMessage(msg);
	        }
	    }
	    return received;
	}
	
	void checkMessages(){
		// receive OSC messages and parse them
	    while (oscServer.messageWaiting()){
	        parseMessage(oscServer.popMessage());
	    }
	}
	
	void sendTimestamp(int count){
		oscClient.queueMessage(oscClient.newMessage.to("/osc-timestamp").add(count).end());
	}
	
}