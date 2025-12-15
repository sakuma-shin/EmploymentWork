#define _USE_MATH_DEFINES
#include <math.h>
#include "easing.h"

float EaseInSine(float a) {
	return 1 - cosf((a * (float)M_PI) / 2);
}

float EaseOutSine(float x) {
	return sinf((x * (float)M_PI) / 2);
}

float EaseInOutSine(float x) {
	return -cosf((x * (float)M_PI) - 1) / 2;
}

float EaseInQuad(float x) {
	return x * x;
}

float EaseOutQuad(float x) {
	return 1 - (1 - x) * (1 - x);
}

float EaseInOutQuad(float x) {
	if (x < 0.5f) {
		return 2.0f * x * x;
	} else {
		return 1 - powf(-2.0f * x + 2.0f, 2.0f) / 2.0f;
	}
}

float EaseInCubic(float x) {
	return x * x * x;
}

float EaseOutCubic(float x) {
	return 1 - powf(1 - x, 3);
}

float EaseInOutCubic(float x) {
	if (x < 0.5) {
		return 4 * x * x * x;
	} else {
		return 1 - powf(-2 * x + 2, 3) / 2;
	}
}

float EaseInQuart(float x) {
	return powf(x, 4);
}

float EaseOutQuart(float x) {
	return 1 - powf(1 - x, 4);
}

float EaseInOutQuart(float x) {
	if (x < 0.5) {
		return 8 * x * x * x * x;
	} else {
		return 1 - powf(-2 * x + 2, 4);
	}
}

float EaseInQuint(float x) {
	return powf(x, 5);
}

float EaseOutQuint(float x) {
	return 1 - powf(1 - x, 5);
}

float EaseInOutQuint(float x) {
	if (x < 0.5) {
		return 16 * x * x * x * x * x;
	} else {
		return 1 - powf(-2 * x + 2, 5) / 2;
	}
}

float EaseInExpo(float x) {
	if (x == 0.0f) {
		return 0;
	} else {
		return powf(2, 10 * x - 10);
	}
}

float EaseOutExpo(float x) {
	if (x == 1) {
		return 1;
	} else {
		return 1 - powf(2, -10 * x);
	}
}

float EaseInOutExpo(float x) {
	if (x == 0.0f) {
		return 0.0f;
	} else if (x == 1.0f) {
		return 1.0f;
	} else if (x < 0.5f) {
		return powf(2.0f, 20.0f * x - 10.0f) / 2.0f;
	} else {
		return (2.0f - powf(2.0f, -20.0f * x + 10.0f)) / 2.0f;
	}
}

float EaseInCirc(float x) {
	return 1.0f - sqrtf(1 - powf(x, 2.0f));
}

float EaseOutCirc(float x) {
	return sqrtf(1.0f - powf(x - 1.0f, 2.0f));
}

float EaseInOutCirc(float x) {
	if (x < 0.5) {
		return (1.0f - sqrtf(1.0f - powf(2.0f * x, 2.0f))) / 2.0f;
	} else {
		return (sqrtf(1.0f - powf(-2.0f * x + 2.0f, 2.0f)) + 1.0f) / 2.0f;
	}
}

float EaseInBack(float x) {
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return c3 * x * x * x - c1 * x * x;
}

float EaseOutBack(float x) {
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return  1.0f + c3 * powf(x - 1.0f, 3.0f) + c1 * powf(x - 1.0f, 2.0f);
}

float EaseInOutBack(float x) {
	const float c1 = 1.70158f;
	const float c3 = c1 * 1.525f;

	if (x < 0.5) {
		return (powf(2.0f * x, 2.0f) * ((c3 + 1.0f) * 2.0f * x - c3)) / 2.0f;
	} else {
		return (powf(2.0f * x - 2.0f, 2.0f) * ((c3 + 1.0f) * (x * 2.0f - 2.0f) + c3) + 2.0f) / 2.0f;
	}
}

float EaseInElastic(float x) {
	const float c4 = (2 * float(M_PI)) / 3;

	if (x == 0) {
		return 0.0f;
	} else if (x == 1) {
		return 1.0f;
	} else {
		return -powf(2.0f, 10.0f * x - 10.0f) * sinf((x * 10.0f - 10.75f) * c4);
	}
}

float EaseOutElastic(float x) {
	const float c4 = (2.0f * float(M_PI)) / 3.0f;

	if (x == 0.0f) {
		return 0.0f;
	} else if (x == 1.0f) {
		return 1.0f;
	} else {
		return powf(2.0f, -10.0f * x) * sinf((x * 10.0f - 0.75f) * c4) + 1.0f;
	}
}

float EaseInOutElastic(float x) {
	const float c5 = (2.0f * float(M_PI)) / 4.5f;

	if (x == 0.0f) {
		return 0.0f;
	} else if (x == 1.0f) {
		return 1.0f;
	} else if (x < 0.5f) {
		return -(powf(2.0f, 20.0f * x - 10.0f) * sinf((20.0f * x - 11.125f) * c5)) / 2.0f;
	} else {
		return (powf(2.0f, -20.0f * x + 10.0f) * sinf((20.0f * x - 11.125f) * c5)) / 2.0f + 1.0f;
	}
}

float EaseOutBounce(float x) {
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (x < 1.0f / d1) {
		return n1 * x * x;
	} else if (x < 2.0f / d1) {
		x -= 1.5f / d1;
		return n1 * x * x + 0.75f;
	} else if (x < 2.5f / d1) {
		x -= 2.25f / d1;
		return n1 * x * x + 0.9375f;
	} else {
		x -= 2.625f / d1;
		return n1 * x * x + 0.984375f;
	}
}

float EaseInBounce(float x) {
	return 1.0f - EaseOutBounce(1.0f - x);
}

float EaseInOutBounce(float x) {
	if (x < 0.5f) {
		return (1.0f - EaseOutBounce(1.0f - 2.0f * x)) / 2.0f;
	} else {
		return (1.0f + EaseOutBounce(2.0f * x - 1.0f)) / 2.0f;
	}
}