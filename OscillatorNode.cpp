/***** OscillatorNode.cpp *****/
#include <OscillatorNode.h>
#include <cmath>

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