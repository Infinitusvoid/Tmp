#include "Extension_Folder.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <sstream>
#include <system_error>

namespace Extension_Folder_
{
	std::string to_lower(std::string s) {
		std::transform(s.begin(), s.end(), s.begin(),
			[](unsigned char c) { return static_cast<char>(std::tolower(c)); });
		return s;
	}

	bool is_image_ext(const fs::path& p) {
		const std::string ext = to_lower(p.extension().string());  // includes leading '.'
		return (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp");
	}

	fs::path unique_target_path(const fs::path& target) {
		if (!fs::exists(target)) return target;
		fs::path parent = target.parent_path();
		std::string stem = target.stem().string();
		std::string ext = target.extension().string();
		std::size_t i = 1;
		fs::path candidate;
		do {
			std::ostringstream oss;
			oss << stem << " (" << i++ << ")" << ext;
			candidate = parent / oss.str();
		} while (fs::exists(candidate));
		return candidate;
	}

	MoveReport move_images_to_subfolder(const fs::path& folder, const fs::path& subfolder)
	{
		MoveReport report;

		// 1) Basic checks
		if (!fs::exists(folder)) {
			report.errors = 1;
			report.error_details.push_back("Source folder does not exist: " + folder.string());
			return report;
		}
		if (!fs::is_directory(folder)) {
			report.errors = 1;
			report.error_details.push_back("Source path is not a directory: " + folder.string());
			return report;
		}

		// 2) Resolve destination:
		//    - If `subfolder` is absolute, use it as-is.
		//    - Otherwise, treat it as a subpath of `folder` (supports nested like "sorted/2025/09").
		fs::path dest = subfolder.is_absolute() ? subfolder : (folder / subfolder);

		// 3) Ensure destination exists (create all missing parts)
		std::error_code ec;
		fs::create_directories(dest, ec);
		if (ec) {
			report.errors = 1;
			report.error_details.push_back("Failed to create destination: " + dest.string() + "  " + ec.message());
			return report;
		}

		// 4) Iterate files in source directory (non-recursive)
		for (const auto& entry : fs::directory_iterator(folder)) {
			if (!entry.is_regular_file()) { report.skipped++; continue; }

			const fs::path src = entry.path();
			if (!is_image_ext(src)) { report.skipped++; continue; }

			// Prepare target path (avoid name collisions)
			fs::path target = dest / src.filename();
			if (fs::exists(target)) {
				target = unique_target_path(target);
				report.collisions++;
			}

			// Try rename (fast move). If it fails (e.g., across drives), copy+remove as fallback.
			ec.clear();
			fs::rename(src, target, ec);
			if (ec) {
				// Fallback: copy then remove
				std::error_code ec_copy;
				fs::copy_file(src, target, fs::copy_options::overwrite_existing, ec_copy);
				if (ec_copy) {
					report.errors++;
					std::ostringstream msg;
					msg << "Failed to move: " << src.string() << "  " << target.string()
						<< "  rename error: " << ec.message()
						<< ", copy error: " << ec_copy.message();
					report.error_details.push_back(msg.str());
					continue;
				}
				std::error_code ec_remove;
				fs::remove(src, ec_remove);
				if (ec_remove) {
					report.errors++;
					std::ostringstream msg;
					msg << "Copied but failed to delete source: " << src.string()
						<< "  " << ec_remove.message();
					report.error_details.push_back(msg.str());
					continue;
				}
				report.copied_fallback++;
				report.moved++;
			}
			else {
				report.moved++;
			}
		}

		return report;
	}

}