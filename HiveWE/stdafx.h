#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <unordered_set>
#include <bitset>
#include <set>
#include <memory>
#include <unordered_map>
#include <sstream>
#include <queue>
#include <string>
#include <variant>
#include <regex>
#include <random>
#include <cctype>
#include <optional>

using namespace std::literals::string_literals;
namespace fs = std::filesystem;

// Qt headers
#include <QtWidgets>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QSettings>
#include <FlowLayout.h>
#include <QDialogButtonBox>

// Dependencies
//#define GLM_FORCE_CXX14
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#ifdef _MSC_VER
#include <SOIL2.h>
#else
#include <SOIL/SOIL.h>
#endif
#include <turbojpeg.h>

// Custom Qt Widgets
#ifdef _DEBUG
#define QSCINTILLA_DLL
#endif
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercustom.h>
#include "ColorButton.h"
#include "JassEditor.h"

#undef _DLL
#include <StormLib.h>
#include <CascLib.h>

// Base Classes
#include "Quadtree.h"
#include "BinaryReader.h"
#include "BinaryWriter.h"
#include "InputHandler.h"
#include "WindowHandler.h"

// File formats
#include "MPQ.h"
#include "CASC.h"
#include "BLP.h"
#include "INI.h"
#include "SLK.h"
#include "MDX.h"
#include "JSON.h"

#include "Utilities.h"

// Resource types
#include "Hierarchy.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "GPUTexture.h"
#include "GroundTexture.h"
#include "CliffMesh.h"
#include "Shader.h"
#include "StaticMesh.h"
#include "SkinnedMesh.h"

#include "WidgetProperties.h"
#include "EditorWidgets.h"
#include "TriggerStrings.h"
#include "Triggers.h"
#include "Imports.h"
#include "Terrain.h"
#include "PathingMap.h"
#include "ShadowMap.h"
#include "Doodads.h"
#include "Units.h"
#include "Brush.h"
#include "PathingBrush.h"
#include "TerrainBrush.h"
#include "DoodadBrush.h"
#include "MapInfo.h"
#include "Map.h"

#include "Camera.h"
#include "HiveWE.h"
#include "GLWidget.h"
#include "QRibbon.h"

// Menus
#include "ObjectEditor.h"
#include "ImportManagerEdit.h"
#include "ImportManager.h"
#include "MapInfoEditor.h"
#include "TriggerEditor.h"
#include "PathingPallete.h"
#include "TerrainPalette.h"
#include "DoodadPalette.h"
#include "TilePicker.h"
#include "TileSetter.h"
#include "TilePather.h"