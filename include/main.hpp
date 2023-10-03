#pragma once

// beatsaber-hook is a modding framework that lets us call functions and fetch field values from in the game
// It also allows creating objects, configuration, and importantly, hooking methods to modify their values
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

// Includes for the functionality of the mod#
#include "GlobalNamespace/StandardLevelDetailView.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ClickableImage.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
#include "UnityEngine/Sprite.hpp"

// Include the modloader header, which allows us to tell the modloader which mod this is, and the version etc.
#include "modloader/shared/modloader.hpp"

#include "include/logger.hpp"
#include "include/WebUtils.hpp"
#include "include/icons.hpp"

#include <string>
#include <regex>

std::string beatsaverAPIUrl = "https://api.beatsaver.com/";

static SafePtrUnity<QuestUI::ClickableImage> button;
static SafePtrUnity<UnityEngine::Sprite> checkedSprite;
static SafePtrUnity<UnityEngine::Sprite> uncheckedSprite;

void setButtonStatus(bool isBookMarked);