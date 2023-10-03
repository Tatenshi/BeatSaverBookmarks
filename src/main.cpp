#include "main.hpp"

MAKE_HOOK_MATCH(LevelRefreshContent, &GlobalNamespace::StandardLevelDetailView::RefreshContent, void, GlobalNamespace::StandardLevelDetailView* self) {
    LevelRefreshContent(self);

    // Load Bookmark checked sprite
    if(!checkedSprite){
        checkedSprite = QuestUI::BeatSaberUI::Base64ToSprite(bookmarkChecked);
    }

    // Load Bookmark unchecked sprite
    if(!uncheckedSprite){
        uncheckedSprite = QuestUI::BeatSaberUI::Base64ToSprite(bookmarkUnchecked);
    }

    // Create Bookmark Button
    if(!button || button.ptr() == NULL || !button.isAlive() || !button.isHandleValid())
    {
        auto canvas = QuestUI::BeatSaberUI::CreateCanvas();
        auto canvasTransform = (UnityEngine::RectTransform*) canvas->get_transform();
        canvasTransform->SetParent(self->get_transform(), false);
        canvasTransform->set_localScale({1, 1, 1});
        canvasTransform->set_sizeDelta({10, 10});
        canvasTransform->set_anchoredPosition({39, 29});

        button = QuestUI::BeatSaberUI::CreateClickableImage(canvasTransform, uncheckedSprite.ptr(), {0,0}, {5, 5});
    }

    // Disable Button until we have an info about the map
    button->get_gameObject()->SetActive(false);

    // Get the map hash
    std::string hash = std::regex_replace((std::string)reinterpret_cast<GlobalNamespace::IPreviewBeatmapLevel*>(self->level)->get_levelID(), std::basic_regex("custom_level_"), "");

    // We need to first request the hash, to get the map id
    // TODO find a better way to do this, so we dont have to make 2 requests
    WebUtils::GetJSONAsync(beatsaverAPIUrl + "maps/hash/" + hash, [hash](long status, bool error, rapidjson::Document const& result){
        getLogger().info("Network-Request to %s with status %ld", (beatsaverAPIUrl + hash).c_str(),status);
        if(status == 200 && !error){
            std::string id = result.GetObject()["id"].GetString();
            // Actually make the map request using the id
            WebUtils::GetJSONAsync(beatsaverAPIUrl + "maps/id/" + id, [hash, id](long status, bool error, rapidjson::Document const& result){
                // Check if user has bookmarked the map
                bool isBookMarked = result.GetObject()["bookmarked"].GetBool();
                // Shedule the Button update
                QuestUI::MainThreadScheduler::Schedule([id, isBookMarked]{
                    // Enable the Button again
                    button->get_gameObject()->SetActive(true);
                    // Rebind the onclick function, so that a click on the bookmark button changes the bookmark status on the server
                    button->get_onPointerClickEvent().clear();
                    button->get_onPointerClickEvent() += [id](auto _) {
                        auto data = R"({ "key": ")" +id+R"(", "bookmarked": )"+ ((button.ptr()->get_sprite() != checkedSprite.ptr()) ? "true" : "false") + " }";
                        WebUtils::PostJSONAsync(beatsaverAPIUrl + "bookmark", data, [data](long postStatus, std::string res){
                            if(postStatus == 200){
                                // If change was successful reflect change in the UI
                                QuestUI::MainThreadScheduler::Schedule([]{
                                    setButtonStatus(button.ptr()->get_sprite() != checkedSprite.ptr());
                                });
                            }
                            else {
                                getLogger().warning("Network-Request failed to %s with data %s with status %ld and res %s", (beatsaverAPIUrl + "bookmark").c_str(), data.c_str(),postStatus, res.c_str());
                            }
                        });
                    };
                    // Set the correct sprite
                    setButtonStatus(isBookMarked);
                });
            });
        }
        else {
            getLogger().warning("Network-Request failed to %s with status %ld", (beatsaverAPIUrl + hash).c_str(),status);
        }
    });
}

void setButtonStatus(bool isBookMarked){
    button->set_sprite(isBookMarked ? checkedSprite.ptr() : uncheckedSprite.ptr());
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo &info)
{
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo = info;

    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load()
{
    // Init things
    il2cpp_functions::Init();
    QuestUI::Init();

    // Register our Settings
    //QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);

    // Install Hooks
    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), LevelRefreshContent);
    getLogger().info("Installed all hooks!");
}