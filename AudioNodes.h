/***** AudioNodes.h *****/

#ifndef AUDIONODES_H_INC
#define AUDIONODES_H_INC

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
		GainNode(int id, float gain) : AudioNode(id, 1, 1, 1, MAX_CCM), gain(gain){}
		void render();
	private:
		float gain;
};

#endif