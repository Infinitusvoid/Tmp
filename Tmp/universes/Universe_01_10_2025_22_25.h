
#include "../vibes/Vibe_01_10_2025_11_21.h"

#include <unordered_map>
#include <limits>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>

namespace Universe_
{
	using namespace Vibe_01_10_2025_11_21_;

	
	struct Vec3_StartEnd
	{
		Vec3 start;
		Vec3 end;
	};

	
	
	void add_sphere
	(
		Spheres_shader_20& spheres_shader_20,
		Vec3_StartEnd position,
		Float_start_end radius,
		Vec3_StartEnd color,
		Float_start_end cube_size, 
		Float_start_end x_rnd_min,
		Float_start_end x_rnd_max,
		Float_start_end y_rnd_min,
		Float_start_end y_rnd_max, 
		Float_start_end thickness,
		Float_start_end jitter
	)
	{
		Spheres_shader_20::Sphere sphere;

		sphere.radius = radius;

		sphere.start_position.x = position.start.x - radius.start * 0.5f;
		sphere.start_position.y = position.start.y - radius.start * 0.5f;
		sphere.start_position.z = position.start.z - radius.start * 0.5f;

		sphere.end_position.x = position.end.x - radius.end * 0.5f;
		sphere.end_position.y = position.end.y - radius.end * 0.5f;
		sphere.end_position.z = position.end.z - radius.end * 0.5f;

		sphere.cube_size = cube_size;

		sphere.x_rnd_min = x_rnd_min;
		sphere.x_rnd_max = x_rnd_max;

		sphere.y_rnd_min = y_rnd_min;
		sphere.y_rnd_max = y_rnd_max;

		sphere.start_color = color.start;
		sphere.end_color = color.end;

		sphere.thickness = thickness;
		sphere.jitter = jitter;

		spheres_shader_20.spheres.push_back(std::move(sphere));
		
	}
	
	







// This header builds a compact, grid-based, line-segment vector font.
// Goals:
//  - Every glyph fits in the [0,1]x[0,1] box (post-normalization).
//  - Full ASCII coverage (letters, digits, common punctuation).
//  - Extensible builder: compose new letters from reusable components (e.g., caron, ring, acute).
//  - Keep compatibility with your existing ASCI_Letters::Line2d and map<int, std::vector<Line2d>> style.
//
// Usage:
//   auto font = ASCI_Letters_::make_full_ascii_stick_font();
//   // render each glyph's segments in [0,1]x[0,1]
//
// To extend (e.g., add , , ): see the examples near the end of make_full_ascii_stick_font().
// The builder composes base Latin letters with accent components, then normalizes.

	namespace ASCI_Letters_
	{
		// ---- Assume these exist in your project ----
		struct Vec2 { float x, y; };
		struct ASCI_Letters
		{
			struct Line2d { Vec2 start, end; };
			// Map from Unicode code point to stroke list. For ASCII you can use 'A'..'Z' etc.
			// For non-ASCII, use the Unicode code point (e.g., 0x010C for ).
			std::unordered_map<uint32_t, std::vector<Line2d>> letters;
		};

		// ---- Metrics & helpers ----
		struct Bounds
		{
			float minX = +std::numeric_limits<float>::infinity();
			float minY = +std::numeric_limits<float>::infinity();
			float maxX = -std::numeric_limits<float>::infinity();
			float maxY = -std::numeric_limits<float>::infinity();
		};

		inline Bounds compute_bounds(const std::vector<ASCI_Letters::Line2d>& segs)
		{
			Bounds b;
			if (segs.empty()) { b.minX = b.minY = 0.0f; b.maxX = b.maxY = 0.0f; return b; }
			for (const auto& s : segs)
			{
				b.minX = std::min({ b.minX, s.start.x, s.end.x });
				b.minY = std::min({ b.minY, s.start.y, s.end.y });
				b.maxX = std::max({ b.maxX, s.start.x, s.end.x });
				b.maxY = std::max({ b.maxY, s.start.y, s.end.y });
			}
			return b;
		}

		inline void translate(std::vector<ASCI_Letters::Line2d>& segs, float dx, float dy)
		{
			for (auto& s : segs) { s.start.x += dx; s.end.x += dx; s.start.y += dy; s.end.y += dy; }
		}

		inline void scale(std::vector<ASCI_Letters::Line2d>& segs, float sx, float sy)
		{
			for (auto& s : segs) { s.start.x *= sx; s.end.x *= sx; s.start.y *= sy; s.end.y *= sy; }
		}

		inline void append(std::vector<ASCI_Letters::Line2d>& dst,
			const std::vector<ASCI_Letters::Line2d>& src)
		{
			dst.insert(dst.end(), src.begin(), src.end());
		}

		inline float saturate(float v) { return v < 0.f ? 0.f : (v > 1.f ? 1.f : v); }

		// Normalize each glyph into [0,1]^2, uniformly scaled + centered, then padded slightly.
		inline void normalize_letters(ASCI_Letters& letters, float pad = 0.02f)
		{
			constexpr float EPS = 1e-9f;
			for (auto& kv : letters.letters)
			{
				auto& segs = kv.second;
				if (segs.empty()) continue;
				auto b = compute_bounds(segs);
				float w = std::max(b.maxX - b.minX, EPS);
				float h = std::max(b.maxY - b.minY, EPS);
				// translate to origin
				translate(segs, -b.minX, -b.minY);
				// uniform scale to fit (1 - 2*pad)
				float box = 1.f - 2.f * pad;
				float s = box / std::max(w, h);
				scale(segs, s, s);
				// recalc bounds, then center inside [pad,1-pad]
				auto nb = compute_bounds(segs);
				float offX = pad + 0.5f * (box - (nb.maxX - nb.minX));
				float offY = pad + 0.5f * (box - (nb.maxY - nb.minY));
				translate(segs, offX - nb.minX, offY - nb.minY);
				// clamp to obliterate tiny FP overshoot
				for (auto& sgm : segs)
				{
					sgm.start.x = saturate(sgm.start.x); sgm.start.y = saturate(sgm.start.y);
					sgm.end.x = saturate(sgm.end.x); sgm.end.y = saturate(sgm.end.y);
				}
			}
		}

		// ---- Grid builder ----
		struct Grid {
			int W = 5; // columns -> x in [0..W-1]
			int H = 7; // rows    -> y in [0..H-1]
			// Convert integer grid to pre-normalized 0..1 space (we still normalize later for uniformity/padding)
			inline float nx(int x) const { return (W <= 1) ? 0.f : float(x) / float(W - 1); }
			inline float ny(int y) const { return (H <= 1) ? 0.f : float(y) / float(H - 1); }
			void seg(std::vector<ASCI_Letters::Line2d>& out, int x1, int y1, int x2, int y2) const
			{
				out.push_back({ Vec2{nx(x1), ny(y1)}, Vec2{nx(x2), ny(y2)} });
			}
		};

		// ---- Accent components (reusable) in grid space ----
		struct Components {
			std::unordered_map<std::string, std::vector<ASCI_Letters::Line2d>> comp;

			void build(const Grid& g)
			{
				using L = ASCI_Letters::Line2d;
				// caron (hek) as small V shape
				{
					std::vector<L> c; g.seg(c, 1, 6, 2, 7); g.seg(c, 2, 7, 3, 6); comp["caron"] = c; // assumes H>=8
				}
				// acute accent
				{
					std::vector<L> c; g.seg(c, 2, 6, 3, 7); comp["acute"] = c;
				}
				// grave
				{ std::vector<L> c; g.seg(c, 2, 7, 3, 6); comp["grave"] = c; }
				// circumflex (small ^)
				{ std::vector<L> c; g.seg(c, 1, 6, 2, 7); g.seg(c, 2, 7, 3, 6); comp["circ"] = c; }
				// dieresis (two short ticks)
				{ std::vector<L> c; g.seg(c, 1, 7, 1, 7); g.seg(c, 3, 7, 3, 7); comp["dieresis"] = c; }
				// ring above (small diamond approximating a circle)
				{ std::vector<L> c; g.seg(c, 2, 7, 1, 6); g.seg(c, 1, 6, 2, 5); g.seg(c, 2, 5, 3, 6); g.seg(c, 3, 6, 2, 7); comp["ring"] = c; }
				// ogonek (tail below)
				{ std::vector<L> c; g.seg(c, 2, 0, 1, -1); g.seg(c, 1, -1, 2, -2); comp["ogonek"] = c; }
				// slash (for )
				{ std::vector<L> c; g.seg(c, 0, 1, 4, 5); comp["slash_diag"] = c; }
			}
		};

		// Place a component relative to base glyph bounds.
		enum class AttachWhere { TopCenter, BottomCenter, Center, ThroughDiag };

		inline std::vector<ASCI_Letters::Line2d>
			place_component(const std::vector<ASCI_Letters::Line2d>& base,
				const std::vector<ASCI_Letters::Line2d>& comp,
				AttachWhere where,
				float scale_rel = 1.0f,   // scale relative to base height
				float y_gap = 0.02f)      // extra gap for accents
		{
			if (comp.empty()) return {};
			auto bb = compute_bounds(base);
			auto cb = compute_bounds(comp);
			float bw = std::max(1e-6f, bb.maxX - bb.minX);
			float bh = std::max(1e-6f, bb.maxY - bb.minY);
			float cw = std::max(1e-6f, cb.maxX - cb.minX);
			float ch = std::max(1e-6f, cb.maxY - cb.minY);

			// scale comp to a fraction of base height
			float s = scale_rel * (bh / ch);
			std::vector<ASCI_Letters::Line2d> out = comp;
			// move comp to origin
			translate(out, -cb.minX, -cb.minY);
			scale(out, s, s);
			auto cnb = compute_bounds(out);

			float tx = 0.f, ty = 0.f;
			switch (where)
			{
			case AttachWhere::TopCenter:
				tx = bb.minX + 0.5f * bw - 0.5f * (cnb.maxX - cnb.minX);
				ty = bb.maxY + y_gap;
				break;
			case AttachWhere::BottomCenter:
				tx = bb.minX + 0.5f * bw - 0.5f * (cnb.maxX - cnb.minX);
				ty = bb.minY - (cnb.maxY - cnb.minY) - y_gap;
				break;
			case AttachWhere::Center:
				tx = bb.minX + 0.5f * bw - 0.5f * (cnb.maxX - cnb.minX);
				ty = bb.minY + 0.5f * bh - 0.5f * (cnb.maxY - cnb.minY);
				break;
			case AttachWhere::ThroughDiag:
				// For : roughly align so slash spans the bowl
				tx = 0.f; ty = 0.f; // comp already designed to span
				break;
			}
			translate(out, tx, ty);
			return out;
		}

		// ---- Builders for base glyphs ----
		inline void build_letters_A_to_Z(std::unordered_map<uint32_t, std::vector<ASCI_Letters::Line2d>>& M, const Grid& g)
		{
			using L = ASCI_Letters::Line2d;

			// SPACE (U+0020)
			M[' '] = {};

			// A
			{ auto& v = M['A']; g.seg(v, 0, 0, 2, 6); g.seg(v, 4, 0, 2, 6); g.seg(v, 1, 3, 3, 3); }
			// B
			{ auto& v = M['B']; g.seg(v, 0, 0, 0, 6); g.seg(v, 0, 6, 3, 6); g.seg(v, 3, 6, 4, 5); g.seg(v, 4, 5, 3, 4); g.seg(v, 3, 4, 0, 4); g.seg(v, 0, 4, 0, 0); g.seg(v, 0, 0, 3, 0); g.seg(v, 3, 0, 4, 1); g.seg(v, 4, 1, 3, 2); g.seg(v, 3, 2, 0, 2); }
			// C
			{ auto& v = M['C']; g.seg(v, 4, 5, 3, 6); g.seg(v, 3, 6, 1, 6); g.seg(v, 1, 6, 0, 5); g.seg(v, 0, 5, 0, 1); g.seg(v, 0, 1, 1, 0); g.seg(v, 1, 0, 3, 0); g.seg(v, 3, 0, 4, 1); }
			// D
			{ auto& v = M['D']; g.seg(v, 0, 0, 0, 6); g.seg(v, 0, 6, 2, 6); g.seg(v, 2, 6, 4, 4); g.seg(v, 4, 4, 4, 2); g.seg(v, 4, 2, 2, 0); g.seg(v, 2, 0, 0, 0); }
			// E
			{ auto& v = M['E']; g.seg(v, 0, 0, 0, 6); g.seg(v, 0, 6, 4, 6); g.seg(v, 0, 3, 3, 3); g.seg(v, 0, 0, 4, 0); }
			// F
			{ auto& v = M['F']; g.seg(v, 0, 0, 0, 6); g.seg(v, 0, 6, 4, 6); g.seg(v, 0, 3, 3, 3); }
			// G
			{ auto& v = M['G']; g.seg(v, 4, 5, 3, 6); g.seg(v, 3, 6, 1, 6); g.seg(v, 1, 6, 0, 5); g.seg(v, 0, 5, 0, 1); g.seg(v, 0, 1, 1, 0); g.seg(v, 1, 0, 3, 0); g.seg(v, 3, 0, 4, 1); g.seg(v, 2, 3, 4, 3); g.seg(v, 4, 3, 4, 2); }
			// H
			{ auto& v = M['H']; g.seg(v, 0, 0, 0, 6); g.seg(v, 4, 0, 4, 6); g.seg(v, 0, 3, 4, 3); }
			// I
			{ auto& v = M['I']; g.seg(v, 1, 6, 3, 6); g.seg(v, 2, 6, 2, 0); g.seg(v, 1, 0, 3, 0); }
			// J
			{ auto& v = M['J']; g.seg(v, 1, 6, 3, 6); g.seg(v, 2, 6, 2, 1); g.seg(v, 2, 1, 1, 0); g.seg(v, 1, 0, 0, 1); }
			// K
			{ auto& v = M['K']; g.seg(v, 0, 0, 0, 6); g.seg(v, 0, 3, 4, 6); g.seg(v, 0, 3, 4, 0); }
			// L
			{ auto& v = M['L']; g.seg(v, 0, 6, 0, 0); g.seg(v, 0, 0, 4, 0); }
			// M
			{ auto& v = M['M']; g.seg(v, 0, 0, 0, 6); g.seg(v, 4, 0, 4, 6); g.seg(v, 0, 6, 2, 3); g.seg(v, 2, 3, 4, 6); }
			// N
			{ auto& v = M['N']; g.seg(v, 0, 0, 0, 6); g.seg(v, 4, 0, 4, 6); g.seg(v, 0, 6, 4, 0); }
			// O
			{ auto& v = M['O']; g.seg(v, 1, 0, 3, 0); g.seg(v, 3, 0, 4, 1); g.seg(v, 4, 1, 4, 5); g.seg(v, 4, 5, 3, 6); g.seg(v, 3, 6, 1, 6); g.seg(v, 1, 6, 0, 5); g.seg(v, 0, 5, 0, 1); g.seg(v, 0, 1, 1, 0); }
			// P
			{ auto& v = M['P']; g.seg(v, 0, 0, 0, 6); g.seg(v, 0, 6, 3, 6); g.seg(v, 3, 6, 4, 5); g.seg(v, 4, 5, 3, 4); g.seg(v, 3, 4, 0, 4); }
			// Q
			{ auto& v = M['Q']; g.seg(v, 1, 0, 3, 0); g.seg(v, 3, 0, 4, 1); g.seg(v, 4, 1, 4, 5); g.seg(v, 4, 5, 3, 6); g.seg(v, 3, 6, 1, 6); g.seg(v, 1, 6, 0, 5); g.seg(v, 0, 5, 0, 1); g.seg(v, 0, 1, 1, 0); g.seg(v, 2, 2, 4, 0); }
			// R
			{ auto& v = M['R']; g.seg(v, 0, 0, 0, 6); g.seg(v, 0, 6, 3, 6); g.seg(v, 3, 6, 4, 5); g.seg(v, 4, 5, 3, 4); g.seg(v, 3, 4, 0, 4); g.seg(v, 0, 4, 4, 0); }
			// S
			{ auto& v = M['S']; g.seg(v, 4, 5, 3, 6); g.seg(v, 3, 6, 1, 6); g.seg(v, 1, 6, 0, 5); g.seg(v, 0, 5, 1, 4); g.seg(v, 1, 4, 3, 2); g.seg(v, 3, 2, 4, 1); g.seg(v, 4, 1, 3, 0); g.seg(v, 3, 0, 1, 0); g.seg(v, 1, 0, 0, 1); }
			// T
			{ auto& v = M['T']; g.seg(v, 0, 6, 4, 6); g.seg(v, 2, 6, 2, 0); }
			// U
			{ auto& v = M['U']; g.seg(v, 0, 6, 0, 1); g.seg(v, 0, 1, 1, 0); g.seg(v, 1, 0, 3, 0); g.seg(v, 3, 0, 4, 1); g.seg(v, 4, 1, 4, 6); }
			// V
			{ auto& v = M['V']; g.seg(v, 0, 6, 2, 0); g.seg(v, 4, 6, 2, 0); }
			// W
			{ auto& v = M['W']; g.seg(v, 0, 6, 1, 0); g.seg(v, 1, 0, 2, 3); g.seg(v, 2, 3, 3, 0); g.seg(v, 3, 0, 4, 6); }
			// X
			{ auto& v = M['X']; g.seg(v, 0, 6, 4, 0); g.seg(v, 0, 0, 4, 6); }
			// Y
			{ auto& v = M['Y']; g.seg(v, 0, 6, 2, 3); g.seg(v, 4, 6, 2, 3); g.seg(v, 2, 3, 2, 0); }
			// Z
			{ auto& v = M['Z']; g.seg(v, 0, 6, 4, 6); g.seg(v, 4, 6, 0, 0); g.seg(v, 0, 0, 4, 0); }

			// a..z reuse A..Z strokes by default (stick font is case-agnostic visually)
			for (char c = 'A'; c <= 'Z'; ++c)
				M[uint32_t(c - 'A' + 'a')] = M[uint32_t(c)];
		}

		inline void build_digits_0_to_9(std::unordered_map<uint32_t, std::vector<ASCI_Letters::Line2d>>& M, const Grid& g)
		{
			using L = ASCI_Letters::Line2d;
			// 0
			{ auto& v = M['0']; g.seg(v, 1, 0, 3, 0); g.seg(v, 3, 0, 4, 1); g.seg(v, 4, 1, 4, 5); g.seg(v, 4, 5, 3, 6); g.seg(v, 3, 6, 1, 6); g.seg(v, 1, 6, 0, 5); g.seg(v, 0, 5, 0, 1); g.seg(v, 0, 1, 1, 0); }
			// 1
			{ auto& v = M['1']; g.seg(v, 2, 6, 2, 0); g.seg(v, 1, 1, 2, 0); g.seg(v, 2, 0, 3, 1); }
			// 2
			{ auto& v = M['2']; g.seg(v, 0, 5, 1, 6); g.seg(v, 1, 6, 3, 6); g.seg(v, 3, 6, 4, 5); g.seg(v, 4, 5, 0, 0); g.seg(v, 0, 0, 4, 0); }
			// 3
			{ auto& v = M['3']; g.seg(v, 0, 5, 1, 6); g.seg(v, 1, 6, 3, 6); g.seg(v, 3, 6, 4, 5); g.seg(v, 4, 5, 3, 4); g.seg(v, 3, 4, 1, 4); g.seg(v, 1, 4, 3, 4); g.seg(v, 3, 4, 4, 3); g.seg(v, 4, 3, 3, 2); g.seg(v, 3, 2, 1, 2); g.seg(v, 1, 2, 0, 1); }
			// 4
			{ auto& v = M['4']; g.seg(v, 3, 6, 3, 0); g.seg(v, 0, 3, 4, 3); g.seg(v, 0, 3, 3, 6); }
			// 5
			{ auto& v = M['5']; g.seg(v, 4, 6, 0, 6); g.seg(v, 0, 6, 0, 4); g.seg(v, 0, 4, 3, 4); g.seg(v, 3, 4, 4, 3); g.seg(v, 4, 3, 4, 1); g.seg(v, 4, 1, 3, 0); g.seg(v, 3, 0, 1, 0); }
			// 6
			{ auto& v = M['6']; g.seg(v, 3, 6, 1, 6); g.seg(v, 1, 6, 0, 5); g.seg(v, 0, 5, 0, 1); g.seg(v, 0, 1, 1, 0); g.seg(v, 1, 0, 3, 0); g.seg(v, 3, 0, 4, 1); g.seg(v, 4, 1, 3, 2); g.seg(v, 3, 2, 0, 2); }
			// 7
			{ auto& v = M['7']; g.seg(v, 0, 6, 4, 6); g.seg(v, 4, 6, 1, 0); }
			// 8
			{ auto& v = M['8']; g.seg(v, 1, 0, 3, 0); g.seg(v, 3, 0, 4, 1); g.seg(v, 4, 1, 3, 2); g.seg(v, 3, 2, 1, 2); g.seg(v, 1, 2, 0, 1); g.seg(v, 0, 1, 1, 0); g.seg(v, 1, 2, 3, 2); g.seg(v, 3, 2, 4, 3); g.seg(v, 4, 3, 3, 4); g.seg(v, 3, 4, 1, 4); g.seg(v, 1, 4, 0, 3); g.seg(v, 0, 3, 1, 2); g.seg(v, 1, 4, 0, 5); g.seg(v, 0, 5, 1, 6); g.seg(v, 1, 6, 3, 6); g.seg(v, 3, 6, 4, 5); g.seg(v, 4, 5, 3, 4); }
			// 9
			{ auto& v = M['9']; g.seg(v, 1, 0, 3, 0); g.seg(v, 3, 0, 4, 1); g.seg(v, 4, 1, 4, 5); g.seg(v, 4, 5, 3, 6); g.seg(v, 3, 6, 1, 6); g.seg(v, 1, 6, 0, 5); g.seg(v, 0, 5, 1, 4); g.seg(v, 1, 4, 4, 4); }
		}

		inline void build_common_punct(std::unordered_map<uint32_t, std::vector<ASCI_Letters::Line2d>>& M, const Grid& g)
		{
			using L = ASCI_Letters::Line2d;
			// Basic punctuation
			{ auto& v = M['-']; g.seg(v, 1, 3, 3, 3); }
			{ auto& v = M['_']; g.seg(v, 0, 0, 4, 0); }
			{ auto& v = M['=']; g.seg(v, 1, 4, 3, 4); g.seg(v, 1, 2, 3, 2); }
			{ auto& v = M['+']; g.seg(v, 2, 5, 2, 1); g.seg(v, 1, 3, 3, 3); }
			{ auto& v = M['/']; g.seg(v, 0, 0, 4, 6); }
			{ auto& v = M['\\']; g.seg(v, 0, 6, 4, 0); }
			{ auto& v = M['|']; g.seg(v, 2, 6, 2, 0); }
			{ auto& v = M['*']; g.seg(v, 2, 5, 2, 1); g.seg(v, 1, 4, 3, 2); g.seg(v, 3, 4, 1, 2); }
			{ auto& v = M['!']; g.seg(v, 2, 6, 2, 2); g.seg(v, 2, 0, 2, 0); }
			{ auto& v = M['?']; g.seg(v, 0, 5, 1, 6); g.seg(v, 1, 6, 3, 6); g.seg(v, 3, 6, 4, 5); g.seg(v, 4, 5, 3, 4); g.seg(v, 3, 4, 2, 3); g.seg(v, 2, 3, 2, 2); g.seg(v, 2, 0, 2, 0); }
			{ auto& v = M['.']; g.seg(v, 2, 0, 2, 0); }
			{ auto& v = M[',']; g.seg(v, 2, 0, 2, 0); g.seg(v, 2, 0, 1, -1); }
			{ auto& v = M[':']; g.seg(v, 2, 5, 2, 5); g.seg(v, 2, 1, 2, 1); }
			{ auto& v = M[';']; g.seg(v, 2, 5, 2, 5); g.seg(v, 2, 1, 2, 1); g.seg(v, 2, 1, 1, 0); }
			{ auto& v = M['"']; g.seg(v, 1, 6, 1, 5); g.seg(v, 3, 6, 3, 5); }
			{ auto& v = M['\'']; g.seg(v, 2, 6, 2, 5); }
			{ auto& v = M['(']; g.seg(v, 3, 6, 2, 5); g.seg(v, 2, 5, 1, 3); g.seg(v, 1, 3, 2, 1); g.seg(v, 2, 1, 3, 0); }
			{ auto& v = M[')']; g.seg(v, 1, 6, 2, 5); g.seg(v, 2, 5, 3, 3); g.seg(v, 3, 3, 2, 1); g.seg(v, 2, 1, 1, 0); }
			{ auto& v = M['[']; g.seg(v, 3, 6, 1, 6); g.seg(v, 1, 6, 1, 0); g.seg(v, 1, 0, 3, 0); }
			{ auto& v = M[']']; g.seg(v, 1, 6, 3, 6); g.seg(v, 3, 6, 3, 0); g.seg(v, 3, 0, 1, 0); }
			{ auto& v = M['{']; g.seg(v, 3, 6, 2, 6); g.seg(v, 2, 6, 1, 5); g.seg(v, 1, 5, 2, 4); g.seg(v, 2, 4, 1, 3); g.seg(v, 1, 3, 2, 2); g.seg(v, 2, 2, 1, 1); g.seg(v, 1, 1, 2, 0); g.seg(v, 2, 0, 3, 0); }
			{ auto& v = M['}']; g.seg(v, 1, 6, 2, 6); g.seg(v, 2, 6, 3, 5); g.seg(v, 3, 5, 2, 4); g.seg(v, 2, 4, 3, 3); g.seg(v, 3, 3, 2, 2); g.seg(v, 2, 2, 3, 1); g.seg(v, 3, 1, 2, 0); g.seg(v, 2, 0, 1, 0); }
			{ auto& v = M['<']; g.seg(v, 3, 6, 1, 3); g.seg(v, 1, 3, 3, 0); }
			{ auto& v = M['>']; g.seg(v, 1, 6, 3, 3); g.seg(v, 3, 3, 1, 0); }
			{ auto& v = M['@']; g.seg(v, 1, 0, 3, 0); g.seg(v, 3, 0, 4, 1); g.seg(v, 4, 1, 4, 5); g.seg(v, 4, 5, 3, 6); g.seg(v, 3, 6, 1, 6); g.seg(v, 1, 6, 0, 5); g.seg(v, 0, 5, 0, 1); g.seg(v, 0, 1, 1, 0); g.seg(v, 1, 3, 3, 3); g.seg(v, 3, 3, 3, 1); }
			{ auto& v = M['#']; g.seg(v, 1, 0, 1, 6); g.seg(v, 3, 0, 3, 6); g.seg(v, 0, 4, 4, 4); g.seg(v, 0, 2, 4, 2); }
			{ auto& v = M['$']; g.seg(v, 2, 6, 2, 0); g.seg(v, 4, 5, 3, 6); g.seg(v, 3, 6, 1, 6); g.seg(v, 1, 6, 0, 5); g.seg(v, 0, 5, 1, 4); g.seg(v, 1, 4, 3, 2); g.seg(v, 3, 2, 4, 1); g.seg(v, 4, 1, 3, 0); g.seg(v, 3, 0, 1, 0); g.seg(v, 1, 0, 0, 1); }
			{ auto& v = M['%']; g.seg(v, 0, 0, 4, 6); g.seg(v, 0, 5, 1, 6); g.seg(v, 1, 6, 2, 5); g.seg(v, 2, 5, 1, 4); g.seg(v, 3, 1, 4, 0); g.seg(v, 4, 0, 3, -1); g.seg(v, 3, -1, 2, 0); }
			{ auto& v = M['^']; g.seg(v, 1, 4, 2, 5); g.seg(v, 2, 5, 3, 4); }
			{ auto& v = M['`']; g.seg(v, 2, 5, 3, 4); }
			{ auto& v = M['~']; g.seg(v, 0, 4, 1, 5); g.seg(v, 1, 5, 2, 4); g.seg(v, 2, 4, 3, 5); g.seg(v, 3, 5, 4, 4); }
			{ auto& v = M['&']; g.seg(v, 3, 0, 1, 2); g.seg(v, 1, 2, 3, 4); g.seg(v, 3, 4, 4, 3); g.seg(v, 4, 3, 2, 1); g.seg(v, 2, 1, 1, 0); }
			{ auto& v = M['~']; g.seg(v, 0, 4, 1, 5); g.seg(v, 1, 5, 2, 4); g.seg(v, 2, 4, 3, 5); g.seg(v, 3, 5, 4, 4); }
		}

		// ---- Composition helpers for extended letters ----
		inline void add_with_top_component(ASCI_Letters& F, uint32_t base_cp, uint32_t out_cp,
			const std::vector<ASCI_Letters::Line2d>& component,
			float scale_rel = 0.18f, float gap = 0.02f)
		{
			auto it = F.letters.find(base_cp);
			if (it == F.letters.end()) return;
			auto base = it->second; // copy
			auto placed = place_component(base, component, AttachWhere::TopCenter, scale_rel, gap);
			append(base, placed);
			F.letters[out_cp] = std::move(base);
		}

		inline void add_with_bottom_component(ASCI_Letters& F, uint32_t base_cp, uint32_t out_cp,
			const std::vector<ASCI_Letters::Line2d>& component,
			float scale_rel = 0.18f, float gap = 0.02f)
		{
			auto it = F.letters.find(base_cp);
			if (it == F.letters.end()) return;
			auto base = it->second; // copy
			auto placed = place_component(base, component, AttachWhere::BottomCenter, scale_rel, gap);
			append(base, placed);
			F.letters[out_cp] = std::move(base);
		}

		// ---- Public: build full ASCII + a few extended examples ----
		inline ASCI_Letters make_full_ascii_stick_font()
		{
			ASCI_Letters font;
			Grid g; g.W = 5; g.H = 8; // 5x7 base with one extra row for accents

			Components C; C.build(g);

			// Base Latin uppercase + lowercase (reused)
			build_letters_A_to_Z(font.letters, g);
			// Digits & punctuation
			build_digits_0_to_9(font.letters, g);
			build_common_punct(font.letters, g);

			// --- Examples of extended letters built by composition ---
			// Caron (hek) variants frequently used in Slovene:      
			if (C.comp.count("caron"))
			{
				const auto& caron = C.comp["caron"];
				add_with_top_component(font, 'C', 0x010C /**/, caron); // 
				add_with_top_component(font, 'c', 0x010D /**/, caron);
				add_with_top_component(font, 'Z', 0x017D /**/, caron);
				add_with_top_component(font, 'z', 0x017E /**/, caron);
				add_with_top_component(font, 'S', 0x0160 /**/, caron);
				add_with_top_component(font, 's', 0x0161 /**/, caron);
			}
			// , , ,  as examples
			if (C.comp.count("ring")) { add_with_top_component(font, 'A', 0x00C5/**/, C.comp["ring"], 0.2f); add_with_top_component(font, 'a', 0x00E5/**/, C.comp["ring"], 0.2f); }
			if (C.comp.count("dieresis")) { add_with_top_component(font, 'A', 0x00C4/**/, C.comp["dieresis"], 0.18f); add_with_top_component(font, 'O', 0x00D6/**/, C.comp["dieresis"], 0.18f); add_with_top_component(font, 'U', 0x00DC/**/, C.comp["dieresis"], 0.18f); }
			if (C.comp.count("acute")) { add_with_top_component(font, 'E', 0x00C9/**/, C.comp["acute"], 0.2f); add_with_top_component(font, 'e', 0x00E9/**/, C.comp["acute"], 0.2f); }
			if (C.comp.count("grave")) { add_with_top_component(font, 'E', 0x00C8/**/, C.comp["grave"], 0.2f); add_with_top_component(font, 'e', 0x00E8/**/, C.comp["grave"], 0.2f); }

			//  = O with a slash through it
			if (C.comp.count("slash_diag"))
			{
				auto slashO = font.letters['O'];
				auto placed = place_component(slashO, C.comp["slash_diag"], AttachWhere::ThroughDiag, 1.0f, 0.0f);
				append(slashO, placed);
				font.letters[0x00D8/**/] = std::move(slashO);
			}

			// Greek sigma ()  simplified stick form example (lowercase sigma)
			{
				auto& v = font.letters[0x03C3/**/];
				g.seg(v, 1, 2, 1, 1); g.seg(v, 1, 1, 2, 0); g.seg(v, 2, 0, 3, 0); g.seg(v, 3, 0, 4, 1);
				g.seg(v, 4, 1, 4, 2); g.seg(v, 4, 2, 3, 3); g.seg(v, 3, 3, 2, 3); g.seg(v, 2, 3, 1, 2);
			}

			// Finish: normalize every glyph to [0,1]^2 with a small padding.
			normalize_letters(font, 0.04f);
			return font;
		}
	}

	
	

	void bug_fix_line_position(float& x, float& y, float& z)
	{
		std::swap(y, z);

		z -= 0.5f;
		x -= 0.5f;
		y -= 0.5f;
	}
	
	
	
	void init(Scene_::Scene& scene, const int clip_number, const int clip_fps, const int clip_length_seconds, const bool capture, const bool capture_png, const bool capture_bmp)
	{
		const bool enable_shader_10_unit_cube = true;
		const bool enable_shader_20_sphere = false; // sphere
		const bool enable_shader_21_line = false; // line
		const bool enable_shader_22_geodesic_line = false; // line with t
		
		const bool enable_ASCII_letters = true;

		{
			Program program;
			program.le.halfLife = 0.02f * 4.0f * 0.0001f;
			program.le.brightness = 0.0f;
			program.le.exposure = 1.0;
			program.le.msaaSamples = 10;

			program.capture.capture = capture;
			program.capture.capture_png = capture_png;
			program.capture.capture_bmp = capture_bmp;

			program.render_display.render_fps = clip_fps;
			program.render_display.number_of_frames = program.render_display.render_fps * clip_length_seconds;
			program.render_display.render_time_start = 0.0;


			// CAPTURED: { pos: [-0.986564, 1.019589, 2.007937] , yaw : 46.080070, pitch : -17.400019, fov : 45.000000 }
			program.camera_start.x = -0.986564;
			program.camera_start.y = 1.019589;
			program.camera_start.z = 2.007937;

			program.camera_start.yaw = 46.080070;
			program.camera_start.pitch = -17.400019;
			program.camera_start.fov = 45.0;


			// CAPTURED: { pos: [0.721016, 1.160607, 3.534045] , yaw : -2.879919, pitch : -11.280016, fov : 45.000000 }

			program.camera_end.x = 0.721016;
			program.camera_end.y = 1.160607;
			program.camera_end.z = 3.534045;

			program.camera_end.yaw = -2.879919;
			program.camera_end.pitch = -11.280016;
			program.camera_end.fov = 45.000000;
			program.configure(scene);

			// program.camera_end.x = program.camera_start.x;
			// program.camera_end.y = program.camera_start.y;
			// program.camera_end.z = program.camera_start.z;

			// program.camera_end.yaw = program.camera_start.yaw;
			// program.camera_end.pitch = program.camera_start.pitch;
			// program.camera_end.fov = program.camera_start.fov;
		}




		if (enable_ASCII_letters)
		{
			std::cout << "enable_ASCII_letters : " << enable_ASCII_letters << "\n";

			ASCI_Letters_::ASCI_Letters letters = ASCI_Letters_::make_full_ascii_stick_font();
			
			

			Lines_shader_21 lines;

			{
				auto draw_line_2d = [&lines](float x0, float y0, float x1, float y1, Vec3 offset_end)
					{
						Lines_shader_21::Line& line = lines.add_line();

						line.x0.start = x0;
						line.y0.start = y0;
						line.z0.start = 0.5;

						line.x1.start = x1;
						line.y1.start = y1;
						line.z1.start = 0.5;

						line.thickness.start = 0.001f;

						line.rgb_t0.x = Random::generate_random_float_0_to_1();
						line.rgb_t0.y = Random::generate_random_float_0_to_1();
						line.rgb_t0.z = Random::generate_random_float_0_to_1();

						line.rgb_t1.x = Random::generate_random_float_0_to_1();
						line.rgb_t1.y = Random::generate_random_float_0_to_1();
						line.rgb_t1.z = Random::generate_random_float_0_to_1();

						line.number_of_cubes = 100;

						line.thickness.end = 0.001f;

						line.copy_start_to_end();

						line.x0.end += offset_end.x;
						line.y0.end += offset_end.y;
						line.z0.end += offset_end.z;

						line.x1.end += offset_end.x;
						line.y1.end += offset_end.y;
						line.z1.end += offset_end.z;

						bug_fix_line_position(line.x0.start, line.y0.start, line.z0.start);
						bug_fix_line_position(line.x1.start, line.y1.start, line.z1.start);

						bug_fix_line_position(line.x0.end, line.y0.end, line.z0.end);
						bug_fix_line_position(line.x1.end, line.y1.end, line.z1.end);

						

						

					};

				// A ok
				// B fail
				// C ok
				// D ok
				// F ok
				// G fail
				// H ok
				// F ok
				// I ok
				// J ok
				// K ok
				// L ok
				// M ok
				// N ok
				// O ok
				// P ok
				// R ok
				// S ok 
				// T ok
				// U ok
				// V ok
				// Z ok
				// W ok
				// X ok
				// Y ok
				// Z ok
				// Q ok


				std::vector<uint32_t> letters_to_write = {
					'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
					'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
					'K', 'L', 'M', 'N', 'O', 'P', 'R', 'S', ' ', 'T',
					'N', 'I', 'C', 'E', ' ', ' ', '2', '0', '2', '5',
					'E', 'X', 'P', 'L', 'O', 'R', 'E', ' ', ' ', ' ',
					'-', '+', '*', '/', '?', '!', '#', '$', '%', '=',
					'G', 'B', 'Q', 'Q', 'Z', 'U', '<', '>', 'Y', 'W'
				};
				
			
				
				for (int i = 0; i < letters_to_write.size(); i++)
				{
					int letter_position_x = i % 10;
					int letter_position_y = 9 - (i / 10);


					float letter_size = 0.1;

					for (auto& l : letters.letters.at(letters_to_write[i]))
					{
						
						Vec3 offset = Vec3(0.0, 0.0, 0.5);

						draw_line_2d
						(
							letter_size * l.start.x + letter_size * float(letter_position_x),
							letter_size * l.start.y  + letter_size * float(letter_position_y),
							letter_size * l.end.x + letter_size * float(letter_position_x) ,
							letter_size * l.end.y + letter_size * float(letter_position_y),
							offset
						);
					}
				}

				

				/*for (auto& l : letters.letters.at('2'))
				{
					draw_line_2d(l.start.x + 1.0, l.start.y, l.end.x + 1.0, l.end.y);
				}*/

				
				


				lines.draw(scene);
			}
			



			
		}

		if (enable_shader_20_sphere) // sphered
		{

			auto spheres_on_lines_init = [](Spheres_shader_20& spheres, Lines_shader_21& lines, int number)
				{

					for (int i = 0; i < number; i++)
					{
						// Spheres_shader_20::Sphere sphere;


						float factor_i = (1.0f / float(number)) * i;

						float radius_start =  Random::generate_random_float_0_to_1() * 0.2;
						float radius_end = radius_start + Random::generate_random_float_0_to_1() * 0.2;

						{
							Lines_shader_21::Line& line = lines.add_line();

							line.x0.start = Random::generate_random_float_0_to_1();
							line.y0.start = 0.0;
							line.z0.start = Random::generate_random_float_0_to_1();

							line.x1.start = line.x0.start;
							line.y1.start = 1.0;
							line.z1.start = line.z0.start;

							line.thickness.start = 0.01f;

							line.rgb_t0.x = Random::generate_random_float_0_to_1();
							line.rgb_t0.y = Random::generate_random_float_0_to_1();
							line.rgb_t0.z = Random::generate_random_float_0_to_1();

							

							line.number_of_cubes = 1000;

							{
								Vec3 position_start = { line.x0.start, line.y0.start, line.z0.start };
								Vec3 position_end = { line.x1.start, line.y1.start, line.z1.start };

								bug_fix_line_position(line.x0.start, line.y0.start, line.z0.start);
								bug_fix_line_position(line.x1.start, line.y1.start, line.z1.start);

								line.copy_start_to_end();

								line.rgb_t1.x = Random::generate_random_float_0_to_1();
								line.rgb_t1.y = Random::generate_random_float_0_to_1();
								line.rgb_t1.z = Random::generate_random_float_0_to_1();


								Vec3 color_start = { Random::generate_random_float_0_to_1(),  Random::generate_random_float_0_to_1(),  Random::generate_random_float_0_to_1() };
								Vec3 color_end = { Random::generate_random_float_0_to_1(),  Random::generate_random_float_0_to_1(),  Random::generate_random_float_0_to_1() };

								float cube_size_start = 0.001;
								float cube_size_end = 0.002;

								Float_start_end x_rnd_min{ 0.0, 0.0 };
								Float_start_end x_rnd_max{ 1.0, 1.0 };
								Float_start_end y_rnd_min{ 0.0, 0.0 };
								Float_start_end y_rnd_max{ 1.0, 1.0 };
								Float_start_end thickness{ 0.1, 0.1 };
								Float_start_end jitter{ 1.0, 1.0 };


								add_sphere
								(
									spheres,
									{
										position_start,
										position_end
									},
							{
								radius_start,
								radius_end
							},
							{
								color_start,
								color_end
							},
									{ cube_size_start, cube_size_end },
									x_rnd_min,
									x_rnd_max,
									y_rnd_min,
									y_rnd_max,
									thickness,
									jitter
								);
							}
						}


						
						
					}
				};

			Spheres_shader_20 spheres;
			// spheres_init_1(sphere, 10);
			Lines_shader_21 lines;

			spheres_on_lines_init(spheres, lines, 10);

			spheres.draw(scene, 1000);
			lines.draw(scene);
		}

		if (enable_shader_21_line) // lines
		{
			auto lines_init = [](Lines_shader_21& lines)
				{
					int number_of_lines = 100;
					const float TAU = 6.2831853071795864769252867665590;
					float step_size = (1.0 / float(number_of_lines)) * TAU;

					for (int i = 0; i < number_of_lines; i++)
					{
						Lines_shader_21::Line& line = lines.add_line();


						line.x0.start = 0.0f;
						line.y0.start = 0.0f;
						line.z0.start = 0.0f;

						line.x1.start = 0.5f * sin(i * step_size);
						line.y1.start = 0.0f;
						line.z1.start = 0.5f * cos(i * step_size);

						line.rgb_t0.x = 0.2 * Random::generate_random_float_0_to_1();
						line.rgb_t0.y = 0.2 * Random::generate_random_float_0_to_1();
						line.rgb_t0.z = 0.2 * Random::generate_random_float_0_to_1();

						line.thickness.start = 0.01 * 0.2;
						line.number_of_cubes = 100;

						line.copy_start_to_end();



						line.x1.end = 0.5f * sin(i * step_size);
						line.y1.end = 0.0f;
						line.z1.end = 0.5f * cos(i * step_size);

						// line.y1.end = 1.0;
						// line.z1.end = 1.0;
					}





				};

			Lines_shader_21 lines;
			lines_init(lines);
			lines.draw(scene);
		}

		if (enable_shader_22_geodesic_line) // lines with t
		{
			auto init_lines_geodesic = [](LinesGeodesic_shader_22& lines_geodesic)
			{
				if (true)
				{
					auto draw_sphere = [](std::vector<LinesGeodesic_shader_22::LineGeodesic>& l, float radius, float x, float y, float z, float thickness)
						{
							for (int i = 0; i < 10; i++)
							{
								LinesGeodesic_shader_22::LineGeodesic L;
								L.samples = 600;
								L.radius.start = radius;

								L.x0.start = 0.0f;
								L.y0.start = 0.1f * i;


								L.x1.start = 1.0f;
								L.y1.start = 0.1f * i;

								L.thickness = { thickness, thickness };
								L.rgb0 = { 1,0,0 };

								L.transform_startEnd.start.position.x = x;
								L.transform_startEnd.start.position.y = y;
								L.transform_startEnd.start.position.z = z;

								L.copy_start_to_end();
								l.emplace_back(std::move(L));
							}

							for (int i = 0; i < 10; i++)
							{
								LinesGeodesic_shader_22::LineGeodesic L;
								L.samples = 600;
								L.radius.start = radius;

								L.x0.start = 0.1f * i;
								L.y0.start = 0.0f;


								L.x1.start = 0.1f * i;
								L.y1.start = 0.5f;

								L.thickness = { thickness, thickness };
								L.rgb0 = { 1,1,0 };

								L.transform_startEnd.start.position.x = x;
								L.transform_startEnd.start.position.y = y;
								L.transform_startEnd.start.position.z = z;

								L.copy_start_to_end();
								l.emplace_back(std::move(L));
							}

							for (int i = 0; i < 10; i++)
							{
								LinesGeodesic_shader_22::LineGeodesic L;
								L.samples = 600;
								L.radius.start = radius;

								L.x0.start = 0.1f * i;
								L.y0.start = 0.5f;


								L.x1.start = 0.1f * i;
								L.y1.start = 1.0f;

								L.thickness = { thickness, thickness };
								L.rgb0 = { 1,0,1 };

								L.transform_startEnd.start.position.x = x;
								L.transform_startEnd.start.position.y = y;
								L.transform_startEnd.start.position.z = z;

								L.copy_start_to_end();
								l.emplace_back(std::move(L));
							}

							for (int j = 0; j < 10; j++)
							{
								for (int i = 0; i < 10; i++)
								{
									LinesGeodesic_shader_22::LineGeodesic L;
									L.samples = 600;
									L.radius.start = radius;

									L.x0.start = 0.1f * i;
									L.y0.start = 0.1f * j;


									L.x1.start = 0.1f * (i + 1);
									L.y1.start = 0.1f * (j + 1);

									L.thickness = { thickness, thickness };
									L.rgb0 = { 1,1,1 };

									L.transform_startEnd.start.position.x = x;
									L.transform_startEnd.start.position.y = y;
									L.transform_startEnd.start.position.z = z;

									L.copy_start_to_end();
									l.emplace_back(std::move(L));
								}
							}

						};

					draw_sphere(lines_geodesic.lines, 0.025, 0.5, 0.5, 0.5, 0.00025); // center of unit cube

					draw_sphere(lines_geodesic.lines, 0.1, 0.0, 0.0, 0.0, 0.001);
					draw_sphere(lines_geodesic.lines, 0.1, 0.0, 0.0, 1.0, 0.001);
					draw_sphere(lines_geodesic.lines, 0.1, 0.0, 1.0, 0.0, 0.001);
					draw_sphere(lines_geodesic.lines, 0.1, 0.0, 1.0, 1.0, 0.001);

					draw_sphere(lines_geodesic.lines, 0.1, 1.0, 0.0, 0.0, 0.001);
					draw_sphere(lines_geodesic.lines, 0.1, 1.0, 0.0, 1.0, 0.001);
					draw_sphere(lines_geodesic.lines, 0.1, 1.0, 1.0, 0.0, 0.001);
					draw_sphere(lines_geodesic.lines, 0.1, 1.0, 1.0, 1.0, 0.001);

				}

				if (true)
				{

					for (int i = 0; i < 1000; i++)
					{
						LinesGeodesic_shader_22::LineGeodesic& L = lines_geodesic.add_line();

						L.samples = 1000;

						L.y0.start = Random::generate_random_float_0_to_1();
						L.x0.start = Random::generate_random_float_0_to_1();

						L.y1.start = L.y0.start + Random::generate_random_float_minus_one_to_plus_one() * 0.1;
						L.x1.start = L.x0.start + Random::generate_random_float_minus_one_to_plus_one() * 0.1;

						L.radius.start = 0.5f;

						// subtle color variation
						L.rgb0 = { 0.0, 1.0, 0.0 };


						L.thickness.start = 0.0001f;


						L.transform_startEnd.start.position.x = 0.5f;
						L.transform_startEnd.start.position.y = 0.5f;
						L.transform_startEnd.start.position.z = 0.5f;

						// make static for now (engine can animate u* if desired)
						L.copy_start_to_end();

						L.rgb1 = { 0.2, 0.7, 4.0 };

						L.radius.end = 1.0f;
					}

				}

				
			};

			LinesGeodesic_shader_22 lines_with_t;
			init_lines_geodesic(lines_with_t);
			lines_with_t.draw(scene);
		}

		if (enable_shader_10_unit_cube)
		{
			UnitCube_shader_10 unit_cube;
			unit_cube.init();
			unit_cube.draw(scene);
		}
	}

	void generate()
	{
		Clip clip;

		// for(int i = 0; i < 30; i++)
		{
			std::cout << "clip : " << clip.clip_number << "\n";

			clip.capture = false;
			clip.capture_png = true;
			clip.capture_bmp = false;

			clip.generate(0, init);

			// for (int i = 1; i < 57; i++)
			// {
			// 	clip.generate(i);
			// }

		}

		// TODO
		// create letters


		// TODO
		// DONE enigine set use F drive ( well in that mode create new folder for each run so that you can run generate many concurently nicely )
		// write the motion extraction kind of thing 
		// some format like ply write and read, texture write and read ( you will generate model that you will upload to sketfab, usual polygon based with textures that go along and pointclouds)
		// audio reading, writing we need first steps into generating visuals that are aligned with audio explore procedural audio generation 

		// Write about how  the code is about expression and understanding not just utility or a way to build tools but itself a peace of art well yea 

		// The coordinate system that we use
		// Y is up 
		// Z is forward 
		// X with Z forms the plane 

		// Unit cube 
		// center 
		// x 0.5
		// y 0.5 when 1.0 top side
		// z 0.5

	}

}




int universe(int argc, char* argv[])
{
	std::cout << "Universe\n";

	Universe_::generate();

	return 0;
}