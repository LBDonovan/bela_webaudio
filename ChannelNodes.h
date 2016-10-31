/***** ChannelNodes.h *****/

#ifndef CHANNELNODES_H_INC
#define CHANNELNODES_H_INC

#include <AudioNode.h>

class ChannelSplitterNode : public AudioNode {
	public:
		ChannelSplitterNode(int id, int channels) : AudioNode(id, 1, 1, channels, MAX_CCM){
			changeNumChannels(channels);
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

#endif