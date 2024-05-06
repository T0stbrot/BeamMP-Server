// BeamMP, the BeamNG.drive multiplayer mod.
// Copyright (C) 2024 BeamMP Ltd., BeamMP team and contributors.
//
// BeamMP Ltd. can be contacted by electronic mail via contact@beammp.com.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "TResourceManager.h"

#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

TResourceManager::TResourceManager() {
    Application::SetSubsystemStatus("ResourceManager", Application::Status::Starting);
    std::string path = Application::Settings.Resource + "/Client";
    if (!fs::exists(path))
        fs::create_directories(path);

    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_regular_file()) {
            std::string file = entry.path().string();
            if (auto pos = file.find(".zip"); pos != std::string::npos && file.length() - pos == 4) {
                std::replace(file.begin(), file.end(), '\\', '/');
                mFileList += file + ';';

                std::string filename = entry.path().filename().string();
                std::string parent_directory = entry.path().parent_path().filename().string();
                if (parent_directory != "Client") {
                    mTrimmedList += "/" + parent_directory;
                }

                mTrimmedList += "/" + filename + ';';

                mFileSizes += std::to_string(size_t(fs::file_size(entry.path()))) + ';';
                mMaxModSize += size_t(fs::file_size(entry.path()));
                mModsLoaded++;
            }
        }
    }

    if (mModsLoaded) {
        beammp_info("Loaded " + std::to_string(mModsLoaded) + " Mods");
    }

    Application::SetSubsystemStatus("ResourceManager", Application::Status::Good);
}
