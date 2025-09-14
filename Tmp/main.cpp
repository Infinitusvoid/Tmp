#include "Engine.h"

#include "CppCommponents/NameGenerators.h"

#include "GUniverse.h"

#include "ShaderWriter.h"

#include "universes/universe.h"









int main(int argc, char* argv[])
{
	std::cout << "Tmp\n";

	
	// Universes
	{
		GUniverse uni("universes");
		uni.activate_universe_index(1);

		// int idxA = uni.create_new_universe_from_the_last();
		// uni.activate_universe_index(idxA);
		// std::cout << "active universe : " << idxA << "\n";

		// Create from a specific source and auto-activate:
		// int idxB = uni.create_from_universe(idxA);

		// Later, switch active universe explicitly:
		// uni.activate_universe_index(idxA);

		// return 0;
	}
	

	
	{
		// ShaderWriter shader_writer;
		// shader_writer.create_new_shader_from_last();

		// ShaderWriter shader_writer;
		// shader_writer.create_new_from_shader_index(0);

		// return 0;
	}

	

	



	init();

	universe(argc, argv);









	return 0;
}

