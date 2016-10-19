/***** AudioNode.cpp *****/
#include <AudioNode.h>
#include <AudioNodes.h>
#include <Bela.h>

AudioNode::AudioNode(int id, int inputs, int outputs, int channelCount, int channelCountMode):
	inputConnections(0),
	inputConnectionsReceived(0),
	ID(id),
	channelCount(channelCount),
	channelCountMode(channelCountMode),
	inputChannels(channelCount),
	outputChannels(channelCount)
{
	context.numInputs = inputs;
	context.numOutputs = outputs;
	context.numChannels = channelCount;
	context.numAudioFrames = 16;
	context.sampleRate = 44100;

	createBuffers();
};

int AudioNode::getInputChannels(){
	return inputChannels;
}
int AudioNode::getOutputChannels(){
	return outputChannels;
}

void AudioNode::changeNumChannels(int numChannels){
	inputChannels = numChannels;
	outputChannels = numChannels;
	context.numChannels = numChannels;
	rt_printf("%i change channels to %i\n", ID, numChannels);
	createBuffers();
}

void AudioNode::createBuffers(){
	inputBufferLength = context.numInputs * getInputChannels() * context.numAudioFrames;
	int outputBufferLength = context.numOutputs * getOutputChannels() * context.numAudioFrames;
	
	delete[] context.inputBuffer;
	delete[] context.outputBuffer;
	
	context.inputBuffer = new float[inputBufferLength];
	context.outputBuffer = new float[outputBufferLength];
	
	for (int i=0; i<inputBufferLength; i++){
		context.inputBuffer[i] = 0.0f;
	}
	for (int i=0; i<outputBufferLength; i++){
		context.outputBuffer[i] = 0.0f;
	}
	
	inputWidth = getInputChannels() * context.numAudioFrames;
}

void AudioNode::connectTo(int output, AudioNode* node, int input){
	OutputConnection connection;
	connection.output = output;
	connection.node = node;
	connection.input = input;
	outputConnections.push_back(connection);
	
	node->addInputConnection(getOutputChannels());
	
	if (channelCountMode == MAX_CCM && node->getInputChannels() > getOutputChannels()){
		changeNumChannels(node->getInputChannels());
	}
}

void AudioNode::addInputConnection(int numChannels){
	inputConnections += 1;
	
	if (channelCountMode == MAX_CCM && numChannels > getInputChannels()){
		rt_printf("%i %i %i\n", ID, numChannels, getInputChannels());
		changeNumChannels(numChannels);
	}
}

void AudioNode::receiveInput(int input, AudioNode* node){
	//rt_printf("receiveInput %i\n", input);
	// assuming they have the same number of channels
	if (getInputChannels() == node->getOutputChannels()){
		for (int i=0; i<inputWidth; i++){
			context.inputBuffer[i + input*inputWidth] += node->context.outputBuffer[i + input*inputWidth];
		}
	} else {
		rt_printf("error, incompatible channels from %i to %i\n", node->ID, ID);
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
		connection.node->receiveInput(connection.input, this);
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

