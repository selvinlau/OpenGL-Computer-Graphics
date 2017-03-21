//
// Class that stores the gooch model params
//

#ifndef GOOCH_H_INCLUDED
#define GOOCH_H_INCLUDED


class Gooch
{
public:
    static const int DEFAULT_EDGE = 0;
    
    Gooch(double b, double y, double alpha, double beta, double edge) : kBlue(0, 0, b), kYellow(y, y, 0), alpha(alpha), beta(beta), edge(edge) { }
    
    Color kBlue;
    Color kYellow;
    double alpha;
    double beta;
    double edge; //Edge line thickness
};

#endif /* end of include guard: GOOCH_H_INCLUDED */
