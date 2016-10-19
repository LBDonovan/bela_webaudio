/***** AudioNode.h *****/

#ifndef AUDIONODE_H_INC
#define AUDIONODE_H_INC

#include <vector>
#include <stddef.h>

#define MAX_CCM 0
#define CLAMPED_MAX_CCM 1
#define EXPLICIT_CCM 2

struct OutputConnection;

class AudioNode{
	public:
		AudioNode(int id, int inputs, int outputs, int channelCount, int channelCountMode);
		
		void connectTo(int output, AudioNode* node, int input);
		
		void receiveInput(AudioNode* node, const OutputConnection* connection);
		
		void resetInputs();
		
	protected:
		
		int inputs;
		int outputs;
		
		int inputChannels;
		int outputChannels;
		
		int audioFrames;
		int sampleRate;
		
		std::vector<std::vector<std::vector<float>>> inputBuffer;
		std::vector<std::vector<std::vector<float>>> outputBuffer;
		
		virtual void render(){};
		
		void addInputConnection(int numChannels);
		
		int inputConnections;
		int inputConnectionsReceived;
		
		std::vector<OutputConnection> outputConnections;
		
		int ID;
		
		int channelCount;
		int channelCountMode;

		void changeNumChannels(int numChannels);
		
		void createBuffers();
		void process();
		
		void resetOutputs();
		
};

struct OutputConnection{
	int output;
	AudioNode* node;
	int input;
};

#endif