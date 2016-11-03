/***** GainNode.h *****/

#ifndef GAINNODE_H_INC
#define GAINNODE_H_INC

#include <AudioNode.h>

class GainNode : public AudioNode {
	public:
		GainNode(int id) : AudioNode(id, 1, 1, 1, MAX_CCM){}
		void render();
};

#endif