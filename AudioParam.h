/***** AudioParam.h *****/

#ifndef AUDIOPARAM_H_INC
#define AUDIOPARAM_H_INC

#include <AudioNode.h>

class AudioParam : public AudioNode {
	public:
		AudioParam(int id, AudioNode* parent, float val) : AudioNode(id, 1, 0, 1, EXPLICIT_CCM), parent(parent), value(val){
			parent->addParam(this);
		}
		void resetInputs();
		float getValueAt(int index);
		void setValue(float val);
	protected:
		void process();
	private:
		AudioNode* parent;
		float value;
};

#endif