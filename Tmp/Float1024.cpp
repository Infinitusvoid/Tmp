#include <vector>

namespace Float1024_
{
	enum class CompositionFuncton
	{
		BASE,

		PI,             // The constant pi
		TAU,

		INVERT,
		NEGATE,         // -x (your INVERT might have meant this)
		RECIPROCAL,     // 1/x (or this)
		ABS,            // Absolute value |x|


		// Trigonometric
		SIN,
		COS,
		TAN,
		ASIN,           // arcsin
		ACOS,           // arccos
		ATAN,           // arctan

		// Exponential & Logarithmic
		SQRT,           // Square root
		EXP,            // e^x
		LOG,            // Natural logarithm (ln)
		LOG2,           // Base-2 logarithm
		LOG10,          // Base-10 logarithm

		// Power
		SQUARE,         // x*x
		CUBE,           // x*x*x

		// Rounding & Fractional
		FLOOR,          // Round down to nearest integer
		CEIL,           // Round up to nearest integer
		ROUND,          // Round to nearest integer
		FRAC,           // The fractional part of a number (e.g., frac(3.14) = 0.14)
		SIGN,           // -1 if x < 0, 0 if x == 0, 1 if x > 0

		// --- Binary Functions (2 inputs) ---
		// Basic Arithmetic
		ADD,
		SUBTRACT,
		MULTIPLY,
		DIVIDE,
		POWER,          // pow(base, exponent)
		MODULO,         // Remainder of division

		// Comparison & Selection
		MIN,            // The smaller of two values
		MAX,            // The larger of two values
		ATAN2,          // atan2(y, x), useful for converting coordinates to angles
		STEP,           // 0 if x < edge, 1 if x >= edge

		// --- Ternary Functions (3 inputs) ---
		LERP,           // Linear interpolation: a * (1-t) + b * t
		CLAMP,          // Clamp a value between a min and a max
		SMOOTHSTEP,     // A smooth transition between 0 and 1
	};

	struct Composition
	{
		CompositionFuncton function;

		std::vector<int> fs;
	};

	

	class Engine
	{
	public:
		Engine()
		{
			compositions.push_back(Composition(CompositionFuncton::BASE, {}));
		}

		void sin(int index)
		{
			if (exist(index))
			{
				compositions.push_back(Composition(CompositionFuncton::SIN, { index }));
			}
			else
			{
				std::abort();
			}
		}

		void add(int index_0, int index_1)
		{
			if (!exist(index_0))
			{
				std::abort();
			}

			if (!exist(index_1))
			{
				std::abort();
			}

			compositions.push_back(Composition(CompositionFuncton::ADD, { index_0, index_1 }));
		}

		

		bool exist(int index)
		{
			return ( index < compositions.size() );
		}

		~Engine()
		{
			compositions.clear();
		}

	private:

		std::vector<Composition> compositions;
	};

	void build()
	{
		Engine engine;

		engine.sin(0);

	}

	
}