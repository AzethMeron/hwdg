#include <pybind11/pybind11.h>
#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)
namespace py = pybind11;

// Graph:
// skierowany
// ważony, wagi ułamkowe i ujemne
// może zawierać tylko jeden edge prowadzacy z node A do node B (1-graf, NIE JEST TO MULTIGRAF)
// moze zawierac sciezki z node A prowadzace do node A

PYBIND11_MODULE(graphs, m) {
	
	#ifdef VERSION_INFO
		m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
	#else
		m.attr("__version__") = "dev";
	#endif
}
