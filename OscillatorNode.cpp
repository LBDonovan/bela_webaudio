/***** OscillatorNode.cpp *****/
#include <OscillatorNode.h>
#include <cmath>

void OscillatorNode::render(){
	if (!state) return;
	
	if (type == OSC_SINE){
		for (int f = 0; f<audioFrames; f++){
			for (int ch = 0; ch<outputChannels; ch++){
				outputBuffer[0][ch][f] = sinf(phase);
			}
			incrementPhase(f);
		}
	} else if (type == OSC_SQUARE){
		for (int f = 0; f<audioFrames; f++){
			for (int ch = 0; ch<outputChannels; ch++){
				if (phase >= 0){
					outputBuffer[0][ch][f] = 1.0f;
				} else {
					outputBuffer[0][ch][f] = -1.0f;
				}
			}
			incrementPhase(f);
		}
	} else if (type == OSC_SAWTOOTH){
		for (int f = 0; f<audioFrames; f++){
			for (int ch = 0; ch<outputChannels; ch++){
				outputBuffer[0][ch][f] = phase / M_PI;
			}
			incrementPhase(f);
		}
	} else if (type == OSC_TRIANGLE){
		for (int f = 0; f<audioFrames; f++){
			for (int ch = 0; ch<outputChannels; ch++){
				if (phase >= 0.0f && phase <= M_PI/2.0f){
					outputBuffer[0][ch][f] = 2.0f/M_PI * phase;
				} else if (phase <= -M_PI/2.0f){
					outputBuffer[0][ch][f] = -2.0f/M_PI * (phase + M_PI);
				} else if (phase > M_PI/2.0f){
					outputBuffer[0][ch][f] = 1.0f - 2.0f/M_PI * (phase - M_PI/2.0f);
				} else if (phase > -M_PI/2.0f && phase < 0.0f){
					outputBuffer[0][ch][f] = 2.0f/M_PI * phase;
				}
			}
			incrementPhase(f);
		}
	}
	
}

void OscillatorNode::incrementPhase(int frame){
	phase += 2.0 * M_PI * params[0]->getValueAt(frame) * pow(2, params[1]->getValueAt(frame) / 1200.0f) * inverseSampleRate;
	if(phase > M_PI)
		phase -= 2.0 * M_PI;
}

void OscillatorNode::setType(int _type){
	type = _type;
}
void OscillatorNode::setState(int _state){
	state = _state;
	if (!state) resetOutputs();
}