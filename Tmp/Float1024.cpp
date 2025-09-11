#include <vector>

namespace Float1024_
{
	enum class CompositionFuncton
	{
		BASE,

		SIN,
		COS,
		INVERT,

		ADD,
		SUBTRACT,
		MULTIPLY,
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