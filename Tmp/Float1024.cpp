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

		int index_0 = -1;
		int index_1 = -1;
		int index_2 = -1;
	};

	

	class Engine
	{
	public:
		Engine()
		{
			compositions.push_back(Composition(CompositionFuncton::BASE, -1, -1, -1));
		}

		~Engine()
		{
			compositions.clear();
		}

		int clone(int index)
		{
			Composition el = compositions.at(index);
			compositions.push_back(el);
		}

		

	private:

		std::vector<Composition> compositions;
	};

	void build()
	{
		Engine engine;

		

	}

	
}