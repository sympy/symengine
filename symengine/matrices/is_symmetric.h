#ifndef SYMENGINE_MATRICES_IS_SYMMETRIC_H
#define SYMENGINE_MATRICES_IS_SYMMETRIC_H

#include <symengine/basic.h>
#include <symengine/assumptions.h>
#include <symengine/matrices/matrix_expr.h>

namespace SymEngine
{

tribool is_symmetric(const MatrixExpr &m,
                     const Assumptions *assumptions = nullptr);

}

#endif
