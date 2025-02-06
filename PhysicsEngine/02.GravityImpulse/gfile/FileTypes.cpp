//
// Created by gaetz on 29/08/2023.
//

#include "FileTypes.hpp"
#include "File.hpp"

namespace gfile {
    str FileTypePath(FileType fileType) {
        switch (fileType) {
            case FileType::Data: {
                return gfile::File::GetGameAssetsPath() + "/Data/";
            }
            case FileType::System: {
                return gfile::File::GetGameAssetsPath() + "/System/";
            }
            case FileType::Texture: {
                return gfile::File::GetGameAssetsPath() + "/Images/";
            }
            case FileType::Shader: {
                return gfile::File::GetGameAssetsPath() + "/Shaders/Compiled/";
            }
            case FileType::Model: {
                return gfile::File::GetGameAssetsPath() + "/Models/";
            }
            default: {
                return "";
            }
        }
    }
}