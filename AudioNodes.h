/***** AudioNodes.h *****/

#ifndef AUDIONODES_H_INC
#define AUDIONODES_H_INC

#include <AudioNode.h>
#include <HardwareNodes.h>
#include <ChannelNodes.h>
#include <AudioParam.h>
#include <OscillatorNode.h>

class GainNode : public AudioNode {
	public:
		GainNode(int id) : AudioNode(id, 1, 1, 1, MAX_CCM){}
		void render();
};

#endif