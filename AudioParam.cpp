/***** AudioParam.cpp *****/
#include <AudioParam.h>

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