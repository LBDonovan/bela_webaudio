#include <Bela.h>
#include <AudioNodes.h>
#include <WebAudioOSC.h>
// #include <Scope.h>

// Scope scope;

AudioSourceNode source(0);
AudioDestinationNode destination(1);

std::vector<AudioNode*> nodes;
std::vector<OscillatorNode*> oscillatorNodes;

unsigned long gAudioFramesElapsed;


bool setup(BelaContext *context, void *userData)
{
	
    osc::setup();
    
    // scope.setup(1, context->audioSampleRate);
    
    nodes.reserve(4096);
    
    nodes.push_back(&source);
    nodes.push_back(&destination);
    
    // the following code sends an OSC message to address /osc-setup
    // then waits 1 second for a reply on /osc-setup-reply
    bool handshakeReceived = osc::handshake();
    
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
    // check OSC messages
    osc::checkMessages();

	// send a timestamp once a second    
    for (int n=0; n<context->audioFrames; n++){
	    if (!(count++%44100)){
	    	osc::sendTimestamp(count);
	    }
    }
    
    gAudioFramesElapsed = context->audioFramesElapsed + context->audioFrames;
    
    for (auto node : oscillatorNodes){
    	node->processParams();
    }
    
	source.receiveInterleavedInput(context);
    destination.setInterleavedOutput(context);
    
    for (auto node : nodes){
    	node->resetInputs();
    }
    
    // for (int n=0; n<context->audioFrames; n++){
    // 	scope.log(context->audioOut[n * context->audioOutChannels]);
    // }
}

void cleanup(BelaContext *context, void *userData)
{

}
