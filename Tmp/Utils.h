#pragma once

#include <filesystem>
#include <string>
#include <system_error>   // std::error_code
#include <cstdint>        // std::uintmax_t

namespace File_
{
    namespace fs = std::filesystem;

    struct CopyResult {
        bool ok = false;
        std::uintmax_t bytes = 0;
        std::string message;
    };

    CopyResult copy_file_safely(const std::string& srcPathStr,
        const std::string& dstPathStr,
        bool overwrite = false)
    {
        CopyResult r{};
        std::error_code ec;

        fs::path src(srcPathStr);
        fs::path dst(dstPathStr);

        // Basic validation
        if (src.empty() || dst.empty()) {
            r.message = "Source or destination path is empty.";
            return r;
        }
        if (fs::equivalent(src, dst, ec)) {
            r.message = "Source and destination refer to the same path.";
            return r;
        }

        // Source must exist and be a regular file
        if (!fs::exists(src)) {
            r.message = "Source file does not exist: " + src.string();
            return r;
        }
        if (!fs::is_regular_file(src)) {
            r.message = "Source is not a regular file: " + src.string();
            return r;
        }

        // If destination is an existing directory, copy "into" it
        if (fs::exists(dst) && fs::is_directory(dst)) {
            dst /= src.filename();
        }

        // Ensure destination directory exists
        if (auto parent = dst.parent_path(); !parent.empty()) {
            fs::create_directories(parent, ec);
            if (ec) {
                r.message = "Failed to create destination directories: " + parent.string() +
                    " (" + ec.message() + ")";
                return r;
            }
        }

        // Choose copy options
        auto options = overwrite ? fs::copy_options::overwrite_existing
            : fs::copy_options::none;

        // Perform copy (copy_file only copies file contents/perm bits)
        bool didCopy = fs::copy_file(src, dst, options, ec);
        if (!didCopy || ec) {
            // If not overwriting, and file exists, explain clearly
            if (!overwrite && fs::exists(dst)) {
                r.message = "Destination already exists (set overwrite=true to replace): " + dst.string();
                return r;
            }
            r.message = "Copy failed: " + (ec ? ec.message() : std::string("unknown error"));
            return r;
        }

        // Success
        r.ok = true;
        r.bytes = fs::file_size(src, ec);
        r.message = "Copied to: " + dst.string();
        return r;
    }
}