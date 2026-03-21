#include "lbmdungeon/detection.h"
#include "kyra/engine/eob.h"

#include "common/config-manager.h"
#include "common/system.h"
#include "engines/advancedDetector.h"
#include "base/plugins.h"

class LbmDungeonMetaEngine : public AdvancedMetaEngine<LbmDungeonGameDescription> {
public:
	const char *getName() const override {
		return "lbmdungeon";
	}

	Common::Error createInstance(OSystem *syst, Engine **engine, const LbmDungeonGameDescription *gd) const override {
		Kyra::GameFlags flags = gd->flags;
		flags.lang = gd->desc.language;
		flags.platform = gd->desc.platform;

		if (flags.lang == Common::UNK_LANG) {
			Common::Language lang = Common::parseLanguage(ConfMan.get("language"));
			flags.lang = (lang != Common::UNK_LANG) ? lang : Common::EN_ANY;
		}

		*engine = new Kyra::EoBEngine(syst, flags);
		return Common::kNoError;
	}
};

REGISTER_PLUGIN_STATIC(LBMDUNGEON, PLUGIN_TYPE_ENGINE, LbmDungeonMetaEngine);
