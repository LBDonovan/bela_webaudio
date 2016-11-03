/***** AudioParam.cpp *****/
#include <AudioParam.h>
#include <rtdk.h>

extern unsigned long gAudioFramesElapsed;

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
	if (events.size()){
		unsigned long time = gAudioFramesElapsed;
		for (auto &sample : inputBuffer[0][0]){
			if (events.size()){
				sample = events.front()->process(time++, &value);
				if (events.front()->hasFinished()){
					// can this cause mode switches?
					events.pop();
				}
			} else {
				sample = value;
			}
		}
	} else {
		std::fill(inputBuffer[0][0].begin(), inputBuffer[0][0].end(), value);
	}
}

void AudioParam::setValue(float val){
	value = val;
}

void AudioParam::addEvent(int type, float target, unsigned long time){
	events.push(new AudioParamEvent(type, target, time));
}

bool AudioParamEvent::hasFinished(){
	return finished;
}

float AudioParamEvent::process(unsigned long time, float* value){
	if (eventType == SET_VALUE){
		if (time >= endTime){
			*value = target;
			finished = true;
			rt_printf("value set to %f\n", target);
			return target;
		} else {
			return *value;
		}
	}
	return *value;
}