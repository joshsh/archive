extern double exp(double x);
extern double log(double x);
extern double log10(double x);
extern double pow(double x, double y);
extern double sqrt(double x);
extern double ceil(double x);
extern double floor(double x);
extern double fabs(double x);
extern double ldexp(double x, int n);
/** \param exp  $side-effect */
extern double frexp(double x, int* exp);
/** \param ip  $side-effect */
extern double modf(double x, double* ip);
extern double fmod(double x, double y);
extern double sin(double x);
extern double cos(double x);
extern double tan(double x);
extern double asin(double x);
extern double acos(double x);
extern double atan(double x);
extern double atan2(double y, double x);
extern double sinh(double x);
extern double cosh(double x);
extern double tanh(double x);
