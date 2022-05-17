#include "parse_ftn.h"
#include <cmath>


namespace Grapher {
	// define functions
	mfun3 normal{ [](double d, double mu, double s)->double {return 1. / std::sqrt(2 * const_pi*s*s)*std::exp(-(d - mu) * (d - mu) / 2 / s / s); } };

	mfun operator+(const mfun& f, const mfun& g) {
		return mfun{ [f,g](double d)->double {return f(d) + g(d); } };
	}
	mfun operator-(const mfun& f, const mfun& g) {
		return mfun{ [f,g](double d)->double {return f(d) - g(d); } };
	}
	mfun operator*(const mfun& f, const mfun& g) {
		return mfun{ [f,g](double d)->double {return f(d) * g(d); } };
	}
	mfun operator/(const mfun& f, const mfun& g) {
		return mfun{ [f,g](double d)->double {return f(d) / g(d); } };
	}
	mfun& operator+=(mfun& f, const mfun& g) {
		f = f + g;
		return f;
	}
	mfun& operator-=(mfun& f, const mfun& g) {
		f = f - g;
		return f;
	}
	mfun& operator*=(mfun& f, const mfun& g) {
		f = f * g;
		return f;
	}
	mfun& operator/=(mfun& f, const mfun& g) {
		f = f / g;
		return f;
	}
	mfun operator-(const mfun& f) {
		return mfun{ [f](double d)->double {return -f(d); } };
	}
	mfun pow(const mfun& f, const mfun& g) {
		return mfun{ [f,g](double d) -> double {return std::pow(f(d),g(d)); } };
	}

	mfun combineTerms(std::istream& in) {
		mfun res = combineFactors(in);
		bool more = true;
		while (more) {
			while (in.peek() == ' ') {
				in.get();
			}
			char op = in.peek();
			if (op == '+' || op == '-') {
				in.get(); // remove +-
				mfun val = combineFactors(in);
				if (op == '+') {
					res += val;
				}
				else {
					res -= val;
				}
			}
			else {
				more = false;
			}
		}
		return res;
	}
	mfun combineFactors(std::istream& in) {
		mfun res = evaluateFactor(in);
		bool more = true;
		while (more) {
			while (in.peek() == ' ') {
				in.get();
			}
			char op = in.peek();
			if (op == '*' || op == '/' || op == '^') {
				in.get(); // remove */
				mfun val = evaluateFactor(in);
				if (op == '*') {
					res *= val;
				}
				else if (op == '/') {
					res /= val;
				}
				else if (op == '^') {
					res = pow(res, val);
				}
			}
			else {
				more = false;
			}
		}
		return res;
	}

	mfun evaluateFactor(std::istream& in) {
		mfun res = [](double)->double {return 0; };
		while (in.peek() == ' ') {
			in.get();
		}
		char c = in.peek();
		if (c == '(') {
			in.get(); // remove (
			res = combineTerms(in);
			if (in.peek() == ')') {
				in.get();
			}
			else {
				std::cout << " ) missing...\n";
			}
		}
		else if (c == 'x') {
			in.get(); // remove x
			res = [](double d) {return d; };
		}
		else if (c == '+' || c == '-') {
			;
		}
		else {
			double ans = std::numeric_limits<double>::max();
			in >> ans;
			res = [ans](double)->double {return ans; };
		}
		return res;
	}



	mfun combineTerms(const std::string& s, size_t& pos) {
		mfun res = combineFactors(s, pos);
		bool more = true;
		while (more) {
			while (s[pos] == ' ') {
				pos++;
			}
			char op = s[pos];
			if (op == '+' || op == '-') {
				pos++; // remove +-
				mfun val = combineFactors(s, pos);
				if (op == '+') {
					res += val;
				}
				else {
					res -= val;
				}
			}
			else {
				more = false;
			}
		}
		return res;
	}
	mfun combineFactors(const std::string& s, size_t& pos) {
		mfun res = evaluateFactor(s, pos);
		bool more = true;
		while (more) {
			while (s[pos] == ' ') {
				pos++;
			}
			char op = s[pos];
			if (op == '*' || op == '/' || op == '^') {
				pos++; // remove * or /
				mfun val = evaluateFactor(s, pos);
				if (op == '*') {
					res *= val;
				}
				else if (op == '/') {
					res /= val;
				}
				else if (op == '^') {
					res = pow(res, val);
				}
			}
			else {
				more = false;
			}
		}
		return res;
	}

	mfun evaluateFactor(const std::string& s, size_t& pos) {
		mfun res = [](double)->double {return 0; };
		while (s[pos] == ' ') {
			pos++;
		}
		char c = s[pos];
		if (c == '(') {
			pos++; // remove (
			res = combineTerms(s, pos);
			if (s[pos] == ')') {
				pos++;
			}
			else {
				std::cout << " ) missing...\n";
			}
		}
		else if (c == 'x') {
			pos++; // remove x
			res = [](double d) {return d; };
		}
		else if (c == '+' || c == '-') {
			;
		}
		else if (c >= '0' && c <= '9') {
			double ans = std::numeric_limits<double>::max();
			std::istringstream iss(s.substr(pos));
			iss >> ans;
			std::ostringstream oss;
			oss << ans;
			pos += (oss.str()).size();
			res = [ans](double)->double {return ans; };
		}
		else if (c == 'e') {
			pos++;
			res = [](double)->double {return const_e; };
		}
		else if (c == 'p' && s[pos + 1] == 'i') {
			++(++pos);
			res = [](double)->double {return const_pi; };
		}
		else {
			std::string fname_and_arg;
			std::istringstream iss(s.substr(pos));
			iss >> fname_and_arg;
			std::string fname = fname_and_arg.substr(0, fname_and_arg.find('('));
			if (fname.compare("sin") == 0) {
				pos += 3;
				pos++; // remove (
				mfun val = combineTerms(s, pos);
				pos++; // remove )
				res = [val](double d)->double { return std::sin(val(d)); };
			}
			else if (fname.compare("cos") == 0) {
				pos += 3;
				pos++; // remove (
				mfun val = combineTerms(s, pos);
				pos++; // remove )
				res = [val](double d)->double { return std::cos(val(d)); };
			}
			else if (fname.compare("tan") == 0) {
				pos += 3;
				pos++; // remove (
				mfun val = combineTerms(s, pos);
				pos++; // remove )
				res = [val](double d)->double { return std::tan(val(d)); };
			}
			else if (fname.compare("abs") == 0) {
				pos += 3;
				pos++; // remove (
				mfun val = combineTerms(s, pos);
				pos++; // remove )
				res = [val](double d)->double { return std::abs(val(d)); };
			}
			else if (fname.compare("log") == 0) {
				pos += 3;
				pos++; // remove (
				mfun val = combineTerms(s, pos);
				pos++; // remove )
				res = [val](double d)->double { return std::log(val(d)); };
			}
			else if (fname.compare("sqrt") == 0) {
				pos += 4;
				pos++; // remove (
				mfun val = combineTerms(s, pos);
				pos++; // remove )
				res = [val](double d)->double { return std::sqrt(val(d)); };
			}
			else if (fname.compare("floor") == 0) {
				pos += 5;
				pos++; // remove (
				mfun val = combineTerms(s, pos);
				pos++; // remove )
				res = [val](double d)->double { return std::floor(val(d)); };
			}
			else if (fname.compare("Normal") == 0) {
				pos += 6;
				pos++; // remove(
				mfun val = combineTerms(s, pos);
				if (s[pos] == ')') {
					pos++; // remove )
					res = [val](double d)->double { return Grapher::normal(val(d), 0, 1); };
				}
				else if (s[pos] == ',') { // >=2 args
					pos++; // remove ,
					mfun arg2 = combineTerms(s, pos);
					if (s[pos] == ',') { // 3 args
						pos++; // remove ,
						mfun arg3 = combineTerms(s, pos);
						pos++; // remove )
						res =  [val, arg2, arg3](double d)->double { return Grapher::normal(val(d), arg2(d), arg3(d)); };
					}
					else if (s[pos] ==')') { // 2args
						pos++; // remove )
						res = [val, arg2](double d)->double {return Grapher::normal(val(d), 0, arg2(d)); };
					}
				}
			}

		}
		return res;
	}

	std::string& reduce(std::string& s) {
		// first delete ' '
		while (s.find(' ') != std::string::npos) {
			s.erase(s.find(' '), 1);
		}
		////TEMP
		//std::cout << s << '\n';
		// then create ( ) for ^
		// note that now the string does not have any white spaces
		size_t pos = s.find('^');
		while (pos != std::string::npos) {
			size_t pos_1 = pos - 1, pos_2 = pos + 1;
			if (s[pos_1] == ')') { // ( )^...
				size_t count = 1;
				while (count > 0) {
					pos_1--; // go backward
					if (s[pos_1] == ')') {
						count++;
					}
					else if (s[pos_1] == '(') {
						count--;
					}
				} // find the matching (
				// pos_1 = (s.substr(0, pos)).find_last_of('(');
			}
			else { // x^... or #^...
				pos_1 = (s.substr(0, pos)).find_last_of("+-*/^");
				if (pos_1 == std::string::npos) { // no +-*/^ in front of x^... or #^ ...
					pos_1 = 0;
				}
				else {
					pos_1++;
				}
			}
			if (s[pos_2] == '(') { // ...^( )
				// find the matching )
				size_t count = 1;
				while (count > 0) {
					pos_2++;
					if (s[pos_2] == '(') {
						count++;
					}
					else if (s[pos_2] == ')') {
						count--;
					}
				}
				//pos_2 = (s.substr(pos+1)).find_first_of(')')+1;
			}
			else { // ...^# or ...^x
				pos_2 = (s.substr(pos + 1)).find_first_of("+-*/^");
				if (pos_2 == std::string::npos) { // no +-*/ after x^... or #^ ...
					pos_2 = s.size();
				}
				else { // otherwise
					pos_2 += pos + 1;
				}
			}
			s.insert(pos_2, 1, ')'); // insert from behind first
			s.insert(pos_1, 1, '('); // to the front
			size_t temp_pos = (s.substr(pos + 2)).find('^');
			if (temp_pos == std::string::npos) {
				break;
			}
			else {
				pos = temp_pos + pos + 2;
			}
		}
		////TEMP
		//std::cout << s << '\n';
		return s;
	}
	//std::vector<double> make_interval(double a, double b, size_t N) {
	//	std::vector<double> itv(N);
	//	for (size_t i = 0; i < N; ++i) {
	//		itv[i] = a + (b - a)*i / static_cast<double>(N - 1);
	//	}
	//	return itv;
	//}
	std::vector<double> eval_f(const std::vector<double>& interval, const mfun& f) {
		size_t N = interval.size();
		std::vector<double> fval(N);
		for (size_t i = 0; i < N; ++i) {
			fval[i] = f(interval[i]);
		}
		return fval;
	}

	dVec linspace(double a, double b, size_t N) {
		dVec vec(N);
		if (N > 1) { // size > 1
			for (size_t i = 0; i < N; ++i) {
				vec[i] = a + (b - a)*i / (N - 1);
			}
		}
		else if (N == 1) { // size = 1
			if (a == b) { // endpoints mathces
				vec[0] = a;
			}
			else { // endpoints don't match
				std::cout << " ERROR making linspace.. size  = 1 but endpoints don't match! \n"
					<< " Enter the size again ... N = ";
				std::cin >> N;
				if (std::cin) {
					if (N > 1) {
						vec = linspace(a, b, N);
					}
					else if (N == 1) {
						std::cout << " Enter the endpoints again... \n"
							<< " a = ";
						std::cin >> a;
						std::cout << " b = ";
						std::cin >> b;
						if (a != b) {
							linspace(a, b, N);
						}
					}
					linspace(a, b, N);
				}
				else {
					// THROW some error
				}
			}
		}
		else { // size == 0
			std::cout << " ERROR making linspace.. size  = 0! \n"
				<< " Enter the size again ... N = ";
			std::cin >> N;
			if (std::cin) {
				linspace(a, b, N);
			}
		}
		return vec;
	}


	Graph::Graph(double _xmin, double _xmax, const mfun& _f, size_t _Nx, size_t _Ny) :
		f(_f), board(_Nx, cVec(_Ny, ' ')), f_computed(true), board_set(false),
		GP{ _xmin, _xmax,_Nx,_Ny }
	{
		fval = eval_f(GP.xaxis, _f);
		double fmin = *std::min_element(fval.begin(), fval.end());
		double fmax = *std::max_element(fval.begin(), fval.end());
		// set yaxis with some margin
		GP.set_yaxis(fmin - (fmax - fmin)*0.1,
			fmax + (fmax - fmin)*0.1, _Ny); // yaxis is of size Ny+1

		// GP setting is done!

		// now set board
		setBoard();
		board_set = true;
	}

	Graph::Graph(GraphProperties _GP, const mfun& _f, bool reset_yaxis) : f(_f),
		GP(_GP), board(GP.Nx, cVec(GP.Ny, ' ')), f_computed(true), board_set(false)
	{
		fval = eval_f(GP.xaxis, _f);
		if (reset_yaxis) {
			double fmin = *std::min_element(fval.begin(), fval.end());
			double fmax = *std::max_element(fval.begin(), fval.end());
			// set yaxis with some margin
			GP.set_yaxis(fmin - (fmax - fmin)*0.1,
				fmax + (fmax - fmin)*0.1, GP.Ny); 
			// yaxis is of size Ny+1
		}
		// now set board
		setBoard();
	}

	void Graph::setBoard() {
		// board is constructed to have ' ' at all places
		// set board
		for (size_t xc = 0; xc < GP.Nx; ++xc) {
			bool flag = true;
			if (fval[xc] < GP.ymin) { // too small so nothing to display
				continue; // go to xc+1
			}
			else { // now fval[xc] is guaranteed to be >= ymin, so have something to display
				for (size_t yc = 0; yc < GP.Ny && flag; ++yc) {
					if (fval[xc] <= GP.yaxis[yc]) { // print on yc-th coordinate
						board[xc][yc] = GP.mark;
						flag = false;
					} // since the flag is set to be false, 
					  // the yc-for loop ends here
				}
			}
		}
		board_set = true;
	}
	//// xaxis, yaxis must have size > 0 
	//Graph::Graph(const dVec& _xaxis, const dVec& _yaxis, const mfun& _f) :
	//	xmin(_xaxis[0]), xmax(_xaxis[_xaxis.size() - 1]), 
	//	ymin(_yaxis[0]), ymax(_yaxis[_yaxis.size()-1]), f(_f),
	//	Nx(_xaxis.size()), Ny(_yaxis.size()), xaxis(_xaxis), yaxis(_yaxis),
	//	mark('*'), fval(eval_f(_xaxis, f)), board(Nx,cVec(Ny)), f_computed(true)
	//{
	//}

	//Graph::Graph(const dVec& _xaxis, size_t _Ny, const mfun& _f):
	//	xmin(_xaxis[0]), xmax(_xaxis[_xaxis.size() - 1]), f(_f),
	//	Nx(_xaxis.size()), xaxis(_xaxis),
	//	mark('*'), fval(eval_f(_xaxis, f)), board(Nx, cVec(Ny)), f_computed(true)
	//{
	//	set_yaxis(_Ny); // possible since f is computed
	//}



	void Graph::set_fval() {
		fval = eval_f(GP.xaxis, f);
		f_computed = true;
	}
	void Graph::set_f(const mfun& newf) {
		f = newf;
		set_fval();
	}

	bool Graph::check_printable() const {
		if (fval.size() == (GP.xaxis).size() && f_computed) {
			return true;
		}
		else {
			return false;
		}
	}

	void Graph::print_Graph(std::ostream& out) const {
		if (check_printable()) {
			for (size_t yc = GP.Ny - 1; yc >= 0 && yc < GP.Ny; --yc) { // for each y (backward)
				if (yc % 10 == 0 || yc == GP.Ny - 1) {
					out << std::setw(GP.yaxis_width) << std::fixed << std::setprecision(4) << GP.yaxis[yc];
				}
				else {
					out << std::setw(GP.yaxis_width) << ' ';
				}
				out << ':'; // set axis
				for (size_t xc = 0; xc < GP.Nx; ++xc) {
					out << board[xc][yc];
				}
				out << '\n';
			}
			out << std::setw(GP.yaxis_width) << ' ' << '+'; // set x axis
			for (size_t xc = 0; xc < GP.Nx; ++xc) {
				out << '-';
			}
			out << '\n';
			out << std::setw(GP.yaxis_width) << ' '; // set x axis (val)
			for (size_t xc = 0; xc < GP.Nx; ++xc) {
				if (xc % 10 == 0) {
					out << std::fixed << std::setprecision(3) << std::setw(GP.xaxis_val) << GP.xaxis[xc];
				}
				if (xc % 10 >= GP.xaxis_val) {
					out << ' ';
				}
			}
			out << GP.xaxis[GP.Nx - 1] << std::endl;
		}
		else {
			out << " Graph not printable. size of xaxis doesn't match with the size of fval vector, or f is not computed yet. \n";
		}
	}

	const GraphProperties& Graph::getGraphProperties() const {
		return GP;
	}

	void MathFun::setGraphProperties_by_input(size_t Nx, size_t Ny) {
		double a, b;
		double ym, yM;
		char set_y;
		std::cout << "Enter an interval [a, b]: a = ";
		std::cin >> a;
		std::cout << " b = ";
		std::cin >> b;
		std::cout << " Set y-axis manually? y/n : ";
		std::cin >> set_y;
		if (set_y == 'y') { // manually set y axis
			std::cout << "Enter an interval [ymin, ymax]: ymin = ";
			std::cin >> ym;
			std::cout << " ymax = ";
			std::cin >> yM;
			GraphProperties _GP{ a,b,ym,yM,Nx,Ny };
			GP = _GP;
		}
		else { // y axis auto set... ftn is given
			GraphProperties _GP{ a,b,Nx,Ny };
			GP = _GP; // yaxis is default set
			GP.set_yaxis(f);
		}

	}

	void MathFun::setGraphProperties_by_input() {
		double a, b;
		double ym, yM;
		char set_y;
		std::cout << "Enter an interval [a, b]: a = ";
		std::cin >> a;
		std::cout << " b = ";
		std::cin >> b;
		std::cout << " Set y-axis manually? y/n : ";
		std::cin >> set_y;
		if (set_y == 'y') { // manually set y axis
			std::cout << "Enter an interval [ymin, ymax]: ymin = ";
			std::cin >> ym;
			std::cout << " ymax = ";
			std::cin >> yM;

			size_t Nx, Ny;
			std::cout << "Enter the width of x axis (# pts) : ";
			std::cin >> Nx;
			std::cout << "Enter the width of y axis (# pts) : ";
			std::cin >> Ny;

			GraphProperties _GP{ a,b,ym,yM,Nx,Ny };
			GP = _GP;
		}
		else { // y axis auto set... ftn is given
			size_t Nx, Ny;
			std::cout << "Enter the width of x axis (# pts) : ";
			std::cin >> Nx;
			std::cout << "Enter the width of y axis (# pts) : ";
			std::cin >> Ny;

			GraphProperties _GP{ a,b,Nx,Ny };
			GP = _GP; // yaxis is default set
			GP.set_yaxis(f);
		}

	}

	MathFun::MathFun() : f([](double d)->double {return d; }) {} 
	// GP default constructed, f = identity

	MathFun::MathFun(const mfun& _f): f(_f) {
		// GP default constructed
	}
	void MathFun::plot_graph() const{
		Graph myGraph{ GP, f };
		myGraph.print_Graph(std::cout);
	}
	void MathFun::plot_graph(const GraphProperties& newGP) {
		GP = newGP; // set to new GP and plot
		plot_graph();
	}
	void MathFun::set_f(const mfun& _f) {
		f = _f;
	}
	void MathFun::set_GP(const GraphProperties& newGP) {
		GP = newGP;
	}
	GraphProperties MathFun::getGP() const {
		return GP;
	}
	mfun MathFun::getf() const {
		return f;
	}

}