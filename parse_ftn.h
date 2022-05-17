#ifndef ___PARSE_FUNCTION___
#define ___PARSE_FUNCTION___

#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <iomanip>

typedef std::function<double(double)> mfun; // mfun = math function
typedef std::function<double(double, double)> mfun2; // mfun with 2 arguments
typedef std::function<double(double, double, double)> mfun3;
typedef std::vector<double> dVec;
typedef std::vector<char> cVec;
typedef std::vector<cVec> cBoard;

namespace Grapher {

	// define constants
	constexpr double const_e = 2.718281828459045;
	constexpr double const_pi = 3.141592653589793;

	


	// overload operators
	mfun operator+(const mfun& f, const mfun& g);
	mfun operator-(const mfun& f, const mfun& g);
	mfun operator*(const mfun& f, const mfun& g);
	mfun operator/(const mfun& f, const mfun& g);
	mfun& operator+=(mfun& f, const mfun& g);
	mfun& operator-=(mfun& f, const mfun& g);
	mfun& operator*=(mfun& f, const mfun& g);
	mfun& operator/=(mfun& f, const mfun& g);
	mfun operator-(const mfun& f);
	mfun pow(const mfun& f, const mfun& g);
	mfun combineTerms(std::istream& in);
	mfun combineFactors(std::istream& in);
	mfun evaluateFactor(std::istream& in);

	mfun combineTerms(const std::string& s, size_t& pos);
	mfun combineFactors(const std::string& s, size_t& pos);
	mfun evaluateFactor(const std::string& s, size_t& pos);

	std::string& reduce(std::string& s);
	// std::vector<double> make_interval(double a, double b, size_t N = 100);
	dVec linspace(double a, double b, size_t N); 
	std::vector<double> eval_f(const std::vector<double>& interval, const mfun& f);


	struct GraphProperties {
		double xmin, xmax, ymin, ymax;
		size_t Nx, Ny;
		const size_t yaxis_width = 7, xaxis_val = 7;
		char mark;
		dVec xaxis, yaxis;

		GraphProperties();
		GraphProperties(double _xmin, double _xmax, double _ymin, double _ymax, size_t _Nx, size_t _Ny);
		GraphProperties(double _xmin, double _xmax, size_t _Nx, size_t _Ny);
		void set_xaxis(double _xmin, double _xmax, size_t _Nx);
		void set_xaxis(const dVec& _axis);
		void set_yaxis(double _ymin, double _ymax, size_t _Ny);
		void set_yaxis(const dVec& _axis);
		void set_yaxis(const mfun&);
		GraphProperties& operator=(GraphProperties that);
	};

	class Graph {
		mfun f;
		bool f_computed, board_set;
		dVec  fval;
		GraphProperties GP;
		cBoard board;
		void set_fval();
	public:
		Graph(double xmin, double xmax, const mfun& f, size_t Nx, size_t Ny);
		// Graph(const mfun& f);
		// Graph(const dVec& _xaxis, const dVec& _yaxis, const mfun& f);
		// Graph(const dVec& _xaxis, size_t _Ny, const mfun& _f);
		// Graph(const dVec& _xaxis, const mfun& f);
		Graph(GraphProperties _GP, const mfun& f, bool reset_yaxis = false);
		void setBoard();
		void set_f(const mfun& newf);
		void print_Graph(std::ostream& out) const;
		bool check_printable() const;
		const GraphProperties& getGraphProperties() const;
	};

	class MathFun {
		mfun f;
		GraphProperties GP;
	public:
		MathFun();
		MathFun(const mfun& f);
		void setGraphProperties_by_input(size_t Nx, size_t Ny);
		void setGraphProperties_by_input();
		void plot_graph() const;
		void plot_graph(const GraphProperties& newGP); // set to new GP and plot
		void set_f(const mfun& _f);
		void set_GP(const GraphProperties& newGP);
		GraphProperties getGP() const;
		mfun getf() const;
	};


}
#endif