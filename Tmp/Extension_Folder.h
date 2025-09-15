// C++17 or later
#include <filesystem>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <system_error>

namespace Extension_Folder_
{

	namespace fs = std::filesystem;

	struct MoveReport {
		std::size_t moved = 0;            // successfully moved
		std::size_t skipped = 0;          // non-image or not a regular file
		std::size_t collisions = 0;       // needed to generate a unique name
		std::size_t copied_fallback = 0;  // rename failed; did copy+remove
		std::size_t errors = 0;           // unrecoverable errors
		std::vector<std::string> error_details;
	};


	

	


	// Moves all images from `folder` into `subfolder` (relative to `folder` unless absolute).
	// Creates the whole subfolder path if needed.
	// Returns a report with counts and error details.
	MoveReport move_images_to_subfolder(const fs::path& folder, const fs::path& subfolder);
}
