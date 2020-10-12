%module basearch


%include <typedefs.i>


%{
    // make SWIG wrap following headers
    #include "libparch/basearch.h"
%}


#define CV __perl_CV
%include "libparch/basearch.h"
