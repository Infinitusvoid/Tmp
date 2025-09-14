#pragma once

// GUniverse.h — single-file, C++17+
// Robust manager for "universes" folder with universe_<index>.h files and an activator header.
// - Monotonic indices via next_id.txt (won't reuse numbers if files are deleted).
// - Active universe tracked in active.txt and reflected in universes/universe.h.

#include <string>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <optional>
#include <regex>
#include <system_error>

class GUniverse {
public:
    explicit GUniverse(std::string universes_dir = "universes")
        : dir_(std::filesystem::path(universes_dir)),
        header_name_("universe.h"),
        file_prefix_("universe_"),
        file_ext_(".h"),
        next_id_file_("next_id.txt"),
        active_file_("active.txt")
    {
        ensure_dir_exists(dir_);

        // 1) Initialize next_id.txt if missing (based on max existing + 1)
        if (!exists(dir_ / next_id_file_)) {
            const int maxIdx = find_max_existing_index();
            write_int_file(dir_ / next_id_file_, maxIdx + 1);
        }

        // 2) Ensure at least one universe exists; if none, create 0 and activate it
        if (find_max_existing_index() < 0) {
            const int idx0 = acquire_next_id(); // returns 0 on fresh folder
            const auto p0 = universe_path(idx0);
            write_stub_universe(p0, idx0);
            write_int_file(dir_ / active_file_, idx0);
            write_activator_header(idx0);
        }

        // 3) Ensure activator header exists and points to a valid file
        if (!exists(dir_ / header_name_)) {
            const int active = read_or_fallback_active();
            write_activator_header(active);
        }
        else {
            // If header exists but points to missing file, repoint to highest existing
            const int pointed = parse_included_index_from_header(dir_ / header_name_).value_or(-1);
            if (pointed < 0 || !exists(universe_path(pointed))) {
                const int fallback = fallback_active_index();
                write_int_file(dir_ / active_file_, fallback);
                write_activator_header(fallback);
            }
        }
    }

    // Create a new universe by copying from the last one (prefer the last created; fallback to highest existing).
    // If none exist, creates an empty stub. Returns the NEW index.
    int create_new_universe_from_the_last() {
        const int newIdx = acquire_next_id();
        const auto dst = universe_path(newIdx);

        // Determine best source
        const int preferredSrc = peek_last_created_index(); // next_id - 1
        int srcIdx = -1;
        if (preferredSrc >= 0 && exists(universe_path(preferredSrc))) {
            srcIdx = preferredSrc;
        }
        else {
            const int mx = find_max_existing_index();
            if (mx >= 0) srcIdx = mx;
        }

        if (srcIdx >= 0) {
            copy_strict(universe_path(srcIdx), dst);
        }
        else {
            // No source available -> write stub
            write_stub_universe(dst, newIdx);
        }

        return newIdx;
    }

    // Create from a specific source index, activate the new one, and return its NEW index.
    int create_from_universe(int source_index) {
        const auto src = universe_path(source_index);
        if (!exists(src)) {
            throw std::runtime_error("Source universe does not exist: " + src.string());
        }

        const int newIdx = acquire_next_id();
        const auto dst = universe_path(newIdx);
        copy_strict(src, dst);

        // Activate the newly created one
        write_int_file(dir_ / active_file_, newIdx);
        write_activator_header(newIdx);

        return newIdx;
    }

    // Point universes/universe.h to universes/universe_<index>.h.
    void activate_universe_index(int index) {
        const auto target = universe_path(index);
        if (!exists(target)) {
            throw std::runtime_error("Cannot activate missing universe: " + target.string());
        }
        write_int_file(dir_ / active_file_, index);
        write_activator_header(index);
    }

    // Utils (optional)
    int active_index() const {
        return read_int_file(dir_ / active_file_).value_or(fallback_active_index());
    }
    int next_id_preview() const {
        return read_int_file(dir_ / next_id_file_).value_or(find_max_existing_index() + 1);
    }
    std::filesystem::path universe_path(int index) const {
        return dir_ / (file_prefix_ + std::to_string(index) + file_ext_);
    }

private:
    std::filesystem::path dir_;
    const std::string header_name_;
    const std::string file_prefix_;
    const std::string file_ext_;
    const std::string next_id_file_;
    const std::string active_file_;

    static bool exists(const std::filesystem::path& p) {
        std::error_code ec;
        return std::filesystem::exists(p, ec);
    }

    static void ensure_dir_exists(const std::filesystem::path& p) {
        std::error_code ec;
        if (!std::filesystem::exists(p, ec)) {
            if (!std::filesystem::create_directories(p, ec)) {
                throw std::runtime_error("Failed to create directory: " + p.string());
            }
        }
    }

    // -------- Monotonic next id handling --------
    int acquire_next_id() {
        const auto f = dir_ / next_id_file_;
        int next = read_int_file(f).value_or(find_max_existing_index() + 1);
        // Sanity: never go backwards
        if (next <= find_max_existing_index()) {
            next = find_max_existing_index() + 1;
        }
        // Reserve this id and bump the file
        write_int_file(f, next + 1);
        return next;
    }

    int peek_last_created_index() const {
        const int next = read_int_file(dir_ / next_id_file_).value_or(find_max_existing_index() + 1);
        return next - 1;
    }

    // -------- File IO helpers --------
    static void write_file(const std::filesystem::path& p, const std::string& content) {
        std::ofstream ofs(p, std::ios::out | std::ios::trunc);
        if (!ofs) throw std::runtime_error("Failed to open for write: " + p.string());
        ofs << content;
        if (!ofs.good()) throw std::runtime_error("Write failed: " + p.string());
    }

    static void copy_strict(const std::filesystem::path& src, const std::filesystem::path& dst) {
        if (!exists(src)) throw std::runtime_error("Source does not exist: " + src.string());
        if (exists(dst))   throw std::runtime_error("Destination already exists: " + dst.string());
        std::error_code ec;
        if (!std::filesystem::copy_file(src, dst, std::filesystem::copy_options::none, ec)) {
            throw std::runtime_error("Copy failed \"" + src.string() + "\" -> \"" + dst.string() + "\": " + ec.message());
        }
    }

    static std::optional<int> read_int_file(const std::filesystem::path& p) {
        if (!exists(p)) return std::nullopt;
        std::ifstream ifs(p);
        if (!ifs) return std::nullopt;
        std::string s; std::getline(ifs, s);
        if (!ifs.good() && !ifs.eof()) return std::nullopt;
        try {
            // Trim whitespace
            const auto trimmed = trim(s);
            if (trimmed.empty()) return std::nullopt;
            size_t pos = 0;
            int v = std::stoi(trimmed, &pos);
            return v;
        }
        catch (...) {
            return std::nullopt;
        }
    }

    static void write_int_file(const std::filesystem::path& p, int value) {
        write_file(p, std::to_string(value) + "\n");
    }

    // -------- Universe header (activator) --------
    void write_activator_header(int active_idx) const {
        // The activator lives inside universes/ and is intended to be included as: #include "universes/universe.h"
        // It points to "universes/universe_<active>.h" with a pragma once for safety.
        std::ostringstream oss;
        oss << "// Auto-generated. Do not edit by hand.\n"
            << "#pragma once\n"
            << "#include \"universes/" << file_prefix_ << active_idx << file_ext_ << "\"\n";
        write_file(dir_ / header_name_, oss.str());
    }

    // Attempts to read which universe index the header includes.
    static std::optional<int> parse_included_index_from_header(const std::filesystem::path& header_path) {
        if (!exists(header_path)) return std::nullopt;
        std::ifstream ifs(header_path);
        if (!ifs) return std::nullopt;
        std::string line;
        std::regex r(R"(#include\s*\"universes\/universe_(\d+)\.h\")");
        while (std::getline(ifs, line)) {
            std::smatch m;
            if (std::regex_search(line, m, r)) {
                try { return std::stoi(m[1].str()); }
                catch (...) { return std::nullopt; }
            }
        }
        return std::nullopt;
    }

    // -------- Initial stub for a new universe file --------
    static void write_stub_universe(const std::filesystem::path& p, int idx) {
        std::ostringstream oss;
        oss << "// universe_" << idx << ".h — auto-generated stub\n"
            << "#pragma once\n\n"
            << "// Put your universe " << idx << " content here.\n"
            << "// This file is safe to include via universes/universe.h\n";
        write_file(p, oss.str());
    }

    // -------- Scanning helpers --------
    int find_max_existing_index() const {
        if (!exists(dir_)) return -1;
        int best = -1;
        std::regex pat(R"(^universe_(\d+)\.h$)");
        std::error_code ec;
        for (const auto& e : std::filesystem::directory_iterator(dir_, ec)) {
            if (ec) break;
            if (!e.is_regular_file()) continue;
            const auto name = e.path().filename().string();
            std::smatch m;
            if (std::regex_match(name, m, pat)) {
                try {
                    int i = std::stoi(m[1].str());
                    if (i > best) best = i;
                }
                catch (...) {}
            }
        }
        return best;
    }

    int read_or_fallback_active() const {
        if (auto a = read_int_file(dir_ / active_file_)) {
            if (exists(universe_path(*a))) return *a;
        }
        return fallback_active_index();
    }

    int fallback_active_index() const {
        int mx = find_max_existing_index();
        if (mx < 0) {
            // Extremely unlikely here because ctor creates index 0 when empty,
            // but keep this as a final guard.
            const int idx0 = const_cast<GUniverse*>(this)->acquire_next_id();
            const auto p0 = universe_path(idx0);
            write_stub_universe(p0, idx0);
            return idx0;
        }
        return mx;
    }

    // -------- tiny utils --------
    static std::string trim(const std::string& s) {
        const auto issp = [](unsigned char c) { return std::isspace(c); };
        size_t b = 0, e = s.size();
        while (b < e && issp(static_cast<unsigned char>(s[b]))) ++b;
        while (e > b && issp(static_cast<unsigned char>(s[e - 1]))) --e;
        return s.substr(b, e - b);
    }
};
