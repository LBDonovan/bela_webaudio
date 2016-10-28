/***** AudioNodes.cpp *****/
#include <AudioNodes.h>

void AudioSourceNode::receiveInterleavedInput(BelaContext* ctx){
	for (int n=0; n<ctx->audioFrames; n++){
		for (int ch=0; ch<ctx->audioInChannels; ch++){
			outputBuffer[0][ch][n] = ctx->audioIn[n * ctx->audioInChannels + ch];
		}
	}
	
	for (const auto connection : outputConnections){
		connection.node->receiveInput(this, &connection);
	}
}

void AudioDestinationNode::setInterleavedOutput(BelaContext* ctx){
	for (int n=0; n<ctx->audioFrames; n++){
		for (int ch=0; ch<ctx->audioOutChannels; ch++){
			ctx->audioOut[n * ctx->audioOutChannels + ch] = inputBuffer[0][ch][n];
		}
	}
}

void ChannelSplitterNode::render(){
	for (int ch = 0; ch<inputChannels; ch++){
		std::copy(inputBuffer[0][ch].begin(), inputBuffer[0][ch].end(), outputBuffer[ch][0].begin());
	}
}

void ChannelSplitterNode::changeNumChannels(int numChannels){
	
	inputChannels = numChannels;
	outputChannels = 1;
	
	inputs = 1;
	outputs = numChannels;
	
	// printf("%i ChannelSplitterNode change outputs & input channels to %i\n", ID, numChannels);
	createBuffers();
}

void ChannelMergerNode::render(){
	for (int ch = 0; ch < outputChannels; ch++){
		std::copy(inputBuffer[ch][0].begin(), inputBuffer[ch][0].end(), outputBuffer[0][ch].begin());
	}
}

void ChannelMergerNode::changeNumChannels(int numChannels){
	
	inputChannels = 1;
	outputChannels = numChannels;
	
	inputs = numChannels;
	outputs = 1;
	
	// printf("%i ChannelMergerNode change inputs & output channels to %i\n", ID, numChannels);
	createBuffers();
}

void GainNode::render(){
	for (int i = 0; i<inputs; i++){
		for (int ch = 0; ch<inputChannels; ch++){
			for (int f = 0; f<audioFrames; f++){
				outputBuffer[i][ch][f] = inputBuffer[i][ch][f] * params[0]->getValueAt(f);
			}
		}
	}
}

void AudioParam::process(){
	if (inputConnectionsReceived < inputConnections){
		return;
	}
	// param events need to be implemented here
	parent->receiveParam();
}

float AudioParam::getValueAt(int index){
	return inputBuffer[0][0].at(index);
}

void AudioParam::resetInputs(){
	std::fill(inputBuffer[0][0].begin(), inputBuffer[0][0].end(), value);
}