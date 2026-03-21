#ifndef LBMDUNGEON_DETECTION_H
#define LBMDUNGEON_DETECTION_H

#include "engines/advancedDetector.h"
#include "kyra/detection.h"

struct LbmDungeonGameDescription {
	AD_GAME_DESCRIPTION_HELPERS(desc);

	ADGameDescription desc;
	Kyra::GameFlags flags;
};

#endif // LBMDUNGEON_DETECTION_H
