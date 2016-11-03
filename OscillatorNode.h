/***** OscillatorNode.h *****/

#ifndef OSCILLATORNODE_H_INC
#define OSCILLATORNODE_H_INC

#include <AudioNode.h>

#define OSC_SINE 0
#define OSC_SQUARE 1
#define OSC_SAWTOOTH 2
#define OSC_TRIANGLE 3
#define OSC_CUSTOM 4

class OscillatorNode : public AudioNode {
	public:
		OscillatorNode(int id) : AudioNode(id, 0, 1, 1, MAX_CCM){}
		void render();
		void setType(int _type);
		void setState(int _state);
	private:
		float phase = 0.0f;
		int type = -1;
		int state = 0;
		void incrementPhase(int frame);
};

#endif