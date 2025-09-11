#include <vector>

namespace Float1024_
{
	enum class CompositionFuncton
	{
		ADD,
		SUBTRACT,
		MULTIPLY,
		SIN,
		COS,
		INVERT,
		BASE
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