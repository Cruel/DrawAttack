#include "TweenTransformable.hpp"

namespace util3ds {

int TweenTransformable::getValues(int tweenType, float *returnValues) {
	switch (tweenType) {
		case POSITION_X: returnValues[0] = getPosition().x; return 1;
		case POSITION_Y: returnValues[0] = getPosition().y; return 1;
		case POSITION_XY:
			returnValues[0] = getPosition().x;
			returnValues[1] = getPosition().y;
			return 2;
		default: return -1;
	}
}

void TweenTransformable::setValues(int tweenType, float *newValues) {
	switch (tweenType) {
		case POSITION_X:
			setPosition(newValues[0], getPosition().y);
			break;
		case POSITION_Y:
			setPosition(getPosition().x, newValues[0]);
			break;
		case POSITION_XY:
			setPosition(newValues[0], newValues[1]);
			break;
		default:
			break;
	}
}

} // namespace util3ds
