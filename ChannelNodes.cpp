/***** ChannelNodes.cpp *****/
#include <ChannelNodes.h>

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