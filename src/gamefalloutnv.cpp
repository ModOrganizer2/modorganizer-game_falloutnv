#include "gamefalloutnv.h"

#include "falloutnvbsainvalidation.h"
#include "falloutnvdataarchives.h"
#include "falloutnvscriptextender.h"
#include "falloutnvmoddatachecker.h"
#include "falloutnvmoddatacontent.h"
#include "falloutnvsavegame.h"

#include "executableinfo.h"
#include "pluginsetting.h"
#include "versioninfo.h"
#include <gamebryolocalsavegames.h>
#include <gamebryogameplugins.h>
#include <gamebryosavegameinfo.h>
#include <gamebryounmanagedmods.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

#include <memory>

using namespace MOBase;

GameFalloutNV::GameFalloutNV()
{
}

bool GameFalloutNV::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }
  registerFeature<ScriptExtender>(new FalloutNVScriptExtender(this));
  registerFeature<DataArchives>(new FalloutNVDataArchives(myGamesPath()));
  registerFeature<BSAInvalidation>(new FalloutNVBSAInvalidation(feature<DataArchives>(), this));
  registerFeature<SaveGameInfo>(new GamebryoSaveGameInfo(this));
  registerFeature<LocalSavegames>(new GamebryoLocalSavegames(myGamesPath(), "fallout.ini"));
  registerFeature<ModDataChecker>(new FalloutNVModDataChecker(this));
  registerFeature<ModDataContent>(new FalloutNVModDataContent(this));
  registerFeature<GamePlugins>(new GamebryoGamePlugins(moInfo));
  registerFeature<UnmanagedMods>(new GamebryoUnmangedMods(this));
  return true;
}

QString GameFalloutNV::gameName() const
{
  return "New Vegas";
}

void GameFalloutNV::detectGame()
{
  m_GamePath = identifyGamePath();
  m_MyGamesPath = determineMyGamesPath("FalloutNV");
}

QList<ExecutableInfo> GameFalloutNV::executables() const
{
  return QList<ExecutableInfo>()
    << ExecutableInfo("NVSE", findInGameFolder(feature<ScriptExtender>()->loaderName()))
    << ExecutableInfo("New Vegas", findInGameFolder(binaryName()))
    << ExecutableInfo("Construction Kit", findInGameFolder("geck.exe"))
    << ExecutableInfo("Fallout Launcher", findInGameFolder(getLauncherName()))
    << ExecutableInfo("LOOT", QFileInfo(getLootPath())).withArgument("--game=\"FalloutNV\"");
}

QList<ExecutableForcedLoadSetting> GameFalloutNV::executableForcedLoads() const
{
  return QList<ExecutableForcedLoadSetting>();
}

QString GameFalloutNV::name() const
{
  return "Fallout NV Support Plugin";
}

QString GameFalloutNV::localizedName() const
{
  return tr("Fallout NV Support Plugin");
}


QString GameFalloutNV::author() const
{
  return "Tannin";
}

QString GameFalloutNV::description() const
{
  return tr("Adds support for the game Fallout New Vegas");
}

MOBase::VersionInfo GameFalloutNV::version() const
{
  return VersionInfo(1, 5, 1, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameFalloutNV::settings() const
{
  return QList<PluginSetting>();
}

void GameFalloutNV::initializeProfile(const QDir &path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + "/FalloutNV", path, "plugins.txt");
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS)
        || !QFileInfo(myGamesPath() + "/fallout.ini").exists()) {
      copyToProfile(gameDirectory().absolutePath(), path, "fallout_default.ini", "fallout.ini");
    } else {
      copyToProfile(myGamesPath(), path, "fallout.ini");
    }

    copyToProfile(myGamesPath(), path, "falloutprefs.ini");
	  copyToProfile(myGamesPath(), path, "falloutcustom.ini");
    copyToProfile(myGamesPath(), path, "GECKCustom.ini");
    copyToProfile(myGamesPath(), path, "GECKPrefs.ini");

  }
}

QString GameFalloutNV::savegameExtension() const
{
  return "fos";
}

QString GameFalloutNV::savegameSEExtension() const
{
  return "nvse";
}

std::shared_ptr<const GamebryoSaveGame> GameFalloutNV::makeSaveGame(QString filePath) const
{
  return std::make_shared<const FalloutNVSaveGame>(filePath, this);
}

QString GameFalloutNV::steamAPPId() const
{
  return "22380";
}

QStringList GameFalloutNV::primaryPlugins() const
{
  return { "falloutnv.esm" };
}

QString GameFalloutNV::gameShortName() const
{
  return "FalloutNV";
}

QStringList GameFalloutNV::validShortNames() const
{
  return { "Fallout3" };
}

QString GameFalloutNV::gameNexusName() const
{
  return "newvegas";
}

QStringList GameFalloutNV::iniFiles() const
{
  return { "fallout.ini", "falloutprefs.ini", "falloutcustom.ini", "GECKCustom.ini", "GECKPrefs.ini"};
}

QStringList GameFalloutNV::DLCPlugins() const
{
  return { "DeadMoney.esm", "HonestHearts.esm", "OldWorldBlues.esm",
           "LonesomeRoad.esm", "GunRunnersArsenal.esm", "CaravanPack.esm",
           "ClassicPack.esm", "MercenaryPack.esm", "TribalPack.esm" };
}

int GameFalloutNV::nexusModOrganizerID() const
{
  return 42572;
}

int GameFalloutNV::nexusGameID() const
{
  return 130;
}
