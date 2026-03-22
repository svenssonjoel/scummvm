#include "lbmdungeon/detection.h"
#include "lbmdungeon/eob.h"
#include "lbmdungeon/eobcommon.h"

#include "common/config-manager.h"
#include "common/system.h"
#include "backends/keymapper/keymapper.h"
#include "engines/advancedDetector.h"
#include "base/plugins.h"

class LbmDungeonMetaEngine : public AdvancedMetaEngine<LbmDungeonGameDescription> {
public:
	const char *getName() const override {
		return "lbmdungeon";
	}

	Common::KeymapArray initKeymaps(const char *target) const override {
		return Kyra::EoBCoreEngine::initKeymaps("eob");
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
