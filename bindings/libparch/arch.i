%module arch


%include <typedefs.i>


%{
    // make SWIG wrap following headers
    #include "libparch/arch.h"
%}


#define CV __perl_CV
%include "libparch/arch.h"
