class CActionBase
{
public:
	char pad_0000[576]; //0x0000
}; //Size: 0x0240

class CActionCameraManager : public CActionBase
{
public:
	char pad_0240[6536]; //0x0240
	class CCameraBase* activeCamera; //0x1BC8
	char pad_1BD0[48]; //0x1BD0
}; //Size: 0x1C00