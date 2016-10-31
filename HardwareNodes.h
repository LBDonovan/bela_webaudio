/***** HardwareNodes.h *****/

#ifndef HARDWARENODES_H_INC
#define HARDWARENODES_H_INC

#include <AudioNode.h>
#include <Bela.h>

class AudioSourceNode : public AudioNode {
	public:
		AudioSourceNode(int id) : AudioNode(id, 0, 1, 2, EXPLICIT_CCM){}
		void receiveInterleavedInput(BelaContext* ctx);
};

class AudioDestinationNode : public AudioNode {
	public:
		AudioDestinationNode(int id) : AudioNode(id, 1, 0, 2, EXPLICIT_CCM){}
		void setInterleavedOutput(BelaContext* ctx);
};

#endif