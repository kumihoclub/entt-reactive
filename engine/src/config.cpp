
#include "config.h"

namespace {

	Config stored_config;
	b32 config_set = false;

}

void Config::set(Config& config) {
	if (!config_set) {
		stored_config = config;
		config_set = true;
	}
}

const Config& Config::get() {
	return stored_config;
}
