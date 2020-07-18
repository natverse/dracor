// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// dracodecodefile
List dracodecodefile(CharacterVector x);
RcppExport SEXP _dracor_dracodecodefile(SEXP xSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< CharacterVector >::type x(xSEXP);
    rcpp_result_gen = Rcpp::wrap(dracodecodefile(x));
    return rcpp_result_gen;
END_RCPP
}
// dracodecode
List dracodecode(RawVector data);
RcppExport SEXP _dracor_dracodecode(SEXP dataSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< RawVector >::type data(dataSEXP);
    rcpp_result_gen = Rcpp::wrap(dracodecode(data));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_dracor_dracodecodefile", (DL_FUNC) &_dracor_dracodecodefile, 1},
    {"_dracor_dracodecode", (DL_FUNC) &_dracor_dracodecode, 1},
    {NULL, NULL, 0}
};

RcppExport void R_init_dracor(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
