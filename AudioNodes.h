/***** AudioNodes.h *****/

#ifndef AUDIONODES_H_INC
#define AUDIONODES_H_INC

#include <AudioNode.h>
#include <Bela.h>

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

class ChannelSplitterNode : public AudioNode {
	public:
		ChannelSplitterNode(int id, int channels) : AudioNode(id, 1, 1, channels, MAX_CCM){
			changeNumChannels(channels);
			// printf("created %i\n", ID);
		}
		void render();
	protected:
		virtual void changeNumChannels(int numChannels);
};
class ChannelMergerNode : public AudioNode {
	public:
		ChannelMergerNode(int id, int channels) : AudioNode(id, 1, 1, channels, EXPLICIT_CCM){
			changeNumChannels(channels);
		}
		void render();
	protected:
		virtual void changeNumChannels(int numChannels);
};

class GainNode : public AudioNode {
	public:
		GainNode(int id) : AudioNode(id, 1, 1, 1, MAX_CCM){}
		void render();
};

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
};

#endif