/***** AudioParam.h *****/

#ifndef AUDIOPARAM_H_INC
#define AUDIOPARAM_H_INC

#include <AudioNode.h>
#include <queue>

#define SET_VALUE 0
#define LINEAR_RAMP 1
#define EXP_RAMP 2
#define SET_TARGET 3
#define SET_CURVE 4

class AudioParamEvent {
	public:
		AudioParamEvent(int type, float target, unsigned long time) : 
			eventType(type), 
			target(target), 
			endTime(time){}
		float process(unsigned long time, float* value);
		bool hasFinished();
	private:
		int eventType;
		float target;
		unsigned long endTime;
		bool finished = false;
};

class AudioParam : public AudioNode {
	public:
		AudioParam(int id, AudioNode* parent, float val) : AudioNode(id, 1, 0, 1, EXPLICIT_CCM), parent(parent), value(val){
			parent->addParam(this);
		}
		void resetInputs();
		float getValueAt(int index);
		void setValue(float val);
		void addEvent(int type, float target, unsigned long time);
	protected:
		void process();
	private:
		AudioNode* parent;
		float value;
		std::queue<AudioParamEvent*> events;
};

#endif