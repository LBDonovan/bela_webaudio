/***** WebAudioOSC.h *****/

#define OSC_RECIEVE 3427
#define OSC_SEND 3426

namespace osc {
	void setup();
	bool handshake();
	void checkMessages();
	void sendTimestamp(int count);
}