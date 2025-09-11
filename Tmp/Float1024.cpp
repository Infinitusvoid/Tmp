#include <vector>

namespace Float1024_
{
	enum class CompositionFuncton
	{
		ADD,
		SUBTRACT,
		MULTIPLY,
		BASE
	};

	struct Composition
	{
		CompositionFuncton function;

		int index_0 = 0;
		int index_1 = 0;
		int index_2 = 0;
	};

	std::vector<Composition> compositions;

	void build()
	{

	}

	
}