#ifndef UTIL3DS_TWEENTRANSFORMABLE_H
#define UTIL3DS_TWEENTRANSFORMABLE_H
#include <cpp3ds/Graphics.hpp>
#include <TweenEngine/Tweenable.h>

namespace util3ds {

class TweenTransformable: public cpp3ds::Transformable, public TweenEngine::Tweenable
{
public:
	static const int POSITION_X  = 1;
	static const int POSITION_Y  = 2;
	static const int POSITION_XY = 3;
	static const int ROTATION    = 4;
	static const int SCALE_X     = 5;
	static const int SCALE_Y     = 6;
	static const int SCALE_XY    = 7;
	static const int ORIGIN_X    = 8;
	static const int ORIGIN_Y    = 9;
	static const int ORIGIN_XY   = 10;

protected:
	virtual int getValues(int tweenType, float *returnValues);
	virtual void setValues(int tweenType, float *newValues);
};

} // namespace util3ds

#endif //UTIL3DS_TWEENTRANSFORMABLE_H
