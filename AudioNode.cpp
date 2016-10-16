/***** AudioNode.cpp *****/
#include <AudioNode.h>
#include <Bela.h>

AudioNode::AudioNode(int inputs, int outputs, int channelCount, int channelCountMode):
	inputConnections(0),
	inputConnectionsReceived(0)
	/*channelCount(channelCount),
	channelCountMode(channelCountMode)*/
{
	context.numInputs = inputs;
	context.numOutputs = outputs;
	context.numChannels = channelCount;
	context.numAudioFrames = 16;
	context.sampleRate = 44100;

	createBuffers();
};

void AudioNode::createBuffers(){
	inputBufferLength = context.numInputs * context.numChannels * context.numAudioFrames;
	int outputBufferLength = context.numOutputs * context.numChannels * context.numAudioFrames;
	
	context.inputBuffer = new float[inputBufferLength];
	context.outputBuffer = new float[outputBufferLength];
	
	for (int i=0; i<inputBufferLength; i++){
		context.inputBuffer[i] = 0.0f;
	}
	for (int i=0; i<outputBufferLength; i++){
		context.outputBuffer[i] = 0.0f;
	}
	
	inputWidth = context.numChannels * context.numAudioFrames;
}

void AudioNode::connectTo(int output, AudioNode* node, int input){
	OutputConnection connection;
	connection.output = output;
	connection.node = node;
	connection.input = input;
	outputConnections.push_back(connection);
	
	node->addInputConnection();
}

void AudioNode::addInputConnection(){
	inputConnections += 1;
}

void AudioNode::receiveInput(int input, AudioNodeContext* sourceContext){
	//rt_printf("receiveInput %i\n", input);
	// assuming they have the same number of channels
	//rt_printf("before: %f to %f\n", context.inputBuffer[i + input*inputWidth], sourceContext->outputBuffer[i + input*inputWidth]);
	for (int i=0; i<inputWidth; i++){
		context.inputBuffer[i + input*inputWidth] += sourceContext->outputBuffer[i + input*inputWidth];
	}
	
	inputConnectionsReceived += 1;
	if (inputConnectionsReceived >= inputConnections){
		process();
	}
	
}

void AudioNode::process(){
	//rt_printf("process\n");
	render(&context);
	
	for (auto connection : outputConnections){
		connection.node->receiveInput(connection.input, &context);
	}
	
	// reset input buffer
	for (int i=0; i<inputBufferLength; i++){
		//context.inputBuffer[i] = 0.0f;
	}
	
	inputConnectionsReceived = 0;
}

void AudioNode::resetInputs(){
	// reset input buffer
	for (int i=0; i<inputBufferLength; i++){
		context.inputBuffer[i] = 0.0f;
	}
}

