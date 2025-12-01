class CActionBase
{
public:
	char pad_0000[352]; //0x0000
}; //Size: 0x0240

class CActionCameraManager : public CActionBase
{
public:
	char pad_0160[6696]; //0x0160
	class CCameraBase* activeCamera; //0x1B88
	char pad_1B90[48]; //0x1B90
}; //Size: 0x1BC0