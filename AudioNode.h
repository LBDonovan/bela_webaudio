/***** AudioNode.h *****/

#ifndef AUDIONODE_H_INC
#define AUDIONODE_H_INC

#include <vector>
#include <stddef.h>

#define MAX_CCM 0
#define CLAMPED_MAX_CCM 1
#define EXPLICIT_CCM 2

struct AudioNodeContext{
	AudioNodeContext() : inputBuffer(NULL), outputBuffer(NULL){}
	int numInputs;
	int numOutputs;
	int numChannels;
	int numAudioFrames;
	int sampleRate;
	float* inputBuffer;
	float* outputBuffer;
};

struct OutputConnection;

class AudioNode{
	public:
		AudioNode(int id, int inputs, int outputs, int channelCount, int channelCountMode);
		
		void connectTo(int output, AudioNode* node, int input);
		
		void receiveInput(int input, AudioNode* node);
		
		void resetInputs();
		
	protected:
		
		AudioNodeContext context;
		
		virtual void render(AudioNodeContext* context){};
		
		void addInputConnection(int numChannels);
		
		int inputConnections;
		int inputConnectionsReceived;
		
		std::vector<OutputConnection> outputConnections;
		
		int ID;
		
		int channelCount;
		int channelCountMode;
		
		int getInputChannels();
		int getOutputChannels();
		void changeNumChannels(int numChannels);

		int inputWidth;
		int inputBufferLength;
		//int outputBufferLength;
		
		int inputChannels;
		int outputChannels;
		
		void createBuffers();
		void process();
		
};

struct OutputConnection{
	int output;
	AudioNode* node;
	int input;
};

#endif