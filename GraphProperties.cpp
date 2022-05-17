#include "parse_ftn.h"

namespace Grapher {
	GraphProperties::GraphProperties() :
		xmin(0), xmax(1), ymin(0), ymax(1),
		Nx(2), Ny(2), mark('*'), xaxis(linspace(0, 1, 2)),
		yaxis(linspace(0, 1, 2)) {}

	GraphProperties::GraphProperties(double _xmin, double _xmax, double _ymin, double _ymax, size_t _Nx, size_t _Ny) :
		xmin(_xmin), xmax(_xmax), ymin(_ymin), ymax(_ymax), 
		Nx(_Nx), Ny(_Ny), mark('*'), xaxis(linspace(_xmin, _xmax, _Nx)),
		 yaxis(linspace(ymin, ymax, _Ny))
	{}
	GraphProperties::GraphProperties(double _xmin, double _xmax, size_t _Nx, size_t _Ny) :
		GraphProperties(_xmin,_xmax,0,1,_Nx,_Ny)
	{
		// note that ymin, ymax, yaxis are not set properly in this constructor
	}

	GraphProperties& GraphProperties::operator=(GraphProperties that) {
		std::swap(xmin, that.xmin);
		std::swap(xmax, that.xmax);
		std::swap(ymin, that.ymin);
		std::swap(ymax, that.ymax);
		std::swap(Nx, that.Nx);
		std::swap(Ny, that.Ny);
		std::swap(mark, that.mark);
		std::swap(xaxis, that.xaxis);
		std::swap(yaxis, that.yaxis);
		return *this;
	}
	void GraphProperties::set_xaxis(double _xmin, double _xmax, size_t _Nx) {
		xmin = _xmin;
		xmax = _xmax;
		Nx = _Nx;
		xaxis = linspace(xmin, xmax, Nx);
	}
	void GraphProperties::set_xaxis(const dVec& _axis) {
		xaxis = _axis;
	}

	void GraphProperties::set_yaxis(double _ymin, double _ymax, size_t _Ny) {
		ymin = _ymin;
		ymax = _ymax;
		yaxis = linspace(ymin, ymax, Ny);
		yaxis.push_back(std::numeric_limits<double>::max());
		// yaxis is of size Ny+1 
	}
	void GraphProperties::set_yaxis(const dVec& _axis) {
		yaxis = _axis;
	}
	void GraphProperties::set_yaxis(const mfun& f) {
		double fmin = f(xaxis[0]), fmax = fmin, fval = fmin;
		for (size_t i = 1; i < Nx; ++i) {
			fval = f(xaxis[i]);
			if (fval < fmin) {
				fmin = fval;
			}
			if (fval > fmax) {
				fmax = fval;
			}
		}
		set_yaxis(fmin - (fmax - fmin)*0.1, 
			fmax + (fmax - fmin)*0.1, Ny);
	}
}