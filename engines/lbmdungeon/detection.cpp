#include "lbmdungeon/detection.h"
#include "common/translation.h"
#include "engines/advancedDetector.h"
#include "base/plugins.h"

static const PlainGameDescriptor lbmDungeonGames[] = {
	{ "lbmdungeon", "LBM Dungeon" },
	{ nullptr, nullptr }
};

// Reuse kyra's GameFlags macro
#define FLAGS(x, y, z, a, b, c, d, e, f, id) { Common::UNK_LANG, Common::UNK_LANG, Common::UNK_LANG, Common::UNK_LANG, Common::kPlatformUnknown, x, y, z, a, b, c, d, e, f, id }

static const LbmDungeonGameDescription gameDescriptions[] = {
	// Eye of the Beholder - DOS English (EoB1 assets)
	{
		{
			"lbmdungeon",
			0,
			AD_ENTRY1s("EOBDATA3.PAK", "379a9388f6e11915c787b8453473874c", 346303),
			Common::EN_ANY,
			Common::kPlatformDOS,
			ADGF_NO_FLAGS,
			GUIO4(GUIO_NOSPEECH, GUIO_MIDIADLIB, GUIO_MIDIPCSPK, GUIO_MIDIPCJR)
		},
		FLAGS(false, false, false, false, false, false, false, false, false, Kyra::GI_EOB1)
	},

	{ AD_TABLE_END_MARKER, FLAGS(false, false, false, false, false, false, false, false, false, 0) }
};

#undef FLAGS

class LbmDungeonMetaEngineDetection : public AdvancedMetaEngineDetection<LbmDungeonGameDescription> {
public:
	LbmDungeonMetaEngineDetection() : AdvancedMetaEngineDetection(gameDescriptions, lbmDungeonGames) {}

	const char *getName() const override {
		return "lbmdungeon";
	}

	const char *getEngineName() const override {
		return "LBM Dungeon";
	}

	const char *getOriginalCopyright() const override {
		return "Eye of the Beholder (C) 1990-1993 Westwood Associates/SSI";
	}
};

REGISTER_PLUGIN_STATIC(LBMDUNGEON_DETECTION, PLUGIN_TYPE_ENGINE_DETECTION, LbmDungeonMetaEngineDetection);
