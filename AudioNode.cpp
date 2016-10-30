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
	sampleRate(44100.0f),
	inputConnections(0),
	inputConnectionsReceived(0),
	paramsReceived(0),
	ID(id),
	channelCount(channelCount),
	channelCountMode(channelCountMode),
	printedMixing(false)
{
	inverseSampleRate = 1.0f/sampleRate;
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

void AudioNode::connectTo(AudioNode* node, int output, int input){
	
	printf("connecting %i output %i to %i input %i\n", ID, output, node->ID, input);
	
	OutputConnection connection;
	connection.node = node;
	connection.output = output;
	connection.input = input;
	outputConnections.push_back(connection);
	
	node->addInputConnection(outputChannels);
	
	if (channelCountMode == MAX_CCM && node->inputChannels > outputChannels){
		printf("%i changing num channels connectTo\n", ID);
		changeNumChannels(node->inputChannels);
	}
}

void AudioNode::addInputConnection(int numChannels){
	inputConnections += 1;
	
	printf("receiving connection from to %i\n", ID);
	
	if (channelCountMode == MAX_CCM && numChannels > inputChannels){
		printf("%i changing num channels addInputConnection\n", ID);
		changeNumChannels(numChannels);
	}
}
void AudioNode::addParam(AudioNode* param){
	printf("receiving parameter connection to %i\n", ID);
	params.push_back(param);
}

void AudioNode::receiveInput(AudioNode* node, const OutputConnection* connection){
	
	// printf("node %i input %i receive from node %i output %i\n", ID, connection->input, node->ID, connection->output);
	// printf("inputs %i (should be %i)\n", inputBuffer.size(), inputs);
	// printf("input channels %i (should be %i)\n", inputBuffer.at(connection->input).size(), inputChannels);
	// printf("frames %i %i\n", inputBuffer.at(connection->input).at(0).size(), inputBuffer.at(connection->input).at(1).size());
	
	// assuming they have the same number of channels
	if (inputChannels == node->outputChannels){
		for (int ch = 0; ch<inputChannels; ch++){
			for (int f = 0; f<audioFrames; f++){
				inputBuffer.at(connection->input).at(ch).at(f) += node->outputBuffer.at(connection->output).at(ch).at(f);
			}
		}
	} else {
		if (!printedMixing){
			printedMixing = true;
			rt_printf("mixing! node %i output %i to node %i input %i\n", node->ID, connection->output, ID, connection->input);
			// printf("%i to %i (really %i) channels\n", node->outputChannels, inputChannels, inputBuffer[0][1].size());
		}
		
		for (int inch = 0; inch < inputChannels; inch++){
			// printf("inch: %i\n", inch);
			for (int outch = 0; outch < node->outputChannels; outch++){
				// printf("outch: %i\n", outch);
				for (int f = 0; f<audioFrames; f++){
					// printf("f: %i\n", f);
					inputBuffer.at(connection->input).at(inch).at(f) += node->outputBuffer.at(connection->output).at(outch).at(f);
				}
			}
		}
	}
	
	inputConnectionsReceived += 1;
	if (inputConnectionsReceived >= inputConnections){
		if (paramsReceived >= params.size()){
			process();
		} else {
			processParams();
		}
	}
	
}

void AudioNode::processParams(){
	for (auto param : params){
		param->process();
	}
}

void AudioNode::receiveParam(){
	paramsReceived += 1;
	if (paramsReceived >= params.size() && inputConnectionsReceived >= inputConnections){
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
	paramsReceived = 0;
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

int AudioNode::getID(){
	return ID;
}

