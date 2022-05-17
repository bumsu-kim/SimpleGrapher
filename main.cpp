#include "parse_ftn.h"
#include <iterator>

int main() {	 
	int more = 0;
	char sameftn = 0;
	char sameGP = 0;

	size_t Nx, Ny;
	std::cout << "Enter the width of x axis (# pts) : ";
	std::cin >> Nx;
	std::cout << "Enter the width of y axis (# pts) : ";
	std::cin >> Ny;
	std::cin.ignore();
	std::cin.clear();
	Grapher::MathFun F{};
	std::cout << "Enter a function of x, f(x) = ";
	std::string expr; std::getline(std::cin, expr);
	size_t pos = 0;
	Grapher::reduce(expr);
	F.set_f(Grapher::combineTerms(expr, pos));
	F.setGraphProperties_by_input(Nx, Ny);
	F.plot_graph();
	std::cout << " Enter any integer > 0 if you want to do it again! \n";
	std::cin >> more;
	while (std::cin && more>0) {
		std::cout << "Use the same function? y/n : ";
		std::cin >> sameftn; std::cin.ignore();
		if (sameftn != 'y') {
			std::cout << "Enter a function of x, f(x) = ";
			std::string expr; std::getline(std::cin, expr);
			size_t pos = 0;
			Grapher::reduce(expr);
			F.set_f(Grapher::combineTerms(expr, pos));
		}
		std::cout << "Use the same graph properties? (yaxis varies automatically) y/n : ";
		std::cin >> sameGP;
		if (sameGP != 'y') {
			std::cout << "Change the graph size (Nx & Ny) ? y/n : ";
			char change_N;
			std::cin >> change_N;
			if (change_N != 'y') {
				F.setGraphProperties_by_input(Nx, Ny);
			}
			else {
				F.setGraphProperties_by_input();
			}
			F.plot_graph();
		}
		else { // same GP
			if (sameftn != 'y') { // diff ftn
				auto newGP = F.getGP();
				newGP.set_yaxis(F.getf());
				F.plot_graph(newGP);
			}
		}
		std::cout << " Enter any integer > 0 if you want to do it again! \n";
		std::cin >> more;
	}

	
	return 0;
}