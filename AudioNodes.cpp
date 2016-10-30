/***** AudioNodes.cpp *****/
#include <AudioNodes.h>
#include <cmath>

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

void AudioParam::setValue(float val){
	value = val;
}

void OscillatorNode::render(){
	if (!state) return;
	if (type == 0){		// sine
		for (int f = 0; f<audioFrames; f++){
			for (int ch = 0; ch<outputChannels; ch++){
				outputBuffer[0][ch][f] = sinf(phase);
			}
			phase += 2.0 * M_PI * params[0]->getValueAt(f) * pow(2, params[1]->getValueAt(f) / 1200.0f) * inverseSampleRate;
			if(phase > 2.0 * M_PI)
				phase -= 2.0 * M_PI;
		}
	}
}

void OscillatorNode::setType(int _type){
	type = _type;
}
void OscillatorNode::setState(int _state){
	state = _state;
}