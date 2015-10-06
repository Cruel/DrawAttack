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
		case ROTATION: returnValues[0] = getRotation(); return 1;
		case SCALE_X: returnValues[0] = getScale().x; return 1;
		case SCALE_Y: returnValues[0] = getScale().y; return 1;
		case SCALE_XY:
			returnValues[0] = getScale().x;
			returnValues[1] = getScale().y;
			return 2;
		case ORIGIN_X: returnValues[0] = getOrigin().x; return 1;
		case ORIGIN_Y: returnValues[0] = getOrigin().y; return 1;
		case ORIGIN_XY:
			returnValues[0] = getOrigin().x;
			returnValues[1] = getOrigin().y;
			return 2;
		default: return -1;
	}
}

void TweenTransformable::setValues(int tweenType, float *newValues) {
	switch (tweenType) {
		case POSITION_X: setPosition(newValues[0], getPosition().y); break;
		case POSITION_Y: setPosition(getPosition().x, newValues[0]); break;
		case POSITION_XY: setPosition(newValues[0], newValues[1]); break;
		case ROTATION: setRotation(newValues[0]); break;
		case SCALE_X: setScale(newValues[0], getScale().y); break;
		case SCALE_Y: setScale(getScale().x, newValues[0]); break;
		case SCALE_XY: setScale(newValues[0], newValues[1]); break;
		case ORIGIN_X: setOrigin(newValues[0], getOrigin().y); break;
		case ORIGIN_Y: setOrigin(getOrigin().x, newValues[0]); break;
		case ORIGIN_XY: setOrigin(newValues[0], newValues[1]); break;
		default:
			break;
	}
}

} // namespace util3ds
