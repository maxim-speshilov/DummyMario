#include "SettingsManager.h"
#include "Utility.h"

SettingsManager::SettingsManager() :
	context_(),
	settings_(),
	callbacks_() {
	registerSettings();
	registerCallbacks();
}

SettingsManager::SettingsManager(Context context) :
	context_(context),
	settings_(),
	callbacks_() {
	registerSettings();
	registerCallbacks();
	getSettingsFromXml("options/settings.xml");
}

size_t SettingsManager::getSetting(std::size_t setting_type, std::size_t setting) {
	return settings_[setting_type][setting];
}

void SettingsManager::getSettingsFromXml(const std::string& filename) {
	TiXmlDocument doc(filename);

	if (!doc.LoadFile())
		std::runtime_error("SettingsManager::getSettingsFromXml - Failed to load settings file " + filename);

	TiXmlElement* kRoot = doc.RootElement();
	TiXmlElement* setting_group = kRoot->FirstChildElement();

	size_t i = 0;
	while (setting_group) {
		TiXmlElement* setting = setting_group->FirstChildElement();

		size_t j = 0;
		while (setting) {
			TiXmlElement* value = setting->FirstChildElement();
			assert(value != NULL);

			auto index = value->Attribute("index");
			assert(index != NULL);
			settings_[i][j] = std::stoi(index);
			setting = setting->NextSiblingElement();
			++j;
		}

		setting_group = setting_group->NextSiblingElement();
		++i;
	}

	for (auto callback_group : callbacks_)
		for (auto callback : callback_group)
			callback();
}

void SettingsManager::saveToXml(const std::string& filename) {
	TiXmlDocument doc(filename);
	TiXmlNode* root(doc.InsertEndChild(TiXmlElement("settings")));

	TiXmlElement graphics("graphics");

	for (auto pair : magic_enum::enum_entries<Graphics>()) {
		TiXmlElement setting(static_cast<std::string>(pair.second));
		auto setting_names = utility::settings::getOptionsStrings(pair.first);
		auto idx = getSetting(0, magic_enum::enum_index<Graphics>(pair.first).value());
		TiXmlElement value("k" + setting_names[idx]);
		value.SetAttribute("index", idx);
		setting.InsertEndChild(value);
		graphics.InsertEndChild(setting);

	}

	TiXmlElement audio("audio");

	for (auto pair : magic_enum::enum_entries<Audio>()) {
		TiXmlElement setting(static_cast<std::string>(pair.second));
		auto setting_names = utility::settings::getOptionsStrings(pair.first);
		auto idx = getSetting(1, magic_enum::enum_index<Audio>(pair.first).value());
		TiXmlElement value("k" + setting_names[idx]);
		value.SetAttribute("index", idx);
		setting.InsertEndChild(value);
		audio.InsertEndChild(setting);

	}

	root->InsertEndChild(graphics);
	root->InsertEndChild(audio);

	doc.SaveFile(filename);
}

void SettingsManager::set(std::size_t setting_type, std::size_t setting, std::size_t value) {
	settings_[setting_type][setting] = value;
	callbacks_[setting_type].at(setting)();
}

void SettingsManager::setContext(Context context) {
	context_ = context;
}

void SettingsManager::registerCallbacks() {
	callbacks_[static_cast<size_t>(Settings::Graphics)].push_back([this]() {
		context_.window->setVerticalSyncEnabled(static_cast<bool>(
			settings_[static_cast<size_t>(Settings::Graphics)]
			[static_cast<size_t>(Graphics::VerticalSync)]));
		});

	callbacks_[static_cast<size_t>(Settings::Graphics)].push_back([this]() {
		context_.window->setMouseCursorVisible(static_cast<bool>(
			settings_[static_cast<size_t>(Settings::Graphics)]
			[static_cast<size_t>(Graphics::MouseCursorVisible)]));
		});

	callbacks_[static_cast<size_t>(Settings::Audio)].push_back([this]() {
		context_.sound_player->setVolume(std::stoi(std::string(
			magic_enum::enum_name<EffectsVolume>(magic_enum::enum_value<EffectsVolume>(settings_[static_cast<size_t>(Settings::Audio)]
			[static_cast<size_t>(Audio::EffectsVolume)])).substr(1u))));
		});

	callbacks_[static_cast<size_t>(Settings::Audio)].push_back([this]() {
		context_.music_player->setVolume(std::stoi(std::string(
			magic_enum::enum_name<MusicVolume>(magic_enum::enum_value<MusicVolume>(settings_[static_cast<size_t>(Settings::Audio)]
				[static_cast<size_t>(Audio::MusicVolume)])).substr(1u))));
		});
}

void SettingsManager::registerSettings() {
	settings_[static_cast<size_t>(Settings::Graphics)]
		= std::vector<size_t>(magic_enum::enum_count<Graphics>());
	settings_[static_cast<size_t>(Settings::Audio)]
		= std::vector<size_t>(magic_enum::enum_count<Audio>());
}

SettingsManager::Context::Context() :
	window(),
	sound_player(),
	music_player(),
	input_manager() {
}


SettingsManager::Context::Context(sf::RenderWindow& window_, SoundPlayer& sound_player_, 
	MusicPlayer& music_player_, InputManager& input_manager_) :
	window(&window_), 
	sound_player(&sound_player_),
	music_player(&music_player_),
	input_manager(&input_manager_) {
}
