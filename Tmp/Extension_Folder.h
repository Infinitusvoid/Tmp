// C++17 or later


#include <filesystem>
#include <string>
#include <vector>

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

    bool delete_folder_dangerous_skips_trash(const fs::path& dir);
    

}


/*
Usage example

int main() {
    // Example 1: move to a simple subfolder inside the source folder
    MoveReport r1 = move_images_to_subfolder(
        "C:/Users/Cosmos/Documents/GitHub/Tmp/Frames",
        "images"  // creates .../Frames/images if missing
    );

    // Example 2: move to a nested subfolder (will create the whole tree)
    MoveReport r2 = move_images_to_subfolder(
        "C:/Users/Cosmos/Documents/GitHub/Tmp/Frames",
        "archive/2025/09/15"
    );

    // Example 3: use an absolute destination (acts like “dump all images there”)
    MoveReport r3 = move_images_to_subfolder(
        "C:/Users/Cosmos/Documents/GitHub/Tmp/Frames",
        "D:/ImageArchive/2025/09/15"
    );

    auto print = [](const char* tag, const MoveReport& r){
        std::cout << tag << " — moved: " << r.moved
                  << ", skipped: " << r.skipped
                  << ", collisions: " << r.collisions
                  << ", copy-fallback: " << r.copied_fallback
                  << ", errors: " << r.errors << "\n";
        for (const auto& e : r.error_details) std::cout << "  - " << e << "\n";
    };

    print("r1", r1);
    print("r2", r2);
    print("r3", r3);
}



*/