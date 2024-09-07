#include <cstdint>

class CActionManager
{
public:
	char pad_0000[16]; //0x0000
	uint32_t gamePaused; //0x0010
	uint32_t unpausedTime; //0x0014
	uint32_t unpausedTime2; //0x0018
	uint32_t generalTime; //0x001C
	uint32_t totalTimeSinceStartup; //0x0020
	uint32_t isLoaded; //0x0024
	char pad_0028[304]; //0x0028
	uint32_t N000014A6; //0x0158
	uint32_t N00006A6E; //0x015C
	char pad_0160[248]; //0x0160
	void* actionFighterManager; //0x0258
	char pad_0260[8]; //0x0260
	void* actionFighterManager2; //0x0268
	char pad_0270[928]; //0x0270
	class CActionCameraManager* actionCameraManager; //0x0610
	char pad_0618[12784]; //0x0618
};