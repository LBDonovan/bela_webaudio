/***** HardwareNodes.cpp *****/
#include <HardwareNodes.h>

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