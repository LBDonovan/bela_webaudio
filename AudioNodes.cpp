/***** AudioNodes.cpp *****/
#include <AudioNodes.h>

void GainNode::render(){
	for (int i = 0; i<inputs; i++){
		for (int ch = 0; ch<inputChannels; ch++){
			for (int f = 0; f<audioFrames; f++){
				outputBuffer[i][ch][f] = inputBuffer[i][ch][f] * params[0]->getValueAt(f);
			}
		}
	}
}