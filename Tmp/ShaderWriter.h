#pragma once

// ShaderWriter.h — single-file, C++17+
// Creates/duplicates GLSL shader pairs in a "shaders" folder.

#pragma once
#include <string>
#include <stdexcept>
#include <filesystem>
#include <regex>
#include <optional>
#include <fstream>
#include <utility>   // std::pair



class ShaderWriter
{
public:
    struct CreatedPair {
        int index;       // created index (e.g., 0, 1, 2, ...)
        std::filesystem::path vertex; // shaders/vertex_<index>.glsl
        std::filesystem::path fragment;
    };

    explicit ShaderWriter(std::string shader_dir = "shaders")
        : dir_(std::filesystem::path(shader_dir))
    {
        ensure_dir_exists(dir_);
    }

    // --- 1) Create new from last ---
    // If no pairs exist, auto-initialize by creating EMPTY vertex_0 / fragment_0, and return index 0.
    CreatedPair create_new_shader_from_last() const {
        if (auto none = find_last_pair(); !none.has_value()) {
            // Folder empty (no full pair) -> create empty base pair at index 0
            return create_empty_pair(0);
        }

        const int lastIdx = find_last_pair()->first;
        const int newIdx = lastIdx + 1;

        const std::filesystem::path vSrc = vertex_path(lastIdx);
        const std::filesystem::path fSrc = fragment_path(lastIdx);
        const std::filesystem::path vDst = vertex_path(newIdx);
        const std::filesystem::path fDst = fragment_path(newIdx);

        copy_strict(vSrc, vDst);
        copy_strict(fSrc, fDst);

        return { newIdx, vDst, fDst };
    }

    // --- 2) Create new from template_K ---
    // Copies vertex_template_K / fragment_template_K into next index after last (or index 0 if none).
    CreatedPair create_new_from_template(int template_index) const {
        const int newIdx = next_index_after_last();

        const std::filesystem::path vTpl = vertex_template_path(template_index);
        const std::filesystem::path fTpl = fragment_template_path(template_index);
        if (!std::filesystem::exists(vTpl)) throw std::runtime_error("Missing template: " + vTpl.string());
        if (!std::filesystem::exists(fTpl)) throw std::runtime_error("Missing template: " + fTpl.string());

        const std::filesystem::path vDst = vertex_path(newIdx);
        const std::filesystem::path fDst = fragment_path(newIdx);

        copy_strict(vTpl, vDst);
        copy_strict(fTpl, fDst);

        return { newIdx, vDst, fDst };
    }

    // --- 3) Create new from shader_index (explicit source) ---
    // Copies vertex_<source>/fragment_<source> into next index after last (or index 0 if none).
    CreatedPair create_new_from_shader_index(int source_index) const {
        const std::filesystem::path vSrc = vertex_path(source_index);
        const std::filesystem::path fSrc = fragment_path(source_index);
        if (!std::filesystem::exists(vSrc)) throw std::runtime_error("Missing source shader: " + vSrc.string());
        if (!std::filesystem::exists(fSrc)) throw std::runtime_error("Missing source shader: " + fSrc.string());

        const int newIdx = next_index_after_last();
        const std::filesystem::path vDst = vertex_path(newIdx);
        const std::filesystem::path fDst = fragment_path(newIdx);

        copy_strict(vSrc, vDst);
        copy_strict(fSrc, fDst);

        return { newIdx, vDst, fDst };
    }

    // Optional: explicitly force an empty base if you want to call it yourself.
    CreatedPair initialize_if_empty() const {
        if (find_last_pair().has_value()) {
            const auto lastIdx = find_last_pair()->first;
            throw std::runtime_error(
                "Folder already initialized (last pair index " + std::to_string(lastIdx) + ").");
        }
        return create_empty_pair(0);
    }

private:
    std::filesystem::path dir_;
    const std::string vertexPrefix_ = "vertex_";
    const std::string fragmentPrefix_ = "fragment_";
    const std::string tplVPrefix_ = "vertex_template_";
    const std::string tplFPrefix_ = "fragment_template_";
    const std::string ext_ = ".glsl";

    static void ensure_dir_exists(const std::filesystem::path& p) {
        std::error_code ec;
        if (!std::filesystem::exists(p, ec)) {
            if (!std::filesystem::create_directories(p, ec)) {
                throw std::runtime_error("Failed to create directory: " + p.string());
            }
        }
    }

    std::filesystem::path vertex_path(int index) const { return dir_ / (vertexPrefix_ + std::to_string(index) + ext_); }
    std::filesystem::path fragment_path(int index) const { return dir_ / (fragmentPrefix_ + std::to_string(index) + ext_); }
    std::filesystem::path vertex_template_path(int i) const { return dir_ / (tplVPrefix_ + std::to_string(i) + ext_); }
    std::filesystem::path fragment_template_path(int i) const { return dir_ / (tplFPrefix_ + std::to_string(i) + ext_); }

    static void copy_strict(const std::filesystem::path& src, const std::filesystem::path& dst) {
        std::error_code ec;
        if (!std::filesystem::exists(src)) throw std::runtime_error("Source does not exist: " + src.string());
        if (!std::filesystem::copy_file(src, dst, std::filesystem::copy_options::none, ec)) {
            if (ec) {
                throw std::runtime_error("Copy failed \"" + src.string() + "\" -> \"" + dst.string() + "\": " + ec.message());
            }
            throw std::runtime_error("Destination already exists: " + dst.string());
        }
    }

    // Create EMPTY files for a new pair (index). Throws if destinations already exist.
    CreatedPair create_empty_pair(int index) const {
        const std::filesystem::path vDst = vertex_path(index);
        const std::filesystem::path fDst = fragment_path(index);
        if (std::filesystem::exists(vDst)) throw std::runtime_error("File already exists: " + vDst.string());
        if (std::filesystem::exists(fDst)) throw std::runtime_error("File already exists: " + fDst.string());

        // Create empty files (zero-byte). If you want a stub, write content here.
        {
            std::ofstream vFile(vDst, std::ios::out | std::ios::trunc);
            if (!vFile) throw std::runtime_error("Failed to create: " + vDst.string());
        }
        {
            std::ofstream fFile(fDst, std::ios::out | std::ios::trunc);
            if (!fFile) throw std::runtime_error("Failed to create: " + fDst.string());
        }

        return { index, vDst, fDst };
    }

    // Returns highest index N where BOTH files exist; else nullopt.
    std::optional<std::pair<int, std::pair<std::filesystem::path, std::filesystem::path>>> find_last_pair() const {
        std::regex vertexPattern("^vertex_(\\d+)\\.glsl$", std::regex::ECMAScript);
        int best = -1;

        std::error_code ec;
        if (!std::filesystem::exists(dir_, ec)) return std::nullopt;

        for (const auto& entry : std::filesystem::directory_iterator(dir_, ec)) {
            if (ec) break;
            if (!entry.is_regular_file()) continue;

            const auto name = entry.path().filename().string();
            std::smatch m;
            if (std::regex_match(name, m, vertexPattern)) {
                int idx = std::stoi(m[1].str());
                if (std::filesystem::exists(vertex_path(idx)) && std::filesystem::exists(fragment_path(idx))) {
                    if (idx > best) best = idx;
                }
            }
        }

        if (best >= 0)
        {
            return std::make_optional(std::make_pair(best, std::make_pair(vertex_path(best), fragment_path(best))));
        }

        return std::nullopt;
    }

    // Next index after last complete pair; if none, return 0.
    int next_index_after_last() const {
        if (auto last = find_last_pair(); last.has_value()) return last->first + 1;
        return 0;
    }
};
