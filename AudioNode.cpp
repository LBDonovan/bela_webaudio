/***** AudioNode.cpp *****/
#include <AudioNode.h>
#include <AudioNodes.h>
#include <Bela.h>

AudioNode::AudioNode(int id, int inputs, int outputs, int channelCount, int channelCountMode):
	inputs(inputs),
	outputs(outputs),
	inputChannels(channelCount),
	outputChannels(channelCount),
	audioFrames(16),
	sampleRate(44100),
	inputConnections(0),
	inputConnectionsReceived(0),
	ID(id),
	channelCount(channelCount),
	channelCountMode(channelCountMode)
{
	createBuffers();
};

void AudioNode::changeNumChannels(int numChannels){
	inputChannels = numChannels;
	outputChannels = numChannels;
	printf("%i change channels to %i\n", ID, numChannels);
	createBuffers();
}

void AudioNode::createBuffers(){
	
	// printf("node %i:\n", ID);

	inputBuffer.resize(inputs); 
	for (int i = 0; i < inputs; i++) {
		inputBuffer[i].resize(inputChannels);
		for (int ch = 0; ch < inputChannels; ch++) {
			inputBuffer[i][ch].resize(audioFrames, 0.0f);
		}
	}
	outputBuffer.resize(outputs); 
	for (int i = 0; i < outputs; i++) {
		outputBuffer[i].resize(outputChannels);
		for (int ch = 0; ch < outputChannels; ch++) {
			outputBuffer[i][ch].resize(audioFrames, 0.0f);
		}
	}

	// printf("inputs: %i\n", inputBuffer.size());
	// for (int i = 0; i<inputBuffer.size(); i++){
	// 	printf("input %i has %i channels (should be %i)\n", i, inputBuffer.at(i).size(), inputChannels);
	// 	for (int ch = 0; ch < inputBuffer.at(i).size(); ch++){
	// 		printf("input %i channel %i has %i frames\n", i, ch, inputBuffer.at(i).at(ch).size());
	// 	}
	// }
	// printf("outputs: %i\n", outputBuffer.size());
	// for (int i = 0; i<outputBuffer.size(); i++){
	// 	printf("output %i has %i channels (should be %i)\n", i, outputBuffer.at(i).size(), outputChannels);
	// 	for (int ch = 0; ch < outputBuffer.at(i).size(); ch++){
	// 		printf("output %i channel %i has %i frames\n", i, ch, outputBuffer.at(i).at(ch).size());
	// 	}
	// }

}

void AudioNode::connectTo(int output, AudioNode* node, int input){
	OutputConnection connection;
	connection.output = output;
	connection.node = node;
	connection.input = input;
	outputConnections.push_back(connection);
	
	node->addInputConnection(outputChannels);
	
	if (channelCountMode == MAX_CCM && node->inputChannels > outputChannels){
		changeNumChannels(node->inputChannels);
	}
}

void AudioNode::addInputConnection(int numChannels){
	inputConnections += 1;
	
	if (channelCountMode == MAX_CCM && numChannels > inputChannels){
		rt_printf("%i %i %i\n", ID, numChannels, inputChannels);
		changeNumChannels(numChannels);
	}
}

void AudioNode::receiveInput(AudioNode* node, const OutputConnection* connection){
	
	// printf("node %i input %i receive from node %i output %i\n", ID, connection->input, node->ID, connection->output);
	// printf("inputs %i\n", inputBuffer.size());
	// printf("input channels %i\n", inputBuffer[connection->input].size());
	// printf("frames %i %i\n", inputBuffer[connection->input][0].size(), inputBuffer[connection->input][1].size());
	
	// assuming they have the same number of channels
	if (inputChannels == node->outputChannels){
		for (int ch = 0; ch<inputChannels; ch++){
			for (int f = 0; f<audioFrames; f++){
				inputBuffer[connection->input][ch][f] += node->outputBuffer[connection->output][ch][f];
			}
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
	render();
	
	for (auto connection : outputConnections){
		connection.node->receiveInput(this, &connection);
	}

	inputConnectionsReceived = 0;
}

void AudioNode::resetInputs(){
	for (auto &input : inputBuffer){
		for (auto &channel : input){
			std::fill(channel.begin(), channel.end(), 0.0f);
		}
	}
}
void AudioNode::resetOutputs(){
	for (auto &output : outputBuffer){
		for (auto &channel : output){
			std::fill(channel.begin(), channel.end(), 0.0f);
		}
	}
}

